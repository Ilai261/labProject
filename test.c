// Project3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
  char inputStr[10] = "  cde fg";
  char outputSrt[10];
  scanStrAndMove(&inputStr,"%s",outputSrt);
  printf("%s",outputSrt);
  scanStrAndMove(&inputStr,"%s",outputSrt);
  printf("%s",outputSrt)
   
}
