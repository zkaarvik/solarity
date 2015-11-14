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

	MAP_WDT_A_holdTimer();
	UART_PC_init();
	UART_GSM_init();

	send_AT_command("AT+GMI");

	set_up_bearer_fido();
	Open_Bearer_Connection();
	Init_HTTP_Service();
	Set_up_HTTP_Para();

	//call the action
	Read_HTTP_Context();

	Transmit_HTTP_Read();
	End_HTTP_Service();
	Close_Bearer_Connection();

}



