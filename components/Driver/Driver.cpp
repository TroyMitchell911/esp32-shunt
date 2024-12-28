//
// Created by troy on 2024/12/28.
//

#include "Driver.h"
#include "HAL_GPIO.h"

void Driver::Init() {
    auto &pcf8574 = Driver::PCF8574::GetInstance();

    HAL::GPIO::ExtConfig(nullptr, Driver::PCF8574::SG, &pcf8574);
}

