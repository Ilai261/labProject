assembler: main.o utilityFunctions.o firstPass.o secondPass.o
	gcc -ansi -Wall -pedantic -g main.o utilityFunctions.o firstPass.o secondPass.o -o assembler -lm 
main.o: main.c utilityFunctions.h 
	gcc -c -ansi -Wall -pedantic main.c -o main.o
utilityFunctions.o: utilityFunctions.c utilityFunctions.h 
	gcc -c -ansi -Wall -pedantic utilityFunctions.c -o utilityFunctions.o 
firstPass.o: firstPass.c utilityFunctions.h 
	gcc -c -ansi -Wall -pedantic firstPass.c -o firstPass.o
secondPass.o: secondPass.c utilityFunctions.h 
	gcc -c -ansi -Wall -pedantic secondPass.c -o secondPass.o
