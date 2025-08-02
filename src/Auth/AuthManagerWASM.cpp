#include "Auth/AuthManagerWASM.hpp"
#include "Utils/PKCEUtils.hpp"

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

    startAuthFlow(codeVerifier, codeChallenge);
}

void AuthManagerWASM::logout()
{
    storage.saveTokens({ "", "" });

    emit loginCompleted();
}

void AuthManagerWASM::refresh()
{
    QNetworkRequest request(config.getRefreshUrl());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setMaximumRedirectsAllowed(0);

    QUrlQuery data;
    data.addQueryItem("refresh_token", storage.loadTokens().second);

    auto reply = networkManager.post(request, data.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, [this, reply] { acceptTokens(reply); });
    connect(reply, &QNetworkReply::errorOccurred, [this](const QNetworkReply::NetworkError error)
    {
        if(error == QNetworkReply::NetworkError::AuthenticationRequiredError
            || error == QNetworkReply::NetworkError::TooManyRedirectsError)
            logout();
    });
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

    const auto state = query.queryItemValue("state", QUrl::FullyDecoded);
    const auto code = query.queryItemValue("code", QUrl::FullyDecoded);

    const auto storedState = em::val::global("eval")(u"localStorage.getItem('oauth_state');"_s.toStdString()).as<std::string>();
    const auto codeVerifier = em::val::global("eval")(u"localStorage.getItem('code_verifier');"_s.toStdString()).as<std::string>();

    if(storedState != state)
    {
        qDebug() << "State mismatch: expected" << storedState << "but got" << state;
        return;
    }

    em::val::global("eval")(u"localStorage.removeItem('oauth_state');"_s.toStdString());
    em::val::global("eval")(u"localStorage.removeItem('code_verifier');"_s.toStdString());

    getTokens(code, QString::fromStdString(codeVerifier));
}

void AuthManagerWASM::startAuthFlow(const QString& codeVerifier, const QString& codeChallenge)
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
    em::val::global("eval")(u"localStorage.setItem('code_verifier', '%1');"_s.arg(codeVerifier).toStdString());
    em::val::global("eval")(u"window.location.href = '%1';"_s.arg(authUrl.toString()).toStdString());
}

void AuthManagerWASM::getTokens(const QString& code, const QString& codeVerifier)
{
    QNetworkRequest request(config.getTokenUrl());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    const auto redirect = em::val::global("eval")("(window.location.origin + window.location.pathname)"s).as<std::string>();

    QUrlQuery data;
    data.addQueryItem("code", code);
    data.addQueryItem("client_id", config.getClientId());
    data.addQueryItem("redirect_uri", QString::fromStdString(redirect));
    data.addQueryItem("code_verifier", codeVerifier);
    data.addQueryItem("app_type", "wasm");

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
