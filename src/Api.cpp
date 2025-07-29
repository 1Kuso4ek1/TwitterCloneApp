#include "Api.hpp"

#include <QJsonArray>
#include <QNetworkReply>

Api::Api(QObject* parent)
    : QObject(parent),
      config(":/config/config.json"),
      authManager(config, this),
      networkManager(this),
      requestFactory({ config.getBaseUrl() })
{
    connect(&authManager, &AuthManager::loginCompleted, this, [this]
    {
        updateLoginState();

        if(!loggedIn)
            emit errorOccurred("Login failed");
    });
}

void Api::updateLoginState()
{
    const auto& [access, refresh] = authManager.getTokenStorage().loadTokens();
    loggedIn = !access.isEmpty() && !refresh.isEmpty();

    emit loggedInChanged(loggedIn);

    if(loggedIn)
        requestFactory.setBearerToken(access.toLatin1());
}

void Api::login()
{
    authManager.login();
}

void Api::getMe()
{
    if(!loggedIn)
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
    if(!loggedIn)
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
    if(!loggedIn)
    {
        emit errorOccurred("User is not logged in");
        return;
    }

    auto req = requestFactory.createRequest("/posts/" + QString::number(postId));
    req.setMaximumRedirectsAllowed(0);

    executeRequest(networkManager.deleteResource(req), [this, postId](const QByteArray& data)
    {
        emit postDeleted(postId);
    });
}

void Api::getUser(const int userId)
{
    const auto req = requestFactory.createRequest("/users/" + QString::number(userId));

    executeRequest(networkManager.get(req), [this](const QByteArray& data)
    {
        emit userReceived(QJsonDocument::fromJson(data).object().toVariantMap());
    });
}

void Api::getFeed(const int limit, const int offset)
{
    if(!loggedIn)
    {
        emit errorOccurred("User is not logged in");
        return;
    }

    auto req = requestFactory.createRequest(
        "/posts?limit=" + QString::number(limit) + "&offset=" + QString::number(offset));
    req.setMaximumRedirectsAllowed(0);

    executeRequest(networkManager.get(req), [this](const QByteArray& data)
    {
        emit feedReceived(QJsonDocument::fromJson(data).array().toVariantList());
    });
}

void Api::getUserPosts(const int userId, const int limit, const int offset)
{
    const auto req = requestFactory.createRequest(
        "/posts/user/" + QString::number(userId) + "?limit="
        + QString::number(limit) + "&offset=" + QString::number(offset));

    executeRequest(networkManager.get(req), [this](const QByteArray& data)
    {
        emit userPostsReceived(QJsonDocument::fromJson(data).array().toVariantList());
    });
}

void Api::executeRequest(QNetworkReply* reply, std::function<void(const QByteArray&)> callback)
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
            authManager.refresh();
            return true;
        }

        emit errorOccurred(message);
        return true;
    }
    return false;
}
