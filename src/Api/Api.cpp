#include "Api/Api.hpp"

#include <QJsonArray>
#include <QNetworkReply>

using namespace Qt::Literals::StringLiterals;

Api::Api(QObject* parent)
    : QObject(parent),
#ifndef Q_OS_WASM
      config(":/config/config.json"),
#else
      config(":/config/config-wasm.json"),
#endif
      requestHandler(authApi),
      authApi(config, this),
      postsApi(requestHandler, networkManager, requestFactory, this),
      usersApi(requestHandler, networkManager, requestFactory, this),
      networkManager(this),
      requestFactory({ config.getBaseUrl() })
{
    connect(&authApi, &AuthApi::loggedInChanged, this, [this]
    {
        requestFactory.setBearerToken(authApi.getAccessToken());
    });
}
