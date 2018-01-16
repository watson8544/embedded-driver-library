/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2017 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "unity.h"
#include "iot_i2c_bus.h"
#include "iot_at24c02.h"

#define I2C_MASTER_SCL_IO           (gpio_num_t)21          /*!< gpio number for I2C master clock IO21*/
#define I2C_MASTER_SDA_IO           (gpio_num_t)15          /*!< gpio number for I2C master data  IO15*/
#define I2C_MASTER_NUM              I2C_NUM_1               /*!< I2C port number for master dev */
#define I2C_MASTER_TX_BUF_DISABLE   0           			/*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0           			/*!< I2C master do not need buffer */
#define I2C_MASTER_FREQ_HZ          100000      			/*!< I2C master clock frequency */

extern "C" void at24c02_obj_test()
{
    int cnt = 2;
    uint8_t ret = 0;
    CI2CBus i2c_bus(I2C_MASTER_NUM, I2C_MASTER_SCL_IO, I2C_MASTER_SDA_IO);
    CAT24C02 at24c02(&i2c_bus);

    while (cnt--) {
        /****One data Test****/
        at24c02.write_byte(0x10, 0x66);
        vTaskDelay(100 / portTICK_RATE_MS);
        at24c02.read_byte(0x10, &ret);
        printf("Value of Address 0x10 Last:%x\n", ret);

        at24c02.write_byte(0x10, 0x55);
        vTaskDelay(100 / portTICK_RATE_MS);
        at24c02.read_byte(0x10, &ret);
        printf("Value of Address 0x10:%x\n", ret);

        /***** some data Test **/
        uint8_t data[5] = { 0x10, 0x11, 0x12, 0x13, 0x14 };
        at24c02.write(0x10, sizeof(data), data);
        vTaskDelay(100 / portTICK_RATE_MS);
        at24c02.read(0x10, sizeof(data), data);
        printf("Value start Address Last0x10:%x,%x,%x,%x,%x,\n", data[0],
                data[1], data[2], data[3], data[4]);

        uint8_t data1[5] = { 0x22, 0x23, 0x24, 0x25, 0x26 };
        at24c02.write(0x10, sizeof(data1), data1);
        vTaskDelay(100 / portTICK_RATE_MS);
        at24c02.read(0x10, sizeof(data1), data1);
        printf("Value start Address 0x10:%x,%x,%x,%x,%x,\n", data1[0], data1[1],
                data1[2], data1[3], data1[4]);

    }
    printf("heap: %d\n", esp_get_free_heap_size());
}

TEST_CASE("Device at24c02 obj test", "[at24c02_cpp][iot][device]")
{
    at24c02_obj_test();
}
