#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define numOfOperations 27
#define numOfGuidanceWords 6
#define DOLLAR '$'
#define COMMA ','

typedef struct  
{
	char operationName[5];
	char operationType;
	int funct;
	int opcode;
	
} operation;

typedef struct  
{
	char symbol[32];
	int address;
	bool isEntry;
	bool isExternal;
	bool isData;
	bool isCode;
} label;



int firstPass(FILE* fp,label** labels,unsigned char** dataArray,unsigned int** codeArray, int* IC, int* DC, operation* operations);
void secondPass(FILE * fp,label * labels, int labelCount ,unsigned int* dataArray, int* IC, int* DC,operation* operations);
void createObject(unsigned int* codeArray, char* fileName);
void createExt(label*, char* fileName);
void createEnt(label*, char* fileName);
