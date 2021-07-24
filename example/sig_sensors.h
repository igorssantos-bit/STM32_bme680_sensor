/*
 * sig_sensors.h
 *
 *  Created on: Oct 8, 2020
 *      Author: igors
 */

#ifndef INC_SIG_SENSORS_H_
#define INC_SIG_SENSORS_H_

#include <stdbool.h>
#include "stm32l0xx_hal.h"
#include "i2c.h"
#include "adc.h"

#include "bme680.h"
#include "bme680_defs.h"

/*! Structure with variables related to HT-ADC sensor .*/
typedef struct{
	uint16_t voltage;
} st_battery_t;

/*! Structure with variables related to BME680 sensor .*/
typedef struct{
	struct bme680_dev gas_sensor;
	float temperature;
	float pressure;
	float humidity;
	uint32_t gas_resistance;
	uint16_t meas_period;
	float gas_weighting;
	float gas_reference;
	uint8_t getgasreference_count;
	float hum_weighting;
	float hum_reference;
	int32_t gas_lower_limit;
	int32_t gas_upper_limit;
} st_bme680_t;

volatile st_battery_t bat;
volatile st_bme680_t bme;

void init_bme680(void);
void bme_GetGasReference(void);
int16_t bme_GetGasScore(void);
int16_t bme_GetHumidityScore(void);
void bme_calculateIAQ(int16_t score);

#endif /* INC_SIG_SENSORS_H_ */
