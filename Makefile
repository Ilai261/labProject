assembler: main.o utilityFunctions.o firstPass.o secondPass.o assemblyData.o createOutputFiles.o
	gcc -ansi -Wall -pedantic -g main.o utilityFunctions.o firstPass.o secondPass.o assemblyData.o createOutputFiles.o -o assembler -lm 
main.o: main.c utilityFunctions.h firstPass.h secondPass.h createOutputFiles.h assemblyData.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o
utilityFunctions.o: utilityFunctions.c utilityFunctions.h 
	gcc -c -ansi -Wall -pedantic utilityFunctions.c -o utilityFunctions.o 
firstPass.o: firstPass.c utilityFunctions.h firstPass.h
	gcc -c -ansi -Wall -pedantic firstPass.c -o firstPass.o
secondPass.o: secondPass.c utilityFunctions.h secondPass.h
	gcc -c -ansi -Wall -pedantic secondPass.c -o secondPass.o
assemblyData.o: assemblyData.c assemblyData.h
	gcc -c -ansi -Wall -pedantic assemblyData.c -o assemblyData.o
createOutputFiles.o: createOutputFiles.c createOutputFiles.h utilityFunctions.h
	gcc -c -ansi -Wall -pedantic createOutputFiles.c -o createOutputFiles.o
