#pragma once
#include "AuthManager.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequestFactory>

class Api final : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
public:
    explicit Api(QObject* parent = nullptr);

    static Api* instance(QQmlEngine*, QJSEngine*) { return new Api; }

    Q_INVOKABLE void updateLoginState();
    Q_INVOKABLE void login();
    Q_INVOKABLE void getMe();
    Q_INVOKABLE void createPost(const QString& content);
    Q_INVOKABLE void deletePost(int postId);
    Q_INVOKABLE void getUser(int userId);
    Q_INVOKABLE void getFeed(int limit = 20, int offset = 0);
    Q_INVOKABLE void getUserPosts(int userId, int limit = 20, int offset = 0);

signals:
    void loggedInChanged(bool loggedIn);
    void profileReceived(const QVariantMap& profile);
    void postCreated(const QVariantMap& post);
    void postDeleted(int postId);
    //void userReceived(const QVariantMap& user);
    void feedReceived(const QVariantList& feed);
    void userPostsReceived(const QVariantList& posts);

    void errorOccurred(const QString& error);

private:
    bool handleError(QNetworkReply::NetworkError error, const QString& message);

private:
    bool loggedIn{};

private:
    AuthManager authManager;

    QNetworkAccessManager networkManager;
    QNetworkRequestFactory requestFactory;
};
