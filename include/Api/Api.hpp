#pragma once
#include <QQmlEngine>

#include "PostsApi.hpp"
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

    Q_PROPERTY(PostsApi* posts READ getPostsApi CONSTANT)
    PostsApi* getPostsApi() { return &postsApi; }

    Q_PROPERTY(UsersApi* users READ getUsersApi CONSTANT)
    UsersApi* getUsersApi() { return &usersApi; }

private:
    Config config;
    RequestHandler requestHandler;

    AuthApi authApi;
    PostsApi postsApi;
    UsersApi usersApi;

    QNetworkAccessManager networkManager;
    QNetworkRequestFactory requestFactory;
};
