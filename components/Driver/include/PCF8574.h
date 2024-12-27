//
// Created by troy on 2024/12/27.
//

#ifndef SHUNT_PCF8574_H
#define SHUNT_PCF8574_H

#include <soc/gpio_num.h>
#include "HAL_I2C.h"

#define GPIO_BASE_NUMBER    50

namespace Driver {
    class PCF8574 {
    public:
        typedef enum {
            GPIO_P0 = GPIO_BASE_NUMBER,
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
        PCF8574(uint32_t scl, uint32_t sda);
        void Set(uint32_t num, uint8_t value);
        static int SG(uint32_t num, uint8_t value, bool read, void *arg);
    };
}



#endif //SHUNT_PCF8574_H
