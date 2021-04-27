#include <iostream>


using namespace std;

namespace fake_sensors {

class HallSensor {
  public:
    HallSensor();

    int readValue() const;

    ~HallSensor();
};

class Wattmeter {
  public:
    Wattmeter();

    void sleep();
    void wake();
    void reset();
    float voltage();
    float shunt_voltage();
    float supply_voltage();
    float current();
    float power();

    ~Wattmeter();
};

}
