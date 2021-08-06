/* This is the file that runs the first pass function of the assembler */

#include "utilityFunctions.h"

int firstPass(FILE* fp,label** labels,unsigned char** dataArray,unsigned int** codeArray, int* IC, int* DC, operation* operations)
{
	char line[81];
	int labelCount = 0;
	int lineLength = 0;
	int lineCount = 1;
	bool isSuccessful = true;
	char c;
	while((c = getc(fp)) != EOF)
	{
		if(c != '\n')
		{
			if(lineLength <= 80)
			{
				line[lineLength] = c;
				lineLength++;
			}
			else
			{
				printf("Line %d: more than 80 characters in line", *(IC) - 100);
				isSuccessful = false;
				
			}
		}
		else
		{
			/*first we will check if the line is empty or a comment, then we will check if is starts with a label*/
			int i = 0;
			bool isEmptyLine = true;
			char firstChar = ' ';
			
			
			for(i; i < lineLength; i++)
			{
				if(!isspace(line[i])){
					isEmptyLine = false;
					firstChar = line[i];
					break;
				}
			}

			if(isEmptyLine == false && firstChar != ';') /* not an empty line and not a comment*/
			{
				char* labelName;
				bool isLabel = false;
				bool isLabelOk = false;
				
				if(sscanf(line, "%s: ",labelName) == 1 && (isLabelOk = checkLabel(labelName,*labels, labelCount, operations, lineCount) ))
				{

					isLabel = true;
					if(labelCount%10 == 0){
						*labels = realloc(*labels,sizeof(label)*(labelCount + 10));
					}
					labelCount++;
				}
				
				if (!isLabelOk) continue;
				/*add the label to the tabel, continue scanning until end of line*/
				char* operationName = NULL;
				sscanf(line, "%s", operationName);
				int guidanceNum = isGuidance(operationName + 1); /*taking the next pointer to skip the point in the first char*/
				if(operationName[0] == '.' && guidanceNum >= 0){/*do if it is a guidance row*/

						if( guidanceNum <= 3){
							if(isLabel){
								label labelToAdd = {labelName,*DC,false,false,true,false};
								(*labels)[labelCount] = labelToAdd ; 
							}
							char * dataSring;
							char * temp;
							while(sscanf(line,"%s",temp)){
								strcat(dataSring,temp);
							}
							 /*fix, there could be spaces*/
							writeDataFromGuidance(guidanceNum,dataArray,DC,dataSring);
						}
						else{
							if(guidanceNum == 4) continue;

							if(guidanceNum == 5){
								char * externName;
								sscanf(line,"%s",externName);
								if(checkLabel(externName,*labels, labelCount, operations, lineCount)){
									if(labelCount%10 == 0){
										*labels = realloc(*labels,sizeof(label)*(labelCount + 10));
									}
									label labelToAdd = {externName,0,false,true,false,false};
									(*labels)[labelCount] = labelToAdd ;
									labelCount++;
								}

							}
						}
				}
				else{
					if(isLabel){
								label labelToAdd = {labelName,*IC,false,false,false,true};
								(*labels)[labelCount] = labelToAdd; 
					}
					
					int operationNumber = operationNum(operations,operationName);
					if(operationNumber == -1){
						printf("Line %d: unknown word used as opartion or guidance", lineCount);
						continue;

					}
					operation currentOperation = operations[operationNumber];
					char* parameters;
					char * temp;
					while(sscanf(line,"%s",parameters)){
						strcat(parameters,temp);
					}
					int codeByte = (*IC -100);
					if(codeByte%40 == 0){
						*codeArray = realloc(*codeArray,codeByte + 40);
					}
					*codeArray[codeByte/4] = oparationCode(currentOperation,parameters);
					*IC +=  4;
				}
				/*sscanf for an operation word*/
			}
			lineCount++;
			
		}
	}
	/*same as else*/
	
	
	
	
	if (!isSuccessful) return -1;
	return labelCount;
}
 