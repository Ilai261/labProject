#include "assemblyData.h"
#include <stdio.h>

bool firstPass(FILE* fp, labelData** labels, int* labelCount, unsigned char** dataArray, unsigned int** codeArray, int* IC, int* DC, int* JOpCounter, operationData* operations);
