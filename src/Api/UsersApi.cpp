#include "Api/UsersApi.hpp"

using namespace Qt::Literals::StringLiterals;

UsersApi::UsersApi(
    RequestHandler& requestHandler,
    QNetworkAccessManager& networkManager,
    QNetworkRequestFactory& requestFactory,
    QObject* parent
) : QObject(parent),
    requestHandler(requestHandler),
    networkManager(networkManager),
    requestFactory(requestFactory)
{}

void UsersApi::getMe()
{
    auto req = requestFactory.createRequest("/users/me");
    req.setMaximumRedirectsAllowed(0);

    requestHandler.executeRequest(networkManager.get(req), [this](const auto& data)
    {
        emit profileReceived(QJsonDocument::fromJson(data).object().toVariantMap());
    });
}

void UsersApi::getUser(const int userId)
{
    const auto req = requestFactory.createRequest(u"/users/%1"_s.arg(QString::number(userId)));

    requestHandler.executeRequest(networkManager.get(req), [this](const auto& data)
    {
        emit userReceived(QJsonDocument::fromJson(data).object().toVariantMap());
    });
}
