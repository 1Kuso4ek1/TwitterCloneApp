#pragma once
#include "AuthManager.hpp"
#include "Config.hpp"

#include <QNetworkAccessManager>

class AuthManagerWASM final : public AuthManager
{
    Q_OBJECT
public:
    explicit AuthManagerWASM(Config& config);

    void login() override;
    void refresh() override;
    void handleCode() override;

    const TokenStorage& getTokenStorage() const override { return storage; }

private:
    void startAuthFlow(const QString& codeChallenge);
    void getTokens(const QString& code);
    void acceptTokens(QNetworkReply* reply);

private:
    QNetworkAccessManager networkManager;

private:
    Config& config;
    TokenStorage storage;
};
