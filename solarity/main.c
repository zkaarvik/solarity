/*
 * -------------------------------------------
 *    MSP432 DriverLib - v2_20_00_08 
 * -------------------------------------------
 *
 * --COPYRIGHT--,BSD,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/******************************************************************************
 * MSP432 Empty Project
 *
 * Description: An empty project that uses DriverLib
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
 * Author: 
*******************************************************************************/
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Sunlink Includes */
#include <SL_Display.h>

static volatile uint8_t RXData[10];
static volatile uint8_t i = 0;
static uint8_t TXData = 0;
static uint8_t ii = 0;

/* SPI Master Configuration Parameter */
const eUSCI_SPI_MasterConfig spiMasterConfig =
{
		EUSCI_B_SPI_CLOCKSOURCE_SMCLK,                // ACLK Clock Source
        24000000,                                     // ACLK = LFXT = 32.768khz
		2000000,                                    // SPICLK = 500khz
		EUSCI_B_SPI_MSB_FIRST,                       // MSB First
		EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
		EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW,    // Polarity
		EUSCI_B_SPI_3PIN                             // 3Wire SPI Mode
};

int main(void)
 {
	volatile uint32_t ii;

    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    //Set SMCLK = ~24MHz
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);

    /*Configure SPI Pins P1.5, P1.6, P1.7*/
	/* SPI --> P5.0 = CS, P1.5 = CLK, P1.6 = MOSI & P1.7 = MISO */
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
			GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
	GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN0); //CS PIN
	GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6); //EN PIN
	GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN2); // \BUSY pin

	GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0);
	GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);

	/* Configuring SPI in 3-wire master mode & enabling it & interrupts */
	SPI_initMaster(EUSCI_B0_MODULE, &spiMasterConfig);
	SPI_enableModule(EUSCI_B0_MODULE);
	SPI_enableInterrupt(EUSCI_B0_MODULE, EUSCI_B_SPI_RECEIVE_INTERRUPT);
	Interrupt_enableInterrupt(INT_EUSCIB0);
//	Interrupt_enableSleepOnIsrExit();

	/* Delay  */
	for(ii=0;ii<10000;ii++);

	GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
	//while(!(GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN2))) //Wait for BUSY rising edge
	for(ii=0;ii<10000;ii++);
	GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0);

	/* SPI, put CS low P5.0 and polling to see if the TX buffer is ready or busy */
	TXData = 0x24;
	while (!(SPI_getInterruptStatus(EUSCI_B0_MODULE,EUSCI_B_SPI_TRANSMIT_INTERRUPT)));
	SPI_transmitData(EUSCI_B0_MODULE, TXData);
	TXData = 0x01;
	while (!(SPI_getInterruptStatus(EUSCI_B0_MODULE,EUSCI_B_SPI_TRANSMIT_INTERRUPT)));
	SPI_transmitData(EUSCI_B0_MODULE, TXData);
	TXData = 0x00;
	while (!(SPI_getInterruptStatus(EUSCI_B0_MODULE,EUSCI_B_SPI_TRANSMIT_INTERRUPT)));
	SPI_transmitData(EUSCI_B0_MODULE, TXData);

	GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0);

	/* Delaying waiting for the slave to have response */
	for(ii=0;ii<10000;ii++);

	GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0);

	TXData = 0x00;
	while (!(SPI_getInterruptStatus(EUSCI_B0_MODULE,EUSCI_B_SPI_TRANSMIT_INTERRUPT)));
	SPI_transmitData(EUSCI_B0_MODULE, TXData);
	TXData = 0x00;
	while (!(SPI_getInterruptStatus(EUSCI_B0_MODULE,EUSCI_B_SPI_TRANSMIT_INTERRUPT)));
	SPI_transmitData(EUSCI_B0_MODULE, TXData);

	GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0);

    while(1)
    {
    	//TXData = 0x24;
		//while (!(SPI_getInterruptStatus(EUSCI_B0_MODULE,EUSCI_B_SPI_TRANSMIT_INTERRUPT)));
		//SPI_transmitData(EUSCI_B0_MODULE, TXData);
    }
}

void euscib0_isr(void)
{
    uint32_t status = SPI_getEnabledInterruptStatus(EUSCI_B0_MODULE);
    SPI_clearInterruptFlag(EUSCI_B0_MODULE, status);
    uint8_t i = 0;
    uint8_t data = 0;

    if(status & EUSCI_B_SPI_RECEIVE_INTERRUPT)
    {
    	data = SPI_receiveData(EUSCI_B0_MODULE);
        RXData[i++] = data;
        if ((i > 5) && (i % 9) == 0) {
             for( ii=0;ii<10;ii++);
             i = 0;
             //GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0);
        }

    }
}
