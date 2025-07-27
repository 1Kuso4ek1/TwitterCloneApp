#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "AuthManager.hpp"

int main(int argc, char *argv[])
{
    const QGuiApplication app(argc, argv);

    qmlRegisterSingletonType<AuthManager>("AuthManager", 1, 0, "AuthManager", AuthManager::instance);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        [] { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("TwitterCloneApp", "Main");

    return app.exec();
}
