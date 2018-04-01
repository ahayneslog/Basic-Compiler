/**
Author: Andrew C. Haynes
File: RDP.c
Purpose: Acts a parser and lexical analyzer for analyzing the syntax of a program.
Allows the user to know if the tested program is legal or not. 
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "RDP.h"

//Tokens
#define ID 300
#define NUM 301
#define BEGIN 400
#define END 401
//holds the next token!
int lookahead = 0;
//current line number in program
int line_number = 1;
//SYMBOL TABLE
char **symbol_table;
//index for symbol table
int count_symbol = 0;
//Program being checked
FILE *fp;
//global string
char *test_string;
//Post-fix table
char **post_fix_table;
//post-fix counter
int post_fix = 0;
//register count
int reg_count = 0;
//final table
char **final_table;
//final count
int final_count = 0;

/**
	Parses a program and checks its syntax.
	If program is legal, print the confirmation and display the symbol table.
	If not, print error. 
**/
int parser(FILE *FP, char *filename) {
	fp = FP;
	create_symbol_table();
	lookahead = lexan();
	match(BEGIN);	
	while(lookahead != END) {
		assign_stmt();
	}
	match(END);
	match('.');
	//check for legal ending where there is nothing else after the period, if there is something, exit!
	error_EOF();
	//Syntax Analyzer was successful! Print that program is legal and show the symbol table.
	print_symbol_table();
	create_output_file(filename);
}
/**
	Checks if the next token is legal,
	if not, print the error_message() and exit syntax analyzer.
**/
void match(int tokenID) {
	if(lookahead == tokenID) {		
		lookahead = lexan();
	}
	else {
		error_message(tokenID);
		exit(1);
	}
}
/**
	Checks the next character if it is...
		-> a space or tab, do nothing
		-> new line, increase line number location in program
		-> a digit, build up a number, return NUM
		-> a character, build up a value and put it in a symbol table, return ID
		-> End of File, leave lexan()
		-> anything else, return it from lexan()
**/
int lexan() {
  	int ch;
	int done = 1;
	while(done) {
		ch = fgetc(fp); //grabs the next character
		if(ch == ' ' || ch == '\t') {
			//do nothing
		}
    		else if(ch == '\n') {
			line_number++;
		}
		else if(ch == '~') {
			read_comment(ch); //reads comment and does not consume '\n'
		} 		
		else if(isdigit(ch)) {
			return isvalid_NUM(ch);
		}
		else if(isalpha(ch) || ch == '_') {
			return isvalid_ID(ch);
		}
		else if(ch == EOF) {			
			done = 0;  //finished the file
		}
		else {
			return ch; //returns --  period, open tag, plus, subtract, close tag, multiplier, divisor, equal sign
		}
	}
}
/**
	Create a symbol table that can be called anywhere in this parser/lexical analyzer.
**/
void create_symbol_table() {
	//memory for symbol_table
	symbol_table = (char **)malloc(sizeof(char *)*100);
	int i;
	for(i = 0; i < 100; i++) {
		symbol_table[i] = (char *)malloc(sizeof(char *)*100);
	}
	//memory for post_fix_table
	post_fix_table = (char **)malloc(sizeof(char *)*100);
	int x;
	for(x = 0; x < 100; x++) {
		post_fix_table[x] = (char *)malloc(sizeof(char *)*100);
	}
	//final table for output file
	final_table = (char **)malloc(sizeof(char *)*200);
	int y;
	for(y = 0; y < 200; y++) {
		final_table[y] = (char *)malloc(sizeof(char *)*200);
	}
	//memory for ID, NUM with registers
	test_string = (char *)malloc(sizeof(char) * 100);
}
/**
	Prints out the symbol table at the end of a successful program syntax check
**/
void print_symbol_table() {
	memcpy(final_table[final_count++], "The program is legal!\n", 100);
	memcpy(final_table[final_count++], ":: SYMBOL TABLE :: \n", 100);
	int i;
	for(i = 0; i < count_symbol; i++) {
		char test[100];
		sprintf(test, "%s\n", symbol_table[i]);
		memcpy(final_table[final_count++], test , 100);
	}
}
/**
	Looks up symbol table and checks if identifier already exists.
**/
int symbol_table_lookup(char *value) {
	int i;
	for(i = 0; i < count_symbol; i++) {
		if(strcmp(symbol_table[i], value) == 0) {
			bzero(test_string, 100);			
			memcpy(test_string, value, 100);
			return 1;
		}
	}
	return 0; //returns 0 if the above symbol table lookup for an identifier fails
}
/**
	Prints an error message depending on what is missing in the format of...
		"ERROR:: Line #: error message
**/
void error_message(int tokenID){
	if(tokenID == 40)
		printf("ERROR:: Line %d: Missing '('\n", line_number);
	else if(tokenID == 41)
		printf("ERROR:: Line %d: Missing ')'\n", line_number);
	else if(tokenID == 61)
		printf("ERROR:: Line %d: Missing '='\n", line_number);
	else if(tokenID == 59)
		printf("ERROR:: Line %d: Missing ';'\n", line_number);
	else if(tokenID == 46)
		printf("ERROR:: Line %d: Missing '.'\n", line_number);
	else if(tokenID == 300)
		printf("ERROR:: Line %d: Missing ID\n", line_number);
	else if(tokenID == 301)
		printf("ERROR:: Line %d: Missing NUM\n", line_number);
	else if(tokenID == 400)
		printf("ERROR:: Line %d: Missing BEGIN\n", line_number); 
 	else if(tokenID == 401)
		printf("ERROR:: Line %d: Missing END\n", line_number);
	else
		printf("ERROR:: You broke me! Please report this.\n");
}
/**
	Checks if there is any characters after 'end .', if there is, state error end exit syntax analyzer.
**/
void error_EOF(){
	int ch;
	ch = fgetc(fp);
	if(ch != EOF) {
		printf("Incorrect program syntax. Program is not ending correctly. Check what is after 'end .'\n");
		exit(1);
	}
}
/**
	Reads the single-line comment and terminates at the next line.
**/
void read_comment(char ch) {
	char value[100];
	int done = 0;
	int count = 0;
	value[count++] = ch;
	char x = fgetc(fp);
	while(!done) {
		if(checkNextCharinComment(x) == 1) {
			done = 1;
		}
		else {
			value[count++] = x;
			x = fgetc(fp);
		}
	}
	ungetc(x, fp);
	//do nothing to the comment. 
}
/**
	Checks if the next character is a newline. 
	If so, returns 1, else return 0. 
**/
int checkNextCharinComment(char x) {
	if(x == '\n') {
		return 1;
	}
	else {
		return 0;
	}
}
/**
	Checks if the identifier is valid or not by first building up the identifier and then checking if it is
	legal or not.
**/
int isvalid_ID(char ch) {
	char value[100];
	int count = 0;
	int done = 0;
	value[count++] = ch;
	char x = fgetc(fp);
	while(!done) {
		if(checkNextChar(x) == 1) {
			if(check_ID(value, count) == 1) {			
				done = 1;
			}
		}
		else {		
			value[count++] = x;
			x = fgetc(fp);
		}
	}
	ungetc(x, fp);
	value[count] = '\0';
	if(strcmp(value, "begin") == 0) {
		memcpy(symbol_table[count_symbol++] , value, 100);		
		return BEGIN;
	}
	else if(strcmp(value, "end") == 0)  {
		memcpy(symbol_table[count_symbol++] , value, 100);		
		return END;
	}
	else {
		if(symbol_table_lookup(value) == 0) {
			memcpy(symbol_table[count_symbol++] , value, 100);
			bzero(test_string, 100);			
			memcpy(test_string, value, 100);
		}
		return ID;
	}
}
/**
	If the next character is space,+,-,/,*,=,(,) return 1
	else 0.
	1 means that I should stop building up the identifier for ID.
	0 means keep building up the identifier. 
**/
int checkNextChar(char ch) {
	if(ch == '=' || ch == ';') {
		return 1;
	}
	else if(ch == ' ' || ch =='\n' || ch == '\t') {
		return 1;
	}
	else if(ch == '+' || ch == '-'){
		return 1;
	}
	else if(ch == '*' || ch == '/'){
		return 1;
	}
	else if(ch == '(' || ch == ')'){
		return 1;
	}
	else  {
		return 0;
	}
}
/**
	Checks if the ID is legal or not. 
	Illegal IDs are...
		- has double _ in the identifier
		- has an _ at the end of the identifier
		- has an _ at the beginning of the identifier
		- has a number in the front of the identifier (taken care of in function check_NUM)
**/
int check_ID(char *value, int count) {	
	int i;	
	if(value[0] != '_') {
		for(i = 1; i < count; i++)
		{
			if(value[i] == '_') {
				//double underscore is illegal for identifier
				if(value[i+1] == '_'){
					printf("ERROR:: Line %d: Illegal variable name, no double underscore.\n", line_number);
					exit(1);
				}
				//underscore is illegal at end of identifier!
				else if(value[count-1] == '_') {
					printf("ERROR:: Line %d: Illegal variable name, no underscore at the end.\n", line_number);
					exit(1);
				}
			}
		}
		return 1;
	}
	//underscore is illegal at start of identifier
	else{
		printf("ERROR:: Line %d: Illegal variable name, no underscore at the beginning.\n", line_number);
		exit(1);
	}
	return 0;
}
/**
	Checks if number is legal or not. 
	Number is illegal if...
		-> The number starts with a digit but has a character in the tail.
			ex: 7a34234
**/
int isvalid_NUM(char ch){
	char value[100];
	int count = 0;
	int done = 0;
	value[count++] = ch;
	char x = fgetc(fp);
	while(!done) {
		if(checkNextChar(x) == 1) {
			if(check_NUM(value, count) == 1) {							
				done = 1;
			}
		}
		else {		
			value[count++] = x;
			x = fgetc(fp);
		}
	}
	ungetc(x, fp);
	int silly = atoi(value);
	bzero(test_string, 100);
	sprintf(test_string, "%d", silly);
	return NUM;
}
/**
	Checks for the illegal case mentioned in isvalid_NUM().
	If found, report error and exit syntax analyzation.
**/
int check_NUM(char *value, int count){
	if(isdigit(value[0])) { 		
		int i;
		for(i = 1; i < count; i++) {
			if(isalpha(value[i])) {
				printf("ERROR:: Line %d: Illegal word. Cannot make into identifier nor number.\n", line_number);
				exit(1);
			}
		} 
		return 1;
	}
	return 0;
}
/**
	Checks if ID is the first thing in the statement, if so, create the statement for the identifier.
	If the next legal character is '=', start creating the expression for the identifier, then close it once
	a ';' is encountered!
**/
void assign_stmt(){
	match(ID); //consume the ID if it is there, then check if '=' follows it
	char variable[100];
	memcpy(variable, test_string, 100);
	if(lookahead != '=') {
		printf("ERROR: Line %d contains the error: Missing '=' for variable declaration\n", line_number);
		exit(1);			
		
	}
	else {
		match(lookahead);
		expression();
		match(';');
		char test[100];
		sprintf(test, "%s = R[%d]\n", variable, reg_count-1);
		memcpy(final_table[final_count++], test, 100);
		display_post_fix();
	}
}
/**
	Starts building the expression by first identifying the term. Once term is identified, 
	expression() looks for + or - and builds another term to go after it. 
**/
void expression() {
	term();
	while(lookahead == '+' || lookahead == '-') {
		int what = lookahead;		
		match(lookahead);
		term();
		char test[100];
		sprintf(test,"R[%d] = R[%d] %c R[%d]\n", reg_count-2, reg_count-2, what, reg_count-1);
		memcpy(final_table[final_count++], test, 100);
		reg_count--;
		post_fix_builder(what);
	}
}
/**
	Starts building the term by first identifying a factor in the expression. 
	Once the factor is built up, term() looks for a * or + and finds another factor in the expression.
**/
void term() {
	factor();
	while(lookahead == '*' || lookahead == '/') {
		int what = lookahead;
		match(lookahead);
		factor();
		char test[100];	
		sprintf(test, "R[%d] = R[%d] %c R[%d]\n", reg_count-2, reg_count-2, what, reg_count-1);
		memcpy(final_table[final_count++], test, 100);
		reg_count--;
		post_fix_builder(what);
	}
}
/**
	Looks for an ID, NUM or parentheses. If an open parantheses is found, build another expression
	until a closing parantheses is found.
**/
void factor() {
	if(lookahead == ID) {
		char test[100];
		sprintf(test, "R[%d] = %s\n", reg_count++, test_string);
		memcpy(final_table[final_count++], test, 100);
		post_fix_builder(0);
		match(ID);
	}
	else if(lookahead == NUM) {
		char test[100];
		sprintf(test, "R[%d] = %s\n", reg_count++, test_string);
		memcpy(final_table[final_count++], test, 100);
		post_fix_builder(0);
		match(NUM);
	}
	else if(lookahead == '(') {				
		match('(');
		expression();
		match(')');
	}
}
/**
	Displays the post-fix presentation of what
	was assigned to a variable in a statement.
**/
void display_post_fix() {
	int i;
	char *output = (char *)malloc(sizeof(char *)*100);
	strcat(output, "Post-Fix: [");
	for(i = 0; i < post_fix; i++) {
		strcat(output, post_fix_table[i]);
		strcat(output, ",");
	}
	output[strlen(output)-1] = '\0';
	strcat(output, "]\n");
	memcpy(final_table[final_count++], output, 100);
	clear_post_fix_builder();
}
/**
	Builds up the post-fix presentation of the current 
	statement. 
**/
void post_fix_builder(int i) {
	if(i == 0) {
		memcpy(post_fix_table[post_fix++], test_string, 100);
	}
	else { //for */+-
		char ch[100];
		sprintf(ch, "%c", i);
		memcpy(post_fix_table[post_fix++], ch, 100);
	}
}
/**
	Clears the post-fix presentation to be used for another
	statement. 
**/
void clear_post_fix_builder() {
	int i;
	//zeros out all used entries in post_fix 
	for(i = 0; i < post_fix; i++) {
		bzero(post_fix_table[i], 100); 
	}
	post_fix = 0; //restart post_fix count!
	reg_count = 0; //restart register 
}
/**
	Creates a file that contains information
	such as uses of registers to compile statements, 
	post-fix representation of statements and
	the symbol table for the program as a whole.
**/
void create_output_file(char *filename) {
	char *token = strtok(filename, ".");
	strcat(token, ".out");	
	FILE *fp = fopen(token, "w");
	if(fp) {
		//print in everything from final_table
		char intro[100];
		sprintf(intro, "Compiling %s\n", filename);
		fprintf(fp, "%s", intro);
		int x;
		for(x = 0; x < final_count; x++) {
			fprintf(fp, "%s", final_table[x]);
		}
	}
	fclose(fp);
}
