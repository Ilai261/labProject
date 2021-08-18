#include "assemblyData.h"

/*creates the object file*/
bool createObject(unsigned int* codeArray, unsigned char* dataArray, int IC, int DC, char* assemblyFileName);

/*creates the entries file*/
bool createEnt(labelData* labels, int labelCount, char* assemblyFileName);

/*creates the externals file*/
bool createExt(extUse* extArray, int extArrayLength, char* assemblyFileName);
