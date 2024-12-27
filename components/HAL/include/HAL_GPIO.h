//
// Created by troy on 2024/8/12.
//

#ifndef RELAY_HAL_GPIO_H
#define RELAY_HAL_GPIO_H

#include <cstdint>
#include <soc/gpio_num.h>

namespace HAL{
#define GPIO_USE_GROUP          0
#define GPIO_USE_SPEED          0

    class GPIO {
    public:
        struct gpio_cfg;

        typedef int (*gpio_opf)(uint32_t num, uint8_t value, bool read, void *arg);
        typedef int (*gpio_init_opf)(struct gpio_cfg *cfg);

        typedef enum {
            GPIO_DISABLE,
            GPIO_INPUT,
            GPIO_OUTPUT,
            GPIO_BOTH,
        }gpio_direction_t;

        typedef enum{
            GPIO_PP,
            GPIO_OD,
        }gpio_mode_t;

        typedef enum {
            GPIO_STATE_LOW,
            GPIO_STATE_HIGH,
            GPIO_STATE_NONE,
        }gpio_state_t;

        typedef struct gpio_cfg{
#if GPIO_USE_GROUP
            uint32_t group;
#endif
            uint32_t pin;
            gpio_direction_t direction;
            gpio_mode_t mode;
            uint8_t pull_up : 1;
            uint8_t pull_down : 1;
#if GPIO_USE_SPEED
            uint32_t speed;
#endif
            gpio_opf op;
            void *op_arg;
        }gpio_cfg_t;

    private:
        bool inited;
        gpio_cfg_t cfg{};
        gpio_state_t state;

    public:
#if GPIO_USE_GROUP
        GPIO(uint32_t group, uint32_t pin);
#else
        GPIO(uint32_t pin);
#endif
        explicit GPIO(HAL::GPIO::gpio_cfg_t gpiocfg);
        /* For ext GPIO */
        GPIO(uint32_t pin, gpio_init_opf init, gpio_opf op, void *op_arg);

        ~GPIO();

    public:
        HAL::GPIO::gpio_cfg_t GetConfig();
        void Reconfigure(HAL::GPIO::gpio_cfg_t gpiocfg);
        void Set(HAL::GPIO::gpio_state_t gpio_state);
        HAL::GPIO::gpio_state_t Get();
    };
}


#endif //RELAY_HAL_GPIO_H
