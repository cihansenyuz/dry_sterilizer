#include "help_func.h"
#include "stdlib.h"

/*

1- get the lenght of a string
2- find a string on a string
3- find an exact string match
4- concatenate 2 strings
5- convert int to string
6- convert string to unsigned int

*/

int str_len(char str[])
{
	int len = 0;
	while(str[len] != '\0')
	{len++;}
	return len;
	
}

int str_find(char str1[],char str2[])
{
	int result = 0;
	int test = 0;
	int i = 0;
	int j = 0;
	int len1 = str_len(str1);
	int len2 = str_len(str2);
	
	for(j=0; j<len2; j++)
	{	
		for(i=0; i<len1; i++)
		{
			if(str1[i] == str2[j+i])
			{
				test++;
				if (test == len1)
				{
					result = 1;
				}
			}
		}
	test = 0;
	}
	return result;
}



int str_exact(char str1[],char str2[])
{
	int result = 0;
	if((str_len(str1) == str_len(str2)) &  str_find(str1,str2))
	{
		result = 1;
	}
	return result;
}


void str_merge(char str1[], char str2[])
{
	int i = 0;
	int j = 0;
	while(str1[i] != '\0')
	{i++;}
	while(str2[j]!= '\0')
	{
		str1[i+j]= str2[j];
		j++;
	}
}

char* int2char(unsigned int num)
{
	char* str;
	str = (char*) malloc(10*sizeof(char));
	char len_str[30];
	int count = 0;
	unsigned int div = 10;
	int j = 0;
		
	while(num >= div)
	{
		len_str[count] = num % div + 0x30;
		num /= 10;
		count++;
	}
	
	len_str[count] = num + 0x30;
	for(j=count; j>=0; j--)
	{
		str[count-j] = len_str[j];
	}
	str[count+1] = '\0';
	return str;
}

int char2int(char str[])
{
	int result = 0;
	int STRlen = str_len(str);
	int j = 0;
	
	for(j=0; j< STRlen; j++)
	{
		result = (result * 10) + (str[j] - 0x30);
	}
	
	return result;
}

void str_empty(char str[])
{
	int i = 0;
	while(str[i] !='\0')
	{
		str[i] = '\0';
		i++;
	}
}
