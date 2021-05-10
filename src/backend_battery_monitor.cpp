#include "backend.hpp"

#include <iostream>
//#include <algorithm>
#include <thread>


using std::min;
using std::max;


namespace Backend {

//// BatteryMonitor class ////
BatteryMonitor::BatteryMonitor() {
    std::cout << "- BATTERY MONITOR CREATED" << std::endl;
}

BatteryMonitor::~BatteryMonitor() {
    std::cout << "- BATTERY MONITOR DESTROYED" << std::endl;
}

void BatteryMonitor::start(Controller * controller) {
    static int const delay_s = 2;
    static float const max_oc_voltage = 13.4;
    static float const min_oc_voltage = 11.8;
    static float const max_oc_current = 0;
    static float const min_oc_current = -700;
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
                100 * (voltage - min_oc_voltage)
                    /(max_oc_voltage - min_oc_voltage);
            percentage = min(max(0, percentage), 100);
            controller->charge_percentage = percentage;

            emit controller->chargePercentageChanged(controller->charge_percentage);
            std::cout << "- BATTERY CHARGE: " << controller->charge_percentage
                << "%" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(delay_s));
    }
}

}  // namespace Backend
