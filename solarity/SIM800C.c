/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "printf.h"


#define RX_BUFFER_SIZE	200  //RX_buffer size
#define DELAY_CHAR_SEND	96000 //delay time of 4 ms = 96000 * (1/24MHz)
#define HTTP_DATA_BUFFER 50000
#define DUMP_CHAR		31


/*********************************************************************************************
 * Flag Variables
 *********************************************************************************************/
bool HTTPFLAG_FLAG = 0;

/*********************************************************************************************
 * Counter Variables
 *********************************************************************************************/
int NumOfCharRecevied = 0; //to keep track of how much character was recevied per AT command
int NumOfHttpData = 0;  //to keep track of how much http data was transferred from GSM to microcontoller

/*********************************************************************************************
 * Buffers to store information/data
 *********************************************************************************************/
static volatile uint8_t RXData[RX_BUFFER_SIZE]; //buffer to store the response of AT commands
static volatile uint8_t HTTPData[HTTP_DATA_BUFFER]; // buffer to store the http data



/* *****************************************************************************************************
 * UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 115200 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 *http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 ********************************************************************************************************/
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

/* *****************************************************************************************************
 * This function is to init the PC UART port to the configured baud rate
 * Used for debugging purposes
 ********************************************************************************************************/
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

/* *****************************************************************************************************
 * This function is to init pins 3.2 and 3.3 to the configured baud rates to communicate
 * with the GSM module
 * pin 3.2 -> RX of the GSM module
 * pin 3.3 -> TX of the GSM module
 ********************************************************************************************************/
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

	/**/
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
    	//printf(EUSCI_A0_MODULE,"Interrupt!!!\r\n");
        //printf(EUSCI_A0_MODULE,"Byte Received: %x\n\r",data);
    }
}
/* *****************************************************************************************************
 * The interrupt when the RXxBuf of EUSCI A2 gets a character from the GSM module
 * Store the response in the RXbuffer
 * Store the response in the HTTPBuffer if the http flag is set
 ********************************************************************************************************/
void euscia2_isr(void)
{

	uint8_t data = 0x00;

    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_MODULE);

    MAP_UART_clearInterruptFlag(EUSCI_A2_MODULE, status);

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


/* *****************************************************************************************************
 * This function is called to check that there was an OK reponse from the AT command
 * Should be called after a certain delay after an AT command was executed to allow time for response
 *
 * INPUT: None
 *
 * RETURNS:
 * 	- True: Found an OK in the RX buffer
 * 	- False: Did not found an OK in the RX buffer, assumed to be ERROR msg
 ********************************************************************************************************/
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


/* *****************************************************************************************************
* This function clears the RX buffer
********************************************************************************************************/
void Clear_RX_Buffer(void){
	int i=0;
	for(i=0;i<RX_BUFFER_SIZE;i++){
		RXData[i] = 0x00;
	}
}


/* *****************************************************************************************************
* This function clears the HTTPData buffer
********************************************************************************************************/
void Clear_HTTP_buffer(void){
	int i=0;
	for(i=0; i<HTTP_DATA_BUFFER; i++){
		HTTPData[i] = 0x00;
	}
}


