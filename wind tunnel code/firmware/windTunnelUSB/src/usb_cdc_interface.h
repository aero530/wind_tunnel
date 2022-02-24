/*
 * usb_cdc_interface.h
 *
 * Created: 11/28/2012 2:59:20 PM
 *  Author: Corvus
 */ 


#ifndef USB_CDC_INTERFACE_H_
#define USB_CDC_INTERFACE_H_

#include "inttypes.h"

/**
 * \brief Print a 8 bit int to udi_cdc_puts
 *
 * \param value     integer to display
 *
 * \return void
 */
void print_value (int value);

/**
 * \brief Print a 8 bit int to udi_cdc_puts
 *
 * \param value     integer to display
 *
 * \return void
 */
void print_value_bin (int value);


/**
 * \brief Print a 32 bit int to udi_cdc_puts
 *
 * \param value     integer to display
 *
 * \return void
 */
void print_value32 (int32_t value);


//! \brief Copy command from USB->USART connection to variable for processing
void copy_command (void);


/**
 * \brief Parse an input string and extract the numeric value after the '='
 *
 * \param send     string to parse
 *
 * \return unsigned long
 */
unsigned long parse_assignment (char input[16]);

/*! \brief Writes a string on CDC line
 * Writes out a character array to the USART->USB connection
 */
void udi_cdc_puts (const char *send);


void usb_cdc_interface_rx_notify(uint8_t port);

#endif /* USB_CDC_INTERFACE_H_ */