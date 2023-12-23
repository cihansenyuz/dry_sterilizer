#include "stm32f10x.h"
#include "gp_driver.h"
#include "systick_delay.h"

#define adc1	1
#define adc2	2

/*
Setup

8.3.7 Enable alternate func. AFIO and the clock for ADC (RCC->APB2ENR)
Find ADC pins in datasheet 3. section table5
Configurate those pins as GPIO input (GPIOx_CRL or CRH)

11.12.2 set all registers zero to avoid any unnecessary setup (ADCx->CR2)
11.12.11 Setup the channel you are going to use (ADCx->SQR3)
11.12.3 Put 1 twice to turn on A/D converter ADON (ADCx->CR2)
11.12.3 Turn on continues conversation CONT (ADCx->CR2)

ADC check

11.12.1 Check status register to know end of conversion EOC (ADCx->SR)

ADC read

11.12.14 Read data register (ADCx->DR)
*/

char adc_init(char adc, short port, short pin);
char adc_check(char adc, short port, short pin);
int adc_get(char adc, short port, short pin);
