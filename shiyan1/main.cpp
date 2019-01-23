#include<iostream>
#include<string.h>
#include<ctype.h>
#include<sstream>
#include<fstream>
#include<vector>
#include<stdio.h>
using namespace std;
enum Tag {
	REL, NUM, ID, ADD, SUB, MUL, DIV, B, S, E, LK, RK, F, Y,
	EE, BE, SE, NE, IF, THEN, ELSE, WHILE, DO, ERROR, FLOAT, INT
};

class Token
{
public:
	Tag tag;//内部标签
	string lexeme;
	Token() {}
	Token(Tag t, string l)
	{
		tag = t;
		lexeme = l;
	}
	void setToken(Tag t, string l)
	{
		tag = t;
		lexeme = l;
	}
	virtual string toString()
	{
		stringstream ss;
		ss << tag;
		return "<" + ss.str() + "," + lexeme + ">";
	}
};
vector<Token> tokens;
int line = 0;
int i = 0;
string str;
struct synbl1
{
	string name;
	int kind;
};
vector<synbl1> synbl;

struct er
{
	int kind;
	int line;
};
vector<er> Error;
void fail1()
{
	er t;
	t.kind = 1;
	t.line = line;
	Error.push_back(t);
	//cout << "error:at line: " << line <<" after '!' want '='"<< endl;
	//i++;
}
void fail()
{
	er t;
	t.kind = 2;
	t.line = line;
	Error.push_back(t);
	//cout << "error:at line: " <<line<< endl;
	i++;
}
int addSynbl(string name)
{
	synbl1 temp;
	temp.name = name;
	int i = 0;
	for (i; i < synbl.size(); i++)
	{
		if (synbl[i].name == name)
		{
			return i;
		}
	}
	synbl.push_back(temp);
	return i;
}

