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

    auto* reply = networkManager.get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply]
    {
        reply->deleteLater();
        if(handleError(reply->error(), reply->errorString()))
            return;

        const auto json = QJsonDocument::fromJson(reply->readAll()).object();

        emit profileReceived(json.toVariantMap());
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
    auto* reply = networkManager.post(req, doc.toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]
    {
        reply->deleteLater();
        if(handleError(reply->error(), reply->errorString()))
            return;

        const auto json = QJsonDocument::fromJson(reply->readAll()).object();

        emit postCreated(json.toVariantMap());
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

    auto* reply = networkManager.deleteResource(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply, postId]
    {
        reply->deleteLater();
        if(handleError(reply->error(), reply->errorString()))
            return;

        emit postDeleted(postId);
    });
}

void Api::getUser(const int userId)
{
    const auto req = requestFactory.createRequest("/users/" + QString::number(userId));
    auto* reply = networkManager.get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply]
    {
        reply->deleteLater();
        if(handleError(reply->error(), reply->errorString()))
            return;

        const auto json = QJsonDocument::fromJson(reply->readAll()).object();

        emit userReceived(json.toVariantMap());
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

    auto* reply = networkManager.get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply]
    {
        reply->deleteLater();
        if(handleError(reply->error(), reply->errorString()))
            return;

        const auto json = QJsonDocument::fromJson(reply->readAll()).array();

        emit feedReceived(json.toVariantList());
    });
}

void Api::getUserPosts(const int userId, const int limit, const int offset)
{
    const auto req = requestFactory.createRequest(
        "/posts/user/" + QString::number(userId) + "?limit="
        + QString::number(limit) + "&offset=" + QString::number(offset));

    auto* reply = networkManager.get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply]
    {
        reply->deleteLater();
        if(handleError(reply->error(), reply->errorString()))
            return;

        const auto json = QJsonDocument::fromJson(reply->readAll()).array();

        emit userPostsReceived(json.toVariantList());
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
