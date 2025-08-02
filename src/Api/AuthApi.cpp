#include "Api/AuthApi.hpp"

#include <QNetworkAccessManager>

AuthApi::AuthApi(Config& config, QObject* parent)
    : QObject(parent), config(config), authManager(config)
{
    connect(&authManager, &AuthManager::loginCompleted, this, [this]
    {
        updateLoginState();

        if(!loggedIn)
            emit errorOccurred("Login failed");
    });
}

void AuthApi::updateLoginState()
{
    const auto& [access, refresh] = authManager.getTokenStorage().loadTokens();
    loggedIn = !access.isEmpty() && !refresh.isEmpty();

    emit loggedInChanged(loggedIn);
}

void AuthApi::login()
{
    authManager.login();
}

void AuthApi::logout()
{
    authManager.logout();
}

void AuthApi::refresh()
{
    authManager.refresh();
}

QByteArray AuthApi::getAccessToken() const
{
    const auto& [access, refresh] = authManager.getTokenStorage().loadTokens();
    return access.toLatin1();
}

void AuthApi::handleLoginCode()
{
    authManager.handleCode();
}
