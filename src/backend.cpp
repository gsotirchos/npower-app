#include "backend.hpp"

#include <iostream>
#include <algorithm>
#include <thread>


namespace Backend {


// Controller class
Controller::Controller()
  : remaining_time{0},
    time{0},
    steps{0},
    speed{0},
    power{0},
    battery_monitor_on{true},
    challenge_monitor_on{true},
    charge_percentage{50}
{
    // create QThread for challenge
    challengeThread = new QThread;

    // create a QThread for the battery monitor
    batteryMonitorThread = new QThread;

    std::cout << "- CONTROLLER CREATED" << std::endl;
}

Controller::~Controller() {
    stopChallenge();
    stopBatteryMonitor();

    std::cout << "- CONTROLLER DESTROYED" << std::endl;
}

std::unique_ptr<SENSORS::HallSensor> Controller::hallSensor =
    std::make_unique<SENSORS::HallSensor>();

std::unique_ptr<SENSORS::Wattmeter> Controller::wattmeter =
    std::make_unique<SENSORS::Wattmeter>();

void Controller::setRemainingTime(int value) {
    remaining_time = value;
    emit remainingTimeChanged(remaining_time);
    std::cout << "- REMAINING TIME: " << remaining_time << std::endl;
}

void Controller::setTime(int value) {
    time = value;
    emit timeChanged(time);
    std::cout << "- TIME: " << time << std::endl;
}

void Controller::setSteps(int value) {
    steps = value;
    emit stepsChanged(steps);
    std::cout << "- STEPS: " << steps << std::endl;
}

void Controller::setSpeed(float value) {
    speed = value;
    emit speedChanged(speed);
    std::cout << "- SPEED: " << speed << std::endl;
}

void Controller::setPower(float value) {
    power = value;
    emit powerChanged(power);
    std::cout << "- POWER: " << power << std::endl;
}

void Controller::startBatteryMonitor() {
    // create a battery monitor QThread
    batteryMonitor = new BatteryMonitor;
    batteryMonitor->moveToThread(batteryMonitorThread);
    connect(
        this, &Controller::runBatteryMonitor,
        batteryMonitor, &BatteryMonitor::start
    );
    connect(
        batteryMonitorThread, &QThread::finished,
        batteryMonitor, &QObject::deleteLater
    );

    // enable monitors, start the thread, and signal battery monitor to run
    battery_monitor_on = true;
    batteryMonitorThread->start();
    emit runBatteryMonitor(this);
}

void Controller::stopBatteryMonitor() {
    // disable monitors and quit & wait for battery monitor QThread
    battery_monitor_on = false;
    batteryMonitorThread->quit();
    batteryMonitorThread->wait();
}

void Controller::startChallenge() {
    // create a challenge QThread
    challenge = new Challenge;
    challenge->moveToThread(challengeThread);
    connect(
        this, &Controller::runChallenge,
        challenge, &Challenge::start
    );
    connect(
        challengeThread, &QThread::finished,
        challenge, &QObject::deleteLater
    );

    // enable monitors, start the thread, and signal challenge to run
    challenge_monitor_on = true;
    challengeThread->start();
    emit runChallenge(this);
}

void Controller::stopChallenge() {
    if (challengeThread->isRunning()) {
        // disable monitors and quit & wait for challenge QThread
        challenge_monitor_on = false;
        challengeThread->quit();
        challengeThread->wait();
    }
}

// Challenge class
Challenge::Challenge() {
    std::cout << "- CHALLENGE QTHREAD CREATED" << std::endl;
}

Challenge::~Challenge() {
    std::cout << "- CHALLENGE QTHREAD DESTROYED" << std::endl;
}

void Challenge::start(Controller* controller) {
    static int const delay_s = 1;

    std::cout << "- CHALLENGE STARTED" << std::endl;

    // monitor steps and power on separate Posix threads
    std::thread t_steps(monitorStepsPThread, controller);
    std::thread t_power(monitorPowerPThread, controller);

    // clock
    while ((controller->remaining_time > 0)
            && (controller->challenge_monitor_on == true)) {
        std::this_thread::sleep_for(std::chrono::seconds(delay_s));

        controller->time++;
        emit controller->timeChanged(controller->time);

        controller->remaining_time--;
        emit controller->remainingTimeChanged(controller->remaining_time);
    }

    // wait for Posix threads to finish
    t_steps.join();
    t_power.join();

    std::cout << "- CHALLENGE FINISHED" << std::endl;
}

// function to monitor steps
void Challenge::monitorStepsPThread(Controller* controller) {
    static int const delay_us = 30;
    int n = 0;
    int new_value = controller->hallSensor->readValue();
    int previous_value = new_value;

    while ((controller->remaining_time > 0)
            && (controller->challenge_monitor_on == true)) {
        new_value = controller->hallSensor->readValue();

        // register step when sensor reading changes from high to low
        if ((new_value < previous_value)
            && (n*delay_us > 200)) {  // ignore rapid motion/noise
            controller->steps++;
            emit controller->stepsChanged(controller->steps);

            // calculate speed = 1/[duration of  1 step] (rpm)
            controller->speed = 60.0/(n*delay_us/1000.0);
            emit controller->speedChanged(controller->speed);
            n = 0;
        } else {
            n++;
        }

        previous_value = new_value;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_us));
    }
}

void Challenge::monitorPowerPThread(Controller* controller) {
    static int const delay_us = 300;

    while ((controller->remaining_time > 0)
            && (controller->challenge_monitor_on == true)) {
        // register only positive values (generated power)
        float power = controller->wattmeter->power();
        if (power > 0.0) {
            controller->power += controller->wattmeter->power();
            emit controller->powerChanged(controller->power);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(delay_us));
    }
}

// BatteryMonitor class
BatteryMonitor::BatteryMonitor() {
    std::cout << "- BATTERY MONITOR CREATED" << std::endl;
}

BatteryMonitor::~BatteryMonitor() {
    std::cout << "- BATTERY MONITOR DESTROYED" << std::endl;
}

void BatteryMonitor::start(Controller* controller) {
    static int const delay_s = 10;
    static float const max_oc_voltage = 13.0;
    static float const min_oc_voltage = 11.8;
    static float const max_oc_current = 0.7;
    static float const min_oc_current = 0;
    static float voltage;
    static float current;
    static int percentage;

    while (controller->battery_monitor_on == true) {
        current = controller->wattmeter->current();
        voltage = controller->wattmeter->voltage();

        // check if idling (~open-cirquit)
        if ((current > min_oc_current)
            && (current < max_oc_current)) {
            percentage =
                100*(voltage - min_oc_voltage)
                    /(max_oc_voltage - min_oc_voltage);
            percentage = std::max(0, percentage);
            percentage = std::min(100, percentage);
            controller->charge_percentage = std::max(0, percentage);

            emit controller->chargePercentageChanged(controller->charge_percentage);
            std::cout << "- BATTERY PERCENTAGE: " << controller->charge_percentage
                << "%" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(delay_s));
    }
}

}  // namespace Backend
