#include "adc_driver.h"
#include "uart_driver.h"
#include "help_func.h"
#include "stdlib.h"

#define OFF_DURITION 1	// mins
#define ON_DURITION 1	// mins
#define ROOM_TEMP_REFERANCE 35 //40 23=40derece
#define HEAT_LED_REFERANCE 450 //250

#define NTC_PIN	1
#define LED_PIN	6
#define RELAY_SSR_PIN 11
#define ON_BUTTON_PIN 8
#define FAN_PIN 4

/* UART Configurations 
	USART2 TX:PA2 RX:PA3
	USART1 TX:PA9 RX:PA10 */
#define UART_PORT 2
#define UART_SPEED 115200

void setup(void);
unsigned short getTemp(void);
unsigned short heatLedIsOn(void);
void printValue(unsigned int);
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
	while(read_GP(PA, ON_BUTTON_PIN))		// wait while ON_BUTTON is not pushed
	{}
	
//		while(1)
//		{
//			write_GP(PA, RELAY_SSR_PIN, 1);	// turn the heater on
//			write_GP(PA, RELAY_SSR_PIN, 0);	// turn the heater off
//		}
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
	serialPort.mode = 0; 					/* 0: process /// 1/2/3: brigde to uart1/2/3 */
	serialPort.signal = 0;				/* message recieved signal */
	serialPort.strategy = 1; 			/* 1:terminator /// 0:interrupt */
}

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

void printValue(unsigned int value)
{
	valueStr = int2char(value);
	uart_send(UART_PORT, valueStr);
	uart_send(UART_PORT, "\n");
	free(valueStr);
}
