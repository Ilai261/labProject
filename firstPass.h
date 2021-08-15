#include "assemblyData.h"
#include <stdio.h>

int firstPass(FILE* fp, labelData** labels, unsigned char** dataArray, unsigned int** codeArray, int* IC, int* DC, int* JOpCounter, operationData* operations, int** labelLines);
