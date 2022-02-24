/**
 * \file
 *
 * \brief Board configuration
 *
 * Copyright (c) 2011 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

//! Definition of USART connection for this example
// Set USART connection to high nibble of port F


//http://www.avrfreaks.net/index.php?name=PNphpBB2&file=printview&t=117582&start=0
/*
13.13.14 REMAP – Pin Remap register
The pin remap functionality is available for PORTC - PORTF only.
* Bit 7:6 – Reserved
These bits are unused and reserved for future use. For compatibility with
future devices, always write these bits to zero when this register is written.

* Bit 5 – SPI: SPI Remap
Setting this bit to one will swap the pin locations of the SCK and MOSI pins
to have pin compatibility between SPI and USART when the USART is operating as
a SPI master.

* Bit 4 – USART0: USART0 Remap
Setting this bit to one will move the pin location of USART0 from Px[3:0] to Px[7:4].

* Bit 3 – TC0D: Timer/Counter 0 Output Compare D
Setting this bit will move the location of OC0D from Px3 to Px7.

* Bit 2 – TC0C: Timer/Counter 0 Output Compare C
Setting this bit will move the location of OC0C from Px2 to Px6.

* Bit 1 – TC0B: Timer/Counter 0 Output Compare B
Setting this bit will move the location of OC0B from Px1 to Px5. If this bit is
set and PWM from both timer/counter 0 and timer/counter 1 is enabled, the
resulting PWM will be an OR-modulation between the two PWM outputs.

* Bit 0 – TC0A: Timer/Counter 0 Output Compare A
Setting this bit will move the location of OC0A from Px0 to Px4. If this bit is
set and PWM from both timer/counter 0 and timer/counter 1 is enabled, the
resulting PWM will be an OR-modulation between the two PWM outputs.
*/

/*
#define  USART               USARTE0
#define  USART_RX_Vect       USARTE0_RXC_vect
#define  USART_TX_Vect       USARTE0_TXC_vect
#define  USART_DRE_Vect      USARTE0_DRE_vect
#define  USART_SYSCLK        SYSCLK_USART0
#define  USART_PORT          PORTE //
#define  USART_PORT_PIN_TX   PIN7_bm
#define  USART_PORT_PIN_RX   PIN6_bm
#define  USART_PORT_SYSCLK   SYSCLK_PORT_E//
*/

// Pins on this TC are used for SPI SS and USART SPI XCK
// #define TIMER_PWM_INT        TCE1 //TCC1

// CLK_SYS / 4 prescale = 32MHz / 4 => 125000 system clock tics per second
// want to overflow 200 times a second (so it goes high once per millisecond)
// => Period should be 125000 / 100 = 1250
// #define	TIMER_EXAMPLE_PERIOD 5990

// #define	FAN_COUNT		4

#endif /* CONF_BOARD_H_INCLUDED */
