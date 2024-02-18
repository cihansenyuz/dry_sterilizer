#include "adc_driver.h"

/* 
* Configurates GPIO port, pin and ADC to initiliaze
*
* @param adc desired ADC
* @param port GPIO port to be used
* @param pin GPIO pin to be used
* @return 1: successful, 0: given GPIO port/pin does not match with ADC channels
*/
char adc_init(char adc, short port, short pin)
{
	char channel;
	char result = 0;
	if(port == PA)
	{
		/*
		PA0 -> ADC12_IN0
		PA1 -> ADC12_IN1
		PA2 -> ADC12_IN2
		PA3 -> ADC12_IN3
		PA4 -> ADC12_IN4
		PA5 -> ADC12_IN5
		PA6 -> ADC12_IN6
		PA7 -> ADC12_IN7
		*/
		if(pin < 8)
		{
			result = 1;
			channel = pin; /* pin number matches with adc channel number */
		}
	}
	else if(port == PB)
	{
		/*
		PB0 -> ADC12_IN8
		PB1 -> ADC12_IN9
		*/
		if(pin<2)
		{
			result = 1;
			channel = 8 + pin; /* (pin number + 8) matches with adc channel number */
		}
	}
	else if(port == PC)
	{
		/*
		PC0 -> ADC12_IN10
		PC1 -> ADC12_IN11
		PC2 -> ADC12_IN12
		PC3 -> ADC12_IN13
		PC4 -> ADC12_IN14
		PC5 -> ADC12_IN15
		*/
		if(pin<6)
		{
			result = 1;
			channel = 10 + pin; /* (pin number + 10) matches with adc channel number */
		}
	}
	if (result == 1)
	{
		gpio_init(port,pin,IN,IN_AN);
		if(adc == adc1)
		{
			RCC->APB2ENR |= 0x201;
			ADC1->CR2 = 0;
			ADC1->SQR3 = channel;
			ADC1->CR2 |= 1;
			delayMS(100);
			ADC1->CR2 |= 1;
			ADC1->CR2 |= 2;
		}
		else if(adc == adc2)
		{
			RCC->APB2ENR |= 0x401;
			ADC2->CR2 = 0;
			ADC2->SQR3 = channel;
			ADC2->CR2 |= 1;
			delayMS(100);
			ADC2->CR2 |= 1;
			ADC2->CR2 |= 2;
		}
	}
	return result;
}
	
/* 
* Checks if value read from ADC is ready to be taken.
*
* @param adc desired ADC
* @param port GPIO port to be used
* @param pin GPIO pin to be used
* @return 1: data is ready, 0: not ready
*/
char adc_check(char adc, short port, short pin)
{
	char check = 0;
	if(adc == adc1)
	{
		if(ADC1->SR & 2) /* status register that tells the data is ready */
		{
			check = 1;
		}
	}
	else if(adc == adc2)
	{
		if(ADC2->SR & 2)
		{
			check = 1;
		}
	}
	return check;
}

/* 
* Gets the ADC value
*
* @param adc desired ADC
* @param port GPIO port to be used
* @param pin GPIO pin to be used
* @return 1: data is read, 0: data couldnot be read
* @note Use adc_check() function before to use this function to be sure that data is ready to be read.
*/
int adc_get(char adc, short port, short pin)
{
	int result = 0;
	int data = 0;
	if(adc == adc1)
	{
		data = ADC1->DR;
	}
	else if(adc == adc2)
	{
		data = ADC2->DR;
	}
	result = (data*1000/0xfff); /* to scale between 0-1000 */
	return result;
}


