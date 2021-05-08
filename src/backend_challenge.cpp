#include "backend.hpp"

#include <iostream>
#include <thread>


namespace Backend {

//// Challenge class ////
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
    while (not controller->targetReached()
            && controller->challenge_monitor_on) {
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

    while (not controller->targetReached()
            && controller->challenge_monitor_on) {
        new_value = controller->hallSensor->readValue();

        // register step when sensor reading changes from high to low
        if ((new_value < previous_value)
            && (n*delay_us > 200)) {  // ignore rapid motion/noise
            controller->steps++;
            emit controller->stepsChanged(controller->steps);

            // calculate speed = 1/[duration of  1 step] (rpm)
            controller->speed = 60.0/(n*delay_us/1000.0);
            emit controller->speedChanged(controller->speed);

            if (controller->speed > controller->max_speed) {
                controller->max_speed = controller->speed;
                emit controller->maxSpeedChanged(controller->max_speed);
            }
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

    while (not controller->targetReached()
            && controller->challenge_monitor_on) {
        // register only positive values (generated power)
        float current = controller->wattmeter->current();
        float power = controller->wattmeter->power()/1000.0;
        if (current > 0.0) {
            controller->power += power;
            emit controller->powerChanged(controller->power);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(delay_us));
    }
}

}  // namespace Backend
