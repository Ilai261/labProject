/* This is the file that runs the second pass function of the assembler */

#include "utilityFunctions.h"
void operationLabelCode(operation currentOperation, char* parameters, unsigned int* codeArray, int lineIdx, label* labels, int labelCount);
void secondPass(FILE* fp, label* labels, int labelCount, unsigned int* codeArray, int* IC, int* DC, operation* operations, int* labelLines)
{
	int numOfLabelLines = labelLines[0];
	int thisLine = 0;
	int lineIdx;
	char temp[80] = "";
	char parameters[80] = "";
	char oglineStr[80];
	char *lineStr;
	char opScanStr[80];
	char* opName;
	char labelName[32];
	int i = 0;
	operation currentOperation;

	for (i = 0; i < labelCount; i++) {
		if (labels[i].isData) {
			labels[i].address += *IC;
		}
	}

	for (lineIdx = 1; lineIdx < numOfLabelLines + 1; lineIdx++) {
		temp[0] = '\0';
		parameters[0] = '\0';
		while (thisLine < labelLines[lineIdx]) {
			fgets(oglineStr, 80, fp);
			lineStr = oglineStr;
			scanStrAndMove(&lineStr, "%s", opScanStr);
			if (strcmp(opScanStr, ".entry") == 0) {
				scanStrAndMove(&lineStr, "%s", labelName);
				labels[labelNum(labels, labelCount, labelName)].isEntry = true;
			}
			thisLine++;
		}
		lineStr = oglineStr;
		scanStrAndMove(&lineStr, "%s", opScanStr);
		if (opScanStr[strlen(opScanStr) - 1] == ':') {
			scanStrAndMove(&lineStr, "%s", opScanStr);	
		}

		opName = opScanStr;

		currentOperation = operations[operationNum(operations, opName)];
		while (scanStrAndMove(&lineStr, "%s", temp) > 0) {
			strcat(parameters, temp);
		}
		operationLabelCode(currentOperation, parameters, codeArray, thisLine, labels, labelCount);
		
	}
	while (fgets(oglineStr, 80, fp) == NULL) {
		lineStr = oglineStr;
		scanStrAndMove(&lineStr, "%s", opScanStr);
		if (strcmp(opScanStr, ".entry") == 0) {
			scanStrAndMove(&lineStr, "%s", labelName);
			labels[labelNum(labels, labelCount, labelName)].isEntry = true;
		}
		thisLine++;
	}
}

