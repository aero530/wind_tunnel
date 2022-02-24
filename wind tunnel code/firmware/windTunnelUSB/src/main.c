/**
 * \file
 *
 * \brief CDC Application Main functions
 */

#include "asf.h"
#include "conf_usb.h"

#include "string.h"
#include "stdint.h"
#include "stdarg.h"

#include "usb_cdc_interface.h"
#include "pwm16.h"

#include <stdio.h>

struct pwm_config pwm_cfg_A;
struct pwm_config pwm_cfg_B;

// USART Serial is a debug interface connection
#define USART_SERIAL                     &USARTD0
#define USART_SERIAL_BAUDRATE            57600
#define USART_SERIAL_CHAR_LENGTH         USART_CHSIZE_8BIT_gc
#define USART_SERIAL_PARITY              USART_PMODE_DISABLED_gc
#define USART_SERIAL_STOP_BIT            false

// LCD serial connects to the character display
#define LCD_SERIAL                     &USARTF0
#define LCD_SERIAL_BAUDRATE            19200
#define LCD_SERIAL_CHAR_LENGTH         USART_CHSIZE_8BIT_gc
#define LCD_SERIAL_PARITY              USART_PMODE_DISABLED_gc
#define LCD_SERIAL_STOP_BIT            false

// ADC is the pot input used to set the fan pwm
#define MY_ADC    ADCA
#define MY_ADC_CH ADC_CH0

static volatile bool main_b_cdc_enable = false;
static volatile uint8_t main_port_open;


double pwm1 = 0;
double pwm2 = 0;
double percent = 0;
double percentCompensated = 0;

// Initialize the adc
static void adc_init(void)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;
	adc_read_configuration(&MY_ADC, &adc_conf);
	adcch_read_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12, ADC_REF_VCC );
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_clock_rate(&adc_conf, 200000UL);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN0, ADCCH_NEG_NONE, 1);
	adc_write_configuration(&MY_ADC, &adc_conf);
	adcch_write_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
}

// write a string to the LCD
void lcd_puts (const char *send) {
	// Cycle through each character individually
	while (*send) {
		usart_putchar(LCD_SERIAL, *send++);
	}
}

// write a numeric value to the LCD
void lcd_print_value (uint16_t value) {
	char buffer[9];
	itoa((long)value, buffer, 10);
	lcd_puts(buffer);
}

// write a string to the debug uart
void uart_puts (const char *send) {
	// Cycle through each character individually
	while (*send) {
		usart_putchar(USART_SERIAL, *send++);
	}
}

// write a numeric value to the debug uart
void uart_print_value (uint16_t value) {
	char buffer[9];
	itoa((long)value, buffer, 10);
	uart_puts(buffer);
}

//-----------------------------------------------------------------------------

typedef struct {
	uint32_t	currentvalue;
	uint32_t	maxvalue;
} counter_overflow_t;

counter_overflow_t pwm_refresh = {0, 10}; //update every 10ms
counter_overflow_t tc_refresh  = {0, 1000}; //update every 2s

//-----------------------------------------------------------------------------

uint8_t verbose;
uint8_t tc_display_enable;


//-----------------------------------------------------------------------------

