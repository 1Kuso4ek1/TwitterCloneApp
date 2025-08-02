#pragma once
#include <QNetworkReply>

#include "Api/AuthApi.hpp"

class RequestHandler final : public QObject
{
    Q_OBJECT
public:
    explicit RequestHandler(AuthApi& authApi, QObject* parent = nullptr);

    void executeRequest(QNetworkReply* reply, const std::function<void(const QByteArray&)>& callback);

    bool handleError(QNetworkReply::NetworkError error, const QString& message);

signals:
    void errorOccurred(const QString& error);

private:
    AuthApi& authApi;
};
