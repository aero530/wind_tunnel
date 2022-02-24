/*
 * usb_cdc_interface.c
 *
 * Created: 11/28/2012 2:59:30 PM
 *  Author: Corvus
 */ 

#include "compiler.h"
#include "usb_cdc_interface.h"
#include "asf.h"
#include "util/atomic.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "main.h"




void print_value (int value)
{
	char buffer[9];
	//itoa(value, buffer, 16);
	//udi_cdc_puts(buffer);
	//udi_cdc_puts("\t");
	itoa(value, buffer, 10);
	udi_cdc_puts(buffer);
	//udi_cdc_puts("\t");
	//itoa(value, buffer, 2);
	//udi_cdc_puts(buffer);
	//udi_cdc_puts("\n\r");
}

void print_value_bin (int value)
{
	char buffer[9];
	//itoa(value, buffer, 16);
	//udi_cdc_puts(buffer);
	//udi_cdc_puts("\t");
	//itoa(value, buffer, 10);
	//udi_cdc_puts("\t");
	itoa(value, buffer, 2);
	udi_cdc_puts("0b");
	udi_cdc_puts(buffer);
	//udi_cdc_puts(buffer);
	//udi_cdc_puts("\n\r");
}

void print_value32 (int32_t value)
{
	char buffer[11];
	sprintf(buffer, "%lu", value);
	udi_cdc_puts(buffer);
	//udi_cdc_puts("\n\r");
}



void copy_command()
{
	// The USART might interrupt this - don't let that happen!
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		//memset(command_in[0], 0, 32);
		
		// Copy the contents of data_in into command_in
		//memcpy(command_in, data_in, 8);
		//output = data_in;
		memcpy((void *)command_in, (const void *)data_in, 31); //memcpy(command_in, data_in, 15);  // the const informs the user that the function will not modify the value pointed to

		// Now clear data_in, the USART can reuse it now
		memset((void *)&data_in[0], 0, 31); //memset(data_in[0], 0, 15);

	}
}


unsigned long parse_assignment (char input[16])
{
	char *pch;
	char cmdValue[16];
	// Find the position the equals sign is in the string, keep a pointer to it
	pch = strchr(input, '=');
	// Copy everything after that point into the buffer variable
	strcpy(cmdValue, pch+1);
	// Now turn this value into an integer and return it to the caller.
	return atoi(cmdValue);
}

void udi_cdc_puts (const char *send)
{
	// Cycle through each character individually
	while (*send) {
		udi_cdc_putc(*send++);
	}
}


void usb_cdc_interface_rx_notify(uint8_t port) {
	if (udi_cdc_is_rx_ready()) {
		data_in[data_count] = udi_cdc_getc();
		
		// udi_cdc_putc(data_in[data_count]); //return character to the screen	
		
		
		// End of line!
		if ((data_in[data_count] == 0x0D) || (data_in[data_count] == 0x0A)) {
			
			command_ready = 1;
			// Reset to 0, ready to go again
			data_count = 0;
		
			//add an extra crcl
			//udi_cdc_putc(0x0D);
			//udi_cdc_putc(0x0A);
		} else {
			data_count++;
		}
	} else {
		//udi_cdc_puts("UDI fifo empty Vect\n\r");
	}
}