/*
 * SL_Display.h
 *
 *  Created on: Oct 25, 2015
 *      Author: Zach
 */

#ifndef SOLARITY_SIM800C_H_
#define SOLARITY_SIM800C_H_


void UART_PC_init();
void UART_GSM_init();
bool send_AT_command(char input[]);
bool check_for_OK();
void Clear_RX_Buffer();
void set_up_bearer_fido();
void set_up_bearer_rogers();
void Read_HTTP_Content();
void HTTP_Read_Done();
void Transmit_HTTP_Read();
void Open_Bearer_Connection();
void Close_Bearer_Connection();
void Init_HTTP_Service();
void End_HTTP_Service();
void Set_up_HTTP_Para();
void print_http_to_pc();
uint8_t * request_to_server();

#endif /* SOLARITY_SL_DISPLAY_H_ */
