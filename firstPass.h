#include "assemblyData.h"
#include <stdio.h>

/*runs the first pass of the assembler. It fills the data array, labels array, and code array as much as possible and checks the code for errors.
If successful - returns true, otherwise returns false*/

bool firstPass(FILE* fp, labelData** labels, int* labelCount, unsigned char** dataArray, unsigned int** codeArray, int* IC, int* DC, int* JOpCounter, operationData* operations);
