#include "Utils/RequestHandler.hpp"

RequestHandler::RequestHandler(AuthApi& authApi, QObject* parent)
    : QObject(parent), authApi(authApi)
{}

void RequestHandler::executeRequest(QNetworkReply* reply, const std::function<void(const QByteArray&)>& callback)
{
    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]
    {
        reply->deleteLater();
        if(handleError(reply->error(), reply->errorString()))
            return;

        callback(reply->readAll());
    });
}

bool RequestHandler::handleError(const QNetworkReply::NetworkError error, const QString& message)
{
    if(error != QNetworkReply::NoError)
    {
        if((error == QNetworkReply::AuthenticationRequiredError
            || error == QNetworkReply::TooManyRedirectsError)
            && authApi.isLoggedIn())
        {
            qDebug() << "Need to refresh tokens";
            authApi.refresh();
            return true;
        }

        emit errorOccurred(message);
        return true;
    }
    return false;
}
