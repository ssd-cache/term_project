#include "stdafx.h"
#include "StringManip.h"
#include "cache_ssd.h"

/************* lowerCaseString *****************
Change input string to lower case
	INPUT: Pointer to a string
	OUTPUT: none
***********************************************/
void lowerCaseString(char* lowStr)
{
	for(; *lowStr; *lowStr++) *lowStr = tolower(*lowStr);
	return;
}

/*************** removeSpaces ***************
remove white spaces of string passed int and
copy back to memory address
 INPUT: Pointer to a string of data
 OUTPUT: none
 *********************************************/
void removeSpaces(char *str1)
{
    int i, j;
    char *str2=str1;
    for (i = 0, j = 0; i < strlen(str1); i++,j++)
    {
        if (str1[i]!=' ' && str1[i] != '\t' && str1[i] != '\n')
            str2[j]=str1[i];
        else
        {
            j--;
        }
    }
    str2[j]=0;
    if (str2!=str1)
	{
		memcpy(str1,str2,strlen(str2)+1);
	}
    return;
}

/*************** removeSpaces ***************
remove white spaces of string passed int and
copy back to memory address
 INPUT: Pointer to a string of data
 OUTPUT: none
 *********************************************/
void splitInput(char *input, void *arg)
{
	/* local variables */
	char* tempStr;
	char* strCpy;
	int span;
	SizeInfo* tempInput = (SizeInfo*)arg;

	/* split command off string */
	span = strspn(input, "0123456789");

	if(span != 0)
	{
		strCpy = (char*)malloc(sizeof(span)+1);
		strncpy(strCpy, input, span);
		tempInput->size = atoi(strCpy);

		tempStr = strtok(input, "0123456789");

		while(tempStr != NULL)
		{
			tempInput->type = (char*)malloc(sizeof(tempStr)+1);
			strcpy(tempInput->type, tempStr);
			tempStr = strtok(NULL, "\n");
		}
	}
	return;
}

/*************** getNumValue ***************

 INPUT: 
 OUTPUT: none
 *********************************************/
unsigned long getByteValue(void *arg)
{
	/* local variables */
	unsigned long value;
	SizeInfo *temp = (SizeInfo*)arg;

	/* functions */
	if(strcmp("gb", temp->type)==0)
	{
		value = temp->size * pow(2.0, 30.0);
	}
	else if(strcmp("mb", temp->type)==0)
	{
		value = temp->size * pow(2.0, 20.0);
	}
	else if(strcmp("kb", temp->type)==0)
	{
		value = temp->size * pow(2.0, 10.0);
	}
	else
	{
		value = temp->size;
	}
	return value;
}

/*************** checkInputValidity ***************

 INPUT: 
 OUTPUT: none
 *********************************************/
void checkInputValidity(char* input)
{
	/* local variables */
	int span;
	
	/* all commands have numbers if there isn't a number exit */
	span = strspn(input, "0123456789");
	if(span > 0)
	{
		return;
	}
	else
	{
		printf("bad data entered...\nexiting program...\n");
		exit(-1);
	}
}

void checkForCommas(char* input)
{
	/* local variables */
	int span = 0;
	char* testStr;

	/* checking for comma's */
	span = strspn(input, ",");
	testStr=strchr(input,',');
	while (testStr!=NULL)
	{
		span++;
		testStr=strchr(testStr+1,',');
	}

	/* read/writes have minimum two commas */
	if(span < 2)
	{
		printf("bad data entered...\nexiting program...\n");
		exit(-1);
	}
	return;
}