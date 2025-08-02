#pragma once
#include <QNetworkRequestFactory>

#include "Utils/Config.hpp"
#include "Utils/RequestHandler.hpp"

class UsersApi final : public QObject
{
    Q_OBJECT
public:
    explicit UsersApi(
        RequestHandler& requestHandler,
        QNetworkAccessManager& networkManager,
        QNetworkRequestFactory& requestFactory,
        QObject* parent = nullptr
    );

    Q_INVOKABLE void getMe();
    Q_INVOKABLE void getUser(int userId);

signals:
    void profileReceived(const QVariantMap& profile);
    void userReceived(const QVariantMap& user);

private:
    RequestHandler& requestHandler;

    QNetworkAccessManager& networkManager;
    QNetworkRequestFactory& requestFactory;
};
