/**
Author: Andrew C. Haynes
File: syntax_analyzer.c
Purpose: Act as head as the Syntax Analyzer project and check files that the user
references on the command line. If the file is good to go, it goes through the parser
and is checked if the program is valid or not. 
If the file is not good to go, the user is notified about it and the program exits.
**/
#include <stdio.h>
#include "RDP.h"

int main(int argc, char *argv[]) {
	//check if user gave a program to check syntaxically
	if(argv[1] != '\0') {
		FILE *fp = fopen(argv[1], "r");		
		//if program exists, proceed with checking if it's valid or not		
		if(fp) {
			//send the program file into lexical analyzer
			parser(fp, argv[1]);
			fclose(fp);
		}
		//if it does not, let the user know
		else {
			printf("You tried checking a non-existing program.\n");
		}
	}
	//if they did not, let the user know
	else {
		printf("You didn't give me any program to check.\n");
	}
	return 0;
}
