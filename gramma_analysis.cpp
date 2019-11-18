#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class File
{
private:
	static fstream in_file;
public:
	static bool open_file()
	{
		in_file.open("input.txt", ios::in);
		if(!in_file)
		{
			return false;
		}
		else
			return true;
	}
	
	static char read_file()
	{
		char c;
		c = in_file.get();
		return c;
	}

};

class Rec_call//递归调用预测分析， 状态转换图见实验报告 
{
	private:
		static char ch;
	public:
		static void start()
		{
			ch = File::read_file();
			procE();
		}
		static void procE() //E状态的递归分析 
		{
			procT();
			cout << "E -> T" << endl;
			if(ch == '+' || ch == '-')
			{
				if (ch == '+')
				{
					cout << "E -> E + T" << endl;;
				}
				else
				{
					cout << "E -> E - T" << endl;;
				}
				ch = File::read_file();
				procE();
			}
		}
		
		static void procT() //T状态的递归分析 
		{
			procF();
			cout << "T -> F" << endl;;
			if(ch == '*' || ch == '/')
			{
				if (ch == '*')
				{
					cout << "E -> E * T" << endl;;
				}
				else
				{
					cout << "E -> E / T" << endl;;
				}
				ch = File::read_file();
				procT();
			}
		}
		
		static void procF() //F状态的递归分析 
		{
			if(ch == '(')
			{
				ch = File::read_file();
				procE();
				if(ch == ')')
				{
					ch = File::read_file();
					cout << "F -> (E)" << endl;;
				}
				else
					report_error();
			}
			else if(is_digit(ch))
			{
				ch = File::read_file();
				cout << "F -> num" << endl;;
			}
			else
				report_error();
		}
		
		static void report_error()
		{
			cout << "ERROR!" << endl;
		}
		
		static bool is_digit(char ch)
		{
			if (ch >= '0' && ch <= '9')
			{
				return true;
			}
			return false;
		}
};

