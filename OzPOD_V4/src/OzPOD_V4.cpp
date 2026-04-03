/*******************************************************************************
 * @file    OzPOD_V4.cpp
 * @brief   Central Firmware
 * 
 * @version V4
 *
 * @cite   LPOD_Particle_V7 
 * 
 * @author  Spencer Hoehl, Spencer.Hoehl@colorado.edu
 * @editor  Percy Smith, percy.smith@colorado.edu
 * @date    April 2, 2026
 * @log     Experimenting with updated ads sensing for alphasense sensor
******************************************************************************/
// Include Particle Device OS APIs
#include "Particle.h"
#include "OzPOD_node.h"
#include "OzPOD_DataStorage.h"

#include "RTClib.h"
#include "Particle-SdFat/SdFat.h"
#include "ads_module.h"
#include "Adafruit_BME680.h" // it gotta be him--> "" 

// Let Device OS manage the connection to the Particle Cloud
#if CELL_ENABLED
  SYSTEM_MODE(AUTOMATIC); //SEMI_AUTOMATIC "w/threading is recommended combination"
#else
  SYSTEM_MODE(MANUAL);
#endif
// Run the application and system concurrently in separate threads
// SYSTEM_THREAD(ENABLED); // Enabled by default in new updates --> commented out

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
// SerialLogHandler logHandler(LOG_LEVEL_INFO);

#if USE_DELAY
  const std::chrono::milliseconds logPeriod = 10s;
  unsigned long lastLog = 0;
#endif

#if SD_ENABLED
  SdFat sd;
  SdFile file;
  char fileName[24];
#endif  //SD_ENABLED
#if DS3231_ENABLED 
  RTC_DS3231 ds3231;  //DO NOT NAME THIS RTC IT THINKS ITS A MACRO !!!
  char bufftime[25];
  int Y,M,D,h,m,s;
#endif //DS3231_ENABLED

#if ADS_ENABLED
  ADS_Module ads_module;
  ads_data ads;
#endif
#if BME_ENABLED
  Adafruit_BME680 bme680;
#endif

#if CELL_ENABLED
  const std::chrono::milliseconds cellPushPeriod = 20s;
  // unsigned long lastLog;
  // The event name to publish with
  const int char_limit = 864;
  const char *eventName = "wireless-test";
  char buff[char_limit] = ""; 
  int i = 0;

  OZPOD_data ozpoddata;
#endif

/***************************************************************************************/
// setup() runs once, when the device is first turned on
void setup() {
  #if SERIAL_ENABLED
    Serial.begin(9600);
  #endif

  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  // pinMode(GREEN_EXTERNAL, OUTPUT);
  // pinMode(RED_EXTERNAL, OUTPUT);

  #if DS3231_ENABLED
    ds3231.begin();
    #if ADJUST_DATETIME
      ds3231.adjust(DateTime(F(__DATE__), F(__TIME__)));
    #endif //ADJUST_DATETIME
  #endif //DS3231_ENABLED
  
  #if ADS_ENABLED
    ads_module.begin();
  #endif  //ADS_ENABLED

  #if BME_ENABLED    
    bme680.begin(BME_SENSOR_ADDR);
    if (!bme680.begin(BME_SENSOR_ADDR)) {
      Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
      while (1);
    } //if(!bme680.begin(BME680_CS))
    //Set up oversampling an initialize filter
    bme680.setTemperatureOversampling(BME680_OS_8X);
    bme680.setHumidityOversampling(BME680_OS_2X);
    bme680.setPressureOversampling(BME680_OS_4X);
    bme680.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme680.setGasHeater(320, 150);
  #endif  //BME_ENABLED
  #if SD_ENABLED
    digitalWrite(SD_CS, LOW);
    sd.begin(SD_CS);
    while(!sd.begin(SD_CS)){
      #if SERIAL_ENABLED
        Serial.println(F("Insert SD Card to begin!"));
        digitalWrite(RED_LED, HIGH);
        // digitalWrite(GREEN_EXTERNAL, LOW);
        // digitalWrite(RED_EXTERNAL, HIGH);
        delay(100);
      #endif  //SERIAL_ENABLED
      sd.begin(SD_CS);
    }
    DateTime now = ds3231.now();
      Y = now.year();
      M = now.month();
      D = now.day();
    sprintf(fileName, "%s_%04u_%02u_%02u.TXT", ozpodID, Y, M, D);
      delay(100);
    file.open(fileName, O_CREAT | O_APPEND | O_WRITE);    
    digitalWrite(RED_LED, LOW);
    // digitalWrite(RED_EXTERNAL, LOW);
    // digitalWrite(GREEN_LED, HIGH);
    #if SERIAL_ENABLED
      Serial.println(fileName);
    #endif //SERIAL_ENABLED
    file.close();
    digitalWrite(SD_CS, HIGH);
    digitalWrite(GREEN_LED, LOW);
  #endif //SD_ENABLED
}

