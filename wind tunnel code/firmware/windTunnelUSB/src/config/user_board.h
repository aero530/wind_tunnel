/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <compiler.h>
#include <conf_board.h>

//#define LED0                        LED0_GPIO
//#define LED1                        LED1_GPIO
//#define LED2                        LED2_GPIO
//#define LED3                        LED3_GPIO
//! Number of LEDs.
//#define LED_COUNT                   4
/*
#define FAN_0_PWM                   IOPORT_CREATE_PIN(PORTC, 0)
#define FAN_1_PWM                   IOPORT_CREATE_PIN(PORTC, 1)
#define FAN_2_PWM                   IOPORT_CREATE_PIN(PORTC, 2)
#define FAN_3_PWM                   IOPORT_CREATE_PIN(PORTC, 3)

#define SPI_PORT					SPIC
#define SPI_INT_VECTOR				SPIC_INT_vect
#define SPI_SS_PIN					IOPORT_CREATE_PIN(PORTC, 4)
#define SPI_MOSI_PIN				IOPORT_CREATE_PIN(PORTC, 5)
#define SPI_MISO_PIN				IOPORT_CREATE_PIN(PORTC, 6)
#define SPI_SCK_PIN					IOPORT_CREATE_PIN(PORTC, 7)


#define LED0_GPIO					IOPORT_CREATE_PIN(PORTB, 0)
#define LED1_GPIO                   IOPORT_CREATE_PIN(PORTB, 1)
#define LED2_GPIO                   IOPORT_CREATE_PIN(PORTB, 2)
#define LED3_GPIO                   IOPORT_CREATE_PIN(PORTB, 3)
*/

#define VARIABLE_RESISTOR		IOPORT_CREATE_PIN(PORTA,0)

#define FAN_PWM_A				IOPORT_CREATE_PIN(PORTC,0)
#define FAN_PWM_B				IOPORT_CREATE_PIN(PORTC,1)

#define UART_RX					IOPORT_CREATE_PIN(PORTD,2)
#define UART_TX					IOPORT_CREATE_PIN(PORTD,3)

#define LCD_RX					IOPORT_CREATE_PIN(PORTF,2)
#define LCD_TX					IOPORT_CREATE_PIN(PORTF,3)


#endif // USER_BOARD_H
