/**
  ******************************************************************************
  * @file    main.h
  * @author  Cihan Senyuz
  * @brief   Header for main.c file.
  *                 This file contains the common defines of the application.
  *     
  * 	
  * 
  * @note   Source code depends on adc_driver.h, uart_driver.h, help_func.h, 
  * and stdlib.h libraries
  ******************************************************************************
  */

#include "adc_driver.h"
#include "uart_driver.h"
#include "help_func.h"
#include "stdlib.h"

#define OFF_DURITION 1	// mins
#define ON_DURITION 1	// mins
#define ROOM_TEMP_REFERANCE 35 //40 23=40derece
#define HEAT_LED_REFERANCE 450 //250

#define NTC_PIN	1       // NTC to read ambient temprature in the car
#define LED_PIN	6       // LED of the car to read if heater is on or off
#define RELAY_SSR_PIN 11// Relay to open or close the circuit of the heater
#define ON_BUTTON_PIN 8 // Button to begin the test
#define FAN_PIN 4       // Fan controller to push cool air inside the car

/* UART Configurations 
	USART2 TX:PA2 RX:PA3
	USART1 TX:PA9 RX:PA10
*/
#define UART_PORT 2
#define UART_SPEED 115200

void setup(void);
unsigned short getTemp(void);
unsigned short heatLedIsOn(void);
void printValue(unsigned int);
