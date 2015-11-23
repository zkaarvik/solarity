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


bool call_server_flag = false;

void init_timer32_0(void)
	{
		MAP_Timer32_initModule(TIMER32_0_MODULE, TIMER32_PRESCALER_256, TIMER32_32BIT, TIMER32_PERIODIC_MODE);

		Timer32_setCount(TIMER32_0_MODULE,1000000);

		Timer32_enableInterrupt(TIMER32_0_MODULE);
		MAP_Interrupt_enableInterrupt(INT_T32_INT1);

		//MAP_Interrupt_enableMaster();
		Timer32_startTimer(TIMER32_0_MODULE, true);


	}

void timer32_0_isr(void)
	{
		uint32_t status = Timer32_getInterruptStatus(TIMER32_0_MODULE);
		volatile uint32_t timervalue = Timer32_getValue(TIMER32_0_MODULE);

		Timer32_clearInterruptFlag(TIMER32_0_MODULE);

		if(status){
			  //printf(EUSCI_A0_MODULE,"rob is an idiot\r\n\n");
			  call_server_flag = true;
			  //httpchar = request_to_server();
			  //printf(EUSCI_A0_MODULE,"Timer 0 value = %n\r\n",timervalue);
		 }

	}

int main()
{
	int count = 1;

	//send_AT_command("AT+IPR=115200");

	//httpchar = request_to_server();

	//send_AT_command("AT+GMI");
	//print_http_to_pc();

	/*
	int j =0 ;
	for(j=0;j<2;j++){
		httpchar = request_to_server();
		SL_D_sendCmd(resetPtrCmd, 3);

		//Upload header (first 16 bytes)
		SL_D_uploadImgData(httpchar[0], 0x10);

		for(counter = 16; counter < 48016; counter+=packetSize)
		{
			//Upload image after the header
			//Upload in chunks of 250 bytes
			//SL_D_uploadImgData(httpchar[counter], packetSize);
			SL_D_uploadImgData(httpchar + counter, packetSize);
		}

		//Send refresh display command
		SL_D_sendCmd(refreshCmd, 3);
	}
	*/
	int counter;
	uint8_t refreshCmd[3] = {0x24, 0x01, 0x00};
	uint8_t resetPtrCmd[3] = {0x20, 0x0D, 0x00};
	uint8_t packetSize = 0xFA;

	uint8_t *httpchar = 0;
	MAP_WDT_A_holdTimer();

	UART_PC_init();
	init_timer32_0();
	SL_D_init();
	UART_GSM_init();

	//low_power_mode();

	while(1){
		if(call_server_flag){
			/*
			if (count == 1){
			httpchar = request_to_server();
			count++;
			}
			else{
				httpchar = request_to_server_secondtime();
			}
			*/
			//full_mode();

			httpchar = request_to_server();

			/*
			SL_D_sendCmd(resetPtrCmd, 3);

			//Upload header (first 16 bytes)

			SL_D_uploadImgData(httpchar[0], 0x10);

			for(counter = 16; counter < 48016; counter+=packetSize)
			{
				//Upload image after the header
				//Upload in chunks of 250 bytes
				//SL_D_uploadImgData(httpchar[counter], packetSize);
				SL_D_uploadImgData(httpchar + counter, packetSize);
			}

			//Send refresh display command
			SL_D_sendCmd(refreshCmd, 3);
			*/
			call_server_flag = false; //reset flag




			//UART_PC_init();
			init_timer32_0();
			//SL_D_init();
			//UART_GSM_init();

			//low_power_mode();//enter low mode


			//init_timer32_0(); //re-init the counter
			//Timer32_setCount(TIMER32_0_MODULE,3000000);
			//Timer32_startTimer(TIMER32_0_MODULE, true);
		}
	}


}



