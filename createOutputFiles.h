#include "assemblyData.h"


void createObject(unsigned int* codeArray, unsigned char* dataArray, int IC, int DC, char* assemblyFileName);

void createEnt(labelData* labels, int labelCount, char* assemblyFileName);

void createExt(extUse* extArray, int extArrayLength, char* assemblyFileName);
