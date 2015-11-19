/*
 * Solarity
 *
 * Description: Solarity Real Time Transit Information Display System
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Author: Sunlink
*******************************************************************************/

/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "stdarg.h"


/* Sunlink Includes */
#include <SL_Display.h>
#include <SIM800C.h>
//#include <printf.h>


int main()
{

	uint8_t *httpchar =0;

	MAP_WDT_A_holdTimer();
	UART_PC_init();
	UART_GSM_init();

	send_AT_command("AT+IPR=115200");

	httpchar = request_to_server();

	send_AT_command("AT+GMI");
	//print_http_to_pc();

}



