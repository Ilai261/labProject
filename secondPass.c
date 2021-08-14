/* This is the file that runs the second pass function of the assembler */

#include "utilityFunctions.h"
bool secondPass(FILE* fp, label* labels, int labelCount, unsigned int* codeArray, int* IC, int* DC, extUse* extArray,int* extArrayLength, operation* operations, int* labelLines)
{
	int numOfLabelLines = labelLines[0];
	int thisLine = 0;
	int lineIdx;
	char temp[80] = "";
	char parameters[80] = "";
	char oglineStr[80];
	char* lineStr;
	char opScanStr[80];
	char* opName;
	char labelName[32];
	int i = 0;
	int linesWithLabels = 0;
	operation currentOperation;
	bool retVal = true;

	for (i = 0; i < labelCount; i++) {
		if (labels[i].isData) {
			labels[i].address += *IC;
		}
	}

	while (fgets(oglineStr, 80, fp) != NULL) {
		int IC;
		temp[0] = '\0';
		parameters[0] = '\0';
		thisLine++;
		int lineLength = strlen(oglineStr);
		if(oglineStr[lineLength -1] == '\n') oglineStr[lineLength -1] = '\0';
		lineStr = oglineStr;
		scanStrAndMove(&lineStr, "%s", opScanStr);
		if (strcmp(opScanStr, ".entry") == 0) {
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
		if (currentOperation.opcode >= 15) {
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

