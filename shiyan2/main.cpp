#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<stack>
#include<set>
#include<cstring>
#include<queue>
#include<fstream>
#include <sstream>
#include<stdio.h>
using namespace std;
enum Tag
{
    REL, NUM, ID, ADD, SUB, MUL, DIV, B, S, E, LK, RK, F, Y,
    EE, BE, SE, NE, IF, THEN, ELSE, WHILE, DO, ERROR,FLOAT,INT,
};
//================================================================================================
//����˵��
//����ǰ�벿��Ϊ�ʷ�������ش��� һֱ����580��
//֮�����Ϊ�﷨������ش��뵽����
//���﷨�����У������﷨����getToken������ȡ����
//================================================================================================
//�ʷ�������ش���
class Token
{
public:
    Tag tag;//�ڲ���ǩ
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
    //�˴�Ӧ�ô��ļ��ж���Դ���� ��ʡ��
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
    if (isDigit(str[i]))//�ַ�������
    {
        int v = 0;
        do
        {
            v = v * 10 + (str[i] - '0');
            i++;
        }
        while (isDigit(str[i]));
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
        //�˴�Ӧ���ظ�����
        stringstream ss;
        ss << v;
        t.setToken(NUM, ss.str());
        return t;

    }
    else if (isAlpha(str[i]))//�ַ�����ĸ
    {
        string b;
        do
        {
            b += str[i];
            i++;
        }
        while (isAlpha(str[i]));

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
    else  //����Ȳ����ַ�Ҳ������ĸ ��Ϊ����
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

string readfile()
{
    char f;
    string str;
    string a;
    string end = "#end";
    char line[1024] = { 0 };
    do
    {
        cout << "��ѡ�����뷽ʽ:1���ļ���2ʹ�ÿ���̨����" << endl;
        cin >> f;
        fflush(stdin);
        if (f == '1' || f == '2')
            break;
    }
    while (1);

    if (f == '1')
    {
        ifstream infile;


        char c;
        do
        {
            do
            {
                cout << "��ȷ���ڳ���ִ���ļ�����������fenxi.txt�ļ������ڴ��ļ�������Ҫ�����ĳ���" << endl;
                cout << "�Ƿ���ڡ�Y/N��" << endl;
                cin >> c;
            }
            while (c != 'y'&&c != 'Y');
            fflush(stdin);
            infile.open("fenxi.txt", ios::in);
            if (!infile)
            {
                cout << "���ļ�ʧ��" << endl;
            }
        }
        while (!infile);
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
        cout << "��ʹ��Ӣ�������������Ҫ���������ݣ���һ��#endΪ�������" << endl;
        while (1)
        {
            cin.getline(line, sizeof(line));
            a = "";
            a += line;
            if (a == end)
                break;
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
    cout << "=============���token����=====================" << endl;
    cout << "�ʷ������������tokenӦΪ�������ֱ��룬����ֵ����Ϊ����չʾʵ��������������ʼ���token���У�������ʾ������" << endl;
    cout << "���г�������ֵΪ����ֵ����ʶ������ֵΪ�ڷ��ű���λ��" << endl;
    cout << "����" << '\t' << "�ֱ���" << '\t' << "����ֵ" << '\t' << "����" << endl;
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
            cout << t.lexeme << '\t' << "����" << endl;
        }
        else if (t.tag == ID)
        {
            cout << t.lexeme << '\t' << "��ʶ��" << endl;
        }
        else if (t.tag == IF || t.tag == WHILE || t.tag == DO || t.tag == ELSE || t.tag == THEN)
        {
            cout << "\t" << "�ؼ���" << endl;
        }
        else if (t.tag == LK || t.tag == RK || t.tag == F || t.tag == Y)
        {
            cout << "\t" << "�ָ���" << endl;
        }
        else if (t.tag == ERROR)
        {
            break;
        }
        else
        {
            cout << "\t" << "�����" << endl;
        }
    }
    cout << "========���ű�=============" << endl;
    cout << "���\t" << "name\t" << endl;
    for (int i = 0; i < synbl.size(); i++)
    {
        cout << i << '\t' << synbl[i].name << endl;
    }
}

void printferror()
{
    cout << "========������Ϣ=============" << endl;
    for (int i = 0; i < Error.size(); i++)
    {
        if (Error[i].kind == 1)
        {
            cout << "error:at line: " << Error[i].line << " after '!' want '='" << endl;
        }
        else
        {
            cout << "error:at line: " << Error[i].line << " �����ַ�" << endl;
        }
    }
}
vector<string> proceout;
void writefile()
{
    ofstream ofile;
    string str;
    char line[1024] = { 0 };

    ofile.open("tokens.txt", ios::out);
    if (!ofile)
    {
        cout << "���ļ�ʧ��" << endl;
        return;
    }
    int i = 0;

    for (i; i < tokens.size(); i++)
    {
        ofile << " (" << tokens[i].tag << "," << tokens[i].lexeme << ")" << endl;
    }
    ofile << "========���ű�=============" << endl;
    ofile << "���\t" << "name\t" << endl;
    for (i = 0; i < synbl.size(); i++)
    {
        ofile << i << "\t" << synbl[i].name << "\t" << endl;
    }

    ofile.close();
    ofile.open("yf.txt", ios::out);
    if (!ofile)
    {
        cout << "���ļ�ʧ��" << endl;
        return;
    }
//    int i = 0;

    ofile << "========��������=============" << endl;
    for (i=0; i < proceout.size(); i++)
    {
        ofile <<proceout[i]<<endl;
    }
    ofile.close();
    cout<<"�������̵Ĳ���ʽ�Ѵ���ڿ�ִ���ļ�ͬһĿ¼��yf.txt�ļ���"<<endl;
}

//===========================================================================
//�﷨������ش���
struct item
{
    int num;//����ʽ�ı��
    int id;//���λ��
    int numI;//�������ĸ�����
}; //��
//enum Tag {
//	REL, NUM, ID, ADD, SUB, MUL, DIV, B, S, E, LK, RK, F, Y,
//	EE, BE, SE, NE, IF, THEN, ELSE, WHILE, DO, ERROR,FLOAT
//};

void coutfh(char x)
{

    int a = x - 'a'+1;
    switch (a)
    {
    case 2:
    {
        cout << "int10";
        break;
    }
    case 3:
    {
        cout << "id";
        break;
    }
    case 4:
    {
        cout << "+";
        break;
    }
    case 5:
    {
        cout << "-";
        break;
    }
    case 6:
    {
        cout << "*";
        break;
    }
    case 7:
    {
        cout << "/";
        break;
    }
    case 8:
    {
        cout << ">";
        break;
    }
    case 9:
    {
        cout << "<";
        break;
    }
    case 10:
    {
        cout << "=";
        break;
    }
    case 11:
    {
        cout << "(";
        break;
    }
    case 12:
    {
        cout << ")";
        break;
    }
    case 13:
    {
        cout << ";";
        break;
    }
    case 14:
    {
        cout << "\'";
        break;
    }
    case 15:
    {
        cout << "==";
        break;
    }
    case 16:
    {
        cout << ">=";
        break;
    }
    case 17:
    {
        cout << "<=";
        break;
    }
    case 18:
    {
        cout << "!=";
        break;
    }
    case 19:
    {
        cout << "if";
        break;
    }
    case 20:
    {
        cout << "then";
        break;
    }
    case 21:
    {
        cout << "else";
        break;
    }
    case 22:
    {
        cout << "while";
        break;
    }
    case 23:
    {
        cout << "do";
        break;
    }
    case 24:
    {
        cout << "error";
        break;
    }
    case 25:
    {
        cout << "float";
        break;
    }
    case 26:
    {
        cout << "int";
        break;
    }
    default:
    {
        cout << x;
        break;
    }
    }

}
string getfh(char x)
{

    int a = x - 'a'+1;
    string s;
    s=x;
    switch (a)
    {
    case 2:
    {
        return "int10";
        break;
    }
    case 3:
    {
        return  "id";
        break;
    }
    case 4:
    {
        return  "+";
        break;
    }
    case 5:
    {
        return  "-";
        break;
    }
    case 6:
    {
        return  "*";
        break;
    }
    case 7:
    {
        return  "/";
        break;
    }
    case 8:
    {
        return  ">";
        break;
    }
    case 9:
    {
        return  "<";
        break;
    }
    case 10:
    {
        return  "=";
        break;
    }
    case 11:
    {
        return  "(";
        break;
    }
    case 12:
    {
        return  ")";
        break;
    }
    case 13:
    {
        return  ";";
        break;
    }
    case 14:
    {
        return  "\'";
        break;
    }
    case 15:
    {
        return  "==";
        break;
    }
    case 16:
    {
        return  ">=";
        break;
    }
    case 17:
    {
        return  "<=";
        break;
    }
    case 18:
    {
        return "!=";
        break;
    }
    case 19:
    {
        return  "if";
        break;
    }
    case 20:
    {
        return  "then";
        break;
    }
    case 21:
    {
        return  "else";
        break;
    }
    case 22:
    {
        return  "while";
        break;
    }
    case 23:
    {
        return  "do";
        break;
    }
    case 24:
    {
        return  "error";
        break;
    }
    case 25:
    {
        return  "float";
        break;
    }
    case 26:
    {
        return  "int";
        break;
    }
    default:
    {
        return  s;
        break;
    }
    }

}
string coutstr(string str,int flag)//flag��=-1Ϊ���ʽ���λ�� ����ֱ�����
{
    // += 2;
    string strr;
    for (int i = 0; i < str.length(); i++)
    {
        if (flag+2 == i&&flag!=-1)
        {
            cout << "." ;
        }

        coutfh(str[i]);
        if (flag == -1)
            cout << ",";
        //cout << str[i];
    }
    if (str.length() == flag+2&&flag != -1)
    {
        cout << "." ;
    }
    if(flag==-3)
    {
        for (int i = 0; i < str.length(); i++)
        {


            strr+=getfh(str[i]);
        }
    }
    return strr;
}
vector<vector<item> > C;
int numitems=0;//C����ĸ�����ͬʱ����Ϊ�������ĸ����ϵı�ţ���ΪI0��I1��I2...
vector<string> proce;//����ʽ
string wffh;//�ķ�����
int numwffh=0;//�ķ����ŵĸ���
string zjfh;//�ս����
int numzjfh;//�ս���ŵĸ���
//����CLOSURE
vector<item> CLOSURE(vector<item> I)
{
    vector<item> J;
    for (int i = 0; i < I.size(); i++)
    {
        J.push_back(I[i]);
    }
    int flag = 0;
    do
    {
        flag = 0;
        for (int i = 0; i < J.size(); i++) //J�е�ÿ���� i��J����ı��
        {
            for (int j=0; j < proce.size(); j++) //G��ÿ������ʽ j�ǲ���ʽ�ı��
            {
                int k = J[i].id;//��¼J��A->a.Bb���λ��
                string css1 = proce[J[i].num];//��G��ȡ�����ΪJ����ı��ʽ��ŵĲ���ʽ ��ΪA->a.Bb
                string css = proce[j];//ȡ��G�Ĳ���ʽ
                if (css[0] == css1[k+2])//�ж�css�����->����Ƿ����A->a.Bb���B ���� css��ΪB->r
                {
                    int ff = 1;//�������B->.r����J��
                    for (int m=0; m < J.size(); m++)
                    {
                        if (J[m].num == j& J[m].id==1)//Ҫ������Ѿ�����
                        {
                            ff = 0;
                        }
                    }
                    if (ff == 1)
                    {
                        item a;
                        a.id = 1;
                        a.num = j;
                        a.numI = J[i].numI;
                        J.push_back(a);
                        flag = 1;
                    }
                }
            }
        }
    }
    while (flag);
    return J;
}
//����GOTO
vector<item> GOTO(vector<item> I, char X)
{
    vector<item> r;
    vector<item> J;
    for (int i=0; i < I.size(); i++)
    {
        string css = proce[I[i].num];//I[i].num��ʾ����ʽ���
        for (int j=1; j < css.length(); j++)
        {
            if (css[j] == X&&I[i].id+2 == j)//&&css[j + 1] != '\0'
            {
                item m;
                m.id = j + 1-2;
                m.num = I[i].num;
                m.numI= I[i].numI;//����ʽ���ڵ���Ŀ����� Ӧ�øı仯
                J.push_back(m);
            }
        }
    }
    return CLOSURE(J);
}


inline bool itemeq(item a, item b)//���������
{
    if (a.num==b.num&&a.id == b.id)//&&a.numI == b.numI
        return 1;
    return 0;
}
bool isin(item a, vector<item> b)      //��a���b��
{
    for (int i = 0; i<b.size(); i++)
    {
        if (itemeq(a, b[i]))
            return 1;
    }
    return 0;
}
bool itemseq(vector<item> a, vector<item> b)  //������Ŀ���Ƿ����
{
    if (a.size() != b.size())
        return 0;
    for (int i = 0; i<a.size(); i++)
    {
        if (!isin(a[i], b))
            return 0;
    }
    return 1;
}
bool itemsinc(vector<item> a) //�жϸ����Ƿ������
{
    for (int i = 0; i < numitems; i++)
    {
        if (itemseq(a, C[i]))
            return 1;
    }
    return 0;
}

//�����
void items()
{
    vector<item> first;//��ʼ��ֻ����s'->s
    vector<item> I;
    item temp;
    temp.id = 1;
    temp.num = 0;
    temp.numI = 0;
    first.push_back(temp);
    vector<item> TEMP;
    //C[0] = CLOSURE(first);
    TEMP = CLOSURE(first);
    /*for (int ii = 0;ii < TEMP.size();ii++)
    {
    	C[numitems].push_back(TEMP[ii]);
    }*/
    C.push_back(TEMP);
    numitems = 1; //C����ĸ���
    int flag = 0;
    do
    {
        flag = 0;
        for (int i=0; i < numitems; i++) //����c�е�ÿһ���
        {
            I = C[i];
            for (int j=0; j < numwffh; j++) //����ÿһ���ķ�����X
            {
                char X = wffh[j];
                vector<item> r;
                r = GOTO(I, X);
                if (r.size()!=0&& itemsinc(r)==0)//GOTO(I,X)�ǿ��Ҳ���C��
                {
                    for (int m =0; m < r.size(); m++)
                    {
                        r[m].numI = numitems;
                    }
                    //	cout << "=================="<<numitems<<"=========================" << endl;
                    //C[numitems] = r;
                    //	for (int ii = 0;ii < r.size();ii++)
                    //	{
                    //		//C[numitems].push_back(r[ii]);
                    //		cout << r[ii].id << " " << r[ii].num << " " << r[ii].numI << endl;
                    //	}
                    C.push_back(r);
                    ++numitems;
                    flag = 1;
                }
            }
        }
    }
    while (flag);
}

//FIRST
bool isVN(char X)//�Ƿ��Ƿ��ս��
{
    for (int i = 0; i < numzjfh; i++)
    {
        if (X == zjfh[i])
        {
            return 0;
        }
    }
    return 1;
}
bool isIN(string temp)//�Ƿ���ڿ�
{
    for (int i = 0; i < temp.length(); i++)
    {
        if (temp[i] == '@')
        {
            return 1;
        }
    }
    return 0;
}
//����ֵΪ0 ��ʾû�м���  ����ֵΪ1��ʾ������
bool addFirst(string &r, string temp,int flag) //flag��ʾ��ȥ�� ��@��
{
    int f=0;
    int i, j;
    int ret = 0;
    if (flag == 1)
    {
        for ( i = 0; i < temp.length(); i++)
        {
            for ( j = 0; j < r.length(); j++)
            {
                if (r[j] == temp[i])
                {
                    f = 1;
                }
            }
            if (f == 0&&temp[i]!='@')
            {
                r += temp[i];
                ret = 1;
            }
        }
    }
    else if (flag == 0)
    {
        for (i = 0; i < r.length(); i++)
        {
            if (r[i] == '@')
            {
                f = 1;
            }
        }
        if (f == 0)
        {
            r += '@';
            ret = 1;
        }
    }

    return ret;
}
//����ֵΪ0 ��ʾû�м���  ����ֵΪ1��ʾ������
bool add1(string &r, char X)
{
    int flag =1;
    int ret = 0;
    for (int i = 0; i < r.length(); i++)
    {
        if (r[i] == X)
        {
            flag = 0;
        }
    }
    if (flag == 1)
    {
        r += X;
        ret = 1;
    }
    return 1;
}
string wfzjfFIRST(char X)
{

    string r;
    for (int i = 0; i < numzjfh; i++) //�ս���ŵ�first����������
    {
        if (X == zjfh[i])
        {
            r = X;
            return r;
        }
    }
    int flag2 = 0;
    for (int i = 0; i < proce.size(); i++)
    {
        string css = proce[i];

        if (css[0] == X)
        {
            if (isVN(css[3]) == 0&&css[3]!='@')
            {
                add1(r, css[3]);
            }
            if (css[3] == '@')
            {
                flag2 = 1;
            }
        }
    }
    if (flag2 == 1)
    {
        r += "@";
    }

    int flag;
    do
    {
        flag = 0;
        for (int i = 0; i < proce.size(); i++)
        {

            string css = proce[i];
            if (css[3] != X)  //zuodigui
            {
                if (css[0] == X &&isVN(css[3]) && css[3] != '@')
                {
                    int aa = 0;
                    string temp = wfzjfFIRST(css[3]);
                    aa = addFirst(r, temp, 1);
                    if (aa == 1)
                    {
                        flag = 1;
                    }
                }
            }
        }
        for (int i = 0; i < proce.size(); i++)
        {
            string css = proce[i];
            if (css[0] == X&&css[3]!='@') //X->Y1Y2...YN
            {
                int k;

                for (k = 3; k < css.length(); k++)
                {
                    if (css[3] == X)//zuodigui
                    {
                        break;
                    }
                    string temp = wfzjfFIRST(css[k]);
                    if (isIN(temp))
                    {
                        int aa=0;
                        aa=addFirst(r, temp, 1);
                        if (aa == 1)
                        {
                            flag = 1;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                if (k == css.length())
                {
                    string temp = "@";
                    int aa = 0;
                    aa=addFirst(r, temp, 0);
                    if (aa == 1)
                    {
                        flag = 1;
                    }
                }
            }
        }
    }
    while (flag);
    //cout << X << ":" << r << endl;
    return r;
}
string FIRST(string a)
{
    string r;
    string temp = wfzjfFIRST(a[0]);
    addFirst(r, temp, 1); //flag��ʾ��ȥ�� ��@��
    int k = 1;
    while (isIN(wfzjfFIRST(a[k - 1])) && k < a.length())
    {
        temp = wfzjfFIRST(a[k]);
        addFirst(r, temp, 1); //flag��ʾ��ȥ�� ��@��
        k++;
    }
    if (k == a.length() && isIN(wfzjfFIRST(a[k - 1])))
    {
        r += '@';
    }
    return r;
}
//FOLLOW
int getnum(char X)
{
    int i = 0;
    int num = -1;
    for (i = 0; i < numwffh; i++)
    {
        if(X==wffh[i])
        {
            num = i;
            break;
        }
    }
    if (X == '$')
    {
        num = numwffh;
    }
    return num;
}
struct
{
    int num;
    char a;
    int f;
    string follow;
} follows[100];
bool addFOLLOW(string &r, string temp)
{
    int ret = 0;
    for (int i = 0; i < temp.length(); i++)
    {
        int f = 0;
        for (int j = 0; j < r.length(); j++)
        {
            if (r[j] == temp[i])
            {
                f = 1;
            }
        }
        if (f == 0 )
        {
            r += temp[i];
            ret = 1;
        }
    }
    return ret;
}
void FOLLOW()
{
    int kkk=0;
    for (int i = 0; i < numwffh; i++)
    {
        follows[i].num = i;
        follows[i].a = wffh[i];
        follows[i].f = 1;
        follows[i].follow = "";
    }
    for (int i = 0; i < numwffh; i++)
    {
        for (int j = 0; j < numzjfh; j++)
        {
            if (follows[i].a == zjfh[j])
            {
                follows[i].f = 0;
            }
        }
    }
    follows[0].follow += '$';
    int flag = 0;
    do
    {
        flag = 0;
        for (int i = 0; i < proce.size(); i++)
        {
            string css = proce[i];
            if (css[3] != '@')
            {
                for (int j = 2; j < css.length(); j++)
                {
                    if (j != css.length() - 1&&css[j+2]!='\0')
                    {
                        if (isVN(css[j + 1]))
                        {
                            string temp = css.substr(j+2);
                            int aa=0;
                            aa=addFirst(follows[getnum(css[j + 1])].follow, FIRST(temp), 1);
                            if (aa == 1)
                            {
                                flag = 1;//cout << flag << endl;
                            }
                        }
                    }
                }
                for (int j = 2; j < css.length(); j++)
                {
                    if (j != css.length() - 1)
                    {
                        if (isVN(css[j + 1]))
                        {
                            string temp = css.substr(j+2);
                            if (j == css.length() - 2 || ((j < css.length() - 1) && isIN(FIRST(temp))))
                            {
                                int aa = 0;
                                aa = addFOLLOW(follows[getnum(css[j + 1])].follow, follows[getnum(css[0])].follow);
                                if (aa == 1)
                                {
                                    flag = 1;//cout << flag << endl;
                                }
                            }

                        }
                    }
                }
            }
        }
        //cout << "==========" << ++kkk << "===================" << endl;
        //for (int i = 0;i < numwffh;i++)
        //{
        //	cout << follows[i].num << " " << follows[i].a << " " << follows[i].f << " " << follows[i].follow << endl;;
        //}
    }
    while (flag);
}

//========================================
//action����
typedef struct
{
    int rs;//r��ʾ��Լ1 s��ʾ����0 -1��ʾû�ж��� 2��ʾgoto 3��ʾacc
    int zt;//��ʾ״̬ -1��ʾû��
} actionn;
actionn **action;
//�ҵ����I��ȵ�J
int findJeqI(vector<item> I)
{
    vector<item> J;
    for (int i = 0; i < C.size(); i++)
    {
        if (itemseq(I, C[i]))
        {
            return i;
        }
    }
    return -1;
}
void getACTION()
{
    //��̬����ռ�
    action = new actionn*[C.size()]; //������
    for (int i = 0; i < C.size(); i++)
        action[i] = new actionn[numwffh+1]; //������
    //��ʼ��
    for (int i = 0; i < numwffh+1; i++)
    {
        for (int j = 0; j < C.size(); j++)
        {
            action[j][i].rs = -1;
            action[j][i].zt = -1;
        }
    }

    int i=0;
    for (i = 0; i < C.size(); i++)
    {
        vector<item> I;
        I = C[i];
        //if (i == 21)
        //{
        //	cout <<"hello"<< endl;
        //}
        for (int j = 0; j < I.size(); j++) //����Ii�е�ÿһ����
        {
            item t;
            t = I[j];
            string css = proce[t.num];
            if (!isVN(css[t.id + 2]) && css[t.id + 2] != '\0')
            {
                int aa;
                aa = findJeqI(GOTO(I, css[t.id + 2]));
                if (-1!=aa)
                {
                    action[i][getnum(css[t.id+2])].rs = 0;
                    action[i][getnum(css[t.id+2])].zt = aa;
                }
            }
            if (isVN(css[t.id + 2])&& css[t.id + 2] != '\0')
            {
                int aa;
                aa = findJeqI(GOTO(I, css[t.id + 2]));
                if (-1 != aa)
                {
                    action[i][getnum(css[t.id + 2])].rs = 2;
                    action[i][getnum(css[t.id + 2])].zt = aa;
                }
            }
            if (css[t.id + 2] == '\0')
            {
                //for (int k = 0;k < numwffh;k++)
                //{
                //	if (!isVN(wffh[k]))
                //	{
                //		action[i][getnum(wffh[k])].rs = 1;
                //		action[i][getnum(wffh[k])].zt = t.num;
                //	}
                //}
                //action[i][numwffh].rs = 1;
                //action[i][numwffh].zt = t.num;
                string sf = follows[getnum(css[0])].follow;
                sf += '$';
                for(int m=0; m<sf.length(); m++)
                {
                    action[i][getnum(sf[m])].rs = 1;
                    action[i][getnum(sf[m])].zt = t.num;
                }
            }
            if (t.num == 0 && t.id == 2)
            {
                action[i][numwffh].rs = 3;
                action[i][numwffh].zt = t.num;
            }

        }
    }
}
void printfitems()
{
    cout << "====================���=================";
    vector<item> I;
    for (int i = 0; i < C.size(); i++)
    {
        cout << "I" << i << ":";
        I = C[i];
        for (int j = 0; j < I.size(); j++)
        {
            //string p = proce[I[j].num];
            //p += ' ';
            //for (int k = 0;k < p.length();k++)
            //{
            //	if (k == I[j].id+2)cout << ".";
            //	cout << p[k];
            //}
            coutstr(proce[I[j].num], I[j].id);
            cout << " ";
        }
        cout << endl;
    }
}
void printfstack(stack<int> zts, stack<char> fhs)
{
    stack<int> t1;
    stack<char> t2;
    int c = zts.size();
    int d = fhs.size();
    for (int i = 0; i < c; i++)
    {
        int a = zts.top();
        t1.push(a);
        zts.pop();
    }
    for (int i = 0; i < d; i++)
    {
        char a = fhs.top();
        t2.push(a);
        fhs.pop();
    }
    for (int i = 0; i < c; i++)
    {
        int a = t1.top();
        cout<<a<<" ";
        zts.push(a);
        t1.pop();
    }
    cout << "\t";
    for (int i = 0; i < d; i++)
    {
        char a = t2.top();
        coutfh(a);
        fhs.push(a);
        t2.pop();
    }
    cout << "\t";

}
//�﷨������
//vector<string> proceout;
void Parsar()
{
    cout << "����﷨�������̣���һ��Ϊ��ǰ���뵥�ʣ��ڶ���Ϊ״̬ջ�е����ݣ�������Ϊ����ջ�е����ݣ�������Ϊ��Լ����ʽ" << endl;
    char a;
    int s;
    Token t;
    if((!isend())&&(t.tag!=ERROR))
    {
        t= getToken();
        a = t.tag+'a';
        if (t.tag != ERROR)
        {
            tokens.push_back(t);
        }
    }
    else
    {
        a = '$';
    }

    stack<int> zts;//״̬ջ
    stack<char> fhs;//����ջ
    int pos = 1;//ָ��ǰ����
    zts.push(0);
    fhs.push('$');
    while (1)
    {
        //cout << endl;
        cout<<a<<"  ";
        printfstack(zts, fhs);

        s = zts.top();//sΪջ����״̬

        //cout<< "   "<<action[s][getnum(a)].rs<<"  ";
        if (action[s][getnum(a)].rs == 0) //action[s,a]=����t
        {
            cout << "   ����" << action[s][getnum(a)].zt<< "  ";
            zts.push(action[s][getnum(a)].zt);//t��ջ
            fhs.push(a);
            // = str[pos];//aΪ��һ������ķ���
            //pos++;
            Token t;
            if((!isend())&&(t.tag!=ERROR))
            {
                t= getToken();
                a = t.tag+'a';
                if (t.tag != ERROR)
                {
                    tokens.push_back(t);
                }
            }
            else
            {
                a = '$';
            }
            cout << endl;
            //cout << a << "  ";
            //cout << endl;
        }
        else if (action[s][getnum(a)].rs == 1)//action[s,a]=��Լ
        {
            cout << "   ��Լ" << action[s][getnum(a)].zt << "  ";
            string css = proce[action[s][getnum(a)].zt];//A->B
            string temp = css.substr(3);
            for (int i = 0; i < temp.size(); i++)
            {
                fhs.pop();//��ջ�е���|B|������
                zts.pop();
            }

            int t = zts.top();//tΪ��ǰ��ջ��״̬
            zts.push(action[t][getnum(css[0])].zt);
            //zts.pop();

            fhs.push(css[0]);
            //cout << css << endl;
            string strr=coutstr(css, -3);
            proceout.push_back(strr);
            cout << endl;
        }
        else if (action[s][getnum(a)].rs == 3)
        {
            cout << "acc" << endl;
            break;
        }
        else
        {
            cout << "error" << endl;
            break;
        }
    }
    writefile();
}
struct fuhao
{
    int kind;
    string fh;
};


void getproce()
{
    string t;
    t = "B->P";
    proce.push_back(t);
    t = "P->DS";
    proce.push_back(t);
    t = "D->LcmD";
    proce.push_back(t);//c��ʾID m��ʾ��
    t = "D->";
    proce.push_back(t);//@��ʾ��
    t = "L->z";
    proce.push_back(t);//b��ʾint
    t = "L->y";
    proce.push_back(t);//y��ʾfloat
    t = "S->cjEm";
    proce.push_back(t);//j��ʾ=
    t = "S->skClS";
    proce.push_back(t);//s��ʾif
    t = "S->skClSuS";
    proce.push_back(t);
    t = "S->vkClS";
    proce.push_back(t);
    t = "S->SS";
    proce.push_back(t);
    t = "C->EhE";
    proce.push_back(t);
    t = "C->EiE";
    proce.push_back(t);
    t = "C->EoE";
    proce.push_back(t);
    t = "E->EdT";
    proce.push_back(t);
    t = "E->EeT";
    proce.push_back(t);
    t = "E->T";
    proce.push_back(t);
    t = "T->F";
    proce.push_back(t);
    t = "T->TfF";
    proce.push_back(t);
    t = "T->TgF";
    proce.push_back(t);
    t = "F->kEl";
    proce.push_back(t);
    t = "F->c";
    proce.push_back(t);
    t = "F->b";
    proce.push_back(t);
    wffh = "BPDSLCETFbcdefghijklmnosuvyz@";
    numwffh = 29;


    zjfh="bcdefghijklmnosuvyz@";//�ս����
    numzjfh=20;//�ս���ŵĸ���



}
void printfff()//���first��follow
{
    cout << "==================FIRST����=============" << endl;
    for (int i = 0; i < wffh.length(); i++)
    {
        coutfh(wffh[i]);
        cout << ": ";
        string a = wfzjfFIRST(wffh[i]);
        coutstr(a, -1);
        cout << endl;
    }
    cout << "==================FOLLOW����=============" << endl;
    for (int i = 0; i < numwffh - numzjfh; i++)
    {
        coutfh(follows[i].a);
        cout << ":";
        coutstr(follows[i].follow, -1);
        cout << endl;
    }
}
void printfaction()
{
    cout << "=============================������======================================" << endl;
    cout << "������Ļ��С����������ʾ��ǰ��Ϊgoto������Ϊaction��r��ʾ��Լ��s��ʾ���룬g��ʾgoto" << endl;
    //cout << numwffh << "   " << C.size() << endl;
    //cout << "wffh:";
    //cout << "\t";
    for (int i = 0; i < numwffh; i++)
    {
        //if (wffh[i] >= 'a'&&wffh[i] <= 'z')
        //{
        coutfh(wffh[i]);
        cout << "\t";
        //}
        //else cout << wffh[i] << "\t";
    }
    cout << "$";
    cout << endl;
    for (int j = 0; j < C.size(); j++)
    {
        cout << "I" << j << ":" << endl;;
        for (int i = 0; i < numwffh + 1; i++)
        {
            char rs;
            switch (action[j][i].rs)
            {
            case -1:
                rs = ' ';
                break;
            case 0:
                rs = 's';
                break;
            case 1:
                rs = 'r';
                break;
            case 2:
                rs = 'g';
                break;
            case 3:
                rs = 'a';
                break;
            }
            if (action[j][i].rs == -1 || action[j][i].rs == 3)
            {
                cout << rs << " \t";
            }
            else
            {
                cout << rs << action[j][i].zt << "\t";
            }
        }
        cout << endl;
    }
}
int main()
{
    str = readfile();//��ȡҪ����������
    cout << str << endl;
    getproce();

    FOLLOW();
    printfff();

    items();
    printfitems();

    getACTION();
    printfaction();

    cout << "������" << endl;
    cout << str << endl;

    Parsar();
    cout << "���������ַ��˳�" << endl;
    getchar();
    return 0;
}