class Fore_ana //非递归预测分析 
{
	/*E'用e表示，ε用o表示*/
	private:
		static char ch;
		static char first[5][3];//5个非终结符号的first集（最多有3个） 
		static char follow[5][6];//5个非终结符号的follow集（最多有6个） 
		static int table[5][8];//预测分析表 
		static vector<char> signal; //符号栈 
		static string formula[11][2]; //产生式，第一项为左部符号，第二项为右部符号 
	public:
		static bool is_nt(char ch_temp) //判断字符是否是非终结符 
		{
			if (ch_temp == 'E' || ch_temp == 'e' || ch_temp == 'T' || ch_temp == 't' || ch_temp == 'F')
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		static void make_first() //制作first集 
		{
			first[0][0] = '('; first[0][1] =  'n';
			first[1][0] = '+'; first[1][1] =  '-'; first[1][2] =  'o';
			first[2][0] = '('; first[2][1] =  'n';
			first[3][0] = '*'; first[3][1] =  '/'; first[3][2] =  'o';
			first[4][0] = '('; first[4][1] =  'n';
		}
		
		static void make_follow() //制作follow集 
		{
			follow[0][0] = '$'; follow[0][1] = ')';
			follow[1][0] = '$'; follow[1][1] = ')';
			follow[2][0] = '$'; follow[2][1] = '+'; follow[2][2] = '-'; follow[2][3] = ')';
			follow[3][0] = '$'; follow[3][1] = '+'; follow[3][2] = '-'; follow[3][3] = ')';
			follow[4][0] = '$'; follow[4][1] = '+'; follow[4][2] = '-'; follow[4][3] = '*'; follow[4][4] = '/'; follow[4][5] = ')';
		}
		
		static void make_formula() //制作产生式集 
		{
			formula[1][0] = 'E'; formula[1][1] = "Te";
			formula[2][0] = 'e'; formula[2][1] = "+Te";
			formula[3][0] = 'e'; formula[3][1] = "-Te";
			formula[4][0] = 'e'; formula[4][1] = "o";
			formula[5][0] = 'T'; formula[5][1] = "Ft";
			formula[6][0] = 't'; formula[6][1] = "*Ft";
			formula[7][0] = 't'; formula[7][1] = "/Ft";
			formula[8][0] = 't'; formula[8][1] = "o";
			formula[9][0] = 'F'; formula[9][1] = "(E)";
			formula[10][0] = 'F'; formula[10][1] = "n";
		}
		
		static string cal_first(char first_ch) //求出符号的first集，并返回一个字符串 
		{
			string result = "";
			if (is_nt(first_ch)) //如果是非终结符号，则把它first数组中全部元素加入字符串 
			{
				int nt_code = cal_n_code(first_ch);
				for (int i = 0; first[nt_code][i] != '\0'; i++)
				{
					if (first[nt_code][i] != 'o') //注意不要添加 ε
						result += first[nt_code][i];
				}
			}
			else {//如果是非终结符号，则直接把它本身加入字符串 
				if (first_ch != 'o')
					result = first_ch;
			}
			return result;
		}
		
		static string cal_follow(char left_signal)//求出符号的follow集，并返回一个字符串 
		{
			string result = "";
			int nt_code = cal_n_code(left_signal);
			for (int i = 0; follow[nt_code][i] != '\0'; i++)
			{
				result += follow[nt_code][i];
			}
			return result;
		}
		
		static void make_table()
		{
			make_first();
			make_follow();
			make_formula();
			/*手绘预测分析表： 
			table[0][0] = 1; table[0][5] = 1;
			table[1][1] = 2; table[1][2] = 3; table[1][6] = 4; table[1][7] = 4;
			table[2][0] = 5; table[2][5] = 5;
			table[3][1] = 8; table[3][2] = 8; table[3][3] = 6; table[3][4] = 7; table[3][6] = 8; table[3][7] = 8;
			table[4][0] = 10; table[4][5] = 9;*/
			for (int i = 1; i <= 10; i++)
			{
				string first_set = cal_first(formula[i][1][0]);
				string follow_set = cal_follow(formula[i][0][0]);
				//对于A -> a 
				for (int j = 0; j < first_set.length(); j++) // 对于first(a)中的所有元素,把A -> a加入table[A,a] 
				{
					int t_code = cal_t_code(first_set[j]);
					int n_code = cal_n_code(formula[i][0][0]);
					table[n_code][t_code] = i;
				}
				if (formula[i][1].find('o') != string::npos) // 如果first(a)中有 ε,对于follow(a)中的所有元素,把A -> a加入table[A,a] 
				{
					for (int j = 0; j < follow_set.length(); j++)
					{
						int t_code = cal_t_code(follow_set[j]);
						int n_code = cal_n_code(formula[i][0][0]);
						table[n_code][t_code] = i;
					}
				}
			}
			/*打印预测分析表： 
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					cout << table[i][j] << " ";
				}
				cout << endl;
			}*/
		}
		
		static int cal_n_code(char ch_temp)//给非终结符编号 
		{
			int result;
			switch (ch_temp)
			{
				case 'E':
					result = 0;
					break;
				case 'e':
					result = 1;
					break;
				case 'T':
					result = 2;
					break;
				case 't':
					result = 3;
					break;
				case 'F':
					result = 4;
					break;
				default:
					result = 5;
					break;
			}
			return result;
		}
		
		static int cal_t_code(char ch_temp)//给终结符编号 
		{
			int result;
			switch (ch_temp)
			{
				case '+':
					result = 1;
					break;
				case '-':
					result = 2;
					break;
				case '*':
					result = 3;
					break;
				case '/':
					result = 4;
					break;
				case '(':
					result = 5;
					break;
				case ')':
					result = 6;
					break;
				case '$':
					result = 7;
					break;
				default:
					break;
			}
			if (ch_temp >= '0' && ch_temp <= '9')//数字统一编号0 
			{
				result = 0;
			}
			return result;
		}
		
		static void report_error()//报错 
		{
			cout << "ERROR!" << endl;
		}
		
		static void rev_push(int formula_no) //符号栈操作，注意逆序入栈 
		{
			//调整输出，与题目一致 
			string left_signal = formula[formula_no][0];
			if (left_signal == "e")
				left_signal = "E'";
			else if (left_signal == "t")
				left_signal = "T'";
			
			string right_signal = formula[formula_no][1];
			if (right_signal == "o")
				right_signal = "ε";
			else if (right_signal == "n")
				right_signal = "num";
			
			cout << left_signal << " -> " << right_signal << endl;
			
			
			//压栈 
			for (int i = formula[formula_no][1].length() - 1; i >= 0 ; i--)
			{
				if (formula[formula_no][1][i] != 'o')
					signal.push_back(formula[formula_no][1][i]);
			}
			/*手工压栈 
			switch (formula)
			{
				case 1:
					cout << "E -> TE'" << endl;
					signal.push_back('e');
					signal.push_back('T');
					break;
				case 2:
					cout << "E' -> +TE'" << endl;
					signal.push_back('e');
					signal.push_back('T');
					signal.push_back('+');
					break;
				case 3:
					cout << "E' -> -TE'" << endl;
					signal.push_back('e');
					signal.push_back('T');
					signal.push_back('-');
					break;
				case 4:
					cout << "E' -> ε" << endl;
					break;
				case 5:
					cout << "T -> FT'" << endl;
					signal.push_back('t');
					signal.push_back('F');
					break;
				case 6:
					cout << "T' -> *FT'" << endl;
					signal.push_back('t');
					signal.push_back('F');
					signal.push_back('*');
					break;
				case 7:
					cout << "T' -> /FT'" << endl;
					signal.push_back('t');
					signal.push_back('F');
					signal.push_back('/');
					break;
				case 8:
					cout << "T' -> ε" << endl;
					break;
				case 9:
					cout << "F -> (E)" << endl;
					signal.push_back(')');
					signal.push_back('E');
					signal.push_back('(');
					break;
				case 10:
					cout << "F -> num" << endl;
					signal.push_back('n');
					break;
				default:
					break;
			}*/
		}
		
		static void start()
		{
			make_table();
			signal.push_back('$');
			signal.push_back('E');
			ch = File::read_file();
			while (ch != '$')
			{
				if (cal_n_code(signal[signal.size() - 1]) == 5) //如果非终结符匹配，则弹出，移动指针 
				{
					if (cal_t_code(ch) == 0) //数字 
					{
						if (signal.back() == 'n')
						{
							ch = File::read_file();
							signal.pop_back();
						}
					}
					else if (ch == signal.back()) //其他非终结符 
					{
						ch = File::read_file();
						signal.pop_back();
					}
				}
				else //根据预测分析表弹栈入栈 
				{
					int formula = table[cal_n_code(signal.back())][cal_t_code(ch)];
					if (formula != 0)
					{
						signal.pop_back();
						rev_push(formula);
					}
				
				}
			}
			cout << "SUCCESS!" << endl;
		}
		
};

fstream File::in_file;
char Rec_call::ch;
char Fore_ana::first[5][3] = {{'\0','\0','\0'}};
char Fore_ana::follow[5][6] = {{'\0','\0','\0','\0','\0'}};
int Fore_ana::table[5][8] = {{0}};
string Fore_ana::formula[11][2];
char Fore_ana::ch;
vector<char> Fore_ana::signal;

int main()
{
	if(File::open_file())
	{
		//Rec_call::start();	
		Fore_ana::start();
		
	}
	else
	{
		cout << "Failure to open the file!" << endl;
	}
	
}
