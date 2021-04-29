#include "backend.hpp"

#include <iostream>
#include <thread>


namespace Backend {


Controller::Controller()
  : remaining_time{0},
    time{0},
    steps{0},
    speed{0},
    power{0},
    monitor_on{true}
{
    challengeThread = new QThread;

    std::cout << "CONTROLLER CREATED" << std::endl;
}

Controller::~Controller() {
    stopChallenge();

    std::cout << "CONTROLLER DESTROYED" << std::endl;
}

std::unique_ptr<SENSORS::HallSensor> Controller::hallSensor =
    std::make_unique<SENSORS::HallSensor>();

std::unique_ptr<SENSORS::Wattmeter> Controller::wattmeter =
    std::make_unique<SENSORS::Wattmeter>();

void Controller::setRemainingTime(int value) {
    remaining_time = value;
    emit remainingTimeChanged(remaining_time);
    std::cout << "REMAINING TIME: " << remaining_time << std::endl;
}

void Controller::startChallenge() {
    // create and start a challenge QThread
    challenge = new Challenge;
    challenge->moveToThread(challengeThread);
    connect(
        this, &Controller::go,
        challenge, &Challenge::startChallenge
    );
    connect(
        challengeThread, &QThread::finished,
        challenge, &QObject::deleteLater
    );

    challengeThread->start();

    // enable monitors and signal challenge to start
    monitor_on = true;
    emit go(this);
}

void Controller::stopChallenge() {
    if (challengeThread->isRunning()) {
        monitor_on = false;
        challengeThread->quit();
        challengeThread->wait();
    }
}

Challenge::Challenge() {
    std::cout << "CHALLENGE QTHREAD CREATED" << std::endl;
}

Challenge::~Challenge() {
    std::cout << "CHALLENGE QTHREAD DESTROYED" << std::endl;
}

void Challenge::startChallenge(Controller* controller) {
    const static int delay_s = 1;

    std::cout << "CHALLENGE STARTED" << std::endl;

    // monitor steps and power on other threads
    std::thread t_steps(monitorStepsPThread, controller);
    std::thread t_power(monitorPowerPThread, controller);

    while ((controller->remaining_time > 0)
            && (controller->monitor_on == true)) {
        std::this_thread::sleep_for(std::chrono::seconds(delay_s));

        controller->time++;
        emit controller->timeChanged(controller->time);

        controller->remaining_time--;
        emit controller->remainingTimeChanged(controller->remaining_time);
    }

    t_steps.join();
    t_power.join();

    std::cout << "CHALLENGE FINISHED" << std::endl;
}

void Challenge::monitorStepsPThread(Controller* controller) {
    const static int delay_us = 30;
    int n = 0;
    int new_value = controller->hallSensor->readValue();
    int previous_value = new_value;

    while ((controller->remaining_time > 0)
            && (controller->monitor_on == true)) {
        new_value = controller->hallSensor->readValue();

        if ((new_value < previous_value)
            && (n*delay_us > 200)) {  // ignore rapid motion/noise
            controller->steps++;
            emit controller->stepsChanged(controller->steps);

            controller->speed = 60.0/(n*delay_us/1000.0);  // rpm
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
    while ((controller->remaining_time > 0)
            && (controller->monitor_on == true)) {
        controller->power += controller->wattmeter->power();
        emit controller->powerChanged(controller->power);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

}  // namespace Backend
