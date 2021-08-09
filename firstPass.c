/* This is the file that runs the first pass function of the assembler */
#include "utilityFunctions.h"

int firstPass(FILE* fp, label** labels, unsigned char** dataArray, unsigned int** codeArray, int* IC, int* DC, operation* operations)
{
	char* line = malloc(81);
	char* ogLine = line;
	int labelCount = 0;
	int lineLength = 0;
	int lineCount = 1;
	bool isSuccessful = true;
	char c;
	bool isNewLine = false;
	while ((c = getc(fp)) != EOF)
	{
		if (isNewLine) {
			line = ogLine;
			lineCount++;
			lineLength = 0;
			isNewLine = false;
		}
		if (c != '\n')
		{
			if (lineLength <= 80)
			{
				line[lineLength] = c;
				lineLength++;
			}
			else
			{
				printf("Line %d: more than 80 characters in line", *(IC)-100);
				isSuccessful = false;

			}
		}
		else
		{
			/*first we will check if the line is empty or a comment, then we will check if is starts with a label*/
			int i = 0;
			bool isEmptyLine = true;
			char firstChar = ' ';
			line[lineLength] = '\0';

			for (i; i < lineLength; i++)
			{
				if (!isspace(line[i])) {
					isEmptyLine = false;
					firstChar = line[i];
					break;
				}
			}

			if (isEmptyLine == false && firstChar != ';') /* not an empty line and not a comment*/
			{
				char labelName[32];
				bool isLabel = false;
				bool isLabelOk = true;
				int i = 0;
				if (scanStrAndMove(&line, "%s", labelName) == 1)
				{
					while (labelName[i] != '\0') i++;
					if (labelName[i - 1] == ':') {
						labelName[i - 1] = '\0';
						if (isLabelOk = checkLabel(labelName, *labels, labelCount, operations, lineCount)) {

							isLabel = true;
							if (labelCount % 10 == 0) {
								*labels = realloc(*labels, sizeof(label) * (labelCount + 10));
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
				char* operationName = malloc(10);
				if (isLabel) {
					scanStrAndMove(&line, "%s", operationName);
				}
				else {
					operationName = labelName;
				}
				int guidanceNum = isGuidance(operationName + 1); /*taking the next pointer to skip the point in the first char*/
				if (operationName[0] == '.' && guidanceNum >= 0) {/*do if it is a guidance row*/

					if (guidanceNum <= 3) {
						if (isLabel) {
							label labelToAdd = { labelName,*DC,false,false,true,false };
							(*labels)[labelCount] = labelToAdd;
						}
						int lengthCounter = 0;
						int addedLength = 0;
						int dataStringCap = 100;
						char* dataString = (char*)calloc(100, 1);
						dataString = "";
						char* temp = NULL;
						while (scanStrAndMove(&line, "%s", temp))
						{
							addedLength = strlen(temp);
							if (lengthCounter + addedLength >= dataStringCap - 1)
							{
								dataString = (char*)realloc(dataString, dataStringCap + 100); /*sussy if there is an error*/
								dataStringCap += 100;
							}
							strcat(dataString, temp);
							lengthCounter += addedLength;
						}
						writeDataFromGuidance(guidanceNum, dataArray, DC, dataString);
					}
					else {
						if (guidanceNum == 4) {
							isNewLine = true;
							continue;
						}
						if (guidanceNum == 5) {
							char  externName[1];
							scanStrAndMove(&line, "%s", externName);
							if (checkLabel(externName, *labels, labelCount, operations, lineCount)) {
								if (labelCount % 10 == 0) {
									*labels = realloc((*labels), sizeof(label) * (labelCount + 10));
								}
								label labelToAdd = { externName,0,false,true,false,false };
								(*labels)[labelCount] = labelToAdd;
								if (isLabel) printf("Line %d: label before extern is meaningless", lineCount);
								labelCount++;
							}

						}
					}
				}
				else {
					if (isLabel) {
						label labelToAdd = { labelName,*IC,false,false,false,true };
						(*labels)[labelCount] = labelToAdd;
					}

					int operationNumber = operationNum(operations, operationName);
					if (operationNumber == -1) {
						printf("Line %d: unknown word used as operation or guidance", lineCount);
						isNewLine = true;
						continue;

					}
					operation currentOperation = operations[operationNumber];
					char parameters[80]  = "";
					char  temp[80] = "";
					while (scanStrAndMove(&line, "%s", temp) > 0) {
						strcat(parameters, temp);
					}
					int codeByte = (*IC - 100);
					if (codeByte % 40 == 0) {
						*codeArray = realloc((*codeArray), codeByte + 40);
					}
					(*codeArray)[codeByte / 4] = oparationCode(currentOperation, parameters);
					*IC += 4;
				}
				/*sscanf for an operation word*/
			}

		}
	}
	/*same as else*/	
	if (!isSuccessful) return -1;
	return labelCount;
}
 