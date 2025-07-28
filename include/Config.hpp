#pragma once
#include <QFile>
#include <QJsonObject>

class Config
{
public:
    explicit Config(const QString& filePath)
    {
        QFile file(filePath);
        if(!file.open(QFile::ReadOnly | QFile::Text))
            qFatal() << "Could not open config file: " << filePath;

        const auto data = file.readAll();
        file.close();

        const auto json = QJsonDocument::fromJson(data);
        if(json.isNull() || !json.isObject())
            qFatal() << "Invalid config file: " << filePath;

        tokenUrl = json["oauth2"]["token_url"].toString();
        refreshUrl = json["oauth2"]["refresh_url"].toString();
        clientId = json["oauth2"]["client_id"].toString();
    }

    QUrl getTokenUrl() const { return tokenUrl; }
    QUrl getRefreshUrl() const { return refreshUrl; }
    QString getClientId() const { return clientId; }

private:
    QUrl tokenUrl, refreshUrl;
    QString clientId;
};
