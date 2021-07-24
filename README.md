# STM32_bme680_sensor

## Overview
This repo uses [Bosh Drivers API](https://github.com/BoschSensortec/BME680_driver) to read Temperature, Pressure, Humidity and Indoor Air Quality (AIQ) using STM32. The sensor heats a small surface to measure the resistance of the gas layer, this indicates the amaount of volatile components in the air and is used as an indirect measurement of air quality. Bosh's BSEC software provides the best solution to calculate IAQ index, but it won't run on plataforms with low ROM profiles.

This code provides a simplified way to measure IAQ on STM32 plataforms based on [Zanduino Drivers](https://github.com/Zanduino/BME680) for Arduino.

## File Information
* bme680_defs.h : This header file has the constants, macros and datatype declarations.
* bme680.c : This header file contains the declarations of the sensor driver APIs.
* bme680.c : This source file contains the definitions of the sensor driver APIs.
* example/sig_sensors.c : This source file contains the functions the plataform uses to measure and calculate IAQ.
* example/sig_sensors.h : This header file contains the functions the plataform uses to measure and calculate IAQ.
* example/main.c : Main loop.
