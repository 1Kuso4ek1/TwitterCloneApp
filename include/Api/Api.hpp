#pragma once
#include <QQmlEngine>

#include <QNetworkReply>
#include <QNetworkRequestFactory>

#include "AuthApi.hpp"

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

    Q_INVOKABLE void getMe();
    Q_INVOKABLE void createPost(const QString& content);
    Q_INVOKABLE void deletePost(int postId);
    Q_INVOKABLE void getUser(int userId);
    Q_INVOKABLE void getFeed(int limit = 20, int offset = 0);
    Q_INVOKABLE void getUserPosts(int userId, int limit = 20, int offset = 0);

signals:
    void profileReceived(const QVariantMap& profile);
    void postCreated(const QVariantMap& post);
    void postDeleted(int postId);
    void userReceived(const QVariantMap& user);
    void feedReceived(const QVariantList& feed);
    void userPostsReceived(const QVariantList& posts);

    void errorOccurred(const QString& error);

private:
    void executeRequest(QNetworkReply* reply, const std::function<void(const QByteArray&)>& callback);

    bool handleError(QNetworkReply::NetworkError error, const QString& message);

private:
    Config config;
    AuthApi authApi;

    QNetworkAccessManager networkManager;
    QNetworkRequestFactory requestFactory;
};
