#include "assemblyData.h"

int operationNum(operationData* operations, char* string);

int labelNum(labelData* labels,int labelCount, char* string);

int isGuidance(char* string);

int intPow(int base, int exp);

int scanStrAndMove(char **readString, char* formatString, char * writeString);

int scanIntAndMove(char **readString, char* formatString, int * writeInt);

int numberLength(int x);

void writeToBits(unsigned int* placeToWrite, int startBit, int endBit, int data);

int moveAndScanInt(char** readString, char* formatString, int* writeInt);