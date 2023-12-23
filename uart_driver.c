#include "uart_driver.h"
#include "systick_delay.h"

void uart_init(unsigned short usart, unsigned long BR)
{
	/*
	USART2 TX:PA2 RX:PA3
	USART1 TX:PA9 RX:PA10 */
	
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
1- define my uart
2- is it bridge or process or both
if process : String to fullfill, counter, signal
*/

void uart_ISR(unsigned short uart, struct uartManager* uartMan)
{
	if(uartMan->mode == 0)
	{
		uartMan->message[uartMan->count] = uart_RX(uart);
		if(uartMan->strategy)
		{	
			/* --- terminator strategy --- */
			if(uartMan->message[uartMan->count] == uartMan->terminator)
			{
				uartMan->message[uartMan->count] = '\0';
				uartMan->count = 0;
				uartMan->signal = 1;
			}
			else
			{
				uartMan->count++;
			}/* -------------------------- */
		}
		else
		{
			/* timer strategy (interrupt) */
			uartMan->count++;
			uartMan->timeCounter = uartMan->timeConstant;
			
			systick_interrup_start();
		}
	}
	else
	{
		uart_TX(uartMan->mode,uart_RX(uart));
	}
}

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

void systick_interrupt(struct uartManager* uart1Man)
{
	if(uart1Man->count != 0)
	{
		if(uart1Man->timeCounter == 0)
		{
			uart1Man->count = 0;
			uart1Man->signal = 1;
			uart1Man->timeConstant = 0;
			systick_init(); /* to stop interrupt */
		}
		else
		{
			uart1Man->timeCounter--;
		}
	}/*
	else if(uart2Man->count != 0)
	{
		if(uart2Man->timeCounter == 0)
		{
			uart2Man->count = 0;
			uart2Man->signal = 1;
			uart2Man->timeConstant = 0;
			systick_init(); // to stop interrupt
		}
		else
		{
			uart2Man->timeCounter--;
		}
	}
	else if(uart3Man->count != 0)
	{
		if(uart3Man->timeCounter == 0)
		{
			uart3Man->count = 0;
			uart3Man->signal = 1;
			uart3Man->timeConstant = 0;
			systick_init(); // to stop interrupt
		}
		else
		{
			uart3Man->timeCounter--;
		}
	}*/
}
