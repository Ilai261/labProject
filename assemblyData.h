
#ifndef ASSEMBLY_DATA_H
#define ASSEMBLY_DATA_H

#include <stdbool.h>

#define numOfOperations 27
#define numOfGuidanceWords 6
#define maxLineStrLength 81
#define maxLabelStrLength 32
#define DOLLAR '$'
#define COMMA ','

typedef struct /*a struct to hold an operation*/
{
	char operationName[5];
	char operationType;
	int funct;
	int opcode;

} operationData;

typedef struct /*a struct to hold a label*/
{
	char symbol[32];
	int address;
	bool isEntry;
	bool isExternal;
	bool isData;
	bool isCode;

} labelData;

typedef struct /*a struct to hold a use of an external label*/
{
	char label[32];
	int IC;
} extUse;

operationData operationsArr[numOfOperations];

#endif
