/* This is the file that runs the first pass function of the assembler */

#include "utilityFunctions.h"

int firstPass(FILE* fp,label** labels,unsigned char** dataArray, int* IC, int* DC, operation* operations)
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
				char* operation = NULL;
				sscanf(line, "%s", operation);
				int guidanceNum = isGuidance(operation + 1); /*taking the next pointer to skip the point in the first char*/
				if(operation[0] == '.' && guidanceNum >= 0){

						if( guidanceNum <= 3){
							if(isLabel){
								label labelToAdd = {labelName,*DC,false,false,true,false};
								(*labels)[labelCount] = labelToAdd ; /*not clear if the value is IC or DC, check later*/
							}
							char * dataSring;
							sscanf(line,"%s",dataSring);
							*dataArray = realloc(*dataArray,*DC+20);
							writeDataFromGuidance(guidanceNum,dataArray,DC,dataSring);
						}
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
 