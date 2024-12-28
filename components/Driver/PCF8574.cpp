//
// Created by troy on 2024/12/27.
//

#include <esp_log.h>
#include "PCF8574.h"
#include "freertos/FreeRTOS.h"

#define TAG "[Driver::PCF8574]"

void Driver::PCF8574::Set(uint32_t num, uint8_t value) {
    num -= GPIO_BASE_NUMBER;

    if ((this->current_value & num) == value)
        return;

    if (value)
        this->current_value |= (1 << (int)num);
    else
        this->current_value &= ~((1 << int(num)));

    i2c->Write(this->current_value);
}

Driver::PCF8574::PCF8574(uint32_t addr, uint32_t scl, uint32_t sda) {
    HAL::I2C::InitBus("I2C0", (gpio_num_t)scl, (gpio_num_t)sda);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    i2c = new HAL::I2C("I2C0", 400000, addr);
    this->current_value = 0x00;
    vTaskDelay(500 / portTICK_PERIOD_MS);
    i2c->Write(this->current_value);
}

int Driver::PCF8574::SG(uint32_t num, uint8_t value, bool read, void *arg) {
    auto *pcf8574 = (Driver::PCF8574*)arg;

    if (!read) {
        pcf8574->Set(num, value);
        return 0;
    }
    return 0;
}

Driver::PCF8574 &Driver::PCF8574::GetInstance() {
    static Driver::PCF8574 pcf8574{0x20, GPIO_NUM_20, GPIO_NUM_21};

    return pcf8574;
}



