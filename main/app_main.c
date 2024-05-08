/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_err.h"

#include "esp_at.h"
#include "esp_at_init.h"
#include "driver/gpio.h"

#define GPIO_OUTPUT_IO_0    0
#define GPIO_OUTPUT_IO_1    1
#define GPIO_OUTPUT_IO_4    4
#define GPIO_OUTPUT_IO_5    5
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1) | (1ULL<<GPIO_OUTPUT_IO_4) | (1ULL<<GPIO_OUTPUT_IO_5))
/*
 * Let's say, GPIO_OUTPUT_IO_0=18, GPIO_OUTPUT_IO_1=19
 * In binary representation,
 * 1ULL<<GPIO_OUTPUT_IO_0 is equal to 0000000000000000000001000000000000000000 and
 * 1ULL<<GPIO_OUTPUT_IO_1 is equal to 0000000000000000000010000000000000000000
 * 1ULL<<GPIO_OUTPUT_IO_4 is equal to 0000000000000000010000000000000000000000
 * 1ULL<<GPIO_OUTPUT_IO_5 is equal to 0000000000000000100000000000000000000000
 * GPIO_OUTPUT_PIN_SEL                0000000000000000110011000000000000000000
 * */
void app_main(void)
{

    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
    gpio_set_level(GPIO_OUTPUT_IO_0, 1);
    gpio_set_level(GPIO_OUTPUT_IO_1, 0);
    gpio_set_level(GPIO_OUTPUT_IO_4, 1);
    gpio_set_level(GPIO_OUTPUT_IO_5, 0);
    esp_at_main_preprocess();

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_at_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    esp_at_init();
    gpio_set_level(GPIO_OUTPUT_IO_1, 1);
}
