#pragma once
#include "Utils/TokenStorage.hpp"

class AuthManager : public QObject
{
    Q_OBJECT
public:
    ~AuthManager() override = default;

    virtual void login() = 0;
    virtual void refresh() = 0;
    virtual void handleCode() = 0;

    virtual const TokenStorage& getTokenStorage() const = 0;

signals:
    void loginCompleted();
};
