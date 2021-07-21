#include <stdio.h>
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
