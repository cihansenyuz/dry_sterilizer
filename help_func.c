#include "help_func.h"

/* gets the lenght of a string
*
* @param string that will be measured
* @return lenght of the string
*/
int str_len(char str[])
{
	int len = 0;
	while(str[len] != '\0')
	{len++;}
	return len;
	
}

/* finds a string in another string
*
* @param string being looked for, string needs to be checked
* @return 1 if exists, 0 if does not
*/
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

/* checks if two string are exactly the same
*
* @param str1 first string
* @param str2 second string
* @return 1 if they are exact, 0 if are not
*/
int str_exact(char str1[],char str2[])
{
	int result = 0;
	if((str_len(str1) == str_len(str2)) &  str_find(str1,str2))
	{
		result = 1;
	}
	return result;
}

/* concatenate two strings into one string
*
* @param str1 first string
* @param str2 second string to be added to first one
* @return new string that is created
*/
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

/* converts to string numbers from integer
*
* @param num number to be converted
* @return char* to new string that is created
* @note ATTENTION! this function allocates memory for returned char*
* Do not forget to deallocate it after you use.
*/
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

/* converts to integer from string numbers
*
* @param str string number to be converted
* @return new integer number that is created
*
*/
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

/* clears the content of a char array
*
* @param str string to be cleared
* @return none
*/
void str_empty(char str[])
{
	int i = 0;
	while(str[i] !='\0')
	{
		str[i] = '\0';
		i++;
	}
}
