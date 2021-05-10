#include "fake_sensors.hpp"

#include <string>


using std::string;
using std::rand;
using std::cout;
using std::endl;
using namespace fake_sensors;


HallSensor::HallSensor(string chip, int pin) {
    (void)chip;
    (void)pin;
};

HallSensor::~HallSensor() = default;

bool HallSensor::readValue() const {
    int value = rand()%2;
    cout << "[FAKE] HALL VALUE: " << value << endl;
    return value;
}


Wattmeter::Wattmeter() = default;
Wattmeter::~Wattmeter() = default;

void Wattmeter::sleep() {
    cout << "[FAKE] SLEEP" << endl;
}

void Wattmeter::wake() {
    cout << "[FAKE] WAKE" << endl;
}

void Wattmeter::reset() {
    cout << "[FAKE] RESET" << endl;
}

float Wattmeter::voltage() {
    float voltage = 11.5 + rand()%150/100.0;
    cout << "[FAKE] VOLTAGE: " << voltage << endl;
    return voltage;
}

float Wattmeter::shunt_voltage() {
    float voltage = 0.2 + rand()%100/100.0;
    cout << "[FAKE] SHUNT_VOLTAGE: " << voltage << endl;
    return voltage;
}

float Wattmeter::supply_voltage() {
    float voltage = 11.5 + rand()%150/100.0;
    cout << "[FAKE] SUPPLY_VOLTAGE: " << voltage << endl;
    return voltage;
}

float Wattmeter::current() {
    float current = rand()%1000*1.0;
    cout << "[FAKE] CURRENT: " << current << endl;
    return current;
}

float Wattmeter::power() {
    float power = rand()%1000*1.0;
    cout << "[FAKE] POWER: " << power << endl;
    return power;
}
