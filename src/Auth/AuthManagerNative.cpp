#include "Auth/AuthManagerNative.hpp"
#include "Utils/Config.hpp"

#include <QDesktopServices>
#include <QNetworkAccessManager>

#ifdef Q_OS_WASM

#endif

AuthManagerNative::AuthManagerNative(Config& config)
    : config(config), handler(6969, this)
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
    connect(&handler, &QAbstractOAuthReplyHandler::tokenRequestErrorOccurred, this, [this]
    {
        handler.close();

        oauth.setToken("");
        oauth.setRefreshToken("");
        storage.saveTokens({ "", "" });

        emit loginCompleted();
    });
}

void AuthManagerNative::login()
{
    oauth.setTokenUrl(config.getTokenUrl());

    if(!handler.isListening())
        handler.listen(QHostAddress::Any, 6969);

    oauth.grant();
}

void AuthManagerNative::refresh()
{
    if(oauth.status() == QAbstractOAuth2::Status::RefreshingToken)
        return;

    oauth.setTokenUrl(config.getRefreshUrl());
    oauth.refreshTokens();
}
