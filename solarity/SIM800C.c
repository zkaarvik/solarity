/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "printf.h"


#define RX_BUFFER_SIZE	1000  //RX_buffer size
#define DELAY_CHAR_SEND	96000 //delay time of 4 ms = 96000 * (1/24MHz)
#define DELAY_HTTP_READ 50 //86 us
#define HTTP_DATA_BUFFER 50000
#define DUMP_CHAR		31


/**/
bool HTTPFLAG_FLAG = 0;
int NumOfCharRecevied = 0;
int NumOfHttpData = 0;
static volatile uint8_t RXData[RX_BUFFER_SIZE];
static volatile uint8_t HTTPData[HTTP_DATA_BUFFER];

/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 115200 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 *http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
const eUSCI_UART_Config uartConfig =
{
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
	13,                                     // BRDIV = 13
    0,                                       // UCxBRF = 0
    37,                                       // UCxBRS = 37
    EUSCI_A_UART_NO_PARITY,                  // No Parity
    EUSCI_A_UART_LSB_FIRST,                  // MSB First
    EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
    EUSCI_A_UART_MODE,                       // UART mode
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
	//EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION //no-oversampling
};
void UART_PC_init()
{
	//Used for debugging purposes

	/* Selecting P1.2 and P1.3 in UART mode */
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
			GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

	/* Setting DCO to 24MHz */
	CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);

	/* Configuring UART Module */
	MAP_UART_initModule(EUSCI_A0_MODULE, &uartConfig);
	MAP_UART_enableModule(EUSCI_A0_MODULE);

}

void UART_GSM_init()
{

	/* Configure pins P3.2 and P3.3 in UART mode.*/
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
				   GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

	/* Setting DCO to 24MHz */
	CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);

	/* Configuring UART Module */
	MAP_UART_initModule(EUSCI_A2_MODULE, &uartConfig);
	MAP_UART_enableModule(EUSCI_A2_MODULE);

	/* Enabling interrupts */
	MAP_UART_enableInterrupt(EUSCI_A2_MODULE, EUSCI_A_UART_RECEIVE_INTERRUPT);
	MAP_Interrupt_enableInterrupt(INT_EUSCIA2);

	UART_selectDeglitchTime(EUSCI_A2_MODULE,EUSCI_A_UART_DEGLITCH_TIME_200ns);

	/* Enabling interrupts */
	//MAP_Interrupt_enableSleepOnIsrExit();
	MAP_Interrupt_enableMaster(); // allows the processor to respond to interrupts.
}


 //EUSCI A0 UART ISR - Echoes data back to PC host
void euscia0_isr(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_MODULE);

    MAP_UART_clearInterruptFlag(EUSCI_A0_MODULE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT)
    {
    	//MAP_UART_receiveData(EUSCI_A0_MODULE);
    	printf(EUSCI_A0_MODULE,"Interrupt!!!\r\n");
        //printf(EUSCI_A0_MODULE,"Byte Received: %x\n\r",data);
    }
}

/*EUSCI A2 UART ISR - Reads back the Command being sent and the response */
void euscia2_isr(void)
{

	uint8_t data = 0x00;

    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_MODULE);

    MAP_UART_clearInterruptFlag(EUSCI_A2_MODULE, status);

    //Goes into this loop when HTTPREAD is called
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT){
    	data = MAP_UART_receiveData(EUSCI_A2_MODULE);
    	//Store the data in the httpbuffer, we dont want to output the result to pc terminal b/c we may lose bytes along the way
    	if(HTTPFLAG_FLAG & (NumOfCharRecevied>DUMP_CHAR)){
    		HTTPData[NumOfHttpData++] = data;
    	}

    	//store the response in the RX buffer and echo back the response to pc for debugging
    	else{
    		MAP_UART_transmitData(EUSCI_A0_MODULE,data); //echo back to PC
    		RXData[NumOfCharRecevied++] = data;
    	}
    }

}

/*
 void euscia2_isr(void)
{

	uint8_t data = 0x00;

    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_MODULE);

    MAP_UART_clearInterruptFlag(EUSCI_A2_MODULE, status);


    //Goes into this loop when HTTPREAD is called
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT & HTTPFLAG_FLAG & (NumOfCharRecevied>DUMP_CHAR))
	{
    	int i=0;

    	for(i=0;i<1;i++){

    		data = MAP_UART_receiveData(EUSCI_A2_MODULE);
			//MAP_UART_transmitData(EUSCI_A0_MODULE,data); //echo back to PC for debugging purposes
			HTTPData[NumOfHttpData++] = data;
    	}
	}

    else if(status & EUSCI_A_UART_RECEIVE_INTERRUPT)
    {
    	data = MAP_UART_receiveData(EUSCI_A2_MODULE);
    	MAP_UART_transmitData(EUSCI_A0_MODULE,data); //echo back to PC for debugging purposes
    	RXData[NumOfCharRecevied++] = data;
    }

}
 */


//This function is to check for the "OK\r\n" in the RX_buffer
bool check_for_OK(void){
	size_t len = strlen(RXData);
	const uint8_t O_char = 'O';
	const uint8_t K_char = 'K';
	const uint8_t CarRet_char = 0x0D;
	const uint8_t NewLine_char = 0x0A;
	int i=0;

	//Checks for "OK\r\n" meesage in the RX buffer
	for(i=0;i<len;i++){
		if(RXData[i]== O_char){
			if(RXData[i+1]== K_char){
				if(RXData[i+2]== CarRet_char){
					if(RXData[i+3]== NewLine_char){
						return true; //return True if there was an OK from AT command
					}
				}
			}

		}
	}
	return false; //return False if there was no OK response from AT command
}


