/* This is the file that runs the main function of the assembler */

#include "utilityFunctions.h"

int main(int argc, char *argv[])
{
	operation operations[numOfOperations] = {/*add this array to the operation num function*/
		{"add",'R',1,0},
		{"sub",'R',2,0},
		{"and",'R',3,0},
		{"or",'R',4,0},
		{"nor",'R',5,0},
		{"move",'R',1,1},
		{"mvhi",'R',2,1},
		{"mvlo",'R',3,1},
		{"addi",'I',0,10},
		{"subi",'I',0,11},
		{"andi",'I',0,12},
		{"ori",'I',0,13},
		{"nori",'I',0,14},
		{"bne",'I',0,15},
		{"beq",'I',0,16},
		{"blt",'I',0,17},
		{"bgt",'I',0,18},
		{"lb",'I',0,19},
		{"sb",'I',0,20},
		{"lw",'I',0,21},
		{"sw",'I',0,22},
		{"lh",'I',0,23},
		{"sh",'I',0,24},
		{"jmp",'J',0,30},
		{"la",'J',0,31},
		{"call",'J',0,32},
		{"stop",'J',0,63}
	};
	label* labels = NULL;
	int labelCount = 0;
	unsigned int* codeArray = malloc(10);
	unsigned char* dataArray = malloc(10);
	int* labelLines = malloc(40);
	int IC = 100;
	int DC = 0;
	int x ;
	char* kaki = "$3,$5,$9";
	FILE* fp = NULL;
	bool firstPassSuccessful = false;
	labelLines[0] = 0;
	printf("NHKSD\n\nBUGO\n\n");
	for (x = 1; x < argc; x++){
		char* fileName = argv[x];
		fp = fopen(fileName, "r");
		if(!fileApproved(fileName) || fp == NULL)
		{
			printf("Error, couldn't open file %s\n", fileName);
			continue;
		}
		labelCount = firstPass(fp,&labels,&dataArray, &codeArray, &IC, &DC, operations,&labelLines);
		firstPassSuccessful = labelCount >= 0;
		fclose(fp);
		fp = fopen(fileName, "r");
		if(firstPassSuccessful == true)
		{
			secondPass(fp,labels,labelCount,codeArray, &IC, &DC, operations, labelLines);
			createObject(codeArray, fileName);
			/*createExt(labels,  fileName);
			createEnt(labels, fileName);*/
		}
		
		fclose(fp);
	}
	return 0;
}
