/**
@file Thermocouple.h

@brief Driver for Multiplexed AD849x Thermocouple Reader Board
@details Type K thermocouples read using AD849x.  8 channel multiplexed.

Usage:

@code
#include "LK202_25.h"
#include "UART_polled.h"
#include "adc_atmega32u4.h"
#include "Thermocouple.h"

int main (void) {
	uint8_t string_size = 21; //including null terminator
	char string_buffer[string_size];
	double temperature_values[8];

	InitUART();
	_delay_ms(500);
	LCD_init();
	thermocouple_init();

	while (1) {
		LCD_home();
		
		// Loop through each thermocouple on the multiplexer and read its value
		for (uint8_t scount=0; scount<8; scount++){
			temperature_values[scount] = thermocouple_read(scount, 100);
		}
		
		snprintf(string_buffer,string_size,"%4.1f %4.1f %4.1f %4.1f",temperature_values[0],temperature_values[1],temperature_values[2],temperature_values[3]);
		LCD_printstring(string_buffer);
		
		LCD_goto(1,2);
		snprintf(string_buffer,string_size,"%4.1f %4.1f %4.1f %4.1f",temperature_values[4],temperature_values[5],temperature_values[6],temperature_values[7]);
		LCD_printstring(string_buffer);

		_delay_ms(100);
	}
	return 0;
}
@endcode

@author Phil Spindler
@date 8/26/2013
*/


#ifndef THERMOCOUPLE_H_
#define THERMOCOUPLE_H_

#include "inttypes.h"


//#define TC_SELECT_PORT	PORTA /**< Port that the multiplexer select pins are connected to */
//#define TC_SELECT_R		PORTA_DIR //DDRA /**< Port register that the multiplexer select pins are connected to */
//#define TC_SELECT_0	 	PINA7 /**< Pin that multiplexer select 0 is connected to */
//#define TC_SELECT_1		PINA6 /**< Pin that multiplexer select 1 is connected to */
//#define TC_SELECT_2		PINA5 /**< Pin that multiplexer select 2 is connected to */
//#define TC_REF		1 /**< ADC pin number that the thermocouple offset voltage is connected to (pin A1 = ADC channel 1) */
//#define TC_IN		2 /**< ADC pin number that the AD849x thermocouple output is connected to (pin A2 = ADC channel 2) */



/* Cable Pinout
  TC   -    uC
  3.3   1-1  3.3
  GND   2-2  GND
  Sel0  3-3  PA7
  Sel1  4-4  PA6
  Sel2  5-5  PA5
  Out   6-8  PA2
  Ref   7-9  PA1
  NC    8-NC
  */
#define TC_SELECT_0	 	IOPORT_CREATE_PIN(PORTA, 7) /**< Pin that multiplexer select 0 is connected to */
#define TC_SELECT_1		IOPORT_CREATE_PIN(PORTA, 6) /**< Pin that multiplexer select 1 is connected to */
#define TC_SELECT_2		IOPORT_CREATE_PIN(PORTA, 5) /**< Pin that multiplexer select 2 is connected to */

#define TC_ADC		ADCA
#define TC_REF		ADC_CH1 /**< ADC pin number that the thermocouple offset voltage is connected to (pin A1 = ADC channel 1) */
#define TC_IN		ADC_CH2 /**< ADC pin number that the AD849x thermocouple output is connected to (pin A2 = ADC channel 2) */



/**
* Initialize ADC and the pins used for multiplexer selection
*
*
*/
void thermocouple_init(void);


/**
* Set the select pins high/low to set the multiplexer to enable a specific channel
*
* @param ch The thermocouple channel to select (0 through 7)
*
*/
void thermocouple_select(uint8_t ch);


/**
* Performs multiple ADC conversions on the thermocouple channel and the thermocouple offset voltage reference.
* The ADC readings are averaged and used to compute the temperature.
* 
* @note Microcontroller has 12bit ADC = 4096 as a max value
* 
* Voltage_of_tcref = (adc_read(TCREF)*5[V_aref]*1000[mV/V]/1024[maxcounts]) in mV
* 
* Voltage_of_tcin = (adc_read(TCIN)*5[V_aref]*1000[mV/V]/1024[maxcounts]) in mV
* 
* Temperature = ( Voltage_of_tcin - Voltage_of_tcref ) / 5 in degC
	
* @note Use a precision voltage reference to increase accuracy.  Determine expected output voltage from AD849x based on max use temperature
* (reference AN-1087).  For example, chip will output approx 1V at 200C.  Then use a precision voltage reference of that voltage as
* the V_aref for the microcontroller and sub the voltage reference value in for 5[V_aref] in the calculation.
*
* @param ch The thermocouple channel to select (0 through 7)
* @param num_readings The number of ADC conversions used to average
*
* @retval temperature in degrees C
*
*/
double thermocouple_read(uint8_t ch, uint8_t num_readings);


#endif /* THERMOCOUPLE_H_ */