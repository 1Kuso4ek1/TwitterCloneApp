#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QFontDatabase>

#include "Api/Api.hpp"

int main(int argc, char *argv[])
{
    const QGuiApplication app(argc, argv);

    qmlRegisterUncreatableType<AuthApi>("Api", 1, 0, "AuthApi", "Only available in the Api singleton");
    qmlRegisterSingletonType<Api>("Api", 1, 0, "Api", Api::instance);

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
