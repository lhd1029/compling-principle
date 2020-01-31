#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#define BUFFER_SIZE_HALF 1024

using namespace std;

bool is_letter(char C);
bool is_digit(char C);
bool is_underline(char C);

void create_reserved_table();

bool is_reserved(string token);
bool is_defined(string token);
void table_insert(string token);
float SToF(string token);
int SToI(string token);

void read_file();

void get_char();

void get_nbc();

void cat();

void retract();

void deal_state_dfa();


int state = 0;//indicate the current state of DFA
char C = 0;//the char currently read in
string token = "";//the string currently identified
char buffer[5000];//input buffer
int buffer_forward = 0;//the pointer of the buffer
int lexemebegin = 0;//the pointer of the start of the lexeme currently read
int left_eof = 1024;//end sign of left side 
int right_eof = 2049;//end sign of right side 

int file_forward;//the pointer of the file
int has_read = 0;

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
char temp_buffer[BUFFER_SIZE_HALF];//the temp buffer, in case that the forward pointer retract(), but the buffer has already been flushed.


void deal_state_dfa()
{
	do
	{
		switch (state)
		{
		case 0:
		{
			token = "";
			get_char();
			get_nbc();
			if (is_letter(C))
			{
				state = 1;
			}
			else if (is_digit(C) && C != '0')
			{
				state = 2;
			}
			else if (C == '<')
			{
				state = 8;
			}
			else if (C == '>')
			{
				state = 9;
			}
			else if (C == '=')
			{
				state = 10;
			}
			else if (C == '+')
			{
				state = 11;
			}
			else if (C == '-')
			{
				state = 12;
			}
			else if (C == '*')
			{
				state = 13;
			}
			else if (C == '/')
			{
				state = 14;
			}
			else if (C == '%')
			{
				state = 18;
			}
			else if (C == '!')
			{
				state = 19;
			}
			else if (C == '&')
			{
				state = 20;
			}
			else if (C == '|')
			{
				state = 21;
			}
			else if (C == ':')
			{
				state = 22;
			}
			else if(C == '0')
			{
				state = 23;
			}
			else if (C == '#')
			{
				state = 31;
			}
			else if (C == '"')
			{
				state = 32;
			}
			else if (C < 128 && C >= 0)
			{
				state = 34;
			}
			else
			{
				state = 35;
			}
			break;
		}

		case 1://identifiers
		{
			cat();
			get_char();
			if (is_letter(C) || is_digit(C))
			{
				state = 1;
			}
			else
			{
				retract();
				state = 0;
				if (is_reserved(token) == false && is_defined(token) == false)//the token is not defined nor reserved
				{
					table_insert(token);
					output_list.push_back(pair<string, string>("id " + token, std::to_string((long long)&symbol_table[token])));
				}
				else if (is_reserved(token) == true)
				{
					output_list.push_back(pair<string, string>(token,""));
					reserved_num++;
				}
				else if (is_defined(token) == true)
				{
					output_list.push_back(pair<string, string>("id " + token, std::to_string((long long)&symbol_table[token])));
				}
			}
			break;
		}

		case 2://numbers
		{
			cat();
			get_char();
			if (isdigit(C))
			{
				state = 2;
			}
			else if (C == '.') //digital number
			{
				state = 3;
			}
			else if (C == 'e' || C == 'E') //expont number
			{
				state = 5;
			}
			else
			{
				if (is_letter(C))
				{
					token = "";
					while (is_letter(C))
					{
						cat();
						get_char();
					}
					//error_list.pusf_back(pair<int, string>(line_num, "Invalid suffix " + token + " on integer constant"));
				}
				else
				{
					output_list.push_back(pair<string, string>("NUM", token));
					num_num++;
				}
				retract();
				state = 0;
			}
			break;
		}

		case 3://dots
		{
			cat();
			get_char();
			if (is_digit(C))
			{
				state = 4;
			}
			else
			{
				output_list.push_back(pair<string, string>("NUM", token));
				num_num++;
				state = 0;
			}
			break;
		}

		case 4://digital numbers
		{
			cat();
			get_char();
			if (is_digit(C))
			{
				state = 4;
			}
			else if (C == 'e' || C == 'E')
			{
				state = 5;
			}
			else //a digital number
			{
				retract();
				state = 0;
				token += '0';
				output_list.push_back(pair<string, string>("NUM", token));
				num_num++;
			}
			break;
		}

		case 5://exponent
		{
			cat();
			get_char();
			if (is_digit(C))
			{
				state = 7;
			}
			else if (C == '+' || C == '-')
			{
				state = 6;
			}
			else
			{
				retract();
				//error_list.pusf_back(pair<int, string>(line_num, "Wrong type of number! Exponent has no digits"));
				state = 0;
			}
			break;
		}

		case 6://exponent with + or -
		{
			cat();
			get_char();
			if (is_digit(C))
			{
				state = 7;
			}
			else
			{
				retract();
				//error_list.pusf_back(pair<int, string>(line_num, "Wrong type of number! Exponent has no digits"));
				state = 0;
			}
			break;
		}

		case 7://exponent
		{
			cat();
			get_char();
			if (is_digit(C))
			{
				state = 7;
			}
			else
			{
				retract();
				state = 0;
				output_list.push_back(pair<string, string>("NUM", token));
				num_num++;
			}
			break;
		}

		case 8://<
		{
			cat();
			get_char();
			switch (C)
			{
			case '=':
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>("relop", "LE"));
				break;
			}
			case '<':
			{
				cat();
				get_char();
				if (C == '=')
				{
					cat();
					state = 0;
					output_list.push_back(pair<string, string>(token, ""));
				}
				else
				{
					retract();
					state = 0;
					output_list.push_back(pair<string, string>(token, ""));
				}
				break;
			}
			default:
			{
				retract();
				state = 0;
				output_list.push_back(pair<string, string>("relop", "LT"));
				break;
			}
			}
			break;
		}

		case 9://>
		{
			cat();
			get_char();
			switch (C)
			{
			case '=':
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>("relop", "GE"));
				break;
			}
			case '>':
			{
				cat();
				get_char();
				if (C == '=')
				{
					cat();
					state = 0;
					output_list.push_back(pair<string, string>(token, ""));
				}
				else
				{
					retract();
					state = 0;
					output_list.push_back(pair<string, string>(token, ""));
				}
				break;
			}
			default:
			{
				retract();
				state = 0;
				output_list.push_back(pair<string, string>("relop", "GT"));
				break;
			}
			}
			break;
		}

		case 10://=
		{
			cat();
			get_char();
			if (C == '=')
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>("relop", "EQ"));
			}
			else
			{
				retract();
				state = 0;
				output_list.push_back(pair<string, string>("assign-op", ""));
			}
			break;
		}

		case 11://+
		{
			cat();
			get_char();
			switch (C)
			{
			case '+':
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
				break;
			}
			case '=':
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
				break;
			}
			default:
				retract();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
				break;
			}
			break;
		}

		case 12://-
		{
			cat();
			get_char();
			switch (C)
			{
			case '-':
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
				break;
			}
			case '=':
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
				break;
			}
			case '>':
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
				break;
			}
			default:
				retract();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
				break;
			}
			break;
		}

		case 13://*
		{
			cat();
			get_char();
			if (C == '=')
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
			}
			else
			{
				retract();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
			}
			break;
		}

		case 14:///
		{
			cat();
			get_char();
			switch (C)
			{
			case '/':
			{
				cat();
				state = 15;
				ch_num -= 2;//ch in notes shouldn't be counted
				break;
			}
			case '*':
			{
				cat();
				state = 16;
				break;
			}
			default:
				retract();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
				break;
			}
			break;
		}

		case 15:////
		{
			get_char();
			if (C != '\n')
			{
				if ((C != ' ') && (C != '\t'))
				{
					ch_num--;//ch in notes shouldn't be counted
				}
				state = 15;
			}
			else
			{
				state = 0;
			}
			break;
		}

		case 16:///*
		{
			get_char();
			if (C != '*')
			{
				state = 16;
			}
			else
			{
				state = 17;
			}
			break;
		}

		case 17://*/
		{
			get_char();
			if (C == '*')
			{
				state = 17;
			}
			else if (C == '/')
			{
				state = 0;
			}
			else
			{
				state = 16;
			}
			break;
		}

		case 18://%
		{
			cat();
			get_char();
			if (C == '=')
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
			}
			else
			{
				retract();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
			}
			break;
		}

		case 19://!
		{
			cat();
			get_char();
			if (C == '=')
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>("relop", "NE"));
			}
			else
			{
				retract();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
			}
			break;
		}

		case 20://&
		{
			cat();
			get_char();
			if (C == '=')
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
			}
			else if (C == '&')
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
			}
			else
			{
				retract();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
			}
			break;
		}

		case 21://|
		{
			cat();
			get_char();
			if (C == '=')
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
			}
			else if (C == '|')
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
			}
			else
			{
				retract();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
			}
			break;
		}

		case 22://:
		{
			cat();
			get_char();
			if (C == ':')
			{
				cat();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
			}
			else
			{
				retract();
				state = 0;
				output_list.push_back(pair<string, string>(token, ""));
			}
			break;
		}

		case 23://digit 0
		{
			cat();
			get_char();
			if (isdigit(C))
			{
				state = 23;
			}
			else if (C == 'x' || C == 'X')
			{
				state = 24;
			}
			else
			{
				retract();
				state = 0;
				output_list.push_back(pair<string, string>("NUM", token));
			}
			break;
		}

		case 24://0x
		{
			cat();
			get_char();
			if (isdigit(C))
			{
				state = 25;
			}
			else
			{
				retract();
				state = 0;
				//error_list.pusf_back(pair<int, string>(line_num, "Wrong type of number! Invalid suffix \"x\" on integer constant"));
			}
			break;
		}

		case 25://0x7
		{
			cat();
			get_char();
			if (isdigit(C))
			{
				state = 25;
			}
			else if (C == 'p' || C == 'P')
			{
				state = 26;
			}
			else if (C == '.')
			{
				state = 29;
			}
			else
			{
				if (is_letter(C))
				{
					token = "";
					while (is_letter(C))
					{
						cat();
						get_char();
					}
					//error_list.pusf_back(pair<int, string>(line_num, "Invalid suffix " + token + " on integer constant"));
				}
				else
				{
					output_list.push_back(pair<string, string>("NUM", token));
					num_num++;
				}
				retract();
				state = 0;
			}
			break;
		}
		
		case 26://0x7p
		{
			cat();
			get_char();
			if (C == '+' || C == '-')
			{
				state = 27;
			}
			else if (isdigit(C))
			{
				state = 28;
			}
			else
			{
				retract();
				state = 0;
				//error_list.pusf_back(pair<int, string>(line_num, "Wrong type of number! Exponent has no digits"));
			}
			break;
		}

		case 27://0x7p+
		{
			cat();
			get_char();
			if (isdigit(C))
			{
				state = 28;
			}
			else
			{
				retract();
				state = 0;
				//error_list.pusf_back(pair<int, string>(line_num, "Wrong type of number! Exponent has no digits"));
			}
			break;
		}

		case 28://0x7p4 or 0x7p-4
		{
			cat();
			get_char();
			if (isdigit(C))
			{
				state = 28;
			}
			else
			{
				retract();
				state = 0;
				output_list.push_back(pair<string, string>("NUM", token));
				num_num++;
			}
			break;
		}

		case 29://0x7.
		{
			cat();
			get_char();
			if (isdigit(C))
			{
				state = 30;
			}
			else
			{
				//error_list.pusf_back(pair<int, string>(line_num, "Wrong type of number! Hexadecimal floating constants require an exponent"));
			}
			break;
		}

		case 30://0x7.7
		{
			cat();
			get_char();
			if (isdigit(C))
			{
				state = 30;
			}
			else if (C == 'p' || C == 'P')
			{
				state = 26;
			}
			else
			{
				retract();
				state = 0;
				output_list.push_back(pair<string, string>("NUM", token));
				num_num++;
			}
			break;
		}

		case 31://#
		{
			ch_num--;
			state = 15;
			break;
		}

		case 32:// "
		{
			cat();
			get_char();
			if (C == '\\')
			{
				state = 33;
			}
			else if (C == '"')
			{
				cat();
				output_list.push_back(pair<string, string>("String", token));
				state = 0;
			}
			else
			{
				state = 32;
			}
			break;
		}

		case 33:// '\'
		{
			cat();
			get_char();// the letter after '\' should not be in the string
			state = 32;
			break;
		}

		case 34://others
		{
			cat();
			state = 0;
			output_list.push_back(pair<string, string>(token, ""));
			break;
		}

		case 35:
		{
			//error_list.pusf_back(pair<int, string>(line_num, "Stray " + to_string(int(C)) + " in the program"));
			get_char();
			//////////error_list.pusf  _back(pair<int, string>(line_num, "Stray " + to_string(int(C)) + " in the program"));
			state = 0;
			break;
		}
		}
		
	} while (C != EOF);
}

