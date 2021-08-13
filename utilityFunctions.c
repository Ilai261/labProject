#include "utilityFunctions.h"
bool checkLabel(char *labelName,label* labels, int labelCount, operation* operations, int lineCount) 
{
	if(strlen(labelName) > 31)
	{
		printf("Line %d: more than 31 characters in a label", lineCount);
		return false;
	}
	
	if(!isalpha(labelName[0]))
	{
		printf("Line %d: label has to start with an alphabetic character", lineCount);
		return false;
	} 
	
	if(labelNum(labels,labelCount,labelName)  != -1){
		printf("Line %d: a label with that name already exists", lineCount);
		return false;
		
	}
	
	if(operationNum(operations,labelName)  != -1){
		printf("Line %d: label name can't be an operation name", lineCount);
		return false;
		
	}
	
	if(isGuidance(labelName) != -1){
		printf("Line %d: label name can't be A guidance word name", lineCount);
		return false;
		
	}
	return true;
	
		
}

int operationNum(operation* operations, char* string){
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

int labelNum(label* labels,int labelCount, char* string){
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
	char *guidanceWords[numOfGuidanceWords] = {"db","dw","dh","asciz","entry","extern"};
	int i;
	 for(i = 0; i < numOfGuidanceWords; i++)
	{
		if(strcmp(string,guidanceWords[i]) == 0){
			return i;
		}
	}
	return -1;
}


void writeDataFromGuidance(int guidanceNum,unsigned char** dataArray,int *DC,char* dataString){
	if(guidanceNum == 0){
		int i = 0;
		int num = 0u;
		*dataArray = realloc(*dataArray,*DC+40);
		
		if(scanIntAndMove(&dataString,"%hhd", &num) > 0) 
		{
			(*dataArray)[*DC+i] = num;
			i++;
		}
		while(scanIntAndMove(&dataString,",%hhd", &num) > 0){
			(*dataArray)[*DC+i] = num;
			i++;
		}
		*DC += i;
	}
	if(guidanceNum == 1){
		int i = 0;
		int num = 0u;
		*dataArray = realloc(*dataArray,*DC+80);
		
		if(scanIntAndMove(&dataString,"%hd", &num) > 0) {
			(*dataArray)[*DC+i] = num;
			i +=2;
		}
		while(scanIntAndMove(&dataString,",%hd", &num) > 0){
			(*dataArray)[*DC+i] = num;
			i +=2;
		}
		*DC += i;
	}
	if(guidanceNum == 2){
		int i = 0;
		int  num;
		*dataArray = realloc(*dataArray,*DC+160);
		
		if(scanIntAndMove(&dataString,"%d", (int*)&num) > 0) {
			(*dataArray)[*DC+i] = num;
			i += 4;
		}
		while(scanIntAndMove(&dataString,",%d", (int*)&num) > 0){
			(*dataArray)[*DC+i] = num;
			i += 4;
		}
		*DC += i;
	}
	if(guidanceNum == 3){
		int i;
		int sLength;
		sLength = strlen(dataString);
		*dataArray = realloc(*dataArray,*DC+sLength);
		 for(i = 1; i< sLength - 1; i++){
			(*dataArray)[*DC+i] = dataString[i];
		 }
		 *DC += sLength;
	}
}
int operationCode(operation currentOperation, char* parameters){
	
	unsigned int retVal = 0u;
	if(currentOperation.operationType == 'R'){
		if (currentOperation.opcode == 0) {
			int registerArray[3];
			if (sscanf(parameters, "$%d,$%d,$%d", &registerArray[0], &registerArray[1], &registerArray[2]) > 0) {
				writeToBits(&retVal, 6, 10, currentOperation.funct);
				writeToBits(&retVal, 11, 15, registerArray[2]);
				writeToBits(&retVal, 16, 20, registerArray[1]);
				writeToBits(&retVal, 21, 25, registerArray[0]);
				writeToBits(&retVal, 26, 31, currentOperation.opcode);
				return retVal;
			}
		}
		else {
			
			int registerArray[2];
			if (sscanf(parameters, "$%d,$%d", &registerArray[0], &registerArray[1]) > 0) {
				writeToBits(&retVal, 6, 10, currentOperation.funct);
				writeToBits(&retVal, 16, 20, registerArray[1]);
				writeToBits(&retVal, 21, 25, registerArray[0]);
				writeToBits(&retVal, 26, 31, currentOperation.opcode);
				return retVal;
			}
		}
	}
	
	if (currentOperation.operationType == 'I') {
		if (currentOperation.opcode < 15) {
			int paramArray[2];
			short int immed = 0;
			if (sscanf(parameters, "$%d,%hd,$%d", &paramArray[0], &immed, &paramArray[1]) > 0) {
				writeToBits(&retVal, 15, 15, (immed > 0)? 1 : 0);
				writeToBits(&retVal, 0, 14, abs(immed));
				writeToBits(&retVal, 16, 20, paramArray[1]);
				writeToBits(&retVal, 21, 25, paramArray[0]);
				writeToBits(&retVal, 26, 31, currentOperation.opcode);
				return retVal;
			}

		}
		if (currentOperation.opcode < 19) {
			int paramArray[2];
			if (sscanf(parameters, "$%d,$%d", &paramArray[0], &paramArray[1]) > 0) {
				writeToBits(&retVal, 16, 20 , paramArray[1]);
				writeToBits(&retVal, 21, 25, paramArray[0]);
				writeToBits(&retVal, 26, 31, currentOperation.opcode);
				writeToBits(&retVal, 26, 31, currentOperation.opcode);
				return retVal;
			}
		}
		else {
			int paramArray[2];
			short int immed = 0;
			if (sscanf(parameters, "$%d,%hd,$%d", &paramArray[0], &immed, &paramArray[1]) > 0) {
				writeToBits(&retVal, 15, 15, (immed > 0) ? 1 : 0);
				writeToBits(&retVal, 0, 14, abs(immed));
				writeToBits(&retVal, 16, 20, paramArray[1]);
				writeToBits(&retVal, 21, 25, paramArray[0]);
				writeToBits(&retVal, 26, 31, currentOperation.opcode);
			}
		}
	}
	if (currentOperation.operationType == 'J') {
		if (currentOperation.opcode == 30) {
			int regNum = 0;
			if (sscanf(parameters, "$%d", &regNum) > 0) {
				writeToBits(&retVal, 25, 25, 1);
				writeToBits(&retVal, 0, 24, regNum);
				writeToBits(&retVal, 26, 31, currentOperation.opcode);
			}
			else {
				writeToBits(&retVal, 25, 25, 0);
				writeToBits(&retVal, 26, 31, currentOperation.opcode);
			}
		}
		if (currentOperation.opcode <= 32) {
			writeToBits(&retVal, 25, 25, 0);
			writeToBits(&retVal, 26, 31, currentOperation.opcode);
		}
		if (currentOperation.opcode == 63) {
			writeToBits(&retVal, 26, 31, currentOperation.opcode);
			writeToBits(&retVal, 0, 25, 0);
		}
	}
	return retVal;
}

void writeToBits(unsigned int * placeToWrite,int startBit, int endBit, int data)
{
	unsigned int mask = intPow(2,32) - 1;
	mask -= intPow(2,endBit + 1) - 1;
	mask += intPow(2,startBit + 1) - 1;
	*placeToWrite &= mask;
	*placeToWrite  |= data << startBit;
	
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
	while(isspace(*readString[forwardBy])){
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

bool fileApproved(char* fileName) {
	int i = 0;
	while (fileName[i] != '\0') i++;
	if (i >= 5 && fileName[i - 1] == 's' && fileName[i - 2] == 'a' && fileName[i - 3] == '.') return true;
	printf("dood not approve\n");
	return false;
}
char* getFileName(char* fileName) {/*will work in linux*/
	int i = 0;
	char* retval = (char*)malloc(100);
	strcpy(retval, fileName);
	while (retval[i] != '\0') i++;

	if (i >= 5) {
		retval[i - 3] = '\0';
	}
	return retval;
	
}
void createObject(unsigned int* codeArray, char* assemblyFileName) {
	FILE* fp;
	char* fileName = getFileName(assemblyFileName);
	strcat(fileName, ".ob");
	fp = fopen(fileName, "w");
	fclose(fp);
	free(assemblyFileName);
	printf("created");
}

bool parameterCheck(int line, char* parameters, operation currentOperation, int** labelLines)
{
	bool retVal = true;
	*labelLines[0] = 0;
	int i;
	int paramNum = 0;
	int num = 0;
	char* ogParam = parameters;
	char* og_xParam = ogParam;
	int countNumLengths = 0;
	if (currentOperation.operationType == 'R')
	{
		if (currentOperation.opcode == 0)
		{
			if(isdigit(parameters[0]))
			{
				printf("Line %d: invalid register defining, no $", line);
				return false;
			}
			for (i = 0; i++; i <= 2)
			{
				if (scanIntAndMove(&parameters, "%d", &num) > 0) paramNum++;
				if (paramNum > i)
				{
					if (num > 31 || num < 0)
					{
						printf("Line %d: invalid register number", line);
						return false;
					}
					if (*(parameters) != COMMA)
					{
						printf("Line %d: invalid register defining, no comma", line);
						return false;
					}
					og_xParam = parameters;
					while (isdigit(*parameters)) parameters--;
					if (*(parameters) != DOLLAR)
					{
						printf("Line %d: invalid register defining, no $", line);
						return false;
					}
					parameters = og_xParam;
					countNumLengths += numLength(num);
				}
			}
			if (paramNum < 3)
			{
				printf("Line %d: not enough parameters in %s operation", line, currentOperation.operationName);
				return false;
			}
			i = strlen(ogParam);
			if (i > (6 + countNumLengths))
			{
				printf("Line %d: invalid char in %s operation parameters", line, currentOperation.operationName);
				return false;
			}
			return retVal;
		}
		else
		{
			if(isdigit(parameters[0]))
			{
				printf("Line %d: invalid register defining, no $", line);
				return false;
			}
			for (i = 0; i++; i <= 1)
			{
				if (scanIntAndMove(&parameters, "%d", &num) > 0) paramNum++;
				if (paramNum > i)
				{
					if (num > 31 || num < 0)
					{
						printf("Line %d: invalid register number", line);
						return false;
					}
					if (*(parameters) != COMMA)
					{
						printf("Line %d: invalid register defining, no comma", line);
						return false;
					}
					og_xParam = parameters;
					while (isdigit(*parameters)) parameters--;
					if (*(parameters) != DOLLAR)
					{
						printf("Line %d: invalid register defining, no $", line);
						return false;
					}
					parameters = og_xParam;
					countNumLengths += numLength(num);
				}
			}
			if (paramNum < 2)
			{
				printf("Line %d: not enough parameters in %s operation", line, currentOperation.operationName);
				return false;
			}
			i = strlen(ogParam);
			if (i > (4 + countNumLengths))
			{
				printf("Line %d: invalid char in %s operation parameters", line, currentOperation.operationName);
				return false;
			}
			return retVal;
		}
	}
	
	if (currentOperation.operationType == 'I') 
	{
		if (currentOperation.opcode < 15 || currentOperation.opcode > 19) 
		{
			if(isdigit(parameters[0]))
			{
				printf("Line %d: invalid register defining, no $", line);
				return false;
			}
			
			for (i = 0; i++; i <= 2)
			{
				if (scanIntAndMove(&parameters, "%d", &num) > 0) paramNum++;
				if (paramNum > i)
				{
					if (paramNum == 2 && (num > 32677 || num < -32677))
					{
						printf("Line %d: invalid inmed number", line);
						return false;
					}
					if(paramNum != 2 && (num > 31 || num < 0))
					{
						printf("Line %d: invalid register number", line);
						return false;
					}
					if (*(parameters) != COMMA)
					{
						printf("Line %d: invalid register defining, no comma", line);
						return false;
					}
					og_xParam = parameters;
					while (isdigit(*parameters)) parameters--;
					if (*(parameters) != DOLLAR && paramNum != 2) /*a little sussy in here*/
					{
						printf("Line %d: invalid register defining, no $", line);
						return false;
					}
					parameters = og_xParam;
					countNumLengths += numLength(num);
				}
			}
			if (paramNum < 3)
			{
				printf("Line %d: not enough parameters in %s operation", line, currentOperation.operationName);
				return false;
			}
			i = strlen(ogParam);
			if (i > (6 + countNumLengths))
			{
				printf("Line %d: invalid char in %s operation parameters", line, currentOperation.operationName);
				return false;
			}
			return retVal;
		}
		
		}
		else 
		{
			if(isdigit(parameters[0]))
			{
				printf("Line %d: invalid register defining, no $", line);
				return false;
			}
			
			for (i = 0; i++; i <= 1)
			{
				if (scanIntAndMove(&parameters, "%d", &num) > 0) paramNum++;
				if (paramNum > i)
				{
					if (num > 31 || num < 0)
					{
						printf("Line %d: invalid register number", line);
						return false;
					}
					
					if (*(parameters) != COMMA)
					{
						printf("Line %d: invalid register defining, no comma", line);
						return false;
					}
					og_xParam = parameters;
					while (isdigit(*parameters)) parameters--;
					if (*(parameters) != DOLLAR)
					{
						printf("Line %d: invalid register defining, no $", line);
						return false;
					}
					parameters = og_xParam;
					countNumLengths += numLength(num);
				}
			}
			if (paramNum < 2)
			{
				printf("Line %d: not enough parameters in %s operation", line, currentOperation.operationName);
				return false;
			}
			
			i = strlen(ogParam);
			if(!isalpha(parameters[0]))
			{
				printf("Line %d: no label in %s operation parameters", line, currentOperation.operationName);
				return false;
			}
			else
			{
				if (i > (6 + countNumLengths + strlen(parameters)))
				{
					printf("Line %d: invalid char in %s operation parameters", line, currentOperation.operationName);
					return false;
				}
				*labelLines[0]++;
				if(*labelLInes[0] % 10 == 0)
				{
					*labelLines = realloc(*labelLines, *labelLines[0] + 10);
				}
				*labelLines[*(labelLines[0])] = line;
			}
			return retVal;
			
		}
		
	}
	if (currentOperation.operationType == 'J') 
	{
		if (currentOperation.opcode == 30) 
		{
			if(sscanf(parameters, "%d", &num) > 0)
			{
				if(!isalpha(ogParam[0]) && opParam[0] != $)
				{
					printf("Line %d: invalid label or register defining in %s operation", line, currentOperation.operationName);
					return false;
				}
				else if(!isalpha(ogParam[0]))
				{
					if (num > 31 || num < 0)
					{
						printf("Line %d: invalid register number", line);
						return false;
					}
				{
				else
				{
					*labelLines[0]++;
					if(*labelLInes[0] % 10 == 0)
					{
						*labelLines = realloc(*labelLines, *labelLines[0] + 10); /*could be a function*/
					}
					(*labelLines)[**labelLines] = line;
				}
				
			}
			else
			{
				if(!isalpha(ogParam[0]))
				{
					printf("Line %d: invalid label defining", line);
					return false;
				{
				else
				{
					*labelLines[0]++;
					if(*labelLInes[0] % 10 == 0)
					{
						*labelLines = realloc(*labelLines, *labelLines[0] + 10);
					}
					*labelLines[*(labelLines[0])] = line;
				}
			}
			return retval;
		}
		if (currentOperation.opcode <= 32) 
		{
			if(!isalpha(ogParam[0]))
			{
					printf("Line %d: invalid label defining", line);
					return false;
			{
			else
			{
				*labelLines[0]++;
				if(*labelLInes[0] % 10 == 0)
				{
					*labelLines = realloc(*labelLines, *labelLines[0] + 10);
				}
				*labelLines[*(labelLines[0])] = line;
			}
		}
		if (currentOperation.opcode == 63) 
		{
			if(parameters[0] != '\0')
			{
				printf("Line %d: stop function should not have parameters", line);
				return false;
			}
		}
	}
	return retVal;
}

void createEnt(label* labels, char* fileName) 
{

}

void createExt(label* labels, char* fileName) 
{

}
