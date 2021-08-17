/* This is the file that runs the second pass function of the assembler */

#include "secondPass.h"
#include "utilityFunctions.h"

#include <string.h>
/* Completes the binary code that could not be written in the first pass And checks the parameters for errors,
Returns -1 if there is an error, 0 if it didn't Write, 1 if it wrote code and 2 if the label used is external*/
int operationLabelCode(operationData currentOperation, char* parameters, unsigned int* codeArray, int lineNum, int IC, labelData* labels, int labelCount);

/*runs the second pass of the assembler, it fils the parts of data array that could not be written in the first pass and checks the code for errors,
if successful it returns true*/
bool secondPass(FILE* fp, labelData* labels, int labelCount, unsigned int* codeArray, int* IC, int* DC, extUse* extArray,int* extArrayLength, operationData* operations, int* labelLines)
{
	int numOfLabelLines = labelLines[0];
	int thisLine = 0;
	char temp[maxLineStrLength] = "";
	char parameters[maxLineStrLength] = "";
	char oglineStr[maxLineStrLength];
	char* lineStr;
	char opScanStr[maxLineStrLength];
	char* opName;
	char labelName[maxLabelStrLength];
	int i = 0;
	int linesWithLabels = 0;
	operationData currentOperation;
	bool retVal = true;

	for (i = 0; i < labelCount; i++) { /*update the label addresses*/
		if (labels[i].isData) {
			labels[i].address += *IC;
		}
	}

	while (fgets(oglineStr, 80, fp) != NULL) { /*get new line*/
		int IC;
		temp[0] = '\0';
		parameters[0] = '\0';
		thisLine++;
		int lineLength = strlen(oglineStr);
		if(oglineStr[lineLength -1] == '\n') oglineStr[lineLength -1] = '\0';
		lineStr = oglineStr;
		scanStrAndMove(&lineStr, "%s", opScanStr);
		if (strcmp(opScanStr, ".entry") == 0) {  /*update the entry labels*/
			scanStrAndMove(&lineStr, "%s", labelName);
			labels[labelNum(labels, labelCount, labelName)].isEntry = true;
			continue;
		}

		if (opScanStr[strlen(opScanStr) - 1] == ':') {
			scanStrAndMove(&lineStr, "%s", opScanStr);
		}

		opName = opScanStr;
		int opNum = operationNum(operations, opName);
		currentOperation = operations[opNum];
		if (currentOperation.opcode >= 15) {/*write the missing code */
			while (scanStrAndMove(&lineStr, "%s", temp) > 0) {
				strcat(parameters, temp);
			}
			IC = labelLines[linesWithLabels + 1];
			int opReturn = operationLabelCode(currentOperation, parameters, codeArray, thisLine, IC, labels, labelCount);
			if (opReturn == -1) {
				retVal = false;
				continue;
			}
			if (opReturn == 2) {
				extArray[*extArrayLength].IC = IC;
				strcpy(extArray[*extArrayLength].label, parameters);
				linesWithLabels++;
				(*extArrayLength)++;
				continue;
			}
			linesWithLabels += opReturn;
		}
	}
	return retVal;
}

int operationLabelCode(operationData currentOperation, char* parameters, unsigned int* codeArray, int lineNum, int IC, labelData* labels, int labelCount) {
	labelData paramLabel;
	int opPos = (IC - 100) / 4;
	int labelNumber;
	if (currentOperation.operationType == 'J') {
		if (currentOperation.opcode == 30) {
			int regNum = 0;
			if (sscanf(parameters, "$%d", &regNum) > 0) {
				return 0;
			}
			else {
				labelNumber = labelNum(labels, labelCount, parameters);
				if (labelNumber == -1) {
					printf("Line %d: unknown label used as a parameter for J operation\n", lineNum);
					return -1;
				}
				paramLabel = labels[labelNumber];
				if (paramLabel.isExternal) {
					return 2;
				}
				writeToBits(&codeArray[opPos], 0, 24, paramLabel.address);
				return 1;
			}

		}
		if (currentOperation.opcode <= 32) {
			labelNumber = labelNum(labels, labelCount, parameters);
			if (labelNumber == -1) {
				printf("Line %d: unknown label used as a parameter for J operation\n", lineNum);
				return -1;
			}
			paramLabel = labels[labelNumber];
			if (paramLabel.isExternal) {
				return 2;
			}
			writeToBits(&codeArray[opPos], 0, 24, paramLabel.address);
			return 1;
		}
	}
	if (currentOperation.operationType == 'I') {
		if (currentOperation.opcode < 19) {
			int commaCounter = 0;
			short int distanceToLabel;
			while (commaCounter < 2) {
				if (*parameters == COMMA) commaCounter++;
				parameters++;
			}
			labelNumber = labelNum(labels, labelCount, parameters);
			if (labelNumber == -1) printf("Line %d: unknown label used as a parameter for I operation\n", lineNum);
			paramLabel = labels[labelNumber];
			if (paramLabel.isExternal) {
				printf("Line %d: external label cannot be used as a parameter for I operation\n", lineNum);
				return -1;
			}
			distanceToLabel = (short int)(paramLabel.address - IC);
			writeToBits(&codeArray[opPos], 0, 15, distanceToLabel);
			return 1;
		}
	}
	return 0;
}

