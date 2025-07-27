#pragma once
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QObject>
#include <QQmlEngine>

#include <qqmlintegration.h>

#include "Config.hpp"

class AuthManager final : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
public:
    explicit AuthManager(QObject* parent = nullptr);

    static AuthManager* instance(QQmlEngine*, QJSEngine*) { return new AuthManager; }

    Q_INVOKABLE void login();

private:
    Config config;

private:
    QOAuth2AuthorizationCodeFlow oauth;
    QOAuthHttpServerReplyHandler handler;
};
