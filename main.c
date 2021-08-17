/* This is the file that runs the main function of the assembler */

#include "firstPass.h"
#include "secondPass.h"
#include "createOutputFiles.h"
#include "utilityFunctions.h"
#include "assemblyData.h"

#include <stdlib.h>

/*This function gets a string and returns true if it ends with .as, and false if else. Therefore it checks if a file is approved for assmebling in our project.*/
bool fileApproved(char* fileName);

/*This is the main function that runs the assembler*/ 
int main(int argc, char *argv[])
{
	int fileIndex;
	for (fileIndex = 1; fileIndex < argc; fileIndex++){
		labelData* labels = NULL;
		int labelCount = 0;
		unsigned int* codeArray = malloc(10);
		unsigned char* dataArray = malloc(10);
		int* labelLines = malloc(40);
		int IC = 100;
		int DC = 0;
		FILE* fp = NULL;
		bool firstPassSuccessful = false;
		int JOpCounter = 0; /*This variable is used to measure the amount of J operation lines received*/
		char* fileName = argv[fileIndex];
		labelLines[0] = 0;
		fp = fopen(fileName, "r");
		if(!fileApproved(fileName) || fp == NULL)
		{
			printf("Error, couldn't open file %s\n", fileName);
			continue;
		}
		labelCount = firstPass(fp, &labels, &dataArray, &codeArray, &IC, &DC, &JOpCounter, operationsArr, &labelLines);
		firstPassSuccessful = labelCount >= 0;
		fclose(fp);
		fp = fopen(fileName, "r");
		if (firstPassSuccessful == true)
		{
			int extArrayLength = 0;
			extUse* extArray = calloc(JOpCounter, sizeof(extUse)); /*To make the .ext file*/
			if (secondPass(fp, labels, labelCount, codeArray, &IC, &DC, extArray, &extArrayLength, operationsArr, labelLines)) {
				if (!createObject(codeArray, dataArray, IC, DC, fileName)) {
					printf("Error, couldn't create object file");
				}
				if (!createEnt(labels, labelCount, fileName)) {
					printf("Error, couldn't create entries file");
				}
				if (!createExt(extArray, extArrayLength, fileName)) {
					printf("Error, couldn't create externals file");
				}
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

bool fileApproved(char* fileName) {
	int i = 0;
	while (fileName[i] != '\0') i++;
	if (i >= 5 && fileName[i - 1] == 's' && fileName[i - 2] == 'a' && fileName[i - 3] == '.') return true;
	printf("did not approve\n");
	return false;
}
