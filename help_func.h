/**
  ******************************************************************************
  * @file    help_func.h
  * @author  Cihan Senyuz
  * @brief   Header for help_func.c file.
  *                   This file contains the common defines of the application.
  *  
  ******************************************************************************
  */

int str_len(char str[]);
int str_find(char str1[],char str2[]);
int str_exact(char str1[],char str2[]);
void str_merge(char str1[], char str2[]);
char* int2char(unsigned int num); /* WARNING: this function allocates memory for returned char*. Do not forget to deallocate it after you use. */
int char2int(char str[]);
void str_empty(char str[]);
