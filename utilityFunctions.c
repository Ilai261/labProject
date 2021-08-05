#include "utilityFunctions.h"
#include "structsAndFuncs.h"
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
	
	if(labelNum(labels,labelCount,labelName)  == -1){
		printf("Line %d: a label with that name already exists", lineCount);
		return false;
		
	}
	
	if(operationNum(operations,labelName)  == -1){
		printf("Line %d: label name can't be an operation name", lineCount);
		return false;
		
	}
	
	if(isGuidance(labelName)){
		printf("Line %d: label name can't be A guidance word name", lineCount);
		return false;
		
	}
	return true;
	
		
}

int operationNum(operation* operations, char* string){
	/*maybe improve the efficiency*/
	for(int i = 0; i < numOfOperations; i++)
	{
		if(strcmp(string,operations[i].operationName) == 0){
			return i;
		}
	}
	
	return -1;	
}

int labelNum(label* labels,int labelCount, char* string){
	for(int i = 0; i < labelCount; i++)
	{
		if(strcmp( string,labels[i].symbol) == 0){
			return i;
		}
	}
	
	return -1;
}

int isGuidance(char* string){
	char *guidanceWords[numOfGuidanceWords] = {"db","dw","dh","asciz","entry","extern"};
	 for(int i = 0; i < numOfGuidanceWords; i++)
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
		char num;
		*dataArray = realloc(*dataArray,*DC+20);
		
		if(sscanf(dataString,"%hhd", &num)) {
			*dataArray[*DC+i] = num;
			i++;
		}
		while(sscanf(dataString,",%hhd", &num)){
			*dataArray[*DC+i] = num;
			i++;
		}
		*DC += i;
	}
	if(guidanceNum == 1){
		int i = 0;
		short int  num;
		*dataArray = realloc(*dataArray,*DC+40);
		
		if(sscanf(dataString,"%hd", &num)) {
			*dataArray[*DC+i] = num;
			i +=2;
		}
		while(sscanf(dataString,",%hd", &num)){
			*dataArray[*DC+i] = num;
			i +=2;
		}
		*DC += i;
	}
	if(guidanceNum == 2){
		int i = 0;
		int  num;
		*dataArray = realloc(*dataArray,*DC+40);
		
		if(sscanf(dataString,"%d", &num)) {
			*dataArray[*DC+i] = num;
			i += 4;
		}
		while(sscanf(dataString,",%d", &num)){
			*dataArray[*DC+i] = num;
			i += 4;
		}
		*DC += i;
	}
	if(guidanceNum == 3){
		char* s;
		int sLength;
		sscanf(dataString,"\"%s\"", &s);
		sLength = strlen(s);
		*DC += sLength;
		*dataArray = realloc(*dataArray,*DC);
		 int i;
		 for(i = 0; i< sLength; i++){

			*dataArray[*DC+i] = s[i];

		 }
	}
}




