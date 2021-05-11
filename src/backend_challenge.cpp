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

void Challenge::start(Controller * controller) {
    static int const delay_s = 1;

    std::cout << "- CHALLENGE STARTED" << std::endl;

    // monitor steps and power on separate Posix threads
    std::thread t_steps(monitorStepsPThread, controller);
    std::thread t_power(monitorPowerPThread, controller);

    // clock
    while (not controller->targetReached()
            && controller->challenge_monitor_on) {
        std::this_thread::sleep_for(std::chrono::seconds(delay_s));

        controller->setTime(++controller->time);
        controller->setRemainingTime(--controller->remaining_time);
    }

    controller->setTime(--controller->time);

    // wait for Posix threads to finish
    t_steps.join();
    t_power.join();

    emit controller->challengeFinished(controller->challenge_finished);
    std::cout << "- CHALLENGE FINISHED" << std::endl;
}

// function to monitor steps
void Challenge::monitorStepsPThread(Controller * controller) {
    static int const delay_us = 30;
    int n = 0;
    int new_value = controller->hallSensor->readValue();
    int previous_value = new_value;

    while (not controller->targetReached()
            && controller->challenge_monitor_on) {
        new_value = controller->hallSensor->readValue();

        // register step when sensor reading changes from high to low
        if ((new_value < previous_value)
            && (n * delay_us > 200)) {  // ignore rapid motion / noise
            controller->setSteps(++controller->steps);

            // calculate speed = 1 / [duration of  1 step] (rpm)
            controller->setSpeed(60.0 / (n * delay_us / 1000.0));

            if (controller->speed > controller->max_speed) {
                controller->setMaxSpeed(controller->speed);
            }

            n = 0;
        } else {
            n++;
        }

        previous_value = new_value;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_us));
    }
}

void Challenge::monitorPowerPThread(Controller * controller) {
    static int const delay_us = 300;

    while (not controller->targetReached()
            && controller->challenge_monitor_on) {
        float current = controller->wattmeter->current();  // mA

        // register only positive (generated) energy values
        if (current > 0.0) {
            // store power
            float power = controller->wattmeter->power();  // mW
            controller->setPower(power / 1000.0);  // W

            float d_energy = power / 3600.0 * delay_us / 1000.0;  // mWh
            controller->setEnergy(controller->energy + d_energy);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(delay_us));
    }
}

}  // namespace Backend
