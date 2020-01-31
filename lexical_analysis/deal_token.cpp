#include "Deal_token.h"
#include "Declaration.h"

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



/*auto double int struct break else long switch

case enum register typedef char extern return union

const float short unsigned continue for signed void

default goto sizeof volatile do if while static*/

void create_reserved_table()
{
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

void cat()
{
	token += C;
}