#include "fake_sensors.hpp"

#include <string>


using std::string;
using std::rand;
using std::cout;
using std::endl;
using namespace fake_sensors;


HallSensor::HallSensor() = default;
HallSensor::~HallSensor() = default;

bool HallSensor::readValue() const {
    int value = rand()%2;
    cout << "HALL VALUE: " << value << endl;
    return value;
}


Wattmeter::Wattmeter() = default;
Wattmeter::~Wattmeter() = default;

void Wattmeter::sleep() {
    cout << "SLEEP" << endl;
}

void Wattmeter::wake() {
    cout << "WAKE" << endl;
}

void Wattmeter::reset() {
    cout << "RESET" << endl;
}

float Wattmeter::voltage() {
    float voltage = 12.5 + rand()%100/100.0;
    cout << "VOLTAGE: " << voltage << endl;
    return voltage;
}

float Wattmeter::shunt_voltage() {
    float voltage = 0.5 + rand()%100/100.0;
    cout << "SHUNT_VOLTAGE: " << voltage << endl;
    return voltage;
}

float Wattmeter::supply_voltage() {
    float voltage = 12.5 + rand()%100/100.0;
    cout << "SUPPLY_VOLTAGE: " << voltage << endl;
    return voltage;
}

float Wattmeter::current() {
    float current = rand()%100/100.0;
    cout << "CURRENT: " << current << endl;
    return current;
}

float Wattmeter::power() {
    float power = rand()%10;
    cout << "POWER: " << power << endl;
    return power;
}
