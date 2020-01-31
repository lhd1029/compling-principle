#ifndef DECLARATION_H
#define DECLARATION_H
#include <string>
#include <fstream>
#include <map>
#include <vector>

#define BUFFER_SIZE_HALF 1024
using namespace std;

extern int state;//indicate the current state of DFA
extern char C;//the char currently read in
extern char C_temp;
extern string token;//the string currently identified
extern char buffer[];//input buffer
extern char temp_buffer[];//the temp buffer, in case that the forward pointer retract, but the buffer has already been flushed.
extern int buffer_forward;//the pointer of the buffer
extern int left_eof;//end sign of left side 
extern int right_eof;//end sign of right side 
extern int file_forward;//the pointer of the file

extern fstream in_file;//stream of the file, read and write
extern fstream out_file;//stream of the output file, write only

extern map<string, string> reserved_table;//reserved table, the first string is the token reserved, the second string is the sign of token
extern map<string, string> symbol_table;//symbol table, the first string is the sign, the second is the token
extern vector<pair<string, string>> output_list;//output list, the address is 8 bytes
extern vector<pair<int, string>> error_list;//error list, the first varible is the type of error, the second is the location of error

extern string token_type;//if the token is reserved, return the sign, if the token is defined by user, return the token, if the token is not defined, return "-1"

extern int line_num;//the number of state line
extern int ch_num;//the number of character
extern int reserved_num;//the number of reversed character
extern int defined_num;//the number of defined character
extern int num_num;//the number of number
extern int has_read;//when meet the eof, count how many ch has been read
#endif