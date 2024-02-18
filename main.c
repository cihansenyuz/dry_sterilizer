#include "main.h"

struct uartManager serialPort;
//unsigned int heatUpCounter = 0;
char* valueStr;

int main(void)
{
	unsigned int offDurition = OFF_DURITION;
	unsigned int onDurition = ON_DURITION;
	unsigned int i;
	
	setup();
	
	uart_send(UART_PORT, "STM32 is ready. Push the button\n");
	while(read_GP(PA, ON_BUTTON_PIN))		
	{
		// wait until ON_BUTTON is pushed
	}

	while(1)
	{
		// begining of heat up period //
		write_GP(PA, RELAY_SSR_PIN, 1);	// turn the heater on
		uart_send(UART_PORT, "\nHeat up process started\n");
		write_GP(PC, 13, 0);
		
		while(heatLedIsOn())	// wait while heat led is on
		{
			delayMS(10000);
		}
		uart_send(UART_PORT, "Heat up process is over\n");
		// end of heat-up period //
		// target temp reached //
		
		// beginning of on-time durition //
		for(i=0; i < onDurition; i++)
		{
//			delayMS(60000);
		}
		// end of on-time durition //
		write_GP(PC, 13, 1);
		uart_send(UART_PORT, "Cool down process started\n");
		
		// begining of cool down period //
		write_GP(PA, RELAY_SSR_PIN, 0);	// turn the heater off
		write_GP(PA, FAN_PIN, 1);				// turn the fan on
//		delayMS(600000);	// wait for 10 mins
		
		
		while(getTemp() < ROOM_TEMP_REFERANCE)	// check every 5 mins if room temp is reached
		{
			delayMS(5000);
//			delayMS(300000);
		}
		// end of cool down period //
		// room temp reached //
		write_GP(PA, FAN_PIN, 0); // turn the fan off
		uart_send(UART_PORT, "Cool down process is over\n");
		
		// beginning of off-time durition //
		for(i=0; i < offDurition; i++){}
//			delayMS(60000);
		// end of off-time durition //
	}
	
	
	
	return 0;
}

/* 
* Configurates and initiliazes GPIOs, ADC and UART.
*
* @param none
* @return none
*/
void setup(void)
{
	systick_init();
	gpio_init(PC, 13, OUT50, OUT_GP_PP);
	write_GP(PC, 13, 1);
	gpio_init(PA, RELAY_SSR_PIN, OUT10, OUT_GP_PP);
	write_GP(PA, RELAY_SSR_PIN, 1);
	gpio_init(PA, FAN_PIN, OUT10, OUT_GP_PP);
	write_GP(PA, FAN_PIN, 0);
	gpio_init(PA, ON_BUTTON_PIN, IN, IN_PP);
	adc_init(adc1, PA, NTC_PIN);
	adc_init(adc2, PA, LED_PIN);
	uart_init(UART_PORT, UART_SPEED);
	delayMS(100);
	
	/* uart comfigurations */
	serialPort.mode = 0; 				/* 0: process /// 1/2/3: brigde to uart1/2/3 */
	serialPort.signal = 0;				/* message recieved signal */
	serialPort.strategy = 1; 			/* 1:terminator /// 0:interrupt */
}

/* 
* Reads ADC value 100 times in a second of NTC connected pin and calculates the average of reads.
*
* @param none
* @return average value of ADC
*/
unsigned short getTemp(void)
{
	unsigned int sumData = 0;
	unsigned int result;
	
	for(int i=0; i<100;){
		if(adc_check(adc1, PA, NTC_PIN)){
			i++;
			sumData += adc_get(adc1, PA, NTC_PIN);
		}
		delayMS(10);
	}
	result = sumData /100 ;
	printValue(result);
	return result;
}

/* 
* Reads ADC value 100 times in a second of LED connected pin and calculates the average of reads. Determines whether LED is on or off.
*
* @param none
* @return 1: heat LED is on, 0: off
*/
unsigned short heatLedIsOn(void)
{
	unsigned int sumData = 0;
	unsigned int result;
	
	for(int i=0; i<100;){
		if(adc_check(adc2, PA, LED_PIN)){
			i++;
			sumData += adc_get(adc2, PA, LED_PIN);
		}
		delayMS(10);
	}
	result = sumData /100 ;
	printValue(result);
	if(result > HEAT_LED_REFERANCE)
		return 1;
	else
		return 0;
}

/* 
* Prints given integer value to serial port
*
* @param value integer number to be printed
* @return none
*/
void printValue(unsigned int value)
{
	valueStr = int2char(value);
	uart_send(UART_PORT, valueStr);
	uart_send(UART_PORT, "\n");
	free(valueStr);
}
