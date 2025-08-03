#include "Api/UsersApi.hpp"

#include <QFileInfo>
#include <QHttpMultiPart>

#ifdef Q_OS_WASM
    #include <QFileDialog>
#endif

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
#ifdef Q_OS_WASM
    if(path.isEmpty())
        QFileDialog::getOpenFileContent("Images (*.png *.jpg *.jpeg)",
            [this](const QString& fileName, const QByteArray& fileContent)
            {
#endif
    const auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imagePart;
    imagePart.setHeader(
        QNetworkRequest::ContentDispositionHeader,
        { u"form-data; name=\"avatar\"; filename=\"%1\""_s
            .arg(
#ifdef Q_OS_WASM
                fileName
#else
                QFileInfo(path.toString()).fileName()
#endif
            ) }
    );

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_WASM)
    QFile file(path.toLocalFile());
#elif !defined(Q_OS_WASM)
    QFile file(path.toString());
#endif

#ifndef Q_OS_WASM
    if(!file.open(QFile::ReadOnly))
    {
        multiPart->deleteLater();

        qDebug() << "Failed to open file" << path;
        return;
    }

    imagePart.setBody(file.readAll());
#else
    imagePart.setBody(fileContent);
#endif
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

#ifdef Q_OS_WASM
            });
#endif
}

