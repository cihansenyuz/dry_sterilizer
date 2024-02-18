/**
  ******************************************************************************
  * @file    systick_delay.h
  * @author  Cihan Senyuz
  * @brief   Header for systick_delay.c file.
  *                   This file contains the common defines of the application.
  * 
  * @note    Source code depends on stm32f10x.h library
  ******************************************************************************
  */

#include "stm32f10x.h"

void systick_init(void);
void createMili(void);
void delayMS(unsigned long);
void systick_interrup_start(void);
