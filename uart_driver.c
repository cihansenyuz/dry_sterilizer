#include "uart_driver.h"

/* 
* Configurates selected uart with interrupt and selected baud rate
*
* @param usart selected uart port
* @param BR selected baud rate
* @return none
*/
void uart_init(unsigned short usart, unsigned long BR)
{
	/* if using USART1 clock speed is 72MHz, else 36MHz */
	unsigned long BRR_calc;
	
	BRR_calc = USART_BRR(usart,BR);
	
	/* Enable the alternate func for PINs */
	RCC->APB2ENR |= 0x01; /* 0001 */
	
	if (usart == 1)
	{
		__disable_irq(); /* for uart interrupt */
	/* Enable UART */
	RCC->APB2ENR |= 0x4000; /* uart1 enabled */
	/* Enable related PINs */ 
	gpio_init(PA,9,OUT50,OUT_AF_PP);
	gpio_init(PA,10,IN,IN_PP);
	/* Setup the baude rate */
	USART1->BRR = BRR_calc;
	/* Enable Uart transmit */
	USART1->CR1 |= 0x08;
	/* Enable Uart recive */
	USART1->CR1 |= 0x04;
	/* Enable Uart */
	USART1->CR1 |= 0x2000;
	/* this part is for uart interrupt setup */
	USART1->CR1 |= 0x20;
		NVIC_EnableIRQ(USART1_IRQn);
		__enable_irq();
	}
	else if (usart == 2)
	{
		__disable_irq();
	RCC->APB1ENR |= 0x20000;
	gpio_init(PA,2,OUT50,OUT_AF_PP);
	gpio_init(PA,3,IN,IN_PP);
	USART2->BRR = BRR_calc;
	USART2->CR1 |= 0x08;
	USART2->CR1 |= 0x04;
	USART2->CR1 |= 0x2000;
	USART2->CR1 |= 0x20;
		NVIC_EnableIRQ(USART2_IRQn);
		__enable_irq();
	}
	/*else if (usart == 3)
	{
		__disable_irq();
	RCC->APB1ENR |= 0x40000;
	gpio_init(PB,10,OUT50,OUT_AF_PP);
	gpio_init(PB,11,IN,IN_PP);
	USART3->BRR = BRR_calc;
	USART3->CR1 |= 0x08;
	USART3->CR1 |= 0x04;
	USART3->CR1 |= 0x2000;
	USART3->CR1 |= 0x20;
		NVIC_EnableIRQ(USART3_IRQn);
		__enable_irq();
	}*/
}

/* 
* Calculates BBR value to meet selected baud rate
*
* @param usart selected uart port
* @param BR selected baud rate
* @return calculated BBR value
*/
unsigned long USART_BRR(unsigned short usart, unsigned long BR)
{
	unsigned long div = 36000000;
	unsigned long dec;
	unsigned long final;
	double frac = 36000000.00;
	double frac2 = 1.00;
	
	if(usart == 1)
	{
	div = 72000000;
	frac = 72000000.00;
	}
	div = div / (BR*16);
	frac = 16*((frac / (BR*16)) - div);
	dec = frac;
	frac2 = 100*(frac-dec);
	if (frac2>50)
	{
		dec++;
		if(dec == 16)
		{
			dec = 0;
			div++;
		}
	}
	final = (div<<4);
	final += dec;
	
	return final;
}

/* 
* Reads one byte from the selected uart port
*
* @param usart selected uart port
* @return recieved char 
*/
char uart_RX(unsigned short uart)
{
	char c;
	if(uart == 1)
	{
		while ((USART1->SR & (0x20)) == 0x00) /* check status register until is High to know data is ready to be read */
		{}
		c = USART1->DR;
		
	}else if(uart == 2)
	{
		while ((USART2->SR & (0x20)) == 0x00) /* check status register until is High to know data is ready to be read */
		{}
		c = USART2->DR;
		
	} else if(uart == 3)
	{
		while ((USART3->SR & (0x20)) == 0x00) /* check status register until is High to know data is ready to be read */
		{}
		c = USART3->DR;

	}
return c;
}

