#pragma once

#include <QObject>
#include <QThread>
#include <qqml.h>

#include <iostream>

#include "npower_sensors.hpp"
#include "fake_sensors.hpp"


#ifndef USE_FAKE_SENSORS
    #define SENSORS npower_sensors
#else
    #define SENSORS fake_sensors
#endif  // USE_FAKE_SENSORS


namespace Backend {

class Challenge;

class Controller : public QObject {
    Q_OBJECT

    Q_PROPERTY(int remainingTime
        MEMBER remaining_time
        WRITE setRemainingTime
        NOTIFY remainingTimeChanged
    )
    Q_PROPERTY(int time
        MEMBER time
        NOTIFY timeChanged
        WRITE setTime
    )
    Q_PROPERTY(int steps
        MEMBER steps
        NOTIFY stepsChanged
        WRITE setSteps
    )
    Q_PROPERTY(float speed
        MEMBER speed
        NOTIFY speedChanged
        WRITE setSpeed
    )
    Q_PROPERTY(float power
        MEMBER power
        NOTIFY powerChanged
        WRITE setPower
    )
    Q_PROPERTY(bool monitorOn
        MEMBER monitor_on
    )

  public:
    Controller();
    ~Controller();

    int remaining_time;
    int time;
    int steps;
    float speed;
    float power;
    static std::unique_ptr<SENSORS::HallSensor> hallSensor;
    static std::unique_ptr<SENSORS::Wattmeter> wattmeter;
    bool monitor_on;

    void setRemainingTime(int value);
    void setTime(int value);
    void setSteps(int value);
    void setSpeed(float value);
    void setPower(float value);

  signals:
    void go(Controller* controller);
    void stop();
    void remainingTimeChanged(int value);
    void timeChanged(int value);
    void stepsChanged(int value);
    void speedChanged(float value);
    void powerChanged(float value);

  public slots:
    void startChallenge();
    void stopChallenge();

  private:
    Challenge* challenge;
    QThread* challengeThread;
};  // class Controller


class Challenge : public QObject {
    Q_OBJECT

  public:
    Challenge();
    ~Challenge();

  public slots:
    void startChallenge(Controller* controller);

  private:
    static void monitorStepsPThread(Controller* controller);
    static void monitorPowerPThread(Controller* controller);
};

}  // namespace Backend
