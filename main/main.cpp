#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "PlantSensor";
static constexpr int LOG_INTERVAL_MS = 5'000;

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Plant Sensor starting...");

    while (true) {
        ESP_LOGI(TAG, "Plant Sensor running...");
        vTaskDelay(pdMS_TO_TICKS(LOG_INTERVAL_MS));
    }
}
