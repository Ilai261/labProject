/* This is the file that runs the second pass function of the assembler */

#include "utilityFunctions.h"
void secondPass(FILE* fp, label* labels, int labelCount, unsigned int* dataArray, int* IC, int* DC, operation* operations, int* labelLines)
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

	for (i = 0; i < labelCount; i++) {
		if (labels[i].isData) {
			labels[i].address += *IC + 100;
		}
	}

	for (lineIdx = 1; lineIdx < numOfLabelLines + 1; lineIdx++) {

		while (thisLine < labelLines[lineIdx]) {
			fgets(oglineStr, 80, fp);
			lineStr = oglineStr;
			scanStrAndMove(&lineStr, "%s", opScanStr);
			if (strcmp(opName, ".entry") == 0) {
				scanStrAndMove(&lineStr, "%s", labelName);
				labels[labelNum(labels, labelCount, labelName)].isEntry = true;
			}
		}
		lineStr = oglineStr;
		scanStrAndMove(&lineStr, "%s", opScanStr);
		if (opScanStr[strlen(opScanStr) - 1] == ':') {
			scanStrAndMove(&lineStr, "%s", opScanStr);	
		}

		opName = opScanStr;
		

		operation currentOperation = operations[operationNum(operations, opName)];
		while (scanStrAndMove(&lineStr, "%s", temp) > 0) {
			strcat(parameters, temp);
		}
		
		
	}
}
void operationLabelCode(operation currentOperation, char* parameters, unsigned int* dataArray,int lineIdx, label* labels, int labelCount) {
	if (currentOperation.operationType == 'J') {
		label paramLabel;
		if (currentOperation.opcode == 30) {
			int regNum = 0;
			if (sscanf(parameters, "$%d", &regNum) > 0) {
				return;
			}
			else {
				paramLabel = labels[labelNum(labels, labelCount, parameters)];
				writeToBits(&dataArray[lineIdx*4], 0, 24, 0);
			}
		}
		if (currentOperation.opcode <= 32) {
			paramLabel = labels[labelNum(labels, labelCount, parameters)];
			writeToBits(&dataArray[lineIdx * 4], 0, 24, 0);
		}








}