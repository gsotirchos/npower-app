#include "backend.hpp"

#include <iostream>
#include <thread>


namespace Backend {


Challenge::Challenge() {
    try {
        hallSensor = std::make_unique<SENSORS::HallSensor>();
    } catch(std::system_error) {
       std::cout << "Error: Failed to initialize Hall Effect sensor.\n";
    }

    try {
        wattmeter = std::make_unique<SENSORS::Wattmeter>();
    } catch(std::exception) {
       std::cout << "Error: Failed to initialize wattmeter.\n";
    }

    std::cout << "CHALLENGE INITIALIZED\n";
}

void Challenge::setRemainingTime(int value) {
    remaining_time = value;
    //emit remainingTimeChanged(value);
}

void Challenge::startChallenge() {
    // start challenge in background
    //std::thread t(Challenge::challengeThread);
    //t.detach();
    std::cout << "START CHALLENGE\n";
}

void Challenge::challengeThread(){
    const static int delay_s = 1;

    //std::thread t_steps(Challenge::recordStepsThread);
    //std::thread t_power(Challenge::recordPowerThread);

    while (remaining_time >= 0) {
        std::this_thread::sleep_for(std::chrono::seconds(delay_s));

        remaining_time--;
        //emit remainingTimeChanged(remaining_time);

        time++;
        //emit timeChanged(time);
    }
}

void Challenge::recordStepsThread() {
    const static int delay_us = 30;
    int n = 0;
    int current_value = hallSensor->readValue();
    int previous_value = current_value;

    while (true) {
        current_value = hallSensor->readValue();

        if ((current_value < previous_value)
            && (n*delay_us > 200)) {  // ignore rapid reciprocal motion
            steps++;
            //emit stepsChanged(steps);

            speed = 60.0/(n*delay_us/1000.0);  // rpm
            //emit speedChanged(speed);
            n = 0;
        } else {
            n++;
        }

        previous_value = current_value;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_us));
    }
}

void Challenge::recordPowerThread() {
    while (true) {
        power = wattmeter->power();
        //emit powerChanged(power);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}


Sensors::Sensors() {
    try {
        hallSensor = std::make_unique<SENSORS::HallSensor>();
    } catch(std::system_error) {
       std::cout << "Error: Failed to initialize Hall Effect sensor.\n";
    }

    try {
        wattmeter = std::make_unique<SENSORS::Wattmeter>();
    } catch(std::exception) {
       std::cout << "Error: Failed to initialize wattmeter.\n";
    }

    std::cout << "SENSORS INITIALIZED\n";
}

Sensors::~Sensors() {
    std::cout << "SENSORS DESTROYED\n";
}

bool Sensors::hallSensorValue() {
    return hallSensor->readValue();
}

float Sensors::wattmeterVoltage() {
    return wattmeter->voltage();
}

float Sensors::wattmeterCurrent() {
    return wattmeter->current();
}

float Sensors::wattmeterPower() {
    return wattmeter->power();
}

void Sensors::setWattmeterState(std::string state) {
    if (state == "sleep") {
        wattmeter->sleep();
    } else if (state == "wake") {
        wattmeter->wake();
    } else {
        wattmeter->reset();
    }
}

}  // namespace Backend
