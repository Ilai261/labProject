#include "utilityFunctions.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int operationNum(operationData* operations, char* string){
	/*maybe improve the efficiency*/
	int i;
	for(i = 0; i < numOfOperations; i++)
	{
		if(strcmp(string,operations[i].operationName) == 0){
			return i;
		}
	}
	
	return -1;	
}

int labelNum(labelData* labels,int labelCount, char* string){
	int i;
	for(i = 0; i < labelCount; i++)
	{
		if(strcmp( string,labels[i].symbol) == 0){
			return i;
		}
	}
	
	return -1;
}

int isGuidance(char* string){
	char *guidanceWords[numOfGuidanceWords] = {"db","dh","dw","asciz","entry","extern"};
	int i;
	 for(i = 0; i < numOfGuidanceWords; i++)
	{
		if(strcmp(string,guidanceWords[i]) == 0){
			return i;
		}
	}
	return -1;
}

void writeToBits(unsigned int * placeToWrite,int startBit, int endBit, int data)
{
	unsigned int mask = intPow(2,32) - 1;
	mask -= intPow(2,endBit + 1) - 1;
	mask += intPow(2,startBit) - 1;
	*placeToWrite &= mask;
	*placeToWrite  |= ((data << startBit)&(~mask));
	
}

int intPow(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

int scanStrAndMove(char **readStringPtr, char* formatString, char * writeString){
	int retVal;
	int forwardBy = 0;
	char* readString = *readStringPtr;
	while(isspace(readString[forwardBy])){
		forwardBy++;
	}
	retVal = sscanf(readString,formatString,writeString);
	forwardBy += strlen(writeString);
	if(retVal > 0) *readStringPtr += forwardBy;
	return retVal;
}

int scanIntAndMove(char **readString, char* formatString, int * writeInt){
	int retVal;
	int forwardBy = 0;
	while (isspace((*readString)[forwardBy])) {
		forwardBy++;
	}
	retVal = sscanf(*readString,formatString,writeInt);
	forwardBy += numberLength(*writeInt)-1;
	if(retVal > 0) *readString += forwardBy;
	return retVal;
}

int numberLength(int num){
	int x = abs(num);
	int retVal = 0;
	if(x == 0) return 1;
	while(x > 0){
		x /= 10;
		retVal++;
	}
	if (num < 0) retVal++;
	return retVal;
}

int moveAndScanInt(char** readString, char* formatString, int* writeInt) {
	int retVal;
	int forwardBy = 0;
	while ((*readString)[forwardBy] != '\0' && !isdigit((*readString)[forwardBy]) && (*readString)[forwardBy] != '-' && (*readString)[forwardBy] != '+') {
		forwardBy++;
	}
	*readString += forwardBy;
	retVal = sscanf(*readString, formatString, writeInt);
	forwardBy = 0;
	forwardBy += numberLength(*writeInt);
	if (retVal > 0) *readString += forwardBy;
	return retVal;
}



