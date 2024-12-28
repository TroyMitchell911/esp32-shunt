/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <nvs_flash.h>
#include <soc/gpio_num.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <esp_log.h>
#include <mqtt_client.h>
#include <esp_ota_ops.h>
#include "cJSON.h"
#include "freertos/FreeRTOS.h"
#include "HAL_WiFi.h"
#include "HAL_MQTT.h"
#include "HomeAssistant.h"
#include "Switch.h"
#include "HAL_GPIO.h"
#include "HAL.h"
#include "BinarySensor.h"
#include "Sensor.h"
#include "Driver.h"

#define TAG "[main]"

static HAL::MQTT *mqtt;
static App::Switch *sw_12v_1;
static App::Switch *sw_12v_2;
static App::Switch *sw_12v_3;
static App::Switch *sw_5v_1;
static App::Switch *io4;
static App::Switch *io5;
static App::Switch *io1;

static void mqtt_event(HAL::MQTT::event_t event, void *data, void *arg) {
    if(event == HAL::MQTT::EVENT_CONNECTED) {
        sw_12v_1->Init();
        sw_12v_2->Init();
        sw_12v_3->Init();
        sw_5v_1->Init();
        io4->Init();
        io5->Init();
        io1->Init();
    }
}
#include "ca_emqx_troy_home.crt"
static void wifi_event(HAL::WiFiMesh::event_t event_id, void *event_data, void *arg) {
    if(event_id == HAL::WiFiMesh::EVENT_GOT_IP) {
        mqtt->BindingCallback(mqtt_event, HAL::MQTT::EVENT_CONNECTED, nullptr);
        mqtt->Start();
    } else if(event_id == HAL::WiFiMesh::EVENT_CONNECTED) {
        delete mqtt;
        sw_12v_1->Init();
        sw_12v_2->Init();
        sw_12v_3->Init();
        sw_5v_1->Init();
        io4->Init();
        io5->Init();
        io1->Init();
    }
}

extern "C" {
__attribute__((noreturn)) void app_main(void) {

    ESP_LOGI(TAG, "This a iot relay device");

    HAL::Init();
    Driver::Init();

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
    HAL::WiFiMesh &mesh = HAL::WiFiMesh::GetInstance();
    sw_12v_1 = new App::Switch(&mesh, CONFIG_ESP32_SHUNT_WHERE,
                         CONFIG_12V_1_DEVICE_NAME,
                         10,
                         1);
    sw_12v_2 = new App::Switch(&mesh, CONFIG_ESP32_SHUNT_WHERE,
                               CONFIG_12V_2_DEVICE_NAME,
                               7,
                               1);
    sw_12v_3 = new App::Switch(&mesh, CONFIG_ESP32_SHUNT_WHERE,
                               CONFIG_12V_3_DEVICE_NAME,
                               6,
                               1);
    sw_5v_1 = new App::Switch(&mesh, CONFIG_ESP32_SHUNT_WHERE,
                               CONFIG_5V_1_DEVICE_NAME,
                               3,
                               1);

    io4 = new App::Switch(&mesh, CONFIG_ESP32_SHUNT_WHERE,
                              CONFIG_IO4_DEVICE_NAME,
                              4,
                              1);

    io5 = new App::Switch(&mesh, CONFIG_ESP32_SHUNT_WHERE,
                          CONFIG_IO5_DEVICE_NAME,
                          5,
                          1);

    io1 = new App::Switch(&mesh, CONFIG_ESP32_SHUNT_WHERE,
                          CONFIG_IO1_DEVICE_NAME,
                          1,
                          1);
    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

    mesh.BindingCallback(wifi_event, nullptr);
    HAL::WiFiMesh::cfg_t mesh_cfg{};
    mesh_cfg.max_connections = CONFIG_MESH_AP_CONNECTIONS;
    mesh_cfg.mesh_ap_pwd = CONFIG_MESH_AP_PASSWD;
    mesh_cfg.router_ssid = CONFIG_WIFI_SSID;
    mesh_cfg.router_pwd = CONFIG_WIFI_PASSWORD;
    mesh_cfg.mesh_channel = CONFIG_MESH_CHANNEL;
    mesh_cfg.max_layer = CONFIG_MESH_MAX_LAYER;
    for(unsigned char & i : mesh_cfg.mesh_id) {
        i = 0x77;
    }
    mqtt = new HAL::MQTT(CA_EMQX_TROY_HOME_URI,
                         CA_EMQX_TROY_HOME_USER,
                         CA_EMQX_TROY_HOME_PWD,
                         ca_emqx_troy_home);
    ESP_LOGI(TAG, "mqtt client pointer: %p", mqtt);
    mesh.SetMQTT(mqtt);
    mesh.Start(&mesh_cfg);

    for(;;){
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
}