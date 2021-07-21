/* This is the file that runs the first pass function of the assembler */
#include "utilityFunctions.h"
#include "structsAndFuncs.h"

firstPass(FILE* fp,label* labels, int* IC, int* DC)
{
	char* line = char[81];
	int lineCount = 0;
	while((c = getc(fp)) != EOF)
	{
		if(c != '\n')
		{
				line[lineCount] = c;
				lineCount++;
		}
		else
		{
			line[lineCount] = '\0';
			int i = 0;
			bool isEmptyLine = true;
			char firstChar = ' ';
			
			for(i; i < LineCount; i++)
			{
				if(!isspace(line[i]))
					isEmptyLine = false;
					firstChar = line[i];
					break;
			}
			if(isEmptyLine == false && firstChar != ';')
			{
				
			}
			/*first we will check if the line is empty or a comment, then we will check if is starts with a label*/
			
		}
	}
	/*same as else*/
}
