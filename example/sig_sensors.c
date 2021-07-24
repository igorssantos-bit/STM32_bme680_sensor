/*
 * sig_sensors.c
 *
 *  Created on: Oct 8, 2020
 *      Author: igors
 */

#include "sig_sensors.h"

extern I2C_HandleTypeDef hi2c1;

void init_bme680(void){
	int8_t rslt = BME680_OK;
	uint8_t set_required_settings;


	/*  Inicialização */
	bme.gas_sensor.dev_id = BME680_I2C_ADDR_PRIMARY;  // SDO Grounded -> 0x76 left aligned
	bme.gas_sensor.intf = BME680_I2C_INTF;
	bme.gas_sensor.handle = (I2C_HandleTypeDef *) &hi2c1;
	bme.gas_sensor.read = user_i2c_read;
	bme.gas_sensor.write = user_i2c_write;
	bme.gas_sensor.delay_ms = user_delay_ms;
	/* amb_temp can be set to 25 prior to configuring the gas sensor
	 * or by performing a few temperature readings without operating the gas sensor. */
	bme.gas_sensor.amb_temp = 25;

	rslt = bme680_init( &bme.gas_sensor );
	if(rslt!=BME680_OK)
		printf("Erro ao inicializar BME = %d\r\n", rslt);


	/*  Configuração */
	/* Set the temperature, pressure and humidity settings */
	bme.gas_sensor.tph_sett.os_hum = BME680_OS_2X;
	bme.gas_sensor.tph_sett.os_pres = BME680_OS_4X;
	bme.gas_sensor.tph_sett.os_temp = BME680_OS_8X;
	bme.gas_sensor.tph_sett.filter = BME680_FILTER_SIZE_3;

	/* Set the remaining gas sensor settings and link the heating profile */
	bme.gas_sensor.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
	/* Create a ramp heat waveform in 3 steps */
	bme.gas_sensor.gas_sett.heatr_temp = 320; /* degree Celsius */
	bme.gas_sensor.gas_sett.heatr_dur = 150; /* milliseconds */

	/* Select the power mode */
	/* Must be set before writing the sensor configuration */
	bme.gas_sensor.power_mode = BME680_FORCED_MODE;

	/* Set the required sensor settings needed */
	set_required_settings = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_FILTER_SEL
			| BME680_GAS_SENSOR_SEL;

	/* Set the desired sensor configuration */
	rslt = bme680_set_sensor_settings(set_required_settings, &bme.gas_sensor);

	/* Set the power mode */
	rslt = bme680_set_sensor_mode(&bme.gas_sensor);

	bme680_get_profile_dur(&bme.meas_period, &bme.gas_sensor);

	bme_GetGasReference();

}

void bme_GetGasReference(void){
	uint8_t readings = 1;
	struct bme680_field_data data;

	while( readings <= 10 ){
		bme680_get_sensor_data(&data, &bme.gas_sensor);
		if(data.status & BME680_GASM_VALID_MSK){
			bme.gas_reference += data.gas_resistance;
			readings++;
		}
		user_delay_ms(bme.meas_period);
	}
	bme.gas_reference = bme.gas_reference / readings;
}

int16_t bme_GetGasScore(void){
	int16_t gas_score = 0;

	//Calculate gas contribution to IAQ index
	gas_score = (0.75 / (bme.gas_upper_limit - bme.gas_lower_limit) * bme.gas_reference - (bme.gas_lower_limit * (0.75 / (bme.gas_upper_limit - bme.gas_lower_limit)))) * 100.00;
	if (gas_score > 75) gas_score = 75; // Sometimes gas readings can go outside of expected scale maximum
	if (gas_score <  0) gas_score = 0;  // Sometimes gas readings can go outside of expected scale minimum

	return gas_score;
}

int16_t bme_GetHumidityScore(void){
	int16_t humidity_score = 0;

	if (bme.humidity >= 38 && bme.humidity <= 42) // Humidity +/-5% around optimum
		humidity_score = 0.25 * 100;
	else
	{ // Humidity is sub-optimal
		if (bme.humidity < 38)
			humidity_score = 0.25 / bme.hum_reference * bme.humidity * 100;
		else
		{
			humidity_score = ((-0.25 / (100 - bme.hum_reference) * bme.humidity) + 0.416666) * 100;
		}
	}

	return humidity_score;
}

void bme_calculateIAQ(int16_t score){
	score = (100 - score) * 5;
	printf("IAQ: %d\r\n", score);

//	printf("IAQ ");
//	if(score >= 301)
//		printf("Hazardous\r\n");
//	else if (score >= 201 && score <= 300 )
//		printf("Very Unhealthy\r\n");
//	else if (score >= 176 && score <= 200 )
//		printf("Unhealthy\r\n");
//	else if (score >= 151 && score <= 175 )
//		printf("Unhealthy for Sensitive Groups\r\n");
//	else if (score >=  51 && score <= 150 )
//		printf("Moderate\r\n");
//	else if (score >=  00 && score <=  50 )
//		printf("Good\r\n");
}

