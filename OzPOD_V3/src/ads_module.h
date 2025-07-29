/*******************************************************************************
 * @file    ads_module.cpp
 * @brief   Splits ADS1115 code from .ino & updates from ADS1015.h --> ADS1115.h
 *
 * @cite    XPOD >> ads_module.cpp by Ajay Kandagal, ajka9053@colorado.edu
 *
 * @cite    Percy Smith, percy.smith@colorado.edu
 * @author  Spencer Hoehl, spencer.hoehl@colorado.edu
 * @date    July 29th, 2025
 * @log     Changed datatype of b4_worker and b4_aux to allow alphasense to read negative values
******************************************************************************/
#ifndef _ADS_MODULE_H
#define _ADS_MODULE_H

#include "Particle.h"
#include "Adafruit_ADS1X15.h"

#include "OzPOD_node.h"

/*! Index: MICS2611, MQ, OPEN_CHANNEL1 OPEN_CHANNEL2, B4_WORKER, VOLT_REF1, B4_AUXILIARY, VOLT_REF2, ADS_SENSOR_COUNT */
enum ads_sensor_id_e
{
  MICS = 0,
  MQ,
  OPEN_CHANNEL1,
  OPEN_CHANNEL2,
  B4_WORKER,
  VOLT_REF1,
  B4_AUXILIARY,
  VOLT_REF2,
  ADS_SENSOR_COUNT
};  //enum ads_sensor_id_e

/*! (per each sensor) addr, channel, status, module (ADS1115) */
struct ads_module_t
{
    uint8_t addr;
    int8_t channel;
    bool status;
    Adafruit_ADS1115 module;
}; //struct ads_module_t

/*! ADS data structure (ALL DATA) as uint16_t */
struct ads_data
{
  uint16_t Mics;
  uint16_t Mq;
  // uint16_t Unused;
  int16_t Worker;
  // uint16_t Unused2;
  int16_t Auxiliary;
  // uint16_t Unused3;
};  //struct ads_heaters

/*! ADS1115 to include 3 Figaros and an Alphasense B4 Sensor */
class ADS_Module {
  public:
    ADS_Module();
    bool begin();

    uint16_t read_raw(ads_sensor_id_e ads_sensor_id);
    int16_t read_b4_aux();
    int16_t read_b4_worker();
    ads_data return_updated();

  private:
    ads_module_t ads_module[ADS_SENSOR_COUNT];
};  //class ADS_Module

#endif  //_ADS_MODULE_H
