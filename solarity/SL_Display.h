/*
 * SL_Display.h
 *
 *  Created on: Oct 25, 2015
 *      Author: Zach
 */

#ifndef SOLARITY_SL_DISPLAY_H_
#define SOLARITY_SL_DISPLAY_H_


void SL_D_init();
void SL_D_sendByte(uint8_t);
void SL_D_sendCmd(uint8_t[], uint8_t);
void SL_D_uploadImgData(uint8_t[], uint8_t);


#endif /* SOLARITY_SL_DISPLAY_H_ */
