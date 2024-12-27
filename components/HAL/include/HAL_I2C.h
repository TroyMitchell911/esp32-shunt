//
// Created by troy on 2024/12/27.
//

#ifndef SHUNT_HAL_I2C_H
#define SHUNT_HAL_I2C_H

#include <driver/gpio.h>
#include <driver/i2c_master.h>

namespace HAL {
    class I2C {
    private:
        i2c_master_bus_handle_t bus_handle = nullptr;
        i2c_master_dev_handle_t dev = nullptr;
    public:
        I2C(uint32_t scl_freq, uint16_t device_addr);
        void InitBus(void *i2c, gpio_num_t scl, gpio_num_t sda);

        void Write(uint8_t byte);

        void Write(uint16_t half_byte);

        void Write(uint32_t word);

        void Write(uint8_t *buffer, uint32_t size);

        void Read(uint8_t *buffer, uint32_t size);
    };
}



#endif //SHUNT_HAL_I2C_H
