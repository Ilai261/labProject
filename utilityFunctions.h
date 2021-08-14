#include "structsAndFuncs.h"
bool checkLabel(char* labelName,label* labels, int labelCount, operation*, int lineCount); 
int operationNum(operation* operations, char* string);
int labelNum(label* labels,int labelCount, char* string);
int isGuidance(char* string);
void writeDataFromGuidance(int guidanceNum,unsigned char** dataArray,int *DC,char* dataString);
int intPow(int base, int exp);
int scanStrAndMove(char **readString, char* formatString, char * writeString);
int scanIntAndMove(char **readString, char* formatString, int * writeInt);
int numberLength(int x);
int operationCode(operation currentOperation, char* parameters);
bool fileApproved(char* fileName);
bool fileApproved(char* fileName);
void writeToBits(unsigned int* placeToWrite, int startBit, int endBit, int data);
bool parameterCheck(int line, int IC, char* parameters, operation currentOperation, int** labelLines);
int operationLabelCode(operation currentOperation, char* parameters, unsigned int* codeArray, int lineNum, int IC, label* labels, int labelCount);
int moveAndScanInt(char** readString, char* formatString, int* writeInt);
bool checkGuidanceParam(int line, int guidanceNum, char* parameters);