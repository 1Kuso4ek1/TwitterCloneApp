#pragma once
#include <QQmlEngine>

#include "UsersApi.hpp"

class Api final : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
public:
    explicit Api(QObject* parent = nullptr);

    static Api* instance(QQmlEngine*, QJSEngine*) { return new Api; }

    Q_PROPERTY(AuthApi* auth READ getAuthApi CONSTANT)
    AuthApi* getAuthApi() { return &authApi; }

    Q_PROPERTY(UsersApi* users READ getUsersApi CONSTANT)
    UsersApi* getUsersApi() { return &usersApi; }

    Q_INVOKABLE void createPost(const QString& content);
    Q_INVOKABLE void deletePost(int postId);
    Q_INVOKABLE void getFeed(int limit = 20, int offset = 0);
    Q_INVOKABLE void getUserPosts(int userId, int limit = 20, int offset = 0);

signals:
    void postCreated(const QVariantMap& post);
    void postDeleted(int postId);
    void feedReceived(const QVariantList& feed);
    void userPostsReceived(const QVariantList& posts);

    void errorOccurred(const QString& error);

private:
    Config config;
    RequestHandler requestHandler;

    AuthApi authApi;
    UsersApi usersApi;

    QNetworkAccessManager networkManager;
    QNetworkRequestFactory requestFactory;
};
