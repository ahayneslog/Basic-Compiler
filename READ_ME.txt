I. Source Codes

	You should have the following files in a folder:
		syntax_analyzer.c
		RDP.c
		RDP.h

	Optional Files:
		project2
		test1.c
		test2.c
		test3.c
		test4.c
		test5.c
		test6.c
		test7.c
		test8.c

II. Methods of Running the Syntax Analyzer Program

	To run the program, compile syntax_analyzer.c and RDP.c by going on the command line and typing in:
		gcc -c syntax_analyzer.c RDP.c
		gcc -o syntax syntax_analyzer.o RDP.o

	You can now run the program in the command line with: 
		./syntax

	EXPECTED INPUT FROM USER:
	The program requires a program file to check, or it will not run. An example of how to use this program is like so:
		./syntax test1.c

	The output will either be a legal program with a symbol table printed out or an error that gives reference to where
	error is in the program file. 

	To test multiple programs, write a script that contains all of the commands above! Reference the project2 file
	to see how this can be done. If running this program on LINUX or MAC, follow as suit. If running on WINDOWS,
	you'll need to create a batch file to follow the same procedure.

	To run the script on LINUX/MAC, do: ./project2
	For Windows: run project2 

III. Output

	When the program finishes running, you will receive several different outputs depending on the success or failure of the syntax
	analysis of your program. If the program succeeded, you will have a "[filename].out" file in your folder that shows the registers
	used throughout the compilation of your program, as well as the post-fix notation of your computation in a statement. If the program
	failed in the syntax analysis, you will receive an error in your shell that states where your error is. 
