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

/* Sunlink Includes */
#include <SL_Display.h>

int main(void)
 {
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    SL_D_init();

    /* Display Refresh */
    uint8_t cmdArr[3] = {0x24, 0x01, 0x00};
    SL_D_sendCmd(cmdArr, 3);

    while(1)
    {
    	__no_operation();
    }
}
