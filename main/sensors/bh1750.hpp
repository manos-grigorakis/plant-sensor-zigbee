#pragma once

#include <cstdint>
#include "driver/i2c_master.h"

class BH1750 {
    public:
        BH1750() = default;
        ~BH1750(); // Destructor

        // Disable copying
        BH1750(const BH1750&) = delete;
        BH1750& operator=(const BH1750&) = delete;

        esp_err_t initialize(i2c_master_bus_handle_t bus_handle);
        float readlux();

    private:
        static constexpr uint8_t CMD_ONE_TIME_HIGH_RES = 0x20;

        i2c_master_dev_handle_t device_handle_ = nullptr;
};