#pragma once

#include <QObject>
#include <qqml.h>

#include <iostream>

#include "npower_sensors.hpp"
#include "fake_sensors.hpp"


#ifndef USE_FAKE_SENSORS
    #define SENSORS npower_sensors
#else
    #define SENSORS fake_sensors
#endif


namespace Backend {

class Challenge : public QObject {
    Q_OBJECT
    Q_PROPERTY(int remainingTime
        MEMBER remaining_time
        WRITE setRemainingTime
        NOTIFY remainingTimeChanged
    )
    Q_PROPERTY(int time MEMBER time NOTIFY timeChanged)
    Q_PROPERTY(int steps MEMBER steps NOTIFY stepsChanged)
    Q_PROPERTY(float speed MEMBER speed NOTIFY speedChanged)
    Q_PROPERTY(float power MEMBER power NOTIFY powerChanged)

  public:
    Challenge();
    void setRemainingTime(int value);

  public slots:
    void startChallenge();

  signals:
    void remainingTimeChanged(const int new_remaining_time);
    void timeChanged(const int new_Time);
    void stepsChanged(const int new_Steps);
    void speedChanged(const float new_Speed);
    void powerChanged(const float new_Power);

  private:
    static std::unique_ptr<SENSORS::HallSensor> hallSensor;
    static std::unique_ptr<SENSORS::Wattmeter> wattmeter;

    static int remaining_time;
    static int time;
    static int steps;
    static float speed;
    static float power;
    static float hallSensor_value;

    static void challengeThread();
    static void recordStepsThread();
    static void recordPowerThread();
};  // class Challenge

class Sensors : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool value READ hallSensorValue)
    Q_PROPERTY(float voltage READ wattmeterVoltage)
    Q_PROPERTY(float current READ wattmeterCurrent)
    Q_PROPERTY(float power READ wattmeterPower)

  public:
    Sensors();
    ~Sensors();

    bool hallSensorValue();
    float wattmeterVoltage();
    float wattmeterCurrent();
    float wattmeterPower();

  public slots:
    void setWattmeterState(std::string state = "reset");

  private:
    std::unique_ptr<SENSORS::HallSensor> hallSensor;
    std::unique_ptr<SENSORS::Wattmeter> wattmeter;
};  // class Sensors

}  // namespace Backend
