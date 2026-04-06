/*******************************************************************************
 * @file    OzPOD_node.h
 * @brief   Settings and config
 *
 * @author  Percy Smith, percy.smith@colorado.edu
 * @date    June 6, 2025
 * @log     Retrofitted OzPOD_node.h from LPOD_V1.2.0 --> Particle
 *          Updated CS & LED Pins 
******************************************************************************/
#ifndef _OzPOD_node_h
#define _OzPOD_node_h

#include "Particle.h"

/****************** SENSOR CONFIGS ********************/
#define SERIAL_ENABLED        1
#define DS3231_ENABLED        1 //I2C
  #define ADJUST_DATETIME     0
#define SD_ENABLED            1 //SPI
#define ADS_ENABLED           1 //I2C
  #define AS_4CH_ENABLED      0 //Primarily for Troubleshooting
#define BME_ENABLED           1 //I2C
#define CELL_ENABLED          1

#define USE_DELAY              1

/****************** PIN DEFINITIONS ********************/
//SD Writing Definitions
#define SD_CS         3  //Default SS for SPI comms
//LED Definitions
#define BLUE_LED      2
#define RED_LED       4
#define GREEN_LED     5
//External LED Definitions
#define GREEN_EXTERNAL      7
#define RED_EXTERNAL        8

/****************** SET ADDR & CONST ********************/
#define BME_SENSOR_ADDR       (0x76)
#define SEALEVELPRESSURE_HPA  (1013.25)

const char lpodID[] = "ZPODO2";

#endif // _OzPOD_node_h
