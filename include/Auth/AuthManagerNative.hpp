#pragma once
#include <QQmlEngine>

#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>

#include "AuthManager.hpp"
#include "Utils/Config.hpp"

class AuthManagerNative final : public AuthManager
{
    Q_OBJECT
public:
    explicit AuthManagerNative(Config& config);

    void login() override;
    void logout() override;
    void refresh() override;
    void handleCode() override { /* Not applicable for native */ }

    const TokenStorage& getTokenStorage() const override { return storage; }

private:
    Config& config;
    TokenStorage storage;

private:
    QOAuth2AuthorizationCodeFlow oauth;
    QOAuthHttpServerReplyHandler handler;
};
