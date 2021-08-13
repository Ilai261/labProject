/* This is the file that runs the second pass function of the assembler */

#include "utilityFunctions.h"
void secondPass(FILE* fp, label* labels, int labelCount, unsigned int* codeArray, int* IC, int* DC, operation* operations, int* labelLines)
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
		oglineStr[strlen(oglineStr) - 1] = '\0';
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
		if (opNum >= 15) {
			currentOperation = operations[opNum];
			while (scanStrAndMove(&lineStr, "%s", temp) > 0) {
				strcat(parameters, temp);
			}
			IC = labelLines[linesWithLabels + 1];
			linesWithLabels += operationLabelCode(currentOperation, parameters, codeArray, thisLine, IC, labels, labelCount);
		}


		/*while (fgets(oglineStr, 80, fp) == NULL) {
			lineStr = oglineStr;
			scanStrAndMove(&lineStr, "%s", opScanStr);
			if (strcmp(opScanStr, ".entry") == 0) {
				scanStrAndMove(&lineStr, "%s", labelName);
				labels[labelNum(labels, labelCount, labelName)].isEntry = true;
			}
			thisLine++;
		}*/
	}
}

