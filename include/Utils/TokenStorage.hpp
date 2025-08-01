#pragma once
#include <QSettings>

class TokenStorage final : public QObject
{
    Q_OBJECT
public:
    explicit TokenStorage(QObject* parent = nullptr)
        : QObject(parent), settings("IDN", "TwitterClone")
    {}

    void saveTokens(const QPair<QString, QString>& tokens);

    QPair<QString, QString> loadTokens() const;

private:
    QSettings settings;
};
