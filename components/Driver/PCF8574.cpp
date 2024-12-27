//
// Created by troy on 2024/12/27.
//

#include <portmacro.h>
#include "PCF8574.h"
#include "freertos/FreeRTOS.h"

Driver::PCF8574::PCF8574(gpio_num_t scl, gpio_num_t sda) {
    HAL::I2C::InitBus("I2C0", scl, sda);

    i2c = new HAL::I2C("I2C0", 100000, 0x21);
    this->current_value = 0x00;
    i2c->Write(this->current_value);
}

void Driver::PCF8574::Set(pcf8574_gpio_t num, uint8_t value) {
    if ((this->current_value & num) == value)
        return;

    if (value)
        this->current_value |= (1 << (int)num);
    else
        this->current_value &= ~((1 << int(num)));

    i2c->Write(this->current_value);
}


