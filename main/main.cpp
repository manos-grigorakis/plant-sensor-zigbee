#include <cmath>

#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "bh1750.hpp"

static const char* TAG = "PlantSensor";
static constexpr int LOG_INTERVAL_MS = 5'000;

extern "C" void app_main(void)
{
    // Configure I2C bus
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = GPIO_NUM_23,
        .scl_io_num = GPIO_NUM_24,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags = { 
            .enable_internal_pullup = true,
            .allow_pd = false
        },
    };

    // Create I2C bus
    i2c_master_bus_handle_t bus_handle = nullptr;

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));

    // Initialize light sensor
    BH1750 lightSensor;
    ESP_ERROR_CHECK(lightSensor.initialize(bus_handle));

    ESP_LOGI(TAG, "Plant Sensor starting...");

    while (true) {
        ESP_LOGI(TAG, "Plant Sensor running...");

        float lux = lightSensor.readlux();

        if(std::isnan(lux)) {
            ESP_LOGW(TAG, "Light measurement unavailable");
        } else {
            ESP_LOGI(TAG, "Light: %.2f lux", lux);
        }

        // Delay
        vTaskDelay(pdMS_TO_TICKS(LOG_INTERVAL_MS));
    }
}
