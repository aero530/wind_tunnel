/**
 * \file
 *
 * \brief User board initialization template
 *
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include <ioport.h>


void board_init(void) {
		/*
		ioport_configure_pin(LED0_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
		ioport_configure_pin(LED1_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
		ioport_configure_pin(LED2_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
		ioport_configure_pin(LED3_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);

		//ioport_configure_pin(FAN_0_PWM, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW | IOPORT_WIRED_AND);
		//ioport_configure_pin(FAN_1_PWM, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW | IOPORT_WIRED_AND);
		//ioport_configure_pin(FAN_2_PWM, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW | IOPORT_WIRED_AND);
		//ioport_configure_pin(FAN_3_PWM, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW | IOPORT_WIRED_AND);
		
		ioport_configure_pin(FAN_0_PWM, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW );
		ioport_configure_pin(FAN_1_PWM, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW );
		ioport_configure_pin(FAN_2_PWM, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW );
		ioport_configure_pin(FAN_3_PWM, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW );

		
		// Configure pins for SPI slave mode
		ioport_configure_pin(SPI_SCK_PIN,  IOPORT_DIR_INPUT);
		ioport_configure_pin(SPI_MOSI_PIN, IOPORT_DIR_INPUT);
		ioport_configure_pin(SPI_MISO_PIN, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
		ioport_configure_pin(SPI_SS_PIN,   IOPORT_PULL_UP | IOPORT_DIR_INPUT);

		PORTE.REMAP |= PORT_USART0_bm; // Move USARTE to high nibble.
		*/
		
		ioport_configure_pin(FAN_PWM_A,			IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);	// init fan off
		ioport_configure_pin(FAN_PWM_B,			IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);	// init fan off
		ioport_configure_pin(VARIABLE_RESISTOR,	IOPORT_DIR_INPUT);
		
		ioport_configure_pin(UART_RX,			IOPORT_DIR_INPUT );
		ioport_configure_pin(UART_TX,			IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
		
		ioport_configure_pin(LCD_RX,			IOPORT_DIR_INPUT );
		ioport_configure_pin(LCD_TX,			IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
		
		
}
