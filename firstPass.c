/* This is the file that runs the first pass function of the assembler */
#include "utilityFunctions.h"

int firstPass(FILE* fp, label** labels, unsigned char** dataArray, unsigned int** codeArray, int* IC, int* DC, operation* operations, int ** labelLines)
{
	char* line = malloc(81);
	char* ogLine = line;
	int labelCount = 0;
	int lineLength = 0;
	int lineCount = 1;
	bool isSuccessful = true;
	char c;
	bool isNewLine = false;
	while (true)
	{
		c = getc(fp);
		if (isNewLine) {
			line = ogLine;
			lineCount++;
			lineLength = 0;
			isNewLine = false;
		}
		if (c != '\n' && c != EOF)
		{
			if (lineLength <= 80)
			{
				line[lineLength] = c;
				lineLength++;
			}
			else
			{
				printf("Line %d: more than 80 characters in line\n", lineCount);
				isSuccessful = false;

			}
		}
		else
		{
			/*first we will check if the line is empty or a comment, then we will check if is starts with a label*/
			int i;
			bool isEmptyLine = true;
			char firstChar = ' ';
			line[lineLength] = '\0';

			for (i = 0; i < lineLength; i++)
			{
				if (!isspace(line[i])) {
					isEmptyLine = false;
					firstChar = line[i];
					break;
				}
			}

			if (isEmptyLine == false && firstChar != ';') /* not an empty line and not a comment*/
			{
				label labelToAdd = {"",0,false,false,false,false};
				char dataString[80] = "";
				char  temp[80] = "";
				char parameters[80] = "";
				int guidanceNum;
				char* operationName;
				int operationNumber;
				operation currentOperation;
				char *labelName = malloc(32);
				bool isLabel = false;
				bool isLabelOk = true;
				int codeByte;
				int i = 0;
				if (scanStrAndMove(&line, "%s", labelName) == 1)
				{
					while (labelName[i] != '\0') i++;
					if (labelName[i - 1] == ':') {
						labelName[i - 1] = '\0';
						isLabelOk = checkLabel(labelName, *labels, labelCount, operations, lineCount);
						if (isLabelOk)
						 {

							isLabel = true;
							if (labelCount % 10 == 0) {
								*labels = (label*)realloc(*labels, sizeof(label) * (labelCount + 10));
							}
							labelCount++;
						}
					}
				}


				if (!isLabelOk) {
					isNewLine = true;
					continue;
				}
				/*add the label to the tabel, continue scanning until end of line*/
				operationName = malloc(10);
				if (isLabel) {
					scanStrAndMove(&line, "%s", operationName);
				}
				else {
					free(operationName);
					operationName = labelName;
				}
				guidanceNum = isGuidance(operationName + 1); /*taking the next pointer to skip the point in the first char*/
				if (operationName[0] == '.' && guidanceNum >= 0) {/*do if it is a guidance row*/

					if (guidanceNum <= 3) {
						if (isLabel) {
							labelToAdd.symbol[0] = '\0'; labelToAdd.address = *(DC); labelToAdd.isEntry = false; 
							labelToAdd.isExternal = false; labelToAdd.isData = true; labelToAdd.isCode = false;
							strcat(labelToAdd.symbol, labelName);
							(*labels)[labelCount - 1] = labelToAdd;
						}


						while (scanStrAndMove(&line, "%s", temp) > 0) {
							strcat(dataString, temp);
						}
						writeDataFromGuidance(guidanceNum, dataArray, DC, dataString);
					}
					else {
						if (guidanceNum == 4) {
							isNewLine = true;
							continue;
						}
						if (guidanceNum == 5) {
							char  externName[32];
							scanStrAndMove(&line, "%s", externName);
							if (checkLabel(externName, *labels, labelCount, operations, lineCount)) {
								if (labelCount % 10 == 0) {
									*labels = (label*)realloc((*labels), sizeof(label) * (labelCount + 10));
								}
								labelCount++;
								labelToAdd.symbol[0] = '\0'; labelToAdd.address = 0; labelToAdd.isEntry = false; 
								labelToAdd.isExternal = true; labelToAdd.isData = false; labelToAdd.isCode = false;
								strcat(labelToAdd.symbol, externName);
								(*labels)[labelCount - 1] = labelToAdd;
								if (isLabel) printf("Line %d: label before extern is meaningless", lineCount);
							}

						}
					}
				}
				else {
					if (isLabel) {
						labelToAdd.symbol[0] = '\0'; labelToAdd.address = *(IC); labelToAdd.isEntry = false; 
						labelToAdd.isExternal = false; labelToAdd.isData = false; labelToAdd.isCode = true;
						strcat(labelToAdd.symbol, labelName);
						(*labels)[labelCount-1] = labelToAdd;
					}

					operationNumber = operationNum(operations, operationName);
					if (operationNumber == -1) {
						printf("Line %d: unknown word used as operation or guidance", lineCount);
						isNewLine = true;
						continue;

					}

					currentOperation = operations[operationNumber];

					while (scanStrAndMove(&line, "%s", temp) > 0) {
						strcat(parameters, temp);
					}

					codeByte = (*IC - 100);
					if (codeByte % 40 == 0) {
						*codeArray = realloc((*codeArray), codeByte + 40);
					}
					if (parameterCheck( lineCount,*IC, parameters, currentOperation, labelLines)) {
						(*codeArray)[codeByte / 4] = operationCode(currentOperation, parameters);
						*IC += 4;
					}

				}
				if (isLabel) {
					free(operationName);
				}
				free(labelName);
				isNewLine = true;
				/*sscanf for an operation word*/
			}

		}
		if (c == EOF)break;
	}
	/*same as else*/	
	if (!isSuccessful) return -1;
	return labelCount;
}
 
