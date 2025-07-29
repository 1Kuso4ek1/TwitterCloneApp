#include "AuthManager.hpp"
#include "Config.hpp"

#include <QDesktopServices>

AuthManager::AuthManager(Config& config, QObject* parent)
    : QObject(parent),
      config(config),
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

        handler.close();
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

    if(!handler.isListening())
        handler.listen(QHostAddress::Any, 6969);

    oauth.grant();
}

void AuthManager::refresh()
{
    if(oauth.status() == QAbstractOAuth2::Status::RefreshingToken)
        return;

    oauth.setTokenUrl(config.getRefreshUrl());
    oauth.refreshTokens();
}