/* 
* Writes a char to the selected uart port
*
* @param usart selected uart port
* @param c char to send
* @return none
*/
void uart_TX(unsigned short uart, char c)
{
	if(uart == 1)
	{
		while ((USART1->SR & (0x80)) == 0x00) /* check status register until is High to know data is ready to be read */
		{}
		USART1->DR = c;
		
	}else if(uart == 2)
	{
		while ((USART2->SR & (0x80)) == 0x00) /* check status register until is High to know data is ready to be read */
		{}
		USART2->DR = c;
		
	} else if(uart == 3)
	{
		while ((USART3->SR & (0x80)) == 0x00) /* check status register until is High to know data is ready to be read */
		{}
		USART3->DR = c;
	}
}

/* 
* Writes one byte to the selected uart port
*
* @param usart selected uart port
* @param hex byte in hexadecimal to be send
* @return none
*/
void uart_TX_hex(unsigned short uart, unsigned short hex)
{
	if(uart == 1)
	{
		while ((USART1->SR & (0x80)) == 0x00) /* check status register until is High to know data is ready to be read */
		{}
		USART1->DR = hex;
		
	}else if(uart == 2)
	{
		while ((USART2->SR & (0x80)) == 0x00) /* check status register until is High to know data is ready to be read */
		{}
		USART2->DR = hex;
		
	} else if(uart == 3)
	{
		while ((USART3->SR & (0x80)) == 0x00) /* check status register until is High to know data is ready to be read */
		{}
		USART3->DR = hex;
	}
}

/* 
* Provides uart message management according to mode, strategy, terminator, timeConstant sets once there is uart interrupt
*
* @param usart selected uart port
* @param uartMan uart configuration sets
* @return none
*/
void uart_ISR(unsigned short uart, struct uartManager* uartMan)
{
	/* Check which mode is set */
	if(uartMan->mode == 0) 
	{	
		/* --- mode: process --- */
		uartMan->message[uartMan->count] = uart_RX(uart); /* get message byte by byte */
		/* Check which strategy is set */
		if(uartMan->strategy)
		{	
			/* --- strategy: terminator --- */
			/* Check if it is the end of the message */
			if(uartMan->message[uartMan->count] == uartMan->terminator)
			{
				uartMan->message[uartMan->count] = '\0'; /* overwrite the terminator char with null */
				uartMan->count = 0;
				uartMan->signal = 1;
			}
			else
			{

				uartMan->count++;
			}
			/* ----------------------------- */
		}
		else
		{
			/* --- strategy: interrupt --- */
			uartMan->count++;
			uartMan->timeCounter = uartMan->timeConstant;
			systick_interrup_start();
			/* ---------------------------- */
		}
		/* ------------------------ */
	}
	else 
	{	
		/* --- mode: bridge --- */
		uart_TX(uartMan->mode,uart_RX(uart)); /* just mirror the message */
		/* -------------------- */
	}
}

/* 
* Writes all bytes given as string to the selected uart port
*
* @param usart selected uart port
* @param str string to be send
* @return none
*/
void uart_send(unsigned short uart, char str[])
{
	int i = 0;
	while(str[i] !='\0')
	{
		uart_TX(uart,str[i]);
		i++;
		delayMS(1);
	}
}

/* 
* Writes all bytes given as string to the selected uart port and gives some interval for a reply
*
* @param usart selected uart port
* @param str string to be send
* @param uartMan uart configuration sets
* @return none
* @note if time interval is not enough, change timeOut variable manually
*/
void uart_message(unsigned short uart, char str[], struct uartManager* uartMan)
{
	unsigned long timeOut = 120000000;
	uart_send(uart,str);
	while(uartMan->signal == 0 & timeOut != 0)
	{
		timeOut--;
	}
	uartMan->signal = 0;
}

/* 
* Creates delay according to uart manager time constant
*
* @param uartMan uart configuration sets
* @return none
*/
void systick_interrupt(struct uartManager* uartMan)
{
	if(uartMan->count != 0)
	{
		if(uartMan->timeCounter == 0)
		{
			uartMan->count = 0;
			uartMan->signal = 1;
			uartMan->timeConstant = 0;
			systick_init(); /* to stop interrupt */
		}
		else
		{
			uartMan->timeCounter--;
		}
	}
}
