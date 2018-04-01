#ifndef _RDP_h
#define _RDP_h

//PARSER AND MATCH for Recursive-Descent Parsing
int parser(FILE *fp, char *filename);
void match(int tokenID);

//creates symbol table
void create_symbol_table();
//prints symbol table
void print_symbol_table();

//checks next character for ID and NUM
int checkNextChar(char ch);

//Called in analysis of ID token in lexan()
int isvalid_ID(char ch);
int check_ID(char *value, int count);

//Comment Functions
void read_comment(char ch);
int checkNextCharinComment(char x);
//Called in analysis of NUM token in lexan()
int isvalid_NUM(char ch);

//called if MATCH fails
void error_message(int tokenID);
//called if program does not end correctly
void error_EOF();

//Lexical Analyzer
int lexan();

//display post_fix table
void display_post_fix();

//builds post_fix table
void post_fix_builder(int i);

//clear post_fix table for next statement
void clear_post_fix_builder();

//creates ".out" file
void create_output_file(char *filename);

//Statement recursions
void assign_stmt();
void expression();
void term();
void factor();
#endif
