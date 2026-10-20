#include "bh1750.hpp"

#include <cmath>
#include <cstdint>

#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "BH1750";

// Destructor
BH1750::~BH1750() {
    // Release BH1750 device resources
    if (device_handle_ != nullptr) {
        ESP_ERROR_CHECK(i2c_master_bus_rm_device(device_handle_));
    }
}

esp_err_t BH1750::initialize(i2c_master_bus_handle_t bus_handle) {
    // Configure BH1750 device
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x23,
        .scl_speed_hz = 100000,
        .scl_wait_us = 0,
    };

    // Register BH1750 on the I2C bus
    return i2c_master_bus_add_device(bus_handle, &dev_cfg, &device_handle_);
}

float BH1750::readlux() {
    // Start one time high resolution measurement
    esp_err_t err = i2c_master_transmit(
        device_handle_, &CMD_ONE_TIME_HIGH_RES, 1, 1000);

    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start measurement: %s", esp_err_to_name(err));
        return NAN;
    }

    // Wait for the measurement
    vTaskDelay(pdMS_TO_TICKS(200));

    uint8_t data[2] = {};

    // Read data of measurement
    err = i2c_master_receive(device_handle_, data, sizeof(data), 1000);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read measurement: %s", esp_err_to_name(err));
        return NAN;
    }

    // Combine the two 8-bit bytes into one 16-bit value
    uint16_t raw = (static_cast<uint16_t>(data[0]) << 8) | data[1];

    // Convert raw sensor metrics to lux
    return raw / 1.2f;
}