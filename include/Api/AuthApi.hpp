#pragma once
#include <QObject>

#ifndef Q_OS_WASM
    #include "Auth/AuthManagerNative.hpp"
    using AuthManagerImpl = AuthManagerNative;
#else
    #include "Auth/AuthManagerWASM.hpp"
    using AuthManagerImpl = AuthManagerWASM;
#endif

class AuthApi final : public QObject
{
    Q_OBJECT
public:
    explicit AuthApi(Config& config, QObject* parent = nullptr);

    Q_INVOKABLE void updateLoginState();
    Q_INVOKABLE void handleLoginCode();
    Q_INVOKABLE void login();
    Q_INVOKABLE void logout();

    void refresh();

    bool isLoggedIn() const { return loggedIn; }
    QByteArray getAccessToken() const;

signals:
    void loggedInChanged(bool loggedIn);
    void errorOccurred(const QString& error);

private:
    bool loggedIn{};

private:
    Config& config;
    AuthManagerImpl authManager;
};
