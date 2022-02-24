/**
 * @file Thermocouple.c
 *
 * @brief Driver for Multiplexed AD849x Thermocouple Reader Board
 * @details Type K thermocouples read using AD849x.  8 channel multiplexed.
 * @author Phil Spindler
 * @date 8/26/2013
 */


#include "Thermocouple.h"
#include "inttypes.h"
#include "avr/io.h"
#include "util/delay.h"
//#include "adc_atmega32u4.h"
#include "asf.h"
#include "adc.h"


void thermocouple_init(void) {
	//adc_init();
	
	struct adc_config adc_conf;
	struct adc_channel_config adc_ch_conf_ch1;
	struct adc_channel_config adc_ch_conf_ch2;

	adc_read_configuration(&TC_ADC, &adc_conf);
	adcch_read_configuration(&TC_ADC, TC_IN, &adc_ch_conf_ch2);
	adcch_read_configuration(&TC_ADC, TC_REF, &adc_ch_conf_ch1);

	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12, ADC_REF_BANDGAP);

	adc_set_clock_rate(&adc_conf, 200000UL);

	adc_write_configuration(&TC_ADC, &adc_conf);
	
	adcch_set_input(&adc_ch_conf_ch2, ADCCH_POS_PIN2, ADCCH_NEG_NONE, 1);
	adcch_set_input(&adc_ch_conf_ch1, ADCCH_POS_PIN1, ADCCH_NEG_NONE, 1);
	
	adcch_write_configuration(&TC_ADC, 2, &adc_ch_conf_ch2);
	adcch_write_configuration(&TC_ADC, 1, &adc_ch_conf_ch1);
	
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
	//adc_set_conversion_trigger(&adc_conf, ADC_TRIG_FREERUN_SWEEP, 2, 0);
	adc_write_configuration(&TC_ADC, &adc_conf);
	
	adc_enable(&TC_ADC);
	 
	
	//TC_SELECT_R |= (1<<TC_SELECT_0); // set SELECT_0 pin as output
	//TC_SELECT_R |= (1<<TC_SELECT_1); // set SELECT_1 pin as output
	//TC_SELECT_R |= (1<<TC_SELECT_2); // set SELECT_2 pin as output
		
	ioport_configure_pin(TC_SELECT_0, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW); // set SELECT_0 pin as output
	ioport_configure_pin(TC_SELECT_1, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW); // set SELECT_1 pin as output
	ioport_configure_pin(TC_SELECT_2, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW); // set SELECT_2 pin as output
}


