#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <iostream>

#include "npower_sensors.hpp"
#include "fake_sensors.hpp"


int main(int argc, char *argv[]) {
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    //try {
        fake_sensors::HallSensor hallSensor;
    //} catch(std::system_error) {
    //    std::cout << "Error: Failed to initialize Hall Effect sensor." << std::endl;
    //}

    //try {
        fake_sensors::Wattmeter wattmeter;
    //} catch(std::exception) {
    //    std::cout << "Error: Failed to initialize wattmeter." << std::endl;
    //}

    //int value = hallSensor.readValue();
    //float voltage = wattmeter.voltage();
    //float power = wattmeter.power();

    return app.exec();
}
