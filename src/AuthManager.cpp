#include "AuthManager.hpp"
#include "Config.hpp"

#include <QDesktopServices>

AuthManager::AuthManager(QObject* parent)
    : QObject(parent),
      config(":/config/config.json"),
      handler(6969, this)
{
    handler.setCallbackPath("/oauth");

    oauth.setAuthorizationUrl({ "https://accounts.google.com/o/oauth2/v2/auth" });
    oauth.setClientIdentifier(config.getClientId());
    oauth.setRequestedScopeTokens({ "profile" });
    oauth.setReplyHandler(&handler);

    if(const auto& [access, refresh] = storage.loadTokens();
       !access.isEmpty() && !refresh.isEmpty())
    {
        oauth.setToken(access);
        oauth.setRefreshToken(refresh);
    }

    connect(&oauth, &QAbstractOAuth2::authorizeWithBrowser, this, &QDesktopServices::openUrl);
    connect(&oauth, &QAbstractOAuth2::granted, this, [this]
    {
        handler.close();

        storage.saveTokens({ oauth.token(), oauth.refreshToken() });

        emit loginCompleted();
    });
}

void AuthManager::login()
{
    oauth.setTokenUrl(config.getTokenUrl());

    if(handler.isListening())
        oauth.grant();
}

void AuthManager::refresh()
{
    oauth.setTokenUrl(config.getRefreshUrl());
    oauth.refreshTokens();
}
