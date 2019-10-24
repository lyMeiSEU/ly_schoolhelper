#include "stdafx.h"
#include"structs.h"

using namespace std;


static vector<string> split(const string& str, const string& delim);
static string& trim(string &s);
static void remove_comment(string& s);


//��lex.l�ļ�����
int read_and_parse_lex(string filePath,map<string,string>& m,vector<Rules>& vRules,vector<string>&P1,vector<string>&P4)
{
	//�����ļ�
	ifstream in;
	in.open(filePath.c_str(), ios::in);
	if (!in)
	{
		cout << "Open lex.l failed!" << endl;
		return 1;
	}
	string line;	//�����洢ÿ�е��ַ���
	enum State{beforePart1,Part1,Part2,Part3,Part4};	//����ָʾ��ǰ��дָ����������
	State st = beforePart1;
	vector<string> vBuf;	//�����ݴ��ַ����ָ�Ľ����
	int lineCount=0;	//�����кţ����ڴ�����ʾ
	bool hasError=false;	//��ʶ�Ƿ��д����д���������ֹ����
	bool findUpperPar=false;	//�Ƿ���������
	bool findUpperMarks = false; //�Ƿ���������
	bool findUPPerBrackets = false;//�Ƿ�����������
	string lhd;	//�洢��ߵ��ַ���
	string rhd;	//�洢�ұߵ��ַ���
	vector<string>action;//�洢����
	while (!in.eof()&&!hasError)
	{
		
		getline(in,line);
		++lineCount;
		
		//��ĳ��Ϊ�գ�ֱ������
		if (line.empty())
		{
			continue; 
		}

		//����״ִ̬����Ӧ����
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
				//ȥ����βע��
			//	line = split(line, "/*").at(0);
				//�ָ��������
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
			//��P3���֣���Ҫע��հ��ַ�
			else
			{
				
				//ȥ����βע��
				//removeComment(line);
				//ȥ����β�ո�
				trim(line);

				//�������{}֮���Ҵ��в�Ϊ}
				if (findUpperPar && line.compare("}")!=0)
				{
					action.push_back(line);
					findUpperPar = true;
					break;
				}
				//����ǰ��Ϊ}
				if (findUpperPar && line.compare("}")==0)
				{
					
					vRules.push_back(Rules{ lhd, action });
					action.clear();
					findUpperPar = false;
					break;

				}


				//��������{}֮��
				if (!findUpperPar)
				{
					
					//��[��"��ͷ�������⴦��
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


//�ַ����ָ��
vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if ("" == str) return res;
	//�Ƚ�Ҫ�и���ַ�����string����ת��Ϊchar*����  
	char * strs = new char[str.length() + 1]; //��Ҫ����  
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d,delim.c_str());

	char *p = strtok(strs, d);
	while (p) {
		string s = p; //�ָ�õ����ַ���ת��Ϊstring����  
		res.push_back(s); //����������  
		p = strtok(NULL, d);
	}
	delete strs;
	delete d;
	
	return res;
}

//ȥ���ַ�������β�ո�
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
//ȥ��ע��
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