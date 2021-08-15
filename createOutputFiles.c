#include "createOutputFiles.h"
#include "utilityFunctions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* getFileName(char*);

void createObject(unsigned int* codeArray, unsigned char* dataArray, int IC, int DC, char* assemblyFileName) {
	FILE* fp;
	int byteCount = 0;
	char* fileName = getFileName(assemblyFileName);
	strcat(fileName, ".ob");
	fp = fopen(fileName, "w"); /*change to if != NULL sussy*/
	fprintf(fp, "	%d %d", IC - 100, DC);
	while (byteCount < IC - 100) {
		if (byteCount % 4 == 0) fprintf(fp, "\n%04d ", byteCount + 100);
		fprintf(fp, "%02X ", ((unsigned char*)codeArray)[byteCount]);
		byteCount++;
	}
	while (byteCount < IC - 100 + DC) {
		if (byteCount % 4 == 0) fprintf(fp, "\n%04d ", byteCount + 100);
		fprintf(fp, "%02X ", dataArray[byteCount - IC + 100]);
		byteCount++;
	}
	fclose(fp);
	free(fileName);
}


void createEnt(labelData* labels, int labelCount, char* assemblyFileName)
{
	FILE* fp;
	int i = 0;
	char* fileName = getFileName(assemblyFileName);
	strcat(fileName, ".ent");
	fp = fopen(fileName, "w");
	for (i = 0; i < labelCount; i++) {
		if (labels[i].isEntry) {
			fprintf(fp, "%s ", labels[i].symbol);
			fprintf(fp, "%04d\n", labels[i].address);
		}
	}
	free(fileName);
	fclose(fp);
}
void createExt(extUse* extArray, int extArrayLength, char* assemblyFileName) {
	FILE* fp;
	int i = 0;
	char* fileName = getFileName(assemblyFileName);
	strcat(fileName, ".ext");
	fp = fopen(fileName, "w");
	for (i = 0; i < extArrayLength; i++) {
		fprintf(fp, "%s ", extArray[i].label);
		fprintf(fp, " %04d\n", extArray[i].IC);
	}
	free(fileName);
	fclose(fp);
}

char* getFileName(char* fileName)
{
	int i = 0;
	char* retval = (char*)calloc(100, 1);
	strcpy(retval, fileName);
	while (retval[i] != '\0') i++;

	if (i >= 5) {
		retval[i - 3] = '\0';
	}
	return retval;

}