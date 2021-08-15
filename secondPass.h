#include "assemblyData.h"
#include <stdio.h>

bool secondPass(FILE* fp, labelData* labels, int labelCount, unsigned int* codeArray, int* IC, int* DC, extUse* extArray, int* extArrayLength, operationData* operations, int* labelLines);
