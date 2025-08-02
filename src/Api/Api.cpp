#include "Api/Api.hpp"

#include <QJsonArray>
#include <QNetworkReply>

using namespace Qt::Literals::StringLiterals;

Api::Api(QObject* parent)
    : QObject(parent),
#ifndef Q_OS_WASM
      config(":/config/config.json"),
#else
      config(":/config/config-wasm.json"),
#endif
      authApi(config, this),
      networkManager(this),
      requestFactory({ config.getBaseUrl() })
{
    connect(&authApi, &AuthApi::loggedInChanged, this, [this]
    {
        requestFactory.setBearerToken(authApi.getAccessToken());
    });
}

void Api::getMe()
{
    if(!authApi.isLoggedIn())
    {
        emit errorOccurred("User is not logged in");
        return;
    }

    auto req = requestFactory.createRequest("/users/me");
    req.setMaximumRedirectsAllowed(0);

    executeRequest(networkManager.get(req), [this](const QByteArray& data)
    {
        emit profileReceived(QJsonDocument::fromJson(data).object().toVariantMap());
    });
}

void Api::createPost(const QString& content)
{
    if(!authApi.isLoggedIn())
    {
        emit errorOccurred("User is not logged in");
        return;
    }

    auto req = requestFactory.createRequest("/posts");
    req.setMaximumRedirectsAllowed(0);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    const QJsonDocument doc(QJsonObject{ { "content", content } });

    executeRequest(networkManager.post(req, doc.toJson()), [this](const QByteArray& data)
    {
        emit postCreated(QJsonDocument::fromJson(data).object().toVariantMap());
    });
}

void Api::deletePost(const int postId)
{
    if(!authApi.isLoggedIn())
    {
        emit errorOccurred("User is not logged in");
        return;
    }

    auto req = requestFactory.createRequest(u"/posts/%1"_s.arg(QString::number(postId)));
    req.setMaximumRedirectsAllowed(0);

    executeRequest(networkManager.deleteResource(req), [this, postId](const QByteArray& data)
    {
        emit postDeleted(postId);
    });
}

void Api::getUser(const int userId)
{
    const auto req = requestFactory.createRequest(u"/users/%1"_s.arg(QString::number(userId)));

    executeRequest(networkManager.get(req), [this](const QByteArray& data)
    {
        emit userReceived(QJsonDocument::fromJson(data).object().toVariantMap());
    });
}

void Api::getFeed(const int limit, const int offset)
{
    if(!authApi.isLoggedIn())
    {
        emit errorOccurred("User is not logged in");
        return;
    }

    auto req = requestFactory.createRequest(u"/posts?limit=%1&offset=%2"_s.arg(limit).arg(offset));
    req.setMaximumRedirectsAllowed(0);

    executeRequest(networkManager.get(req), [this](const QByteArray& data)
    {
        emit feedReceived(QJsonDocument::fromJson(data).array().toVariantList());
    });
}

void Api::getUserPosts(const int userId, const int limit, const int offset)
{
    const auto req = requestFactory.createRequest(
        u"/posts/user/%1?limit=%2&offset=%3"_s.arg(userId).arg(limit).arg(offset));

    executeRequest(networkManager.get(req), [this](const QByteArray& data)
    {
        emit userPostsReceived(QJsonDocument::fromJson(data).array().toVariantList());
    });
}

void Api::executeRequest(QNetworkReply* reply, const std::function<void(const QByteArray&)>& callback)
{
    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]
    {
        reply->deleteLater();
        if(handleError(reply->error(), reply->errorString()))
            return;

        callback(reply->readAll());
    });
}

bool Api::handleError(const QNetworkReply::NetworkError error, const QString& message)
{
    if(error != QNetworkReply::NoError)
    {
        if(error == QNetworkReply::AuthenticationRequiredError
            || error == QNetworkReply::TooManyRedirectsError)
        {
            qDebug() << "Need to refresh tokens";
            authApi.refresh();
            return true;
        }

        emit errorOccurred(message);
        return true;
    }
    return false;
}