bool isend();
bool isDigit(char c)
{
	int a = c - '0';
	if (a >= 0 && a < 10)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool isAlpha(char c)
{
	if (c >= 'a'&&c <= 'z')
	{
		return true;
	}
	else if (c >= 'A'&&c <= 'Z')
	{
		return true;
	}
	else
	{
		return false;
	}
}
Token getToken()
{
	//int i = index;
	//此处应该从文件中读入源程序 现省略
begin:
	Token t;
	for (i; i != str.length(); i++)
	{
		if (str[i] == ' ' || str[i] == '\t')
			continue;
		else if (str[i] == '\n')
			line = line + 1;
		else
			break;
	}
	if (isDigit(str[i]))//字符是数字
	{
		int v = 0;
		do
		{
			v = v * 10 + (str[i] - '0');
			i++;
		} while (isDigit(str[i]));
		if (str[i] != '.')
		{
			stringstream ss;
			ss << v;
			t.setToken(NUM, ss.str());
			return t;
		}
		float x = v;
		float d = 10;
		for (;;)
		{
			i++;
			if (!isDigit(str[i]))
			{
				break;
			}
			x = x + (str[i] - '0') / d;
			d = d * 10;
		}
		i--;
		//此处应返回浮点数
		stringstream ss;
		ss << v;
		t.setToken(NUM, ss.str());
		return t;

	}
	else if (isAlpha(str[i]))//字符是字母
	{
		string b;
		do
		{
			b += str[i];
			i++;
		} while (isAlpha(str[i]));

		if (b == "if")
		{
			t.setToken(IF, b);
			return t;
		}
		else if (b == "then")
		{
			t.setToken(THEN, b);
			return t;
		}
		else if (b == "else")
		{
			t.setToken(ELSE, b);
			return t;
		}
		else if (b == "while")
		{
			t.setToken(WHILE, b);
			return t;
		}
		else if (b == "do")
		{
			t.setToken(DO, b);
			return t;
		}
		else if (b == "int")
		{
			t.setToken(INT, b);
			return t;
		}
		else if (b == "float")
		{
			t.setToken(FLOAT, b);
			return t;
		}
		else
		{
			int temp = addSynbl(b);
			stringstream ss;
			ss << temp;
			t.setToken(ID, ss.str());
			return t;
		}
	}
	else  //下面既不是字符也不是字母 即为符号
	{
		int state = 0;
		while (1)
		{
			switch (state)
			{
			case 0:
				if (str[i] == '+')
					state = 1;
				else if (str[i] == '-')
					state = 2;
				else if (str[i] == '*')
					state = 3;
				else if (str[i] == '/')
					state = 4;
				else if (str[i] == '>')
					state = 5;
				else if (str[i] == '<')
					state = 6;
				else if (str[i] == '=')
					state = 7;
				else if (str[i] == '(')
					state = 8;
				else if (str[i] == ')')
					state = 9;
				else if (str[i] == ';')
					state = 10;
				else if (str[i] == '\'')
					state = 11;
				else if (str[i] == '!')
					state = 16;
				else
				{
					fail();
					if (!isend())
					{
						goto begin;
					}
					else
					{
						t.setToken(ERROR, "END");
						return t;
					}

				}
				i++;
				break;
			case 1:
				t.setToken(ADD, "+");
				return t;
			case 2:
				t.setToken(SUB, "-");
				return t;
			case 3:
				t.setToken(MUL, "*");
				return t;
			case 4:
				t.setToken(DIV, "/");
				return t;
			case 5:
				if (str[i] == '=')
				{
					state = 13;
					break;
				}
				t.setToken(B, ">");
				return t;
			case 6:
				if (str[i] == '=')
				{
					state = 14;
					break;
				}
				t.setToken(S, "<");
				return t;
			case 7:
				if (str[i] == '=')
				{
					state = 12;
					break;
				}
				t.setToken(E, "=");
				return t;
			case 8:
				t.setToken(LK, "(");
				return t;
			case 9:
				t.setToken(RK, ")");
				return t;
			case 10:
				t.setToken(F, ";");
				return t;
			case 11:
				t.setToken(Y, "\'");
				return t;
			case 12:
				t.setToken(EE, "==");
				return t;
			case 13:
				t.setToken(BE, ">=");
				return t;
			case 14:
				t.setToken(SE, "<=");
				return t;
			case 15:
				t.setToken(NE, "!=");
				return t;
			case 16:
				if (str[i] == '=')
				{
					state = 15;
					break;
				}
				else
				{
					fail1();
					state = 15;
					break;
				}
			}
		}
	}
}
bool isend()
{
	int k = i;
	while (k != str.length() - 1)
	{
		if (str[k] == ' ')
		{
			k++;
		}
		else if (str[k] == '\t')
		{
			k++;
		}
		else if (str[k] == '\n')
		{
			k++;
			//line++;
		}
		else
			break;
	}
	if (k == str.length() - 1)
	{
		return true;
	}
	else
		return false;
}

string readFile()
{
	char f;
	string str;
	string a;
	string end = "#end";
	char line[1024] = { 0 };
	do
	{
		cout << "请选择输入方式:1读文件，2使用控制台输入" << endl;
		cin >> f;
		fflush(stdin);
		if (f == '1' || f == '2') break;
	} while (1);

	if (f == '1')
	{
		ifstream infile;


		char c;
		do
		{
			do
			{
				cout << "请确定在程序执行文件加下面已有fenxi.txt文件，请在此文件内输入要分析的程序" << endl;
				cout << "是否存在“Y/N”" << endl;
				cin >> c;
			} while (c != 'y'&&c != 'Y');
			fflush(stdin);
			infile.open("fenxi.txt", ios::in);
			if (!infile)
			{
				cout << "打开文件失败" << endl;
			}
		} while (!infile);
		int i = 0;
		while (!infile.eof())
		{
			infile.getline(line, sizeof(line));
			str += line;
			str += "\n";
			//cout << ++i << ":" <<line<< "len:"<<str.length()<<endl;
			string a;
			a = line;
			//writefile(line);
		}
		str += "$";
		infile.close();
	}
	else if (f == '2')
	{
		cout << "请使用英文输入符号输入要分析的内容，新一行#end为结束标记" << endl;
		while (1)
		{
			cin.getline(line, sizeof(line));
			a = "";
			a += line;
			if (a == end) break;
			str += line;
			//str+="\n";
			if(a.length()>=1)
			if(a[a.length()-1]!='\n')
				str+="\n";

		}
		str += '$';

	}

	return str;
}


void printftokenandsynbl()
{
	cout << "=============输出token序列=====================" << endl;
	cout << "词法分析器输出的token应为（单词种别码，属性值），为便于展示实验结果，现输出单词及其token序列，并且显示其类型" << endl;
	cout << "其中常数属性值为自身值，标识符属性值为在符号表中位置" << endl;
	cout << "单词" << '\t' << "种别码" << '\t' << "自身值" << '\t' << "类型" << endl;
	for (int i = 0; i < tokens.size(); i++)
	{
		Token t = tokens[i];
		if (t.tag == ID)
		{
			istringstream is(t.lexeme);
			int i;
			is >> i;
			cout << synbl[i].name << '\t' << t.tag << '\t';
		}
		else
		{
			cout << t.lexeme << '\t' << t.tag << '\t';
		}
		if (t.tag == NUM)
		{
			cout << t.lexeme << '\t' << "常数" << endl;
		}
		else if (t.tag == ID)
		{
			cout << t.lexeme << '\t' << "标识符" << endl;
		}
		else if (t.tag == IF || t.tag == WHILE || t.tag == DO || t.tag == ELSE || t.tag == THEN)
		{
			cout << "\t" << "关键字" << endl;
		}
		else if (t.tag == LK || t.tag == RK || t.tag == F || t.tag == Y)
		{
			cout << "\t" << "分隔符" << endl;
		}
		else if (t.tag == ERROR)
		{
			break;
		}
		else
		{
			cout << "\t" << "运算符" << endl;
		}
	}
	cout << "========符号表=============" << endl;
	cout << "编号\t" << "name\t" << endl;
	for (int i = 0; i < synbl.size(); i++)
	{
		cout << i << '\t' << synbl[i].name << endl;
	}
}
void printferror()
{
	cout << "========错误信息=============" << endl;
	for (int i = 0; i < Error.size(); i++)
	{
		if (Error[i].kind == 1)
		{
			cout << "error:at line: " << Error[i].line << " after '!' want '='" << endl;
		}
		else
		{
			cout << "error:at line: " << Error[i].line << " 错误字符" << endl;
		}
	}
}
void writefile()
{
	ofstream ofile;
	string str;
	char line[1024] = { 0 };

	ofile.open("tokens.txt", ios::out);
	if (!ofile)
	{
		cout << "打开文件失败" << endl;
		return;
	}
	int i = 0;

	for (i; i < tokens.size(); i++)
	{
		ofile << " (" << tokens[i].tag << "," << tokens[i].lexeme << ")" << endl;
	}
	ofile << "========符号表=============" << endl;
	ofile << "编号\t" << "name\t" << endl;
	for (i = 0; i < synbl.size(); i++)
	{
		ofile << i << "\t" << synbl[i].name << "\t" << endl;
	}

	ofile.close();
    cout<<"token序列与符号表已存放在可执行文件同一目录下tokens.txt文件中"<<endl;
}
int main()
{
	Token t;
	str = readFile();//获取要分析的内容
	while ((!isend())&&(t.tag!=ERROR))
	{
		 t= getToken();
		if (t.tag != ERROR) {
			tokens.push_back(t);
		}

	}

	printftokenandsynbl();//输出符号表
	printferror();//输出错误信息
	writefile();

	cout << "输入任意字符退出" << endl;
	getchar();
	return 0;
}