void thermocouple_select(uint8_t ch) {
	
	/*  Multiplexer selection table (enable pin pulled high in hardware)
	2 1 0 EN	ON Switch Pair
	X X X 0		NONE
	0 0 0 1		1
	0 0 1 1		2
	0 1 0 1		3
	0 1 1 1		4
	1 0 0 1		5
	1 0 1 1		6
	1 1 0 1		7
	1 1 1 1		8
	*/
	
	
	switch (ch)	{
		case 0 :
		//TC_SELECT_PORT &= ~(1<<TC_SELECT_0); // set low
		//TC_SELECT_PORT &= ~(1<<TC_SELECT_1); // set low
		//TC_SELECT_PORT &= ~(1<<TC_SELECT_2); // set low
		gpio_set_pin_low(TC_SELECT_0); // set low
		gpio_set_pin_low(TC_SELECT_1); // set low
		gpio_set_pin_low(TC_SELECT_2); // set low
		break;
		
		case 1 :
		//TC_SELECT_PORT |=  (1<<TC_SELECT_0); // set high
		//TC_SELECT_PORT &= ~(1<<TC_SELECT_1); // set low
		//TC_SELECT_PORT &= ~(1<<TC_SELECT_2); // set low
		gpio_set_pin_high(TC_SELECT_0); // set high
		gpio_set_pin_low(TC_SELECT_1); // set low
		gpio_set_pin_low(TC_SELECT_2); // set low
		break;
		
		case 2 :
		//TC_SELECT_PORT &= ~(1<<TC_SELECT_0); // set low
		//TC_SELECT_PORT |=  (1<<TC_SELECT_1); // set high
		//TC_SELECT_PORT &= ~(1<<TC_SELECT_2); // set low
		gpio_set_pin_low(TC_SELECT_0); // set low
		gpio_set_pin_high(TC_SELECT_1); // set high
		gpio_set_pin_low(TC_SELECT_2); // set low
		break;
		
		case 3 :
		//TC_SELECT_PORT |=  (1<<TC_SELECT_0); // set high
		//TC_SELECT_PORT |=  (1<<TC_SELECT_1); // set high
		//TC_SELECT_PORT &= ~(1<<TC_SELECT_2); // set low
		gpio_set_pin_high(TC_SELECT_0); // set high
		gpio_set_pin_high(TC_SELECT_1); // set high
		gpio_set_pin_low(TC_SELECT_2); // set low
		break;
		
		case 4 :
		//TC_SELECT_PORT &= ~(1<<TC_SELECT_0); // set low
		//TC_SELECT_PORT &= ~(1<<TC_SELECT_1); // set low
		//TC_SELECT_PORT |=  (1<<TC_SELECT_2); // set high
		gpio_set_pin_low(TC_SELECT_0); // set low
		gpio_set_pin_low(TC_SELECT_1); // set low
		gpio_set_pin_high(TC_SELECT_2); // set high
		break;
		
		case 5 :
		//TC_SELECT_PORT |=  (1<<TC_SELECT_0); // set high
		//TC_SELECT_PORT &= ~(1<<TC_SELECT_1); // set low
		//TC_SELECT_PORT |=  (1<<TC_SELECT_2); // set high
		gpio_set_pin_high(TC_SELECT_0); // set high
		gpio_set_pin_low(TC_SELECT_1); // set low
		gpio_set_pin_high(TC_SELECT_2); // set high
		break;
		
		case 6 :
		//TC_SELECT_PORT &= ~(1<<TC_SELECT_0); // set low
		//TC_SELECT_PORT |=  (1<<TC_SELECT_1); // set high
		//TC_SELECT_PORT |=  (1<<TC_SELECT_2); // set high
		gpio_set_pin_low(TC_SELECT_0); // set low
		gpio_set_pin_high(TC_SELECT_1); // set high
		gpio_set_pin_high(TC_SELECT_2); // set high
		break;
		
		case 7 :
		//TC_SELECT_PORT |=  (1<<TC_SELECT_0); // set high
		//TC_SELECT_PORT |=  (1<<TC_SELECT_1); // set high
		//TC_SELECT_PORT |=  (1<<TC_SELECT_2); // set high
		gpio_set_pin_high(TC_SELECT_0); // set high
		gpio_set_pin_high(TC_SELECT_1); // set high
		gpio_set_pin_high(TC_SELECT_2); // set high
		break;
	}
	_delay_ms(1); // add a 1ms delay to allow the multiplexer to switch
}	


double thermocouple_read(uint8_t ch, uint8_t num_readings) {
	double temperature=0;
	double result_in=0;
	double result_ref=0;
	
	thermocouple_select(ch);
	
	// Thermocouple temperature calculation
	// Microcontroller has 10bit ADC = 1024 as a max value
	// V_tcref = (adc_read(TCREF)*5[V_aref]*1000[mV/V]/1024[maxcounts]) mV
	// V_tcin = (adc_read(TCIN)*5[V_aref]*1000[mV/V]/1024[maxcounts])  mV
	// Temperature = ( V_tcin - V_tcref ) / 5 degC
	
	// Use a precision voltage reference to increase accuracy.  Determine expected output voltage from AD849x based on max use temperature
	// (reference AN-1087).  For example, chip will output approx 1V at 200C.  Then use a precision voltage reference of that voltage as
	// the V_aref for the microcontroller and sub the voltage reference value in for 5[V_aref] in the calculation.
	
	for (uint8_t tcount=0; tcount<num_readings; tcount++){
		
		adc_start_conversion(&TC_ADC, TC_IN);
		adc_wait_for_interrupt_flag(&TC_ADC, TC_IN);
		result_in = adc_get_result(&TC_ADC, TC_IN);
		
		adc_start_conversion(&TC_ADC, TC_REF);
		adc_wait_for_interrupt_flag(&TC_ADC, TC_REF);
		result_ref = adc_get_result(&TC_ADC, TC_REF);
		
		temperature = temperature + ((double)result_in-(double)result_ref)*1000/4096*1/5;
		
		//temperature = temperature + ((double)adc_read(TC_IN)-(double)adc_read(TC_REF))*1000/1024*2.56/5;
	}
	
	temperature = temperature/num_readings;
	
	udi_cdc_puts(" input ADC Conversion=");
	print_value32(result_in);
	udi_cdc_puts(", ref ADC Conversion=");
	print_value32(result_ref);
	udi_cdc_puts(", temperature=");
	print_value32(temperature);
	udi_cdc_puts("\n\r");
	
	
	
	//if (temperature>300) { // if temperature converted is over 300 then it is an open thermocouple (the 300 number is based on the tcref)
	//	temperature = 0;
	//}
	
	return temperature;
}