#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "Declaration.h"
#include "File.h"
#include "Deal_token.h"
#include "Dfa_deal.h"
using namespace std;
int state = 0;//indicate the current state of DFA
char C = 0;//the char currently read in
char C_temp;
string token = "";//the string currently identified
char buffer[2050];//input buffer
char temp_buffer[BUFFER_SIZE_HALF];//the temp buffer, in case that the forward pointer retract, but the buffer has already been flushed.
int buffer_forward = 0;//the pointer of the buffer
int left_eof = BUFFER_SIZE_HALF;//end sign of left side 
int right_eof = 2 * BUFFER_SIZE_HALF + 1;//end sign of right side 

int file_forward;//the pointer of the file

map<string, string> reserved_table;//reserved table, the first string is the token, the second string is the sign of token
map<string, string> symbol_table;//symbol table, the first string is the sign, the second is the token
vector<pair<string, string>> output_list;//output list, the address is 8 bytes
vector<pair<int, string>> error_list;//error list, the first varible is the type of error, the second is the location of error

fstream in_file;//stream of the file, read and write
fstream out_file;//stream of the output file, write only

string token_type;//if the lexeme is keyword, return the sign, if the lexeme is defined by user, return -1

int line_num = 1;//the number of state line
int ch_num = 0;//the number of character
int reserved_num = 0;//the number of reversed character
int defined_num = 0;//the number of defined character
int num_num = 0;//the number of number
int error_num = 0;//the number of error
int has_read = 0;//when meet the eof, count how many ch has been read

int main(int argc, char* argv[]) {
	create_reserved_table();
	in_file.open(argv[1], ios::in);
	out_file.open("output.txt", ios::out);
	if (in_file)
	{
		read_file();
		deal_state_dfa();
		
		for (unsigned int i = 0; i < output_list.size(); i++)
		{
			cout << output_list.at(i).first << " " << output_list.at(i).second << endl;
		}
		cout << "The state line number is: " << line_num - 1 << endl;
		cout << "The character number is: " << ch_num << endl;
		cout << "The error list is: " << endl;
		if (error_list.size() == 0)
		{
			cout << "Congratulations! No lexical error is found." << endl;
		}
		else
		{
			for (unsigned int i = 0; i < error_list.size(); i++)
			{
				cout << "At the line of " << error_list.at(i).first + 1 << ", the type of error is: " << error_list.at(i).second << endl;
			}
		}

		cout << "The number of NUM is: " << num_num << endl;
		cout << "The number of reserved character appeared in the source code is: " << reserved_num << endl;
		cout << "The number of defined character is: " << symbol_table.size() << endl;
		cout << "The number of error is: " << error_list.size() << endl;
		write_file();
		in_file.close();//close the opened file
	}
	else
	{
		cout << "Wrong to open the file!" << endl;
	}
	return 0;
}