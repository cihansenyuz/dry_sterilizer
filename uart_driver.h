/**
  ******************************************************************************
  * @file    uart_driver.h
  * @author  Cihan Senyuz
  * @brief   Header for uart_driver.c file.
  *                 This file contains the common defines of the application.
  * 
  * 	uartManager has modes "process" and "bridge between uart ports". Firstly, you need
  * to select one of modes by setting mode variable. Then, select a strategy to recieve messages
  * by setting strategy variable. Terminator variable specifies the message end if strategy
  * is set to 1. If strategy is set to 0, timeConstant variable must be set to define how much
  * ms need to be waited until message is ended. If mode is set to 0 (meaning "process"), then
  * signal will raise once terminator recieved or timeConstant is over. You may process the 
  * message catching the signal. If mode is set to 1,2 or 3 (meaning "bridge"), once message
  * is recieved, message will be mirrored to uart1, uart2 or uart3 respectively.
  * 
  * 	uart pins
  * 	USART3 TX:PB10 RX:PB11
  * 	USART2 TX:PA2 RX:PA3
  * 	USART1 TX:PA9 RX:PA10
  * 
  * @note   Source code depends on stm32f10x.h, gp_driver.h and systick_delay.h libraries
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "gp_driver.h"
#include "systick_delay.h"

struct uartManager
{
	unsigned short count; 		/* counter to iterate through the message */
	unsigned short signal; 		/* to signal when message is recieved */
	unsigned short mode; 		/* 0: process /// 1/2/3: brigde to uart1/2/3 */
	unsigned short strategy; 	/* 1:terminator /// 0:interrupt */
	char terminator; 			/* to determine what char is the terminator when terminator strategy is set*/
	unsigned short timeConstant;/* to be used when interrupt strategy is set*/
	unsigned short timeCounter; /* counter for interrupt */
	char message[2000];			/* message holder */
};

void uart_send(unsigned short uart, char str[]);
void uart_ISR(unsigned short uart, struct uartManager* uartMan);
unsigned long USART_BRR(unsigned short usart, unsigned long BR);
void uart_init(unsigned short usart, unsigned long BR);
char uart_RX(unsigned short uart);
void uart_TX(unsigned short uart, char c);
void uart_message(unsigned short uart, char str[], struct uartManager* uartMan);
void systick_interrupt(struct uartManager* uart1Man, struct uartManager* uart2Man,struct uartManager* uart3Man);
void uart_TX_hex(unsigned short uart, unsigned short hex);
