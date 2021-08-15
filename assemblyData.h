
#ifndef ASSEMBLY_DATA_H
#define ASSEMBLY_DATA_H

#include <stdbool.h>

#define numOfOperations 27
#define numOfGuidanceWords 6
#define maxLineStrLength 81
#define maxLabelStrLength 32
#define DOLLAR '$'
#define COMMA ','

typedef struct
{
	char operationName[5];
	char operationType;
	int funct;
	int opcode;

} operationData;

typedef struct
{
	char symbol[32];
	int address;
	bool isEntry;
	bool isExternal;
	bool isData;
	bool isCode;

} labelData;

typedef struct
{
	char label[32];
	int IC;
} extUse;

operationData operationsArr[numOfOperations];

#endif
