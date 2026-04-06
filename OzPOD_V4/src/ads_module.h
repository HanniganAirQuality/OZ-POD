/*******************************************************************************
 * @file    ads_module.cpp
 * @brief   Splits ADS1115 code from .ino & updates from ADS1015.h --> ADS1115.h
 *
 * @cite    XPOD >> ads_module.cpp by Ajay Kandagal, ajka9053@colorado.edu
 *
 * @author  Spencer Hoehl, spencer.hoehl@colorado.edu
 * @editor  Percy Smith, percy.smith@colorado.edu
 * @date    April 03, 2026
 * @log     Added storage for datapoints to troubleshoot alphasense footprint
******************************************************************************/
#ifndef _ADS_MODULE_H
#define _ADS_MODULE_H

#include "Particle.h"
#include "Adafruit_ADS1X15.h"

#include "OzPOD_node.h"

/*! Index: MICS2611, MQ, OPEN_CHANNEL1 OPEN_CHANNEL2, B4_WORKER, VOLT_WORK, VOLT_REF1, B4_AUXILIARY, VOLT_AUX, VOLT_REF2, ADS_SENSOR_COUNT */
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
  int16_t Worker;
  int16_t Auxiliary;
  #if AS_4CH_ENABLED
    uint16_t Volt_Work;
    uint16_t Volt_Ref1;
    uint16_t Volt_Aux;
    uint16_t Volt_Ref2;
  #endif //AS_4CH_ENABLED;
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