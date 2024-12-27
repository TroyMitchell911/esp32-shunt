//
// Created by troy on 2024/12/27.
//

#ifndef SHUNT_HAL_I2C_H
#define SHUNT_HAL_I2C_H

#include <driver/gpio.h>
#include <driver/i2c_master.h>
#include <list>

namespace HAL {
    class I2C {
    private:

        i2c_master_bus_handle_t bus_handle = nullptr;

    public:
        I2C(const char *bus_name, uint32_t scl_freq, uint16_t device_addr);
        static void InitBus(const char *name, gpio_num_t scl, gpio_num_t sda);

        void Write(uint8_t byte);

        void Write(uint16_t half_word);

        void Write(uint32_t word);

        void Write(uint8_t *buffer, uint32_t size);

        void Read(uint8_t *buffer, uint32_t size);


        typedef struct {
            i2c_master_bus_handle_t handle;
            const char *name;
            gpio_num_t scl;
            gpio_num_t sda;
        }i2c_bus_t;
        i2c_master_dev_handle_t dev = nullptr;
    };
}



#endif //SHUNT_HAL_I2C_H
