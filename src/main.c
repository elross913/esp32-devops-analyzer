#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"
#include "driver/uart.h"
#include "esp_log.h"

static const char *TAG = "PROTOCOL_ENGINE";

// --- Configuration des Pins ---
#define UART_TX_IO      17
#define I2C_SDA_IO      21
#define I2C_SCL_IO      22
#define SPI_MOSI_IO     23
#define SPI_SCLK_IO     18
#define SPI_CS_IO       5

// --- Prototypes des tâches ---
void task_uart(void *pvParameters);
void task_i2c(void *pvParameters);
void task_spi(void *pvParameters);

// --- Initialisation Hardware ---
void init_hardware() {
    // UART Setup
    uart_config_t uart_cfg = { .baud_rate = 115200, .data_bits = UART_DATA_8_BITS, .parity = UART_PARITY_DISABLE, .stop_bits = UART_STOP_BITS_1, .flow_ctrl = UART_HW_FLOWCTRL_DISABLE };
    uart_param_config(UART_NUM_2, &uart_cfg);
    uart_set_pin(UART_NUM_2, UART_TX_IO, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_2, 1024, 0, 0, NULL, 0);

    // I2C Setup
    i2c_config_t i2c_cfg = { .mode = I2C_MODE_MASTER, .sda_io_num = I2C_SDA_IO, .scl_io_num = I2C_SCL_IO, .sda_pullup_en = 1, .scl_pullup_en = 1, .master.clk_speed = 100000 };
    i2c_param_config(I2C_NUM_0, &i2c_cfg);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);

    // SPI Setup
    spi_bus_config_t buscfg = { .miso_io_num = -1, .mosi_io_num = SPI_MOSI_IO, .sclk_io_num = SPI_SCLK_IO, .quadwp_io_num = -1, .quadhd_io_num = -1 };
    spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
}

void app_main(void) {
    ESP_LOGI(TAG, "Démarrage du Kernel FreeRTOS...");
    init_hardware();

    // Création des tâches
    // xTaskCreate(Fonction, Nom, Pile(Stack), Paramètres, Priorité, Handle)
    
    xTaskCreate(task_uart, "UART_Task", 3072, NULL, 5, NULL);
    xTaskCreate(task_i2c,  "I2C_Task",  3072, NULL, 4, NULL);
    xTaskCreate(task_spi,  "SPI_Task",  3072, NULL, 6, NULL); // Priorité la plus haute

    ESP_LOGI(TAG, "Système prêt. Observez l'analyseur logique !");
}

// --- Implémentation des Tâches ---

void task_uart(void *pvParameters) {
    char msg[64];
    int count = 0;
    while(1) {
        sprintf(msg, "[UART] Tick %d\r\n", count++);
        uart_write_bytes(UART_NUM_2, msg, strlen(msg));
        vTaskDelay(pdMS_TO_TICKS(500)); // 500ms
    }
}

void task_i2c(void *pvParameters) {
    uint8_t data = 0x00;
    while(1) {
        // Simule l'écriture vers un capteur à l'adresse 0x42
        i2c_master_write_to_device(I2C_NUM_0, 0x42, &data, 1, 100 / portTICK_PERIOD_MS);
        data++;
        vTaskDelay(pdMS_TO_TICKS(200)); // 200ms (plus rapide que l'UART)
    }
}

void task_spi(void *pvParameters) {
    spi_device_handle_t spi;
    spi_device_interface_config_t devcfg = { .clock_speed_hz = 1000000, .mode = 0, .spics_io_num = SPI_CS_IO, .queue_size = 7 };
    spi_bus_add_device(SPI2_HOST, &devcfg, &spi);

    uint8_t tx_buf[2] = {0xBE, 0xEF};
    while(1) {
        spi_transaction_t t = { .length = 16, .tx_buffer = tx_buf };
        spi_device_transmit(spi, &t);
        vTaskDelay(pdMS_TO_TICKS(100)); // 100ms (très fréquent)
    }
}