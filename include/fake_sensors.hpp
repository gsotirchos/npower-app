#include <iostream>


using namespace std;

namespace fake_sensors {

class HallSensor {
  public:
    HallSensor();

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
