/* This is the file that runs the first pass function of the assembler */

#include "utilityFunctions.h"

int firstPass(FILE* fp,label* labels, int* IC, int* DC, operation* operations)
{
	char line = char[81];
	int labelCount = 0;
	int lineLength = 0;
	int lineCount = 1;
	bool isSuccessful = true;
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
			
			
			for(i; i < LineLength; i++)
			{
				if(!isspace(line[i]))
					isEmptyLine = false;
					firstChar = line[i];
					break;
			}
			if(isEmptyLine == false && firstChar != ';') /* not an empty line and not a comment*/
			{
				char* label = NULL;
				if(sscanf(line, "%s: ",label) == 1 && checkLabel(label,labels,labelCount,operations, lineCount))
				{
					/*add the label to the tabel, continue scanning until end of line*/
					labelCount++;	
				}
				/*sscanf for a guidance word*/
				/*sscanf for an operation word*/
			}
			lineCount++;
			
		}
	}
	/*same as else*/
	
	
	
	
	if (!isSuccessful) return -1;
	return labelCount;
}
