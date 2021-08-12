#include "structsAndFuncs.h"
bool checkLabel(char* labelName,label* labels, int labelCount, operation*, int lineCount); 
int operationNum(operation* operations, char* string);
int labelNum(label* labels,int labelCount, char* string);
int isGuidance(char* string);
void writeDataFromGuidance(int guidanceNum,unsigned char** dataArray,int *DC,char* dataString);
int intPow(int base, int exp);
int writeToBits(unsigned int * placeToWrite,int startBit, int endBit, int data );
int scanStrAndMove(char **readString, char* formatString, char * writeString);
int scanIntAndMove(char **readString, char* formatString, int * writeInt);
int numOfDigits(int x);
int operationCode(operation currentOperation, char* parameters);
bool fileApproved(char* fileName);