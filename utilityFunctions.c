#include "utilityFunctions.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/*This function checks the location of an operation in the operations array and returns it. return -1 if it's not in the array.*/
int operationNum(operationData* operations, char* string){
	int i;
	for(i = 0; i < numOfOperations; i++)
	{
		if(strcmp(string,operations[i].operationName) == 0){
			return i;
		}
	}
	
	return -1;	
}

/*This function checks the location of a label in the labels array and returns it. return -1 if it's not in the array.*/
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

/*This function checks if a given string represents a guidance operation*/
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

/*This function gets an unsigned int, startBit and endBit ints and edits the bits from startBit to endBit to data*/
void writeToBits(unsigned int * placeToWrite,int startBit, int endBit, int data)
{
	unsigned int mask = intPow(2,32) - 1;
	mask -= intPow(2,endBit + 1) - 1;
	mask += intPow(2,startBit) - 1;
	*placeToWrite &= mask;
	*placeToWrite  |= ((data << startBit)&(~mask));
	
}

/*This functions receives 2 ints, base and exp and returns base to the power of exp*/
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

/*This function gets a string, skips in it so long there are white chars, scans in it a string in a certain format formatString, and puts it into writeString's reference*/
int scanStrAndMove(char **readStringPtr, char* formatString, char * writeString)
{
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

/*This function gets a string, skips in it so long there are white chars, scans in it in a certain format formatString, and puts it into writeString's reference*/
int scanIntAndMove(char **readString, char* formatString, int * writeInt)
{
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

/*This function returns the decimal length of a given int*/
int numberLength(int num)
{
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

/*This function gets a string, skips in it so long there are no numbers, scans in it in a certain format formatString, and puts it into writeString's reference*/
int moveAndScanInt(char** readString, char* formatString, int* writeInt)
 {
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



