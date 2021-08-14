#include "utilityFunctions.h"
bool checkLabel(char *labelName,label* labels, int labelCount, operation* operations, int lineCount) 
{
	if(strlen(labelName) > 31)
	{
		printf("Line %d: more than 31 characters in a label\n", lineCount);
		return false;
	}
	
	if(!isalpha(labelName[0]))
	{
		printf("Line %d: label has to start with an alphabetic character\n", lineCount);
		return false;
	} 
	
	if(labelNum(labels,labelCount,labelName)  != -1){
		printf("Line %d: a label with that name already exists\n", lineCount);
		return false;
		
	}
	
	if(operationNum(operations,labelName)  != -1){
		printf("Line %d: label name can't be an operation name\n", lineCount);
		return false;
		
	}
	
	if(isGuidance(labelName) != -1){
		printf("Line %d: label name can't be A guidance word name\n", lineCount);
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


void writeDataFromGuidance(int guidanceNum,unsigned char** dataArray,int *DC,char* dataString){
	if(guidanceNum == 0){
		int i = 0;
		int num = 0u;
		*dataArray = realloc(*dataArray, *DC + 40);
		while(moveAndScanInt(&dataString,"%hhd", &num) > 0){
			(*dataArray)[*DC+i] = num;
			i++;
		}
		*DC += i;
	}
	if(guidanceNum == 1){
		int i = 0;
		int num = 0u;
		*dataArray = realloc(*dataArray,*DC+80);
		while(moveAndScanInt(&dataString,"%hd", &num) > 0){
			*((short int*)&((*dataArray)[*DC + i])) = num;
			i +=2;
		}
		*DC += i;
	}
	if(guidanceNum == 2){
		int i = 0;
		int  num;
		*dataArray = realloc(*dataArray,*DC+160);
		while(moveAndScanInt(&dataString,"%d", (int*)&num) > 0){
			*((int*)&((*dataArray)[*DC + i])) = num;
			i += 4;
		}
		*DC += i;
	}
	if(guidanceNum == 3){
		int i;
		int sLength;
		sLength = strlen(dataString);
		if (sLength > 1) {
			dataString[sLength - 1] = '\0';
			dataString++;
			*dataArray = realloc(*dataArray, *DC + sLength);
			for (i = 0; i < sLength - 1; i++) {
				(*dataArray)[*DC + i] = dataString[i];
			}
			*DC += sLength - 1;
		}
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
				writeToBits(&retVal, 21, 25, registerArray[0]);
				writeToBits(&retVal, 11, 15, registerArray[1]);
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
				writeToBits(&retVal, 0, 15, immed);
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
				return retVal;
			}
		}
		else {
			int paramArray[2];
			short int immed = 0;
			if (sscanf(parameters, "$%d,%hd,$%d", &paramArray[0], &immed, &paramArray[1]) > 0) {
				writeToBits(&retVal, 0, 15, immed);
				writeToBits(&retVal, 16, 20, paramArray[1]);
				writeToBits(&retVal, 21, 25, paramArray[0]);
				writeToBits(&retVal, 26, 31, currentOperation.opcode);
				return retVal;
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
				return retVal;
			}
			else {
				writeToBits(&retVal, 25, 25, 0);
				writeToBits(&retVal, 26, 31, currentOperation.opcode);
				return retVal;
			}
		}
		if (currentOperation.opcode <= 32) {
			writeToBits(&retVal, 25, 25, 0);
			writeToBits(&retVal, 26, 31, currentOperation.opcode);
			return retVal;
		}
		if (currentOperation.opcode == 63) {
			writeToBits(&retVal, 26, 31, currentOperation.opcode);
			writeToBits(&retVal, 0, 25, 0);
			return retVal;
		}
	}
	return retVal;
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

bool parameterCheck(int line,int IC, char* parameters, operation currentOperation, int** labelLines)
{
	int i;
	int paramNum = 0;
	int num = 0;
	int countNumLengths = 0;
	char* ogParam = parameters;
	char* og_xParam = ogParam;
	if (currentOperation.operationType == 'R')
	{
		if (currentOperation.opcode == 0)
		{
			if(isdigit(parameters[0]))
			{
				printf("Line %d: invalid register defining, no $\n", line);
				return false;
			}
			for (i = 0; i <= 2; i++)
			{
				if (moveAndScanInt(&parameters, "%d", &num) > 0) paramNum++;
				if (paramNum > i)
				{
					if (num > 31 || num < 0)
					{
						printf("Line %d: invalid register number\n", line);
						return false;
					}
					if (*(parameters) != COMMA && *(parameters) != '\0')
					{
						printf("Line %d: invalid register defining, no comma\n", line);
						return false;
					}
					og_xParam = parameters;
					parameters--;
					while (isdigit(*parameters)) parameters--;
					if (*(parameters) != DOLLAR)
					{
						printf("Line %d: invalid register defining, no $\n", line);
						return false;
					}
					parameters = og_xParam;
					countNumLengths += numberLength(num);
				}
			}
			if (paramNum < 3)
			{
				printf("Line %d: not enough parameters in %s operation\n", line, currentOperation.operationName);
				return false;
			}
			i = strlen(ogParam);
			if (i > (6 + countNumLengths))
			{
				printf("Line %d: invalid char in %s operation parameters\n", line, currentOperation.operationName);
				return false;
			}
			return true;
		}
		else
		{
			if(isdigit(parameters[0]))
			{
				printf("Line %d: invalid register defining, no $\n", line);
				return false;
			}
			for (i = 0; i <= 1; i++)
			{
				if (moveAndScanInt(&parameters, "%d", &num) > 0) paramNum++;
				if (paramNum > i)
				{
					if (num > 31 || num < 0)
					{
						printf("Line %d: invalid register number\n", line);
						return false;
					}
					if (*(parameters) != COMMA && *(parameters) != '\0')
					{
						printf("Line %d: invalid register defining, no comma\n", line);
						return false;
					}
					og_xParam = parameters;
					parameters--;
					while (isdigit(*parameters)) parameters--;
					if (*(parameters) != DOLLAR)
					{
						printf("Line %d: invalid register defining, no $\n", line);
						return false;
					}
					parameters = og_xParam;
					countNumLengths += numberLength(num);
				}
			}
			if (paramNum < 2)
			{
				printf("Line %d: not enough parameters in %s operation\n", line, currentOperation.operationName);
				return false;
			}
			i = strlen(ogParam);
			if (i > (4 + countNumLengths))
			{
				printf("Line %d: invalid char in %s operation parameters\n", line, currentOperation.operationName);
				return false;
			}
			return true;
		}
	}
	
	if (currentOperation.operationType == 'I') 
	{
		if (currentOperation.opcode < 15 || currentOperation.opcode > 19) 
		{
			if(isdigit(parameters[0]))
			{
				printf("Line %d: invalid register defining, no $\n", line);
				return false;
			}
			
			for (i = 0; i <= 2; i++)
			{
				if (moveAndScanInt(&parameters, "%d", &num) > 0) paramNum++;
				if (paramNum > i)
				{
					if (paramNum == 2 && (num > 32677 || num < -32678))
					{
						printf("Line %d: invalid inmed number\n", line);
						return false;
					}
					if(paramNum != 2 && (num > 31 || num < 0))
					{
						printf("Line %d: invalid register number\n", line);
						return false;
					}
					if (*(parameters) != COMMA && *(parameters) != '\0')
					{
						printf("Line %d: invalid register defining, no comma\n", line);
						return false;
					}
					og_xParam = parameters;
					parameters--;
					while (isdigit(*parameters)) parameters--;
					if (*(parameters) != DOLLAR && paramNum != 2) /*a little sussy in here*/
					{
						printf("Line %d: invalid register defining, no $\n", line);
						return false;
					}
					parameters = og_xParam;
					countNumLengths += numberLength(num);
				}
			}
			if (paramNum < 3)
			{
				printf("Line %d: not enough parameters in %s operation\n", line, currentOperation.operationName);
				return false;
			}
			i = strlen(ogParam);
			if (i > (6 + countNumLengths))
			{
				printf("Line %d: invalid char in %s operation parameters\n", line, currentOperation.operationName);
				return false;
			}
			return true;
		}
		
		else 
		{
			if(isdigit(parameters[0]))
			{
				printf("Line %d: invalid register defining, no $\n", line);
				return false;
			}
			
			for (i = 0; i <= 1; i++)
			{
				if (moveAndScanInt(&parameters, "%d", &num) > 0) paramNum++;
				if (paramNum > i)
				{
					if (num > 31 || num < 0)
					{
						printf("Line %d: invalid register number\n", line);
						return false;
					}
					
					if (*(parameters) != COMMA && *(parameters) != '\0')
					{
						printf("Line %d: invalid register defining, no comma\n", line);
						return false;
					}
					og_xParam = parameters;
					parameters--;
					while (isdigit(*parameters)) parameters--;
					if (*(parameters) != DOLLAR)
					{
						printf("Line %d: invalid register defining, no $\n", line);
						return false;
					}
					parameters = og_xParam;
					countNumLengths += numberLength(num);
				}
			}
			if (paramNum < 2)
			{
				printf("Line %d: not enough parameters in %s operation\n", line, currentOperation.operationName);
				return false;
			}
			
			i = strlen(ogParam);
			if(!isalpha(parameters[1]))
			{
				printf("Line %d: no label in %s operation parameters\n", line, currentOperation.operationName);
				return false;
			}
			else
			{
				if (i > (6 + countNumLengths + strlen(parameters)))
				{
					printf("Line %d: invalid char in %s operation parameters\n", line, currentOperation.operationName);
					return false;
				}
				if (*labelLines[0] % 10 == 0)
				{
					*labelLines = realloc(*labelLines, (*labelLines)[0] + 40);
				}
				(*labelLines)[0]++;
				(*labelLines)[(*labelLines)[0]] = IC;
				return true;
			}
			
		}
		return true;
	}
	
	if (currentOperation.operationType == 'J') 
	{
		if (currentOperation.opcode == 30) 
		{
			if(moveAndScanInt(&parameters, "%d", &num) > 0)
			{
				if(!isalpha(ogParam[0]) && ogParam[0] != DOLLAR)
				{
					printf("Line %d: invalid label or register defining in %s operation", line, currentOperation.operationName);
					return false;
				}
				if (!isalpha(ogParam[0]))
				{
					if (num > 31 || num < 0)
					{
						printf("Line %d: invalid register number", line);
						return false;
					}
				}
				else
				{
					if (*labelLines[0] % 10 == 0)
					{
						*labelLines = realloc(*labelLines, (*labelLines)[0] + 40);
					}
					(*labelLines)[0]++;
					(*labelLines)[(*labelLines)[0]] = IC;
					return true;
				}
				
			}
			else
			{
				if (!isalpha(ogParam[0]))
				{
					printf("Line %d: invalid label defining\n", line);
					return false;
				}
				else
				{
					if(*labelLines[0] % 10 == 0)
					{
						*labelLines = realloc(*labelLines, (*labelLines)[0] + 40);
					}
					(*labelLines)[0]++;
					(*labelLines)[(*labelLines)[0]] = IC;
					return true;
				}
			}
			return true;
		}
		if (currentOperation.opcode <= 32) 
		{
			if (!isalpha(ogParam[0]))
			{
				printf("Line %d: invalid label defining\n", line);
				return false;
			}
			else
			{
				if (*labelLines[0] % 10 == 0)
				{
					*labelLines = realloc(*labelLines, (*labelLines)[0] + 40);
				}
				(*labelLines)[0]++;
				(*labelLines)[(*labelLines)[0]] = IC;
				return true;
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
	return true;
}

int operationLabelCode(operation currentOperation, char* parameters, unsigned int* codeArray,int lineNum ,int IC, label* labels, int labelCount) {
	label paramLabel;
	int opPos = (IC - 100) / 4;
	int labelNumber;
	if (currentOperation.operationType == 'J') {
		if (currentOperation.opcode == 30) {
			int regNum = 0;
			if (sscanf(parameters, "$%d", &regNum) > 0) {
				return 0;
			}
			else {
				labelNumber = labelNum(labels, labelCount, parameters);
				if (labelNumber == -1) {
					printf("Line %d: unknown label used as a parameter for J operation\n", lineNum);
					return -1;
				}
				paramLabel = labels[labelNumber];
				if (paramLabel.isExternal) {
					return 2;
				}
				writeToBits(&codeArray[opPos], 0, 24, paramLabel.address);
				return 1;
			}
			
		}
		if (currentOperation.opcode <= 32) {
			labelNumber = labelNum(labels, labelCount, parameters);
			if (labelNumber == -1) {
				printf("Line %d: unknown label used as a parameter for J operation\n", lineNum);
				return -1;
			}
			paramLabel = labels[labelNumber];
			if (paramLabel.isExternal) {
				return 2;
			}
			writeToBits(&codeArray[opPos], 0, 24, paramLabel.address);
			return 1;
		}
	}
	if (currentOperation.operationType == 'I') {
		if (currentOperation.opcode < 19) {
			int commaCounter = 0;
			short int distanceToLabel;
			while (commaCounter < 2) {
				if (*parameters == COMMA) commaCounter++;
				parameters++;
			}
			labelNumber = labelNum(labels, labelCount, parameters);
			if (labelNumber == -1) printf("Line %d: unknown label used as a parameter for I operation\n", lineNum);
			paramLabel = labels[labelNumber];
			if (paramLabel.isExternal) {
				printf("Line %d: external label cannot be used as a parameter for I operation\n", lineNum);
				return -1;
			}
			distanceToLabel = (short int)(paramLabel.address -IC);
			writeToBits(&codeArray[opPos], 0, 15, distanceToLabel);
			return 1;
		}
	}
	return 0;
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
bool checkGuidanceParam(int line, int guidanceNum, char* parameters)
{
	int num;
	int countParamLengths = 0;
	int countParam = 0;
	int parametersLength = strlen(parameters);
	if (guidanceNum < 0)
	{
		return false;
	}
	if (guidanceNum == 0)
	{
		while (moveAndScanInt(&parameters, "%d", &num) > 0)
		{
			if (num > 127 || num < -128)
			{
				printf("Line %d: invalid parameter number, should be between -128 and 127\n", line);
				return false;
			}
			if (*parameters != COMMA && *parameters != '\0')
			{
				printf("Line %d: invalid parameter defining, no comma\n", line);
				return false;
			}
			countParamLengths += (numberLength(num) + 1);
			countParam++;
		}
		if (countParam < 1)
		{
			printf("Line %d: no Parameters in function\n", line);
			return false;
		}
		if (countParamLengths < parametersLength)
		{
			printf("Line %d: invalid char in operation parameters\n", line);

			return false;
		}
		return true;
	}
	if (guidanceNum == 1)
	{
		while (moveAndScanInt(&parameters, "%d", &num) > 0)
		{
			if (num > 32767 || num < -32768)
			{
				printf("Line %d: invalid parameter number, should be between -32768 and 32767\n", line);
				return false;
			}
			if (*parameters != COMMA && *parameters != '\0')
			{
				printf("Line %d: invalid parameter defining, no comma\n", line);
				return false;
			}
			countParamLengths += (numberLength(num) + 1);
			countParam++;
		}
		if (countParam < 1)
		{
			printf("Line %d: no Parameters in function\n", line);
			return false;
		}
		if (countParamLengths < parametersLength)
		{
			printf("Line %d: invalid char in operation parameters\n", line);

			return false;
		}
		return true;
	}
	if (guidanceNum == 2)
	{
		while (moveAndScanInt(&parameters, "%d", &num) > 0)
		{
			if (num > 2147483647 || num < -2147483647)
			{
				printf("Line %d: invalid parameter number, should be between -2147483648 and 2147483647\n", line);
				return false;
			}
			if (*parameters != COMMA && *parameters != '\0')
			{
				printf("Line %d: invalid parameter defining, no comma\n", line);
				return false;
			}
			countParamLengths += (numberLength(num) + 1);
			countParam++;
		}
		if (countParam < 1)
		{
			printf("Line %d: no Parameters in function\n", line);
			return false;
		}
		if (countParamLengths < parametersLength)
		{
			printf("Line %d: invalid char in operation parameters\n", line);

			return false;
		}
		return true;
	}
	if (guidanceNum == 3)
	{
		if (parameters[0] != '"')
		{
			printf("Line %d: invalid string defining, no \" at the beginning of the string definition\n", line);

			return false;
		}
		if (parameters[strlen(parameters) - 1] != '"')
		{
			printf("Line %d: invalid string defining, no \" at the end of the string definition\n", line);

			return false;
		}
		return true;
	}
	if (guidanceNum == 4 || guidanceNum == 5)
	{
		if (!isalpha(parameters[0]))

		{

			printf("Line %d: invalid label defining\n", line);

			return false;

		}
		return true;
	}
	return true;
}
void createObject(unsigned int* codeArray, unsigned char* dataArray,int IC,int DC, char* assemblyFileName) {
	FILE* fp;
	int byteCount = 0;
	char* fileName = getFileName(assemblyFileName);
	strcat(fileName, ".ob");
	fp = fopen(fileName, "w");
	fprintf(fp, "	%d %d", IC - 100, DC);
	while (byteCount < IC - 100) {
		if (byteCount % 4 == 0) fprintf(fp, "\n%04d ", byteCount + 100);
		fprintf(fp,"%02X ", ((unsigned char*)codeArray)[byteCount]);
		byteCount++;
	}
	while (byteCount < IC - 100 + DC) {
		if (byteCount % 4 == 0) fprintf(fp, "\n%04d ", byteCount + 100);
		fprintf(fp,"%02X ", dataArray[byteCount-IC + 100]);
		byteCount++;
	}
	fclose(fp);
	free(assemblyFileName);
}


void createEnt(label* labels, int labelCount,  char* assemblyFileName)
{
	FILE* fp;
	int i = 0;
	char* fileName = getFileName(assemblyFileName);
	strcat(fileName, ".ent");
	fp = fopen(fileName, "w");
	for (i = 0; i < labelCount; i++) {
		if (labels[i].isEntry) {
			fprintf(fp,"%s ", labels[i].symbol);
			fprintf(fp,"%04d\n", labels[i].address);
		}
	}
	free(assemblyFileName);
}
void createExt(extUse* extArray, int extArrayLength, char* assemblyFileName) {
	FILE* fp;
	int i = 0;
	char* fileName = getFileName(assemblyFileName);
	strcat(fileName, ".ext");
	fp = fopen(fileName, "w");
	for (i = 0; i < extArrayLength; i++) {
		fprintf(fp,"%s ", extArray[i].label);
		fprintf(fp," %04d\n", extArray[i].IC);
	}
	free(assemblyFileName);
}