/***************************************************************************************/
// loop() runs over and over again, as quickly as it can execute.
void loop() {
  #if DS3231_ENABLED
    DateTime now = ds3231.now();
    Y = now.year();  M = now.month();  D = now.day();  h = now.hour();  m = now.minute();  s = now.second();
    sprintf(bufftime, "%04u-%02u-%02uT%02u:%02u:%02u", Y, M, D, h, m, s);
  #endif  //DS3231_ENABLED
  
  #if ADS_ENABLED
    ads = ads_module.return_updated();
    // Serial.println(F("you did it"));
  #endif  //ADS_ENABLED

  #if BME_ENABLED
    if(!bme680.performReading())  {
      Serial.println(F("Failed to perform reading :("));
      return;
    }
  #endif  //BME_ENABLED

  if (millis() - lastLog >= logPeriod.count())
  {
    lastLog = millis();
    #if SD_ENABLED
    digitalWrite(SD_CS, LOW);
    sd.begin(SD_CS);
    while(!sd.begin(SD_CS)){
      #if SERIAL_ENABLED
        Serial.println("Cannot find SD Card!");
        digitalWrite(RED_LED, HIGH);
        // digitalWrite(GREEN_EXTERNAL, LOW);
        // digitalWrite(RED_EXTERNAL, HIGH);
        delay(100);
      #endif
      sd.begin(SD_CS);
    } 
    digitalWrite(RED_LED, LOW);
    // digitalWrite(RED_EXTERNAL, LOW);
    if(sd.begin(SD_CS)){
      delay(100);
      file.open(fileName, O_CREAT | O_APPEND | O_WRITE);
      delay(1000);
      digitalWrite(GREEN_LED, HIGH);
      // digitalWrite(RED_EXTERNAL, LOW);
      // digitalWrite(GREEN_EXTERNAL, HIGH);

      delay(100);
      file.println();
      #if DS3231_ENABLED
        file.print(bufftime);
        file.print(F(","));
        delay(100);
      #endif  //DS3231_ENABLED
      
      #if ADS_ENABLED
        file.print(ads.Mics); 
        file.print(F(","));
        delay(100);
        file.print(ads.Mq); 
        file.print(F(","));
        delay(100);
        file.print(ads.Worker); 
        file.print(F(","));
        file.print(ads.Volt_Work); 
        file.print(F(","));
        file.print(ads.Volt_Ref1); 
        file.print(F(","));
        delay(100);
        file.print(ads.Auxiliary); 
        file.print(F(","));
        file.print(ads.Volt_Aux); 
        file.print(F(","));
        file.print(ads.Volt_Ref2); 
        file.print(F(","));
        delay(100);
      #endif  //ADS_ENABLED

      #if BME_ENABLED
        file.print(bme680.readTemperature());
        file.print(F(","));
        file.print(bme680.readPressure());
        file.print(F(","));
        file.print(bme680.readHumidity());
        file.print(F(","));
        file.print(bme680.readGas());
        file.print(F(","));
        delay(100);
      #endif  //BME_ENABLED

      file.sync();
      delay(100);
      file.close();
      delay(100);
      digitalWrite(GREEN_LED, LOW);
    }
    #endif
    #if SERIAL_ENABLED
      Serial.println();
      #if DS3231_ENABLED
        Serial.print(bufftime);
        Serial.print(F(","));
      #endif  //DS3231_ENABLED
      
      #if ADS_ENABLED
        Serial.print(ads.Mics); 
        Serial.print(F(","));
        delay(100);
        Serial.print(ads.Mq); 
        Serial.print(F(","));
        delay(100);
        Serial.print(ads.Worker); 
        Serial.print(F(","));
        Serial.print(ads.Volt_Work); 
        Serial.print(F(","));
        Serial.print(ads.Volt_Ref1); 
        Serial.print(F(","));
        delay(100);
        Serial.print(ads.Auxiliary); 
        Serial.print(F(","));
        Serial.print(ads.Volt_Aux); 
        Serial.print(F(","));
        Serial.print(ads.Volt_Ref2); 
        Serial.print(F(","));
        delay(100);
      #endif  //ADS_ENABLED

      #if BME_ENABLED
        Serial.print(bme680.readTemperature());
        Serial.print(F(","));
        Serial.print(bme680.readPressure());
        Serial.print(F(","));
        Serial.print(bme680.readHumidity());
        Serial.print(F(","));
        Serial.print(bme680.readGas());
        Serial.print(F(","));
        delay(100);
      #endif  //BME_ENABLED
    #endif

    #if CELL_ENABLED
    //Collect data to storage up to size 
      digitalWrite(BLUE_LED, HIGH);
      ozpoddata.Mics[i] = ads.Mics;
      delay(100);
      ozpoddata.Mq[i] = ads.Mq;
      delay(100);
      ozpoddata.B4_Auxiliary[i] = ads.Auxiliary;
      delay(100);
      ozpoddata.B4_Worker[i] = ads.Worker;
      delay(100);
      ozpoddata.Rel_Humidity[i] = bme680.readHumidity();
      delay(100);
      ozpoddata.Temperature[i] = bme680.readTemperature();
      delay(100);
      i = i+1;
      digitalWrite(BLUE_LED, LOW);
      if(i == RUN_TO_PUBLISH) {
        if(Particle.connected())  {
          digitalWrite(BLUE_LED, HIGH);
          digitalWrite(RED_LED, HIGH);
          snprintf(buff, sizeof(buff), 
          "[[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f],[%5d,%5d,%5d,%5d,%2.2f,%2.2f]]",
              ozpoddata.Mics[0], ozpoddata.Mq[0],  ozpoddata.B4_Auxiliary[0], ozpoddata.B4_Worker[0], ozpoddata.Temperature[0], ozpoddata.Rel_Humidity[0],
              ozpoddata.Mics[1], ozpoddata.Mq[1],  ozpoddata.B4_Auxiliary[1], ozpoddata.B4_Worker[1], ozpoddata.Temperature[1], ozpoddata.Rel_Humidity[1],
              ozpoddata.Mics[2], ozpoddata.Mq[2],  ozpoddata.B4_Auxiliary[2], ozpoddata.B4_Worker[2], ozpoddata.Temperature[2], ozpoddata.Rel_Humidity[2],
              ozpoddata.Mics[3], ozpoddata.Mq[3],  ozpoddata.B4_Auxiliary[3], ozpoddata.B4_Worker[3], ozpoddata.Temperature[3], ozpoddata.Rel_Humidity[3],
              ozpoddata.Mics[4], ozpoddata.Mq[4], ozpoddata.B4_Auxiliary[4], ozpoddata.B4_Worker[4], ozpoddata.Temperature[4], ozpoddata.Rel_Humidity[4],
              ozpoddata.Mics[5], ozpoddata.Mq[5],  ozpoddata.B4_Auxiliary[5], ozpoddata.B4_Worker[5], ozpoddata.Temperature[5], ozpoddata.Rel_Humidity[5],
              ozpoddata.Mics[6], ozpoddata.Mq[6],  ozpoddata.B4_Auxiliary[6], ozpoddata.B4_Worker[6], ozpoddata.Temperature[6], ozpoddata.Rel_Humidity[6],
              ozpoddata.Mics[7], ozpoddata.Mq[7],  ozpoddata.B4_Auxiliary[7], ozpoddata.B4_Worker[7], ozpoddata.Temperature[7], ozpoddata.Rel_Humidity[7],
              ozpoddata.Mics[8], ozpoddata.Mq[8], ozpoddata.B4_Auxiliary[8], ozpoddata.B4_Worker[8], ozpoddata.Temperature[8], ozpoddata.Rel_Humidity[8],
              ozpoddata.Mics[9], ozpoddata.Mq[9],  ozpoddata.B4_Auxiliary[9], ozpoddata.B4_Worker[9], ozpoddata.Temperature[9], ozpoddata.Rel_Humidity[9],
              ozpoddata.Mics[10], ozpoddata.Mq[10],  ozpoddata.B4_Auxiliary[10], ozpoddata.B4_Worker[10], ozpoddata.Temperature[10], ozpoddata.Rel_Humidity[10],
              ozpoddata.Mics[11], ozpoddata.Mq[11], ozpoddata.B4_Auxiliary[11], ozpoddata.B4_Worker[11], ozpoddata.Temperature[11], ozpoddata.Rel_Humidity[11],
              ozpoddata.Mics[12], ozpoddata.Mq[12],  ozpoddata.B4_Auxiliary[12], ozpoddata.B4_Worker[12], ozpoddata.Temperature[12], ozpoddata.Rel_Humidity[12],
              ozpoddata.Mics[13], ozpoddata.Mq[13],  ozpoddata.B4_Auxiliary[13], ozpoddata.B4_Worker[13], ozpoddata.Temperature[13], ozpoddata.Rel_Humidity[13],
              ozpoddata.Mics[14], ozpoddata.Mq[14],  ozpoddata.B4_Auxiliary[14], ozpoddata.B4_Worker[14], ozpoddata.Temperature[14], ozpoddata.Rel_Humidity[14],
              ozpoddata.Mics[15], ozpoddata.Mq[15],  ozpoddata.B4_Auxiliary[15], ozpoddata.B4_Worker[15], ozpoddata.Temperature[15], ozpoddata.Rel_Humidity[15],
              ozpoddata.Mics[16], ozpoddata.Mq[16],  ozpoddata.B4_Auxiliary[16], ozpoddata.B4_Worker[16], ozpoddata.Temperature[16], ozpoddata.Rel_Humidity[16],
              ozpoddata.Mics[17], ozpoddata.Mq[17], ozpoddata.B4_Auxiliary[17], ozpoddata.B4_Worker[17], ozpoddata.Temperature[17], ozpoddata.Rel_Humidity[17],
              ozpoddata.Mics[18], ozpoddata.Mq[18], ozpoddata.B4_Auxiliary[18], ozpoddata.B4_Worker[18], ozpoddata.Temperature[18], ozpoddata.Rel_Humidity[18]);
          delay(100);
          Particle.publish(eventName,buff,PRIVATE);
          #if SERIAL_ENABLED
            Serial.println();
            Serial.println(buff);
            Serial.println("Cellular Published");
          #endif //SERIAL_ENABLED
          i = 0;
          digitalWrite(BLUE_LED, LOW);
          digitalWrite(RED_LED, LOW);
        }
      }
    #endif //CELL_ENABLED

  }

}

