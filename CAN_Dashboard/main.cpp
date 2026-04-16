#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "realtime.h"
#include "stm32.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<RealTime>("RealTimeLib", 1, 0, "RealTime");
      qmlRegisterType<Stm32>("STM32_", 1, 0, "Stm32");
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("CAN_Dashboard", "Main");
    return app.exec();
}
