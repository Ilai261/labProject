#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define numOfOperations 27
#define numOfGuidanceWords 6

typedef struct  
{
	char operationName[5];
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



int firstPass(FILE* fp,label** labels,unsigned char** dataArray, int* IC, int* DC, operation* operations);
void secondPass(FILE *,label *, int labelCount ,unsigned char*, int*, int*);
void createObject(unsigned char*);
void createExt(label*);
void createEnt(label*);