// This function is used to clear the RX buffer
void Clear_RX_Buffer(void){
	int i=0;
	for(i=0;i<RX_BUFFER_SIZE;i++){
		RXData[i] = 0x00;
	}
}

void Clear_HTTP_buffer(void){
	int i=0;
	for(i=0; i<HTTP_DATA_BUFFER; i++){
		HTTPData[i] = 0x00;
	}
}



/*Send the AT command to the SIM800C GSM Module
 * Input: String of the Command to sent
 * Returns:
 * 		1 - AT command was successful
 * 		0 - AT command was not successful
*/

bool send_AT_command(char input[]){

	size_t len = strlen(input);
	NumOfCharRecevied = 0;
	Clear_RX_Buffer(); //Clear the RX buffer before sending Command

	__delay_cycles(DELAY_CHAR_SEND*100);

	//Sending the AT command one character at a time
	int i = 0;
	for(i=0;i<len;i++){
		MAP_UART_transmitData(EUSCI_A2_MODULE,input[i]);
		__delay_cycles(DELAY_CHAR_SEND); //delay time before sending the next char
	}

	/*Must send a carriage return and new life character to tell GSM
	module the end of the command
	*/
	MAP_UART_transmitData(EUSCI_A2_MODULE,'\r'); //send the \r
	__delay_cycles(DELAY_CHAR_SEND);
	MAP_UART_transmitData(EUSCI_A2_MODULE,'\n'); // send the \n


	__delay_cycles(DELAY_CHAR_SEND*10); //wait for the OK response

	if(check_for_OK()) return true;
	else return false;
}

//This function set up the APN with a fido network carrier
void set_up_bearer_fido(void){
	 //char ATcommand[] = "AT+SAPBR=3,1,APN,internet.fido.ca";
	 send_AT_command("AT+SAPBR=3,1,APN,internet.fido.ca");

	 send_AT_command("AT+SAPBR=3,1,USER,fido");

	 send_AT_command("AT+SAPBR=3,1,PWD,fido");
}

//This function set up the APN with a roger network carrier
void set_up_bearer_rogers(void){
	 send_AT_command("AT+SAPBR=3,1,APN,internet.com");

	 send_AT_command("AT+SAPBR=3,1,USER,wapuser1");

	 send_AT_command("AT+SAPBR=3,1,PWD,wap");
}


void Read_HTTP_Context(void){
	//Clear_RX_Buffer();//clear the buffer
	size_t len = 0;
	send_AT_command("AT+HTTPACTION=0");

	/*
	 * This delay is very important, it will wait for the response
	 * that indicates that the GSM has finish
	 * reading the website. The amount of time depends if the website has
	 * a large amount of data.
	 * TOD0: add a timeout, dont want to be in this loop forever
	 * TOD0: check for error code?
	 */


	while(len<41){
		len = strlen(RXData);
	}

}

/*This function reads all the data from the HTTPACTION=0 or HTTPDATA command
 * CAN ONLY CALL THIS ACTION AFTER A HTTPACTION=0 OR HTTPDATA AT COMMAND WAS EXECUTED PRIOR
 */
void Transmit_HTTP_Read(void){
	//Clear the http buffer
	NumOfHttpData = 0;
	Clear_HTTP_buffer();

	HTTPFLAG_FLAG =1 ;
	send_AT_command("AT+HTTPREAD");

	int i= 0;


	/*
	 * IMPORTANT: delay to allow the GSM to send all the httpdata to the microcontroller
	 * if the delay isnt enough then the http data is going to be lost
	 */

	for(i=0;i<2000;i++)__delay_cycles(DELAY_CHAR_SEND);
}


//This function opens the APN connection
//MUST CALL THIS FUNCTION TO GET THE GSM TO ENTER GRPS MODE
void Open_Bearer_Connection(void){
	send_AT_command("AT+SAPBR=1,1");


	//To check that the gsm is connected
	send_AT_command("AT+SAPBR=2,1");
}

//This function closes the APN connection
void Close_Bearer_Connection(void){
	send_AT_command("AT+SAPBR=0,1");
	__delay_cycles(DELAY_CHAR_SEND*100);

	//debugging purposes
	printf(EUSCI_A0_MODULE,"Num of http data read: %i",NumOfHttpData);
}

/*
 * This function starts the HTTP service
 * MUST CALL THIS FUNCTION TO START ANY HTTP ACTION
*/
void Init_HTTP_Service(void){
	send_AT_command("AT+HTTPINIT");
}

//This function ends the HTTP service
void End_HTTP_Service(void){
	//HTTPFLAG_FLAG =0;
	send_AT_command("AT+HTTPTERM");
}
//This function set up the HTTP address to read from
void Set_up_HTTP_Para(void){
	send_AT_command("AT+HTTPPARA=URL,http://192.241.210.28:3000/api/image/whoooo"); // setting the httppara, the second parameter is the website you want to access
	//send_AT_command("AT+HTTPPARA=URL,http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html");
	//send_AT_command("AT+HTTPPARA=URL,http://www.bcit.ca");
	//send_AT_command("AT+HTTPPARA=CID,1");
}



/*
void print_http_to_pc(void){
	int i =0;
	for(i=0;i<48016;i++){
		MAP_UART_transmitData(EUSCI_A0_MODULE,HTTPData[i]);
		//__delay_cycles(DELAY_CHAR_SEND); //delay time before sending the next char
	}
}
*/
