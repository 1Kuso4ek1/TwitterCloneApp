#include "Api/UsersApi.hpp"

#include <QHttpMultiPart>
#include <QFileInfo>

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
    auto req = requestFactory.createRequest(u"/users/%1"_s.arg(QString::number(userId)));
    req.setMaximumRedirectsAllowed(0);

    requestHandler.executeRequest(networkManager.get(req), [this](const auto& data)
    {
        emit userReceived(QJsonDocument::fromJson(data).object().toVariantMap());
    });
}

void UsersApi::updateMe(const QString& displayName, const QString& username)
{
    auto req = requestFactory.createRequest("/users/me");
    req.setMaximumRedirectsAllowed(0);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    const QJsonDocument doc(QJsonObject{ { "display_name", displayName }, { "username", username} });

    requestHandler.executeRequest(networkManager.post(req, doc.toJson()), [this](const auto& data)
    {
        const auto obj = QJsonDocument::fromJson(data).object().toVariantMap();

        emit userReceived(obj);
        emit profileReceived(obj);
    });
}

void UsersApi::uploadAvatar(const QUrl& path)
{
    const auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    const auto localFile = path.toLocalFile();

    QHttpPart imagePart;
    imagePart.setHeader(
        QNetworkRequest::ContentDispositionHeader,
        { u"form-data; name=\"avatar\"; filename=\"%1\""_s
            .arg(QFileInfo(localFile).fileName()) }
    );

    const auto file = new QFile(localFile);
    if(!file->open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open file" << localFile;
        return;
    }
    file->setParent(multiPart);

    imagePart.setBodyDevice(file);

    multiPart->append(imagePart);

    auto req = requestFactory.createRequest("/users/me/avatar");
    req.setMaximumRedirectsAllowed(0);

    const auto reply = networkManager.post(req, multiPart);
    multiPart->setParent(reply);

    requestHandler.executeRequest(reply, [this](const auto& data)
    {
        const auto obj = QJsonDocument::fromJson(data).object().toVariantMap();

        emit userReceived(obj);
        emit profileReceived(obj);
    });
}