void read_file()
{
	has_read = 0;
	if (buffer_forward == 0)
	{
		for (int j = 0; j < left_eof; j++)
		{
			buffer[j] = in_file.get();
		}
		
	}
	else if (buffer_forward == left_eof)
	{
		for (int i = 0; i < BUFFER_SIZE_HALF; i++)
		{
			temp_buffer[i] = buffer[i + BUFFER_SIZE_HALF + 1];
			buffer[i + BUFFER_SIZE_HALF + 1] = in_file.get();
			if(in_file.eof())
			{
				has_read = i;
			}	
		}
		buffer_forward = left_eof + 1;
	}
	else if (buffer_forward == right_eof)
	{
		for (int i = 0; i < BUFFER_SIZE_HALF; i++)
		{
			temp_buffer[i] = buffer[i];
			buffer[i] = in_file.get();
			if(in_file.eof())
			{
				has_read = i;
			}
		}
		
		buffer_forward = 0;
	}
}

void write_file()
{
	for (unsigned int i = 0; i < output_list.size(); i++)
	{
		out_file << output_list.at(i).first << " " << output_list.at(i).second << endl;
	}
	out_file << "The state line number is: " << line_num << endl;
	out_file << "The character number is: " << ch_num << endl;
	out_file << "The error list is: " << endl;
	if (error_list.size() == 0)
	{
		out_file << "Congratulations! No lexical error is found." << endl;
	}
	else
	{
		for (unsigned int i = 0; i < error_list.size(); i++)
		{
			out_file << "At the line of " << error_list.at(i).first + 1 << ", the type of error is: " << error_list.at(i).second << endl;
		}
	}

	out_file << "The number of NUM is: " << num_num << endl;
	out_file << "The number of reserved character appeared in the source code is: " << reserved_num << endl;
	out_file << "The number of defined character is: " << symbol_table.size() << endl;
	out_file << "The number of error is: " << error_list.size() << endl;
}

void get_char()
{
	C = buffer[buffer_forward];
	buffer_forward++;
	if (buffer_forward == left_eof || buffer_forward == right_eof)
	{
		read_file();
	}
	if (C == '\n')
	{
		line_num++;
	}
	if ((C != '\n') && (C != ' ') && (C != '\t') && (C != EOF) && (C < 128 && C >= 0))
	{
		ch_num++;
	}
}

void retract()
{
	if (buffer_forward == 0)
	{
		buffer_forward = right_eof - 1;
		for (int i = 0; i < BUFFER_SIZE_HALF; i++)
		{
			buffer[i] = temp_buffer[i];
		}
		if(in_file.eof())
		{
			in_file.close();
			in_file.open("jobshop.c", ios::in);
			in_file.seekg(-has_read,ios::end);
		}
		else
		{
			in_file.seekg(-BUFFER_SIZE_HALF,ios::cur);
		}
		
	}
	else if (buffer_forward == left_eof + 1)
	{
		buffer_forward = left_eof - 1;
		for (int i = 0; i < BUFFER_SIZE_HALF; i++)
		{
			buffer[i + left_eof + 1] = temp_buffer[i];
		}
		if(in_file.eof())
		{
			in_file.close();
			in_file.open("jobshop.c", ios::in);
			in_file.seekg(-has_read,ios::end);
		}
		else
		{
			in_file.seekg(-BUFFER_SIZE_HALF,ios::cur);
		}
	}
	else
	{
		buffer_forward--;
	}
	if ((C != '\n') && (C != ' ') && (C != '\t'))
	{
		ch_num--;
	}
	if (C == '\n')
	{
		line_num--;
	}
}

