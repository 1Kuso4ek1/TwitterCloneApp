#include "Api/PostsApi.hpp"

#include <QJsonArray>

using namespace Qt::Literals::StringLiterals;

PostsApi::PostsApi(
    RequestHandler& requestHandler,
    QNetworkAccessManager& networkManager,
    QNetworkRequestFactory& requestFactory,
    QObject* parent
) : QObject(parent),
    requestHandler(requestHandler),
    networkManager(networkManager),
    requestFactory(requestFactory)
{}

void PostsApi::createPost(const QString& content)
{
    auto req = requestFactory.createRequest("/posts");
    req.setMaximumRedirectsAllowed(0);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    const QJsonDocument doc(QJsonObject{ { "content", content } });

    requestHandler.executeRequest(networkManager.post(req, doc.toJson()), [this](const auto& data)
    {
        emit postCreated(QJsonDocument::fromJson(data).object().toVariantMap());
    });
}

void PostsApi::deletePost(const int postId)
{
    auto req = requestFactory.createRequest(u"/posts/%1"_s.arg(QString::number(postId)));
    req.setMaximumRedirectsAllowed(0);

    requestHandler.executeRequest(networkManager.deleteResource(req), [this, postId](const auto&)
    {
        emit postDeleted(postId);
    });
}

void PostsApi::getFeed(const int limit, const int offset)
{
    auto req = requestFactory.createRequest(u"/posts?limit=%1&offset=%2"_s.arg(limit).arg(offset));
    req.setMaximumRedirectsAllowed(0);

    requestHandler.executeRequest(networkManager.get(req), [this](const auto& data)
    {
        emit feedReceived(QJsonDocument::fromJson(data).array().toVariantList());
    });
}

void PostsApi::getUserPosts(const int userId, const int limit, const int offset)
{
    const auto req = requestFactory.createRequest(
        u"/posts/user/%1?limit=%2&offset=%3"_s.arg(userId).arg(limit).arg(offset));

    requestHandler.executeRequest(networkManager.get(req), [this](const auto& data)
    {
        emit userPostsReceived(QJsonDocument::fromJson(data).array().toVariantList());
    });
}

void PostsApi::likePost(const int postId)
{
    auto req = requestFactory.createRequest(u"/like/%1"_s.arg(postId));
    req.setMaximumRedirectsAllowed(0);

    requestHandler.executeRequest(networkManager.post(req, nullptr), [this, postId](const auto&)
    {
        emit postLiked(postId);
    });
}

void PostsApi::unlikePost(const int postId)
{
    auto req = requestFactory.createRequest(u"/like/%1"_s.arg(postId));
    req.setMaximumRedirectsAllowed(0);

    requestHandler.executeRequest(networkManager.deleteResource(req), [this, postId](const auto&)
    {
        emit postUnliked(postId);
    });
}
