
/**
  ******************************************************************************
  * @file    gp_driver.h
  * @author  Cihan Senyuz
  * @brief   Header for gp_driver.c file.
  *                   This file contains the common defines of the application.
  * 
  * @note    All adresses are from the user manual on 3.3 Memory Map section
  ******************************************************************************
  */

/* Reset and clock control RCC register adress: 0x4002 1000 */
/* APB2 peripheral clock enable register (RCC_APB2ENR) offset is 0x18 */
#define gpioClockEnable (*((volatile unsigned long *) 0x40021018)) /* 0x18 added to offset */

/* GPIO adresses */
#define gpioA (*((volatile unsigned long *) 0x40010800))
#define gpioB (*((volatile unsigned long *) 0x40010C00))
#define gpioC (*((volatile unsigned long *) 0x40011000))

/* list of GPIO ports */
#define PA 1
#define PB 2
#define PC 3

/* list of directions */
#define IN 0
#define OUT10 1
#define OUT2 2
#define OUT50 3

/* list of options for input */
#define IN_AN 0 /* analog mode */
#define IN_F 1 /* floating input */
#define IN_PP 2 /*pull-up push-down */

/* list of options for output */
#define OUT_GP_PP 0 /* general purpose push-pull */
#define OUT_GP_OD 1 /* general purpose open-drain */
#define OUT_AF_PP 2 /* alternate func. push-pull */
#define OUT_AF_OD 3 /* alternate func. open-drain */

/* State definition */
#define LOW 0
#define HIGH 1

void gpio_init (unsigned short port, unsigned short pin, unsigned dir, unsigned short opt);
int read_GP (unsigned short port, unsigned short pin);
void write_GP (unsigned short port, unsigned short pin, unsigned short state);
void toggle_GP (unsigned short port, unsigned short pin);