void get_nbc()
{
	while (C == ' ' || C == '\n' || C == '\t')
	{
		get_char();
	}
}

void cat()
{
	token += C;
}

bool is_letter(char C)
{
	if ((C >= 'a' && C <= 'z') || (C >= 'A' && C <= 'Z') || (C == '_'))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool is_digit(char C)
{
	if (C >= '0' && C <= '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool is_underline(char C)
{
	if (C == '_')
	{
		return true;
	}
	else
	{
		return false;
	}
}

void create_reserved_table()
{
/*auto double int struct break else long switch

case enum register typedef char extern return union

const float short unsigned continue for signed void

default goto sizeof volatile do if while static*/
	reserved_table.insert(pair<string, string>("auto", "reserved"));
	reserved_table.insert(pair<string, string>("double", "reserved"));
	reserved_table.insert(pair<string, string>("int", "reserved"));
	reserved_table.insert(pair<string, string>("struct", "reserved"));
	reserved_table.insert(pair<string, string>("break", "reserved"));
	reserved_table.insert(pair<string, string>("else", "reserved"));
	reserved_table.insert(pair<string, string>("long", "reserved"));
	reserved_table.insert(pair<string, string>("switch", "reserved"));
	reserved_table.insert(pair<string, string>("case", "reserved"));
	reserved_table.insert(pair<string, string>("enum", "reserved"));
	reserved_table.insert(pair<string, string>("register", "reserved"));
	reserved_table.insert(pair<string, string>("typedef", "reserved"));
	reserved_table.insert(pair<string, string>("char", "reserved"));
	reserved_table.insert(pair<string, string>("extern", "reserved"));
	reserved_table.insert(pair<string, string>("return", "reserved"));
	reserved_table.insert(pair<string, string>("union", "reserved"));
	reserved_table.insert(pair<string, string>("const", "reserved"));
	reserved_table.insert(pair<string, string>("float", "reserved"));
	reserved_table.insert(pair<string, string>("short", "reserved"));
	reserved_table.insert(pair<string, string>("unsigned", "reserved"));
	reserved_table.insert(pair<string, string>("continue", "reserved"));
	reserved_table.insert(pair<string, string>("for", "reserved"));
	reserved_table.insert(pair<string, string>("signed", "reserved"));
	reserved_table.insert(pair<string, string>("void", "reserved"));
	reserved_table.insert(pair<string, string>("default", "reserved"));
	reserved_table.insert(pair<string, string>("goto", "reserved"));
	reserved_table.insert(pair<string, string>("sizeof", "reserved"));
	reserved_table.insert(pair<string, string>("volatile", "reserved"));
	reserved_table.insert(pair<string, string>("do", "reserved"));
	reserved_table.insert(pair<string, string>("if", "reserved"));
	reserved_table.insert(pair<string, string>("while", "reserved"));
	reserved_table.insert(pair<string, string>("static", "reserved"));
}

bool is_reserved(string token)
{
	if (reserved_table.find(token) == reserved_table.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool is_defined(string token)
{
	if (symbol_table.find(token) == symbol_table.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void table_insert(string token)
{
	symbol_table.insert(pair<string, string>(token, token));
}

int SToI(string token)
{
	return std::stoi(token);
}

float SToF(string token)
{
	return std::stof(token);
}



int main(int argc, char* argv[]) {
	create_reserved_table();
	in_file.open("jobshop.c", ios::in);
	out_file.open("output.txt",ios::out);
	if (in_file)
	{
		read_file();
		deal_state_dfa();
		/*for(int i = 0; i < 2050; i++)
		{
			if(i!=left_eof&&i!=right_eof)
			{
				if(buffer[i] == EOF)
				{
					printf("*************ÕâÊÇeof***********");
				}
				printf("%c", buffer[i]);
			}
			else
			{
				printf("%d\n",i);
			}
		}*/
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
