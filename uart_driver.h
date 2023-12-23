#include "stm32f10x.h"
#include "gp_driver.h"
#include "systick_delay.h"



/*
Definition of UART Manager
bit0 -> count
bit1 -> signal
bit2 -> process/bridge
bit3 -> terminator (1:terminator 0:interrupt)
bit4 -> terminator char
bit5 -> time cst
bit6 -> time counter
*/

struct uartManager
{
	unsigned short count;
	unsigned short signal;
	unsigned short mode; /* 0: process /// 1/2/3: brigde to uart1/2/3 */
	unsigned short strategy; /* 1:terminator /// 0:interrupt */
	char terminator;
	unsigned short timeConstant;
	unsigned short timeCounter;
	char message[2000];
};

void uart_send(unsigned short uart, char str[]);
void uart_ISR(unsigned short uart, struct uartManager* uartMan);
unsigned long USART_BRR(unsigned short usart, unsigned long BR);
void uart_init(unsigned short usart, unsigned long BR);
char uart_RX(unsigned short uart);
void uart_TX(unsigned short uart, char c);
void uart_message(unsigned short uart, char str[], struct uartManager* uartMan);
void systick_interrupt(struct uartManager* uart1Man);
void uart_TX_hex(unsigned short uart, unsigned short hex);

