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
    oauth.setTokenUrl(config.getTokenUrl());
    oauth.setClientIdentifier(config.getClientId());
    oauth.setRequestedScopeTokens({ "profile" });
    oauth.setReplyHandler(&handler);

    connect(&oauth, &QAbstractOAuth2::authorizeWithBrowser, this, &QDesktopServices::openUrl);
    connect(&oauth, &QAbstractOAuth2::granted, this, [this]
    {
        qDebug() << oauth.token().toLatin1();
        qDebug() << oauth.refreshToken().toLatin1();
        handler.close();
    });
}

void AuthManager::login()
{
    if(handler.isListening())
        oauth.grant();
}
