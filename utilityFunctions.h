#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
typedef struct  
{
	char[5] operationName;
	char operationType;
	int funct;
	int opcode;
	
} operation;

typedef struct  
{
	char* symbol;
	int value;
	bool isEntry;
	bool isExternal;
	bool isData;
	bool isCode;
} label;

char* checkLabel(char* label,label* labels, int labelCount, operation*, int lineCount); 
int operationNum(operation* operations, char* string);
int labelNum(label* labels,int labelCount, char* string);
int isGuidance(char* string);
