#pragma once
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QObject>
#include <QQmlEngine>

#include "Config.hpp"
#include "TokenStorage.hpp"

class AuthManager final : public QObject
{
    Q_OBJECT
public:
    explicit AuthManager(Config& config, QObject* parent = nullptr);

    void login();
    void refresh();

    const TokenStorage& getTokenStorage() const { return storage; }

signals:
    void loginCompleted();

private:
    Config& config;
    TokenStorage storage;

private:
    QOAuth2AuthorizationCodeFlow oauth;
    QOAuthHttpServerReplyHandler handler;
};
