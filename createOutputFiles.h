#include "assemblyData.h"


bool createObject(unsigned int* codeArray, unsigned char* dataArray, int IC, int DC, char* assemblyFileName);

bool createEnt(labelData* labels, int labelCount, char* assemblyFileName);

bool createExt(extUse* extArray, int extArrayLength, char* assemblyFileName);
