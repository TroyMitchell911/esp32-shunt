//
// Created by troy on 2024/12/27.
//

#include <vector>
#include <cstring>
#include "include/HAL_I2C.h"
#include "esp_check.h"

#define TAG "[HAL::I2C]"

static std::list<HAL::I2C::i2c_bus_t> buses;;

HAL::I2C::I2C(const char *bus_name, uint32_t scl_freq, uint16_t device_addr) {
    esp_err_t ret = ESP_OK;
    i2c_device_config_t i2c_dev_conf{};
    i2c_bus_t *bus = nullptr;

    for (auto & it : buses) {
        if (strcmp(it.name, bus_name) == 0)
            bus = &it;
    }

    if (!bus) {
        ESP_LOGE(TAG, "Can't find [%s] i2c bus", bus_name);
        return;
    }

    this->bus_handle = bus->handle;

    i2c_dev_conf.scl_speed_hz = scl_freq,
    i2c_dev_conf.device_address = device_addr;

    ret = i2c_master_bus_add_device(this->bus_handle,&i2c_dev_conf, &this->dev);

    if (ret != ESP_OK && this->dev) {
        i2c_master_bus_rm_device(this->dev);
        return;
    }
}

void HAL::I2C::Write(uint8_t byte) {
    std::vector<uint8_t> buffer = {byte};
    i2c_master_transmit(this->dev, buffer.data(), buffer.size(), -1);
}

void HAL::I2C::Write(uint16_t half_byte) {
    std::vector<uint8_t> buffer(sizeof(half_byte));
    std::memcpy(buffer.data(), &half_byte, sizeof(half_byte));  // 按字节复制
    i2c_master_transmit(this->dev, buffer.data(), buffer.size(), -1);
}

void HAL::I2C::Write(uint32_t word) {
    std::vector<uint8_t> buffer(sizeof(word));
    std::memcpy(buffer.data(), &word, sizeof(word));  // 按字节复制
    i2c_master_transmit(this->dev, buffer.data(), buffer.size(), -1);
}

void HAL::I2C::Write(uint8_t *buffer, uint32_t size) {
    i2c_master_transmit(this->dev, buffer, size, -1);
};

void HAL::I2C::Read(uint8_t *buffer, uint32_t size) {
    i2c_master_receive(this->dev, buffer, size, -1);
}

void HAL::I2C::InitBus(const char *name, gpio_num_t scl, gpio_num_t sda) {
    i2c_master_bus_config_t i2c_bus_config{};
    i2c_bus_t bus;

    for (auto & it : buses) {
        if ((it.scl == scl || it.sda == sda) || strcmp(name, it.name) == 0) {
            ESP_LOGE(TAG, "[%s] has been exist", name);
            return;
        }
    }

    bus.sda = sda;
    bus.scl = scl;
    bus.name = name;
    bus.handle = nullptr;

    i2c_bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
    i2c_bus_config.i2c_port = -1;
    i2c_bus_config.scl_io_num = scl;
    i2c_bus_config.sda_io_num = sda;
    i2c_bus_config.glitch_ignore_cnt = 7;

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &bus.handle));

    buses.push_back(bus);
}

