#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define numOfOperations 26
#define numOfGuidanceWords 6

typedef struct  
{
	char[5] operationName;
	char operationType;
	int funct = 0;
	int opcode;
	
} operation;

typedef struct  
{
	char* symbol;
	int value;
	bool isEntry = false;
	bool isExternal = false;
	bool isData = false;
	bool isCode = false;
} label;



int firstPass(FILE* fp,label** labels, int* IC, int* DC, operation* operations)
secondPass(FILE*,label*, int labelCount ,unsigned char*, int*, int*);
createObject(unsigned char*);
createExt(label*);
createEnt(label*);