void process_command()
{

	char command_string[4];
	switch (command_in[0]) {
		
		case 'h': // Output help
		udi_cdc_puts("Help:");
		udi_cdc_puts("\r\n");
		udi_cdc_puts("'d' : disable pwms");
		udi_cdc_puts("\r\n");
		udi_cdc_puts("'e' : enable pwms");
		udi_cdc_puts("\r\n");
		udi_cdc_puts("'v' : toggle verbose output");
		udi_cdc_puts("\r\n");
		udi_cdc_puts("'s=00' : pwm value in hex");
		udi_cdc_puts("\r\n");
		break;
		
		case 'd': // disable pwms
			//pwm_refresh.enabled = 0;
			percent = 0;
			percentCompensated = 0;
			pwm1 = 0;
			pwm2 = 0;
			pwm_set_duty_cycle_percent(&pwm_cfg_A, pwm1);
			pwm_set_duty_cycle_percent(&pwm_cfg_B, pwm2);
			if (verbose) {
				udi_cdc_puts("PWM outputs disabled");
				udi_cdc_puts("\r\n");
			}
		break;
		
		case 'e': // enable pwms
			//pwm_refresh.enabled = 1;
			udi_cdc_puts("PWM outputs enabled");
			udi_cdc_puts("\r\n");
		break;
		
		case 'v': // toggle  verbose output
			if (verbose) {
				verbose=0;
				udi_cdc_puts("Verbose output disabled");
				udi_cdc_puts("\r\n");
			} else {
				verbose=1;
				udi_cdc_puts("Verbose output enabled");
				udi_cdc_puts("\r\n");
			}
			//udi_cdc_puts("\n\r");
		break;
		
		case 's': //simulate spi input
			if (command_in[1] == '=') {
				//input must be in the format s=xx
				// first is the address, second two are the pwm values, third two are the duration

				//http://www.dfstermole.net/OAC/hstrings.html
				strncpy(command_string, &command_in[2], 4);
				command_string[3]='\0';
				percent = strtol(command_string, NULL, 10);
				//data_buffer[bytes_received] = atoi(channel_address);
	
				// Set valid range for percent
				if (percent < 0) {
					percent = 0;
				}
				if (percent > 100) {
					percent = 100;
				}


				double percentSquared = percent*percent;
				double percentCubed = percentSquared*percent;
								
				if (percent < 20) {
					//percentCompensated = 0.003001*percentCubed - 0.042495*percentSquared + 0.898157*percent - 0.053577;
					percentCompensated = 0.002351*percentCubed - 0.028552*percentSquared + 0.830766*percent - 0.039634;
				} else {
					//percentCompensated = 0.000082*percentCubed - 0.010512*percentSquared + 1.164923*percent + 5.749069;
					percentCompensated = 0.000083*percentCubed - 0.010387*percentSquared + 1.132475*percent + 7.080597;
				}
	
				//percentCompensated = percent;			
				
				/*
				// pwm1 should never be below 0.  pwm2 will sometimes goe negative (based on curve fit) but really should be limited to 0
				if (pwm1 < 0) {
					pwm1 = 0;
				}
				if (pwm2 < 0) {
					pwm2 = 0;
				}
				
				// This should never happen but put in for safety check
				if (pwm1 > 100) {
					pwm1 = 100;
				}
				if (pwm2 > 100) {
					pwm2 = 100;
				}
*/
				
				// calculate pwm percentages
				// scale the pwm outputs so one turns off before the other (to get more low end resolution)
				pwm1 = (percentCompensated*0.90) + 10;
				pwm2 = (percentCompensated-20) * 1.25;
				
				if (pwm1 < 0.0) {
					pwm1 = 0.0;
				} else if (pwm1 > 100.0) {
					pwm1 = 100.0;
				}
				
				if (pwm2 < 0.0) {
					pwm2 = 0.0;
				} else if (pwm2 > 100.0) {
					pwm2 = 100.0;
				}
				
				
				pwm_set_duty_cycle_percent(&pwm_cfg_A, (uint8_t)pwm1);
				pwm_set_duty_cycle_percent(&pwm_cfg_B, (uint8_t)pwm2);
		
				if (verbose) {
					udi_cdc_puts("command in=");
					udi_cdc_puts(command_in);
					udi_cdc_puts(", command string=");
					udi_cdc_puts(command_string);
					udi_cdc_puts(", percent=");
					print_value32(percent);
					udi_cdc_puts(", pwm1=");
					print_value32(pwm1);
					udi_cdc_puts(", pwm2=");
					print_value32(pwm2);
					udi_cdc_puts("\r\n");
				} 
			}				
		break;
		
		case 'g': // display settings
			if (verbose) {
				udi_cdc_puts("percent=");
				print_value32(percent);
				udi_cdc_puts(", pwm1=");
				print_value32(pwm1);
				udi_cdc_puts(", pwm2=");
				print_value32(pwm2);
				udi_cdc_puts("\r\n");
			} else {
				print_value32(percent);
				udi_cdc_puts(",");
				print_value32(pwm1);
				udi_cdc_puts(",");
				print_value32(pwm2);
				udi_cdc_puts("\r\n");
			}
		break;
	
		
		default:
			udi_cdc_puts("NOT RECOGNISED.  Enter h for list of commands");
			udi_cdc_puts("\r\n");
		break;
	}
}



