#pragma once
#include <QNetworkRequestFactory>

#include "Utils/Config.hpp"
#include "Utils/RequestHandler.hpp"

class PostsApi final : public QObject
{
    Q_OBJECT
public:
    explicit PostsApi(
        RequestHandler& requestHandler,
        QNetworkAccessManager& networkManager,
        QNetworkRequestFactory& requestFactory,
        QObject* parent = nullptr
    );

    Q_INVOKABLE void createPost(const QString& content);
    Q_INVOKABLE void deletePost(int postId);
    Q_INVOKABLE void getFeed(int limit = 20, int offset = 0);
    Q_INVOKABLE void getUserPosts(int userId, int limit = 20, int offset = 0);
    Q_INVOKABLE void likePost(int postId);
    Q_INVOKABLE void unlikePost(int postId);

signals:
    void postCreated(const QVariantMap& post);
    void postDeleted(int postId);
    void feedReceived(const QVariantList& feed);
    void userPostsReceived(const QVariantList& posts);
    void postLiked(int postId);
    void postUnliked(int postId);

private:
    RequestHandler& requestHandler;

    QNetworkAccessManager& networkManager;
    QNetworkRequestFactory& requestFactory;
};