/* *****************************************************************************************************
* This function sends the AT command to the GSM module
*
* INPUT:
* 	- char input[]: The string of the AT command to execute
*
* Returns:
* 	-True: AT command was successful
* 	-False: AT command was not successful
********************************************************************************************************/
bool send_AT_command(char input[]){

	size_t len = strlen(input);
	NumOfCharRecevied = 0; //reset the counter
	Clear_RX_Buffer(); //Clear the RX buffer before sending Command

	//__delay_cycles(DELAY_CHAR_SEND*100);

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


/* *****************************************************************************************************
* This function sets up the APN for an Fido network carrier
********************************************************************************************************/
void set_up_bearer_fido(void){

	//delay before sending next command
	 send_AT_command("AT+SAPBR=3,1,APN,internet.fido.ca");
	 __delay_cycles(DELAY_CHAR_SEND*100);
	 send_AT_command("AT+SAPBR=3,1,USER,fido");
	 __delay_cycles(DELAY_CHAR_SEND*100);
	 send_AT_command("AT+SAPBR=3,1,PWD,fido");
	 __delay_cycles(DELAY_CHAR_SEND*100);
}

/* *****************************************************************************************************
* This function sets up the APN for an Rogers network carrier
********************************************************************************************************/
void set_up_bearer_rogers(void){
	 send_AT_command("AT+SAPBR=3,1,APN,internet.com");
	 __delay_cycles(DELAY_CHAR_SEND*100);
	 send_AT_command("AT+SAPBR=3,1,USER,wapuser1");
	 __delay_cycles(DELAY_CHAR_SEND*100);
	 send_AT_command("AT+SAPBR=3,1,PWD,wap");
	 __delay_cycles(DELAY_CHAR_SEND*100);
}

/* *****************************************************************************************************
* This function execute the AT+HTTPACTION=0 command, which reads the data from the assigned URL
********************************************************************************************************/
void Read_HTTP_Content(void){
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


/* *****************************************************************************************************
* This function reads all the data from the HTTPACTION=0 or HTTPDATA command
* CAN ONLY THIS FUNCTION AFTER A HTTPACTION=0 or HTTPDATA AT COMMAND WAS EXECUTED PRIOR
********************************************************************************************************/
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

/* *****************************************************************************************************
* This function opens the APN connection
* MUST CALL THIS FUNCTION TO GET THE GSM TO ENTER GRPS MODE
********************************************************************************************************/
void Open_Bearer_Connection(void){
	send_AT_command("AT+SAPBR=1,1");

	//To check that the gsm is connected
	//send_AT_command("AT+SAPBR=2,1");
}

/* *****************************************************************************************************
* This function closes the APN connection
********************************************************************************************************/
void Close_Bearer_Connection(void){
	send_AT_command("AT+SAPBR=0,1");
	__delay_cycles(DELAY_CHAR_SEND*100);

	//debugging purposes
	printf(EUSCI_A0_MODULE,"Num of http data read: %i",NumOfHttpData);
}

/* *****************************************************************************************************
* This function starts the HTTP service
* MUST CALL THIS FUNCTION TO START ANY HTTP-RELATED AT COMMANDS
********************************************************************************************************/
void Init_HTTP_Service(void){
	send_AT_command("AT+HTTPINIT");
}

/* *****************************************************************************************************
* This funtions ends the HTTP service
********************************************************************************************************/
void End_HTTP_Service(void){
	//HTTPFLAG_FLAG =0;
	send_AT_command("AT+HTTPTERM");
}

/* *****************************************************************************************************
* This function sets up the HTTP address to read from
********************************************************************************************************/
void Set_up_HTTP_Para(void){
	send_AT_command("AT+HTTPPARA=URL,http://192.241.210.28:3000/api/image/whoooo"); // setting the httppara, the second parameter is the website you want to access
	//send_AT_command("AT+HTTPPARA=URL,http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html");
	//send_AT_command("AT+HTTPPARA=URL,http://www.bcit.ca");
	//send_AT_command("AT+HTTPPARA=CID,1");
}

/* *****************************************************************************************************
* This function set the GSM module to connect to the internet then connect to the server
* and grabs the data and store it in the HTTPBuffer array
*
* INPUTS: None
*
* RETURNS:
* 	-	pointer to the address of the HTTPDATA buffer
********************************************************************************************************/
uint8_t * request_to_server(void){


	set_up_bearer_rogers();
	__delay_cycles(DELAY_CHAR_SEND*100);
	Open_Bearer_Connection();
	__delay_cycles(DELAY_CHAR_SEND*100);
	Init_HTTP_Service();
	__delay_cycles(DELAY_CHAR_SEND*100);
	Set_up_HTTP_Para();
	__delay_cycles(DELAY_CHAR_SEND*100);

	//call the action
	Read_HTTP_Content();
	__delay_cycles(DELAY_CHAR_SEND*100);

	Transmit_HTTP_Read();
	__delay_cycles(DELAY_CHAR_SEND*100);
	End_HTTP_Service();
	__delay_cycles(DELAY_CHAR_SEND*100);
	Close_Bearer_Connection();
	__delay_cycles(DELAY_CHAR_SEND*100);

	return HTTPData;
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
