//
// Created by troy on 2024/12/27.
//

#ifndef SHUNT_PCF8574_H
#define SHUNT_PCF8574_H

#include <soc/gpio_num.h>
#include "HAL_I2C.h"

namespace Driver {
    class PCF8574 {
    public:
        typedef enum {
            GPIO_P0,
            GPIO_P1,
            GPIO_P2,
            GPIO_P3,
            GPIO_P4,
            GPIO_P5,
            GPIO_P6,
            GPIO_P7
        } pcf8574_gpio_t;
    private:
        HAL::I2C *i2c;
        uint8_t current_value;
    public:
        PCF8574(gpio_num_t scl, gpio_num_t sda);
        void Set(pcf8574_gpio_t num, uint8_t value);
    };
}



#endif //SHUNT_PCF8574_H
