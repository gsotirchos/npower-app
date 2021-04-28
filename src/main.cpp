#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "backend.hpp"

int main(int argc, char *argv[]) {
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<Backend::Sensors>("npower.backend.sensors", 1, 0, "Sensors");
    qmlRegisterType<Backend::Challenge>("npower.backend.challenge", 1, 0, "Challenge");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
