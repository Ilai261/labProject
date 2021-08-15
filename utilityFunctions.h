#include "assemblyData.h"

bool checkLabel(char* labelName,labelData* labels, int labelCount, operationData*, int lineCount); 

int operationNum(operationData* operations, char* string);

int labelNum(labelData* labels,int labelCount, char* string);

int isGuidance(char* string);

void writeDataFromGuidance(int guidanceNum,unsigned char** dataArray,int *DC,char* dataString);

int intPow(int base, int exp);

int scanStrAndMove(char **readString, char* formatString, char * writeString);

int scanIntAndMove(char **readString, char* formatString, int * writeInt);

int numberLength(int x);

int operationCode(operationData currentOperation, char* parameters);

bool fileApproved(char* fileName);

bool fileApproved(char* fileName);

void writeToBits(unsigned int* placeToWrite, int startBit, int endBit, int data);

bool parameterCheck(int line, int IC, char* parameters, operationData currentOperation, int** labelLines);

int operationLabelCode(operationData currentOperation, char* parameters, unsigned int* codeArray, int lineNum, int IC, labelData* labels, int labelCount);

int moveAndScanInt(char** readString, char* formatString, int* writeInt);

bool checkGuidanceParam(int line, int guidanceNum, char* parameters);