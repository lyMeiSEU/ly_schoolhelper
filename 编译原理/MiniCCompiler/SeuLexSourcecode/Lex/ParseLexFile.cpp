#include "stdafx.h"
#include"structs.h"

using namespace std;


static vector<string> split(const string& str, const string& delim);
static string& trim(string &s);
static void remove_comment(string& s);


//对lex.l文件解析
int read_and_parse_lex(string filePath,map<string,string>& m,vector<Rules>& vRules,vector<string>&P1,vector<string>&P4)
{
	//读入文件
	ifstream in;
	in.open(filePath.c_str(), ios::in);
	if (!in)
	{
		cout << "Open lex.l failed!" << endl;
		return 1;
	}
	string line;	//用来存储每行的字符串
	enum State{beforePart1,Part1,Part2,Part3,Part4};	//用来指示当前读写指针所处部分
	State st = beforePart1;
	vector<string> vBuf;	//用来暂存字符串分割的结果；
	int lineCount=0;	//保存行号，用于错误提示
	bool hasError=false;	//标识是否有错误，有错误立即终止解析
	bool findUpperPar=false;	//是否发现上括弧
	bool findUpperMarks = false; //是否发现上引号
	bool findUPPerBrackets = false;//是否发现上中括号
	string lhd;	//存储左边的字符串
	string rhd;	//存储右边的字符串
	vector<string>action;//存储动作
	while (!in.eof()&&!hasError)
	{
		
		getline(in,line);
		++lineCount;
		
		//若某行为空，直接跳过
		if (line.empty())
		{
			continue; 
		}

		//根据状态执行相应操作
		switch (st)
		{
		case beforePart1:
			if (line.compare("%{")==0)
			{
				st = Part1;
			}
			else
			{
				cout << "ERROR: No entry sign %{ " << endl;
				hasError = true;
			}
			break;
		case Part1:
			if (line.compare("%}")==0)
			{
				st = Part2;

			}
			else
			{
				P1.push_back(line);
			}
			break;
		
		case Part2:
			if (line.compare("%%")==0)
			{
				st = Part3;
			}
			else
			{
				//去掉句尾注释
			//	line = split(line, "/*").at(0);
				//分割成两部分
				vBuf = split(line, " ");
				if (vBuf.size() == 1)
				{
					vBuf = split(line, "\t");
				}
				if (vBuf.size() == 2)
				{
					m.insert(pair<string, string>(vBuf[0], vBuf[1]));

				}

			}
			break;




		case Part3:
			if (line.compare("%%")==0)
			{
				st = Part4;
			}
			//在P3部分，需要注意空白字符
			else
			{
				
				//去掉句尾注释
				//removeComment(line);
				//去掉首尾空格
				trim(line);

				//如果处在{}之间且此行不为}
				if (findUpperPar && line.compare("}")!=0)
				{
					action.push_back(line);
					findUpperPar = true;
					break;
				}
				//若当前行为}
				if (findUpperPar && line.compare("}")==0)
				{
					
					vRules.push_back(Rules{ lhd, action });
					action.clear();
					findUpperPar = false;
					break;

				}


				//若不处于{}之间
				if (!findUpperPar)
				{
					
					//对[和"开头的作特殊处理
					if (line[0] == '[')
					{
						findUPPerBrackets = true;
						int i = 1;
						for (i = 1; i < line.size(); i++)
						{
							if (line[i] == ']')
							{
								findUPPerBrackets=!findUPPerBrackets;
							}
							if ((line[i + 1]  == ' '|| line[i + 1] == '\t') &&findUPPerBrackets == false)
							{
								break;
							}

						}
						lhd = line.substr(0, i+1);
						rhd = line.substr(i+1);
						trim(rhd);



					}
					if (line[0] == '\"')
					{
						findUpperMarks = true;
						int i = 1;
						for (i = 1; i < line.size(); i++)
						{
							if (line[i] == '\"')
							{
								findUpperMarks=!findUpperMarks;
							}

							if ((line[i+1] == ' '||line[i+1]=='\t')&&findUpperMarks == false)
							{
								break;
							}

						}
						lhd = line.substr(0, i+1);
						rhd = line.substr(i + 1);
						trim(rhd);

					}
					if (line[0] != '\''&&line[0] != '[')
					{
						int i = 0;
						for (i = 0; i < line.size(); i++)
						{
							if (line[i] == ' '||line[i]=='\t')
								break;
						}

						lhd = line.substr(0, i );
						rhd = line.substr(i, line.size());
						trim(rhd);

					}

					if (rhd.compare("{")!=0)
					{
						action.push_back(rhd);
						vRules.push_back(Rules{ lhd, action });
						action.clear();
					}
					if (rhd.compare("{")==0)
					{
						
						findUpperPar = true;
					}


				}

			}
			break;

		case Part4:
			P4.push_back(line);
			break;

		}
		
	}
	in.close();
	return 0;
}


//字符串分割函数
vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if ("" == str) return res;
	//先将要切割的字符串从string类型转换为char*类型  
	char * strs = new char[str.length() + 1]; //不要忘了  
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d,delim.c_str());

	char *p = strtok(strs, d);
	while (p) {
		string s = p; //分割得到的字符串转换为string类型  
		res.push_back(s); //存入结果数组  
		p = strtok(NULL, d);
	}
	delete strs;
	delete d;
	
	return res;
}

//去除字符串的首尾空格
string& trim(string &s)
{
	if (s.empty())
	{
		return s;
	}
	s.erase(0, s.find_first_not_of(' '));
	s.erase(0, s.find_first_not_of('\t'));
	s.erase(s.find_last_not_of(' ') + 1);
	s.erase(s.find_last_not_of('\t') + 1);
	return s;
}
//去除注释
void remove_comment(string& s)
{
	
	char* p = new char[100];
	strcpy(p,s.c_str());
	char* p1=strstr(p, "/*");

	if (p1 == NULL)return;
	int k = p1 - p;
	s = s.substr(0, k);
	delete p;
	
	
}