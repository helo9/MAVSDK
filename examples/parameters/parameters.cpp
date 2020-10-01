/**
 * @file offboard_velocity.cpp
 * @brief Example that demonstrates usage of parameter plugin.
 *
 * @authors Author: Jonathan Hahn <>
 * 
 */

#include <iostream>
#include <chrono>
#include <future>
#include <thread>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/param/param.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

using namespace std;
using namespace mavsdk;
using namespace std::chrono;
using namespace std::this_thread;

int main()
{
    Mavsdk dc;

    {
        auto prom = std::make_shared<std::promise<void>>();
        auto future_result = prom->get_future();

        std::cout << "Waiting to discover system..." << std::endl;
        dc.register_on_discover([prom](uint64_t uuid) {
            std::cout << "Discovered system with UUID: " << uuid << std::endl;
            prom->set_value();
        });

        auto connection_result = dc.add_serial_connection("/dev/ttyACM0");

        if (connection_result != ConnectionResult::Success) {
            std::cout << "Connection failed" << std::endl;
            return 1;
        }

        future_result.get();
    }

    dc.register_on_timeout([](uint64_t uuid) {
        std::cout << "System with UUID timed out: " << uuid << std::endl;
        std::cout << "Exiting." << std::endl;
        exit(0);
    });

    System& system = dc.system();
    auto param = std::make_shared<Param>(system);
    auto telemetry = std::make_shared<Telemetry>(system);

    /*while (!telemetry->health_all_ok()) {
        std::cout << "Waiting for system to be ready" << std::endl;
        sleep_for(seconds(1));
    }*/

    auto tmp = param->get_all_params();

    cout << "Integer Parameters:" << std::endl;
    for (auto param : tmp.int_params) {
        std::cout << param.name << " : " << param.value << std::endl;
    }

    cout << "Float Parameters:" << std::endl;
    for (auto param : tmp.float_params) {
        std::cout << param.name << " : " << param.value << std::endl;
    }
    
    cout << "Test" << endl;
    return 0;
}