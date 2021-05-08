#include <iostream>
#include <cstdlib>
#include <ctime>


using namespace std;

namespace fake_sensors {

class HallSensor {
  public:
    HallSensor(string chip = "", int pin = 0);

    bool readValue() const;

    ~HallSensor();
};

class Wattmeter {
  public:
    Wattmeter();

    float voltage();
    float shunt_voltage();
    float supply_voltage();
    float current();
    float power();
    void sleep();
    void wake();
    void reset();

    ~Wattmeter();
};

}
