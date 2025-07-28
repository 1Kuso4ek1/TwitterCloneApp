#include "TokenStorage.hpp"

#include <QSettings>

void TokenStorage::saveTokens(const QPair<QString, QString>& tokens)
{
    settings.setValue("access_token", tokens.first);
    settings.setValue("refresh_token", tokens.second);
    settings.sync();

    qDebug() << settings.fileName();
}

QPair<QString, QString> TokenStorage::loadTokens() const
{
    return {
        settings.value("access_token").toString(),
        settings.value("refresh_token").toString()
    };
}