/*! \brief Main function. Execution starts here.
 */
int main(void)
{
	irq_initialize_vectors();
	cpu_irq_enable();

	// Initialize the sleep manager
	sleepmgr_init();

	sysclk_init();
	
	// configure debug uart
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = USART_SERIAL_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT
	};
	sysclk_enable_module(SYSCLK_PORT_D, PR_USART0_bm);
	usart_init_rs232(USART_SERIAL, &USART_SERIAL_OPTIONS);
	
	// config lcd uart
	static usart_rs232_options_t LCD_SERIAL_OPTIONS = {
		.baudrate = LCD_SERIAL_BAUDRATE,
		.charlength = LCD_SERIAL_CHAR_LENGTH,
		.paritytype = LCD_SERIAL_PARITY,
		.stopbits = LCD_SERIAL_STOP_BIT
	};
	sysclk_enable_module(SYSCLK_PORT_F, PR_USART0_bm);
	usart_init_rs232(LCD_SERIAL, &LCD_SERIAL_OPTIONS);
	
	board_init();
	
	pmic_init();

	// dont need these two ui things
	//	ui_init();
	//	ui_powerdown();

	// initialize the pwms at 24kHx
	pwm_init(&pwm_cfg_A, PWM_TCC0, PWM_CH_A, 24000);
	pwm_init(&pwm_cfg_B, PWM_TCC0, PWM_CH_B, 24000);

	// initialize fans at 0% then wait for 2s before starting
	pwm_start(&pwm_cfg_A, 0);
	pwm_start(&pwm_cfg_B, 0);

	// Display boot screen on lcd

	// clear screen
	usart_putchar(LCD_SERIAL, 0xFE);
	usart_putchar(LCD_SERIAL, 0x58);

	// Go home
	usart_putchar(LCD_SERIAL, 0xFE);
	usart_putchar(LCD_SERIAL, 0x48);

	lcd_puts("     Wind Tunnel");
	// Set cursor position
	usart_putchar(LCD_SERIAL, 0xFE);
	usart_putchar(LCD_SERIAL, 0x47);
	usart_putchar(LCD_SERIAL, 0x01); // col
	usart_putchar(LCD_SERIAL, 0x02); // row
	lcd_puts("    ramping fans");

	// delay for 2s while fans ramp up to 50%
	delay_ms(2000);

	// initialize the adc
	//adc_init();
	//adc_enable(&MY_ADC);

	// clear lcd before starting loop

	// clear screen
	//usart_putchar(LCD_SERIAL, 0xFE);
	//usart_putchar(LCD_SERIAL, 0x58);

	// Initialize Horizontal Bar
	//usart_putchar(LCD_SERIAL, 0xFE);
	//usart_putchar(LCD_SERIAL, 0x68);

	// Start USB stack to authorize VBus monitoring
	udc_start();

	// The main loop manages only the power mode
	// because the USB management is done by interrupt
	while (true) {
		sleepmgr_enter_sleep();
		
		if (command_ready == 1) {
			copy_command();
			process_command();
			command_ready = 0;
		}
	}
}

void main_suspend_action(void)
{
	//ui_powerdown();
}

void main_resume_action(void)
{
	//ui_wakeup();
}

void main_sof_action(void)
{
	if (!main_b_cdc_enable)
		return;
	//ui_process(udd_get_frame_number());
}

bool main_cdc_enable(uint8_t port)
{
	main_b_cdc_enable = true;
	main_port_open = 0;
	// Open communication
	//uart_open(port);
	return true;
}

void main_cdc_disable(uint8_t port)
{
	main_b_cdc_enable = false;
	// Close communication
	//uart_close(port);
}

void main_cdc_set_dtr(uint8_t port, bool b_enable)
{
	if (b_enable) {
		// Host terminal has open COM
		main_port_open |= 1 << port;
		//ui_com_open(port);
	}else{
		// Host terminal has close COM
		main_port_open &= ~(1 << port);
		//ui_com_close(port);
	}
}