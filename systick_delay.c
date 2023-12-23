#include "systick_delay.h"
#include "stm32f10x.h"


void systick_init(void)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = 0x00FFFFFF;
	SysTick->VAL = 0;
	SysTick->CTRL = 5;
}

void createMili(void)
{
	SysTick->LOAD = 72000-1;
	SysTick->VAL = 0;
	while((SysTick->CTRL & 0x00010000) == 0);
}

void delayMS(unsigned long t)
{
	for(;t>0;t--)
	createMili();
}

void systick_interrup_start(void)
{
	__disable_irq();
	SysTick->CTRL = 0;
	SysTick->LOAD = 720000; /* to count every milisecond */
	SysTick->VAL = 0;
	SysTick->CTRL = 7; /* to initiate the interrup */
	__enable_irq();
}
