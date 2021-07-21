/* This is the file that runs the first pass function of the assembler */
#include "utilityFunctions.h"
#include "structsAndFuncs.h"

firstPass(FILE* fp,label* labels, int* IC, int* DC)
{
	char* line = (char*)calloc(1, 10);
	line[10] = '\0';
	int lineCount = 0;
	while((c = getc(fp)) != EOF)
	{
		if(c != '\n')
		{
			if(line[lineCount + 1] == '\0')
			{
				line = (char*)realloc(line, lineCount + 10);
				line[lineCount + 10] = '\0';
			}
			line[lineCount] = c;
			lineCount++;
		}
		else
		{
			/*first we will chack if the line is empty or a comment, then we will chack if is starts with a labal*/
			
		}
	}



}
