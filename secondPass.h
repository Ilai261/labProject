#include "assemblyData.h"
#include <stdio.h>

/*runs the second pass of the assembler. it fills the parts of data array that could not be written in the first pass and checks the code for errors.
if successful it returns true*/

bool secondPass(FILE* fp, labelData* labels, int labelCount, unsigned int* codeArray, int* IC, int* DC, extUse* extArray, int* extArrayLength, operationData* operations);
