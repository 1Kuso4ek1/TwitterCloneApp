#include "AuthManagerWASM.hpp"
#include "PKCEUtils.hpp"

#include <emscripten.h>
#include <emscripten/val.h>

#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace em = emscripten;

using namespace std::string_literals;

AuthManagerWASM::AuthManagerWASM(Config& config)
    : config(config), networkManager(this) {}

void AuthManagerWASM::login()
{
    const auto codeVerifier = PKCEUtils::generateCodeVerifier();
    const auto codeChallenge = PKCEUtils::generateCodeChallenge(codeVerifier);

    startAuthFlow(codeChallenge);
}

void AuthManagerWASM::refresh()
{
    QNetworkRequest request(config.getRefreshUrl());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery data;
    data.addQueryItem("refresh_token", storage.loadTokens().second);

    auto reply = networkManager.post(request, data.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, [this, reply] { acceptTokens(reply); });
}

void AuthManagerWASM::handleCode()
{
    em::val location = em::val::global("location");

    const auto search = QString::fromStdString(location["search"].as<std::string>());

    if(search.isEmpty())
        return;

    const QUrlQuery query({ search.mid(1) });

    qDebug() << location["search"].as<std::string>();

    if(query.hasQueryItem("error"))
    {
        qDebug() << "OAuth error:" << query.queryItemValue("error");
        return;
    }

    if(!query.hasQueryItem("code") || !query.hasQueryItem("state"))
    {
        qDebug() << "Nothing was received";
        return;
    }

    const auto state = query.queryItemValue("state");
    const auto code = query.queryItemValue("code");

    const auto storedState = em::val::global("eval")(u"localStorage.getItem('oauth_state');"_s.toStdString()).as<std::string>();

    if(storedState != state)
    {
        qDebug() << "State mismatch: expected" << storedState << "but got" << state;
        return;
    }

    qDebug() << "OAuth code received:" << code;

    em::val::global("eval")(u"localStorage.removeItem('oauth_state');"_s.toStdString());

    getTokens(code);
}

void AuthManagerWASM::startAuthFlow(const QString& codeChallenge)
{
    const auto state = PKCEUtils::generateState();
    const auto redirect = em::val::global("eval")("(window.location.origin + window.location.pathname)"s).as<std::string>();

    QUrl authUrl("https://accounts.google.com/o/oauth2/auth");

    QUrlQuery query;
    query.addQueryItem("client_id", config.getClientId());
    query.addQueryItem("redirect_uri", QString::fromStdString(redirect));
    query.addQueryItem("response_type", "code");
    query.addQueryItem("scope", "profile");
    query.addQueryItem("code_challenge", codeChallenge);
    query.addQueryItem("code_challenge_method", "S256");
    query.addQueryItem("state", state);

    authUrl.setQuery(query);

    em::val::global("eval")(u"localStorage.setItem('oauth_state', '%1');"_s.arg(state).toStdString());
    em::val::global("eval")(u"window.location.href = '%1';"_s.arg(authUrl.toString()).toStdString());
}

void AuthManagerWASM::getTokens(const QString& code)
{
    QNetworkRequest request(config.getTokenUrl());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery data;
    data.addQueryItem("code", code);

    auto reply = networkManager.post(request, data.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, [this, reply] { acceptTokens(reply); });
}

void AuthManagerWASM::acceptTokens(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        const auto object = QJsonDocument::fromJson(reply->readAll()).object();

        const auto access = object["access_token"].toString();
        const auto refresh = object["refresh_token"].toString();

        storage.saveTokens({ access, refresh });

        emit loginCompleted();
    }
    else
        qDebug() << "Token request error:" << reply->errorString();

    reply->deleteLater();
}
