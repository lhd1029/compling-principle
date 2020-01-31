#include "Declaration.h"
#include "Dfa_deal.h"
#include "Deal_token.h"
#include "File.h"


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
					error_list.push_back(pair<int, string>(line_num, "Invalid suffix " + token + " on integer constant"));
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
				error_list.push_back(pair<int, string>(line_num, "Wrong type of number! Exponent has no digits"));
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
				error_list.push_back(pair<int, string>(line_num, "Wrong type of number! Exponent has no digits"));
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
				error_list.push_back(pair<int, string>(line_num, "Wrong type of number! Invalid suffix \"x\" on integer constant"));
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
					error_list.push_back(pair<int, string>(line_num, "Invalid suffix " + token + " on integer constant"));
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
				error_list.push_back(pair<int, string>(line_num, "Wrong type of number! Exponent has no digits"));
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
				error_list.push_back(pair<int, string>(line_num, "Wrong type of number! Exponent has no digits"));
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
				error_list.push_back(pair<int, string>(line_num, "Wrong type of number! Hexadecimal floating constants require an exponent"));
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
			error_list.push_back(pair<int, string>(line_num, "Stray " + to_string(int(C)) + " in the program"));
			get_char();
			error_list.push_back(pair<int, string>(line_num, "Stray " + to_string(int(C)) + " in the program"));
			state = 0;
			break;
		}
		}
		
	} while (C != EOF);
}
