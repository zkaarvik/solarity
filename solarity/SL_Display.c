/*
 * SL_Display.c
 *
 *  Created on: Oct 25, 2015
 *      Author: Zach
 */

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* DriverLib Includes */
#include "driverlib.h"

//static volatile uint8_t RXData[10];

/* SPI Master Configuration Parameter */
const eUSCI_SPI_MasterConfig spiMasterConfig =
{
		EUSCI_B_SPI_CLOCKSOURCE_SMCLK,                	// SMCLK Clock Source
        24000000,                                     	// SMCLK = 24Mhz
		2000000,                                    	// SPICLK = 2MHz
		EUSCI_B_SPI_MSB_FIRST,                       	// MSB First
		EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
		EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW,    	// Polarity
		EUSCI_B_SPI_3PIN                             	// 3Wire SPI Mode
};

void SL_D_init()
{
	//Set SMCLK = ~24MHz
	CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);

	/*Configure SPI Pins P1.5, P1.6, P1.7*/
	/* SPI --> P5.0 = CS, P1.5 = CLK, P1.6 = MOSI & P1.7 = MISO */
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
			GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
	GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN0); //CS PIN
	GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6); //EN PIN
	GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN2); // BUSY pin

	/* Configuring SPI in 3-wire master mode & enabling it & interrupts */
	SPI_initMaster(EUSCI_B0_MODULE, &spiMasterConfig);
	SPI_enableModule(EUSCI_B0_MODULE);
	SPI_enableInterrupt(EUSCI_B0_MODULE, EUSCI_B_SPI_RECEIVE_INTERRUPT);
	Interrupt_enableInterrupt(INT_EUSCIB0);
	//Interrupt_enableSleepOnIsrExit();

	/* Begin Initialization, first set CS and EN inactive */
	GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0); //CS
	GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6); //EN

	/* Activate EN */
	GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6); //EN

	/* Delay 3ms(T_startup) then wait for busy signal to rise (T_init)  */
	__delay_cycles(158000); //3ms @ 48Mhz = 144000 cycles
	while(!(GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN2))); //Wait for BUSY rising edge

	/* Initialization complete. Display is ready to receive commands */
}

void SL_D_sendByte(uint8_t TXData)
{
	/* Polling to see if the TX buffer is ready or busy */
	while (!(SPI_getInterruptStatus(EUSCI_B0_MODULE,EUSCI_B_SPI_TRANSMIT_INTERRUPT)));
	/* Transmit byte to display */
	SPI_transmitData(EUSCI_B0_MODULE, TXData);
}

void SL_D_sendCmd(uint8_t cmdArr[], uint8_t cmdSize)
{
	uint8_t i; //Max cmd size is 255, so uint8_t is an acceptable type

	/* Begin command, activate CS */
	GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0); //CS
	/* 6.0us (T_S) between CS low and first bit */
	__delay_cycles(320); //6us @ 48Mhz = 288 cycles

	/*Send command*/
	for(i=0; i<cmdSize; i++)
	{
		SL_D_sendByte(cmdArr[i]);
	}

	/* SPI Command Transfer complete, 11us (T_E) delay before CS high (inactive) */
	__delay_cycles(560); //11us @ 48Mhz = 528 cycles
	GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0); //CS

	/* TCM is busy processing command. Wait for T_A + min T_BUSY = 26us, then wait for busy signal to deactivate, then wait T_NS 2us */
	__delay_cycles(1300); //26us @ 48Mhz = 1248 cycles
	while(!(GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN2))); //BUSY
	__delay_cycles(120); //2us @ 48Mhz = 96 cycles

	/* After command processing, receive response. Activate CS then wait before sending initial bit (T_S) */
	GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0); //CS
	__delay_cycles(320); //6us @ 48Mhz = 288 cycles

	/* Send dummy bits, while receiving response */
	SL_D_sendByte(0x00);
	SL_D_sendByte(0x00);

	/* Response received, deactivate CS after T_E (11us), communication finished */
	__delay_cycles(560); //11us @ 48Mhz = 528 cycles
	GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0);
}

void euscib0_isr(void)
{
	uint8_t data = 0x00;
    uint32_t status = SPI_getEnabledInterruptStatus(EUSCI_B0_MODULE);
    SPI_clearInterruptFlag(EUSCI_B0_MODULE, status);

    if(status & EUSCI_B_SPI_RECEIVE_INTERRUPT)
    {
    	data = SPI_receiveData(EUSCI_B0_MODULE);
        //RXData[i++] = data; //Not currently retaining received data..
    }
}
