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
    Q_INVOKABLE void getFeed(int limit = 20, int offset = 0);

signals:
    void loggedInChanged(bool loggedIn);
    void profileReceived(const QVariantMap& profile);
    void feedReceived(const QVariantList& feed);

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
