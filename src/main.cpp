#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Api.hpp"

int main(int argc, char *argv[])
{
    const QGuiApplication app(argc, argv);

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
