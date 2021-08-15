/* This is the file that runs the main function of the assembler */

#include "firstPass.h"
#include "secondPass.h"
#include "createOutputFiles.h"
#include "utilityFunctions.h"
#include "assemblyData.h"

#include <stdlib.h>

int main(int argc, char *argv[])
{
	
	labelData* labels = NULL;
	int labelCount = 0;
	unsigned int* codeArray = malloc(10);
	unsigned char* dataArray = malloc(10);
	int* labelLines = malloc(40);
	int IC = 100;
	int DC = 0;
	int x;
	FILE* fp = NULL;
	bool firstPassSuccessful = false;
	int JOpCounter = 0;
	labelLines[0] = 0;
	for (x = 1; x < argc; x++){
		char* fileName = argv[x];
		fp = fopen(fileName, "r");
		if(!fileApproved(fileName) || fp == NULL)
		{
			printf("Error, couldn't open file %s\n", fileName);
			continue;
		}
		labelCount = firstPass(fp,&labels,&dataArray, &codeArray, &IC, &DC,&JOpCounter, operationsArr,&labelLines);
		firstPassSuccessful = labelCount >= 0;
		fclose(fp);
		fp = fopen(fileName, "r");
		if (firstPassSuccessful == true)
		{
			int extArrayLength = 0;
			extUse* extArray = calloc(JOpCounter, sizeof(extUse));
			if (secondPass(fp, labels, labelCount, codeArray, &IC, &DC, extArray, &extArrayLength, operationsArr, labelLines)) {
				createObject(codeArray, dataArray, IC, DC, fileName);
				createEnt(labels, labelCount, fileName);
				createExt(extArray, extArrayLength, fileName);
			}
			free(labelLines);
			free(codeArray);
			free(dataArray);
			free(extArray);
		}
		fclose(fp);
	}
	return 0;
}
