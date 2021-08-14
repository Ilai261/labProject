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
typedef struct
{
	char label[32];
	int IC;
} extUse;

int firstPass(FILE* fp, label** labels, unsigned char** dataArray, unsigned int** codeArray, int* IC, int* DC, int* JOpCounter, operation* operations, int** labelLines);
bool secondPass(FILE* fp, label* labels, int labelCount, unsigned int* codeArray, int* IC, int* DC, extUse* extArray, int* extArrayLength, operation* operations, int* labelLines);
void createObject(unsigned int* codeArray, unsigned char* dataArray,int IC,int DC, char* assemblyFileName);
void createEnt(label* labels, int labelCount,  char* assemblyFileName);
void createExt(extUse* extArray,int extArrayLength, char* assemblyFileName);
