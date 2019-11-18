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

class Rec_call//�ݹ����Ԥ������� ״̬ת��ͼ��ʵ�鱨�� 
{
	private:
		static char ch;
	public:
		static void start()
		{
			ch = File::read_file();
			procE();
		}
		static void procE() //E״̬�ĵݹ���� 
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
		
		static void procT() //T״̬�ĵݹ���� 
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
		
		static void procF() //F״̬�ĵݹ���� 
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

class Fore_ana //�ǵݹ�Ԥ����� 
{
	/*E'��e��ʾ������o��ʾ*/
	private:
		static char ch;
		static char first[5][3];//5�����ս���ŵ�first���������3���� 
		static char follow[5][6];//5�����ս���ŵ�follow���������6���� 
		static int table[5][8];//Ԥ������� 
		static vector<char> signal; //����ջ 
		static string formula[11][2]; //����ʽ����һ��Ϊ�󲿷��ţ��ڶ���Ϊ�Ҳ����� 
	public:
		static bool is_nt(char ch_temp) //�ж��ַ��Ƿ��Ƿ��ս�� 
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
		static void make_first() //����first�� 
		{
			first[0][0] = '('; first[0][1] =  'n';
			first[1][0] = '+'; first[1][1] =  '-'; first[1][2] =  'o';
			first[2][0] = '('; first[2][1] =  'n';
			first[3][0] = '*'; first[3][1] =  '/'; first[3][2] =  'o';
			first[4][0] = '('; first[4][1] =  'n';
		}
		
		static void make_follow() //����follow�� 
		{
			follow[0][0] = '$'; follow[0][1] = ')';
			follow[1][0] = '$'; follow[1][1] = ')';
			follow[2][0] = '$'; follow[2][1] = '+'; follow[2][2] = '-'; follow[2][3] = ')';
			follow[3][0] = '$'; follow[3][1] = '+'; follow[3][2] = '-'; follow[3][3] = ')';
			follow[4][0] = '$'; follow[4][1] = '+'; follow[4][2] = '-'; follow[4][3] = '*'; follow[4][4] = '/'; follow[4][5] = ')';
		}
		
		static void make_formula() //��������ʽ�� 
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
		
		static string cal_first(char first_ch) //������ŵ�first����������һ���ַ��� 
		{
			string result = "";
			if (is_nt(first_ch)) //����Ƿ��ս���ţ������first������ȫ��Ԫ�ؼ����ַ��� 
			{
				int nt_code = cal_n_code(first_ch);
				for (int i = 0; first[nt_code][i] != '\0'; i++)
				{
					if (first[nt_code][i] != 'o') //ע�ⲻҪ��� ��
						result += first[nt_code][i];
				}
			}
			else {//����Ƿ��ս���ţ���ֱ�Ӱ�����������ַ��� 
				if (first_ch != 'o')
					result = first_ch;
			}
			return result;
		}
		
		static string cal_follow(char left_signal)//������ŵ�follow����������һ���ַ��� 
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
			/*�ֻ�Ԥ������� 
			table[0][0] = 1; table[0][5] = 1;
			table[1][1] = 2; table[1][2] = 3; table[1][6] = 4; table[1][7] = 4;
			table[2][0] = 5; table[2][5] = 5;
			table[3][1] = 8; table[3][2] = 8; table[3][3] = 6; table[3][4] = 7; table[3][6] = 8; table[3][7] = 8;
			table[4][0] = 10; table[4][5] = 9;*/
			for (int i = 1; i <= 10; i++)
			{
				string first_set = cal_first(formula[i][1][0]);
				string follow_set = cal_follow(formula[i][0][0]);
				//����A -> a 
				for (int j = 0; j < first_set.length(); j++) // ����first(a)�е�����Ԫ��,��A -> a����table[A,a] 
				{
					int t_code = cal_t_code(first_set[j]);
					int n_code = cal_n_code(formula[i][0][0]);
					table[n_code][t_code] = i;
				}
				if (formula[i][1].find('o') != string::npos) // ���first(a)���� ��,����follow(a)�е�����Ԫ��,��A -> a����table[A,a] 
				{
					for (int j = 0; j < follow_set.length(); j++)
					{
						int t_code = cal_t_code(follow_set[j]);
						int n_code = cal_n_code(formula[i][0][0]);
						table[n_code][t_code] = i;
					}
				}
			}
			/*��ӡԤ������� 
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					cout << table[i][j] << " ";
				}
				cout << endl;
			}*/
		}
		
		static int cal_n_code(char ch_temp)//�����ս����� 
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
		
		static int cal_t_code(char ch_temp)//���ս����� 
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
			if (ch_temp >= '0' && ch_temp <= '9')//����ͳһ���0 
			{
				result = 0;
			}
			return result;
		}
		
		static void report_error()//���� 
		{
			cout << "ERROR!" << endl;
		}
		
		static void rev_push(int formula_no) //����ջ������ע��������ջ 
		{
			//�������������Ŀһ�� 
			string left_signal = formula[formula_no][0];
			if (left_signal == "e")
				left_signal = "E'";
			else if (left_signal == "t")
				left_signal = "T'";
			
			string right_signal = formula[formula_no][1];
			if (right_signal == "o")
				right_signal = "��";
			else if (right_signal == "n")
				right_signal = "num";
			
			cout << left_signal << " -> " << right_signal << endl;
			
			
			//ѹջ 
			for (int i = formula[formula_no][1].length() - 1; i >= 0 ; i--)
			{
				if (formula[formula_no][1][i] != 'o')
					signal.push_back(formula[formula_no][1][i]);
			}
			/*�ֹ�ѹջ 
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
					cout << "E' -> ��" << endl;
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
					cout << "T' -> ��" << endl;
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
				if (cal_n_code(signal[signal.size() - 1]) == 5) //������ս��ƥ�䣬�򵯳����ƶ�ָ�� 
				{
					if (cal_t_code(ch) == 0) //���� 
					{
						if (signal.back() == 'n')
						{
							ch = File::read_file();
							signal.pop_back();
						}
					}
					else if (ch == signal.back()) //�������ս�� 
					{
						ch = File::read_file();
						signal.pop_back();
					}
				}
				else //����Ԥ�������ջ��ջ 
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
