#include "createOutputFiles.h"
#include "utilityFunctions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* getFileName(char*);

/*creates the object file*/
bool createObject(unsigned int* codeArray, unsigned char* dataArray, int IC, int DC, char* assemblyFileName) {
	FILE* fp;
	int byteCount = 0;
	char* fileName = getFileName(assemblyFileName);
	strcat(fileName, ".ob");
	fp = fopen(fileName, "w"); /*change to if != NULL sussy*/
	if (fp != NULL) {
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
	}
	else {
		return false;
	}
	free(fileName);
	return true;
}

/*creates the entries file*/
bool createEnt(labelData* labels, int labelCount, char* assemblyFileName)
{
	FILE* fp;
	int i = 0;
	char* fileName = getFileName(assemblyFileName);
	strcat(fileName, ".ent");
	fp = fopen(fileName, "w");
	if (fp != NULL) {
		for (i = 0; i < labelCount; i++) {
			if (labels[i].isEntry) {
				fprintf(fp, "%s ", labels[i].symbol);
				fprintf(fp, "%04d\n", labels[i].address);
			}
		}
		fclose(fp);
	}
	else {
		return false;
	}
	free(fileName);
	return true;
}

/*creates the exturnals file*/
bool createExt(extUse* extArray, int extArrayLength, char* assemblyFileName) {
	FILE* fp;
	int i = 0;
	char* fileName = getFileName(assemblyFileName);
	strcat(fileName, ".ext");
	fp = fopen(fileName, "w");
	if (fp != NULL) {
		for (i = 0; i < extArrayLength; i++) {
			fprintf(fp, "%s ", extArray[i].label);
			fprintf(fp, " %04d\n", extArray[i].IC);
		}
		fclose(fp);
	}
	else {
		return false;
	}
	free(fileName);
	return true;
}

/*removes the .as */
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