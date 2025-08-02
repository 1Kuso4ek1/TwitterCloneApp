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
      requestHandler(authApi),
      authApi(config, this),
      usersApi(requestHandler, networkManager, requestFactory, this),
      networkManager(this),
      requestFactory({ config.getBaseUrl() })
{
    connect(&authApi, &AuthApi::loggedInChanged, this, [this]
    {
        requestFactory.setBearerToken(authApi.getAccessToken());
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

    requestHandler.executeRequest(networkManager.post(req, doc.toJson()), [this](const QByteArray& data)
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

    requestHandler.executeRequest(networkManager.deleteResource(req), [this, postId](const QByteArray& data)
    {
        emit postDeleted(postId);
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

    requestHandler.executeRequest(networkManager.get(req), [this](const QByteArray& data)
    {
        emit feedReceived(QJsonDocument::fromJson(data).array().toVariantList());
    });
}

void Api::getUserPosts(const int userId, const int limit, const int offset)
{
    const auto req = requestFactory.createRequest(
        u"/posts/user/%1?limit=%2&offset=%3"_s.arg(userId).arg(limit).arg(offset));

    requestHandler.executeRequest(networkManager.get(req), [this](const QByteArray& data)
    {
        emit userPostsReceived(QJsonDocument::fromJson(data).array().toVariantList());
    });
}
