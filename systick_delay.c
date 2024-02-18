#include "systick_delay.h"

/* 
* sets control registers of system ticker, reload value of ticker
*
* @param none
* @return none
*/
void systick_init(void)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = 0x00FFFFFF;
	SysTick->VAL = 0;
	SysTick->CTRL = 5;
}

/* sets reload value of ticker to achieve 1 milisecond
* 
* @param none
* @return none
* 
* @note needs systick_init() have called firstly
*/
void createMili(void)
{
	SysTick->LOAD = 72000-1;
	SysTick->VAL = 0;
	while((SysTick->CTRL & 0x00010000) == 0);
}

/* creates delay in miliseconds using createMili()
* 
* @param t desired number of milisecond
* @note needs systick_init() have called firstly
*/
void delayMS(unsigned long t)
{
	for(;t>0;t--)
	createMili();
}

/* enables the systick interrupt by setting control registers of system ticker
* and reloading value of ticker
*
* @param none
* @return none
*/
void systick_interrup_start(void)
{
	__disable_irq();
	SysTick->CTRL = 0;
	SysTick->LOAD = 720000; /* to count every milisecond */
	SysTick->VAL = 0;
	SysTick->CTRL = 7; /* to initiate the interrup */
	__enable_irq();
}
