#include"stdafx.h"
#include"structs.h"

using namespace std;

/*��ӡ���飬nameΪ��������sizeΪ�����С��valueΪ����ֵ��outΪд����ļ���*/
static void print_array(string name, int size, const int *value, ofstream& out);
/*pl(),�����������*/



/*����.c�ļ�,arraysΪ���������������������endVecΪ��̬��Ӧ�Ķ���*/
int generate_c_code(vector<pair<int*, int>>& arrays, vector<Rules>& endVec, vector<string>& part1, vector<string>& part4,int startState,int mode)
{

	ofstream out;
	if (mode == 0)
	{
		out.open("lex.c", ios::out);
	}
	if (mode == 1)
	{
		out.open("lex.c", ios::out);
	}
	/*�����ж�size�Ĵ�С�Ƿ�Ϊ4*/
	if (arrays.size() != 4)
	{
		return -1;
	}
	/*�������Ŀ�ʼ*/
	out << "#define _CRT_SECURE_NO_WARNINGS" << endl; 
	out << "#include\"stdio.h\"" << endl;
	out << "#include\"stdlib.h\"" << endl;

	out << "#include<string.h>" << endl;
	out << "#define START_STATE " << startState << endl;
	/*�������P1��P4*/
	for (int i = 0; i < part1.size(); i++)
	{
		out << part1[i] << endl;
	}

	out << "char* getCharPtr(char* fileName);" << endl;
	out << "int findAction(int action);" << endl; /*��������*/
	//out << "void addToken(int token);" << endl; /*��token����Token����*/
	out << "void comment();" << endl; /*comment������ɶ������*/
	


	/*�������ec��,base��,next��,accept��*/
	vector<string> array_name;
	array_name.push_back(string("yy_ec"));
	array_name.push_back(string("yy_base"));
	array_name.push_back(string("yy_next"));
	array_name.push_back(string("yy_accept"));

	for (int i = 0; i < 4; i++)
	{
		print_array(array_name[i], arrays[i].second, arrays[i].first, out);

	}

	/*�������ɱ���*/
	out << " int yy_current_state = START_STATE;" << endl;
	out << " int yy_last_accepting_state = -1;" << endl;
	out << " char *yy_cp = NULL;" << endl;
	out << " char *yy_last_accepting_cpos = NULL;" << endl;
	out << " int yy_act = 0;" << endl;
	out << " int isEnd=0;" << endl;
	out << " int yy_c=-1;" << endl;
	out << "int correct=1;" << endl;


	/*һЩ��ʼ������*/
	out << "void lex_init(char* fileName)" << endl;
	out << "{" << endl;
	/*����char* getCharPtr(char* fileName)�õ��ļ��ַ�ָ��*/
	out << "yy_cp=getCharPtr(fileName);" << endl;
	out << "}" << endl;

	if (mode == YACC_TEST)
	{
		out << "int yy_lex()" << endl;
		out << "{" << endl;
	}
	if (mode == LEX_TEST)
	{
		out << "int main(int argc,char** argv)" << endl;
		out << "{" << endl;
		out << "if(argc==2)" << endl;
		out << "{" << endl;
		out << "lex_init(argv[1]);" << endl;
		out << "}" << endl;
		out << "else{" << endl;
		out << "printf(\"ERROR: invalid argument!\\n\");" << endl;
		out << "return -1;" << endl;
		out << "}" << endl;
	}

	

	out << "if (isEnd&&correct)" << endl;
	out << "{" << endl;
	out << "	return -1;" << endl;
	out << "}" << endl;
	out << "else if (isEnd && !correct)" << endl;
	out << "{" << endl;
	out << "	return -2;" << endl;
	out << "}" << endl;
	out << "int result = 0;" << endl;
	out << "while (*yy_cp != 0) {" << endl;
	out << "	yy_c = yy_ec[(int)*yy_cp];" << endl;
	out << "	if (yy_accept[yy_current_state])" << endl;
	out << "	{" << endl;
	out << "	yy_last_accepting_state = yy_current_state;" << endl;
	out << "	yy_last_accepting_cpos = yy_cp;" << endl;
	out << "	}" << endl;
	out << "	if (yy_next[yy_base[yy_current_state] + yy_c] == -1 && yy_last_accepting_state != -1)" << endl;
	out << "	{" << endl;
	out << "	yy_current_state = yy_last_accepting_state;" << endl;
	out << "	yy_cp = yy_last_accepting_cpos;" << endl;
	out << "	yy_act = yy_accept[yy_current_state];" << endl;
	out << "	result = findAction(yy_act);" << endl;
	if (mode == YACC_TEST)
	{
	out << "	if (result != -1)" << endl;
	out << "	{" << endl;
	out << "	yy_current_state = START_STATE;" << endl;
	out << "	yy_last_accepting_state = -1;" << endl;
	out << "	++yy_cp;" << endl;
	out << "	yy_current_state = yy_next[yy_base[yy_current_state] + yy_c];" << endl;
	out << "	break;" << endl;
	out << "	}" << endl;
	out << "	if (result == -1)" << endl;
	out << "	{" << endl;
	out << "	yy_current_state = START_STATE;" << endl;
	out << "	yy_last_accepting_state = -1;" << endl;
	out << "	++yy_cp;" << endl;
	out << "	yy_current_state = yy_next[yy_base[yy_current_state] + yy_c];" << endl;
	out << "	continue;" << endl;
	out << "	}" << endl;
	}
	else if (mode == LEX_TEST)
	{	
	out<<"		printf(\" \");"<<endl;
	out << "	yy_current_state = START_STATE;" << endl;
	out << "	yy_last_accepting_state = -1;" << endl;
	out << "	++yy_cp;" << endl;
	out << "	yy_current_state = yy_next[yy_base[yy_current_state] + yy_c];" << endl;
	out << "	continue;" << endl;

	}

	out << "	}" << endl;
	out << "	if (yy_next[yy_base[yy_current_state] + yy_c] == -1 && yy_last_accepting_state == -1)" << endl;
	out << "{" << endl;
	out << "	printf(\"ERROR DETECTED IN INPUT FILE !\");" << endl;
	if (mode == LEX_TEST)
	{
		out << "	return -1;" << endl;
	}
	out << "	}" << endl;
	out << "if (yy_next[yy_base[yy_current_state] + yy_c] != -1) " << endl;
	out << "{" << endl;
	out << "		yy_current_state = yy_next[yy_base[yy_current_state] + yy_c];" << endl;
	out << "	++yy_cp;" << endl;
	out << "	}" << endl;
	out << "}" << endl;
	out << "if (*yy_cp == 0) {" << endl;
	out << "isEnd = 1;" << endl;
	out << "	if (yy_accept[yy_current_state] && yy_cp == yy_last_accepting_cpos + 1)" << endl;
	out << "{" << endl;
	out << "	yy_act = yy_accept[yy_current_state];" << endl;
	out << "	result = findAction(yy_act);" << endl;
	out << "}" << endl;

	out << "else " << endl;
	out << "{" << endl;
	out << "	printf(\"ERROR DETECTED IN INPUT FILE !\");" << endl;
	out << "	correct = 0;" << endl;
	if (mode == LEX_TEST)
	{
		out << "	return -1;" << endl;
	}
	out << "}" << endl;

	out << "}" << endl;
	if (mode == LEX_TEST)
	{
		out << "	return 0;" << endl;
	}
	else {
		out << "	return result;" << endl;
	}
	out << "}" << endl;/*lex_mian��������*/
  
	/*int findAction(int action)����*/
	out << "int findAction(int action)" << endl;
	out << "{" << endl;
	out << "switch (action) " << endl;/*����endVec��ӡswitch���*/
	out << "{" << endl;
	out << "case 0:" << endl;
	/*...�˴�ʡ����һЩ����*/
	out << "break;" << endl;
	for (int i = 0; i < endVec.size(); i++)
	{
		out << "case " << i + 1 << ":" << endl;
		for (int j = 0; j < endVec[i].actions.size(); j++)
		{
			out << endVec[i].actions[j] << endl;
		}
		out << "break;" << endl;

	}
	out << "default:" << endl;
	out << "break;" << endl;
	out << "}" << endl; /*int findAction(int state��������������*/
	out << "return -1;" << endl;
	out << "}" << endl;/*int findAction(int action)������������*/
	/*char* getCharPtr(char* fileName)����*/
	out << "char* getCharPtr(char* fileName){" << endl;
	out << "char* cp=NULL;" << endl;
	out << "FILE *fp;" << endl;
	out << "fp=fopen(fileName,\"r\");" << endl;
	out << "if(fp==NULL)" << endl;
	out << "{" << endl;
	out << "printf(\"can't open file\");" << endl;
	out << "exit(0);" << endl;
	out << "}" << endl;
	out << "fseek(fp,0,SEEK_END);" << endl;
	out << "int flen = ftell(fp);" << endl; /* �õ��ļ���С */
	out << "cp = (char *)malloc(flen + 1);" << endl; /* �����ļ���С��̬�����ڴ�ռ� */
	out << "if (cp == NULL)" << endl;
	out << "{" << endl;
	out << "fclose(fp);" << endl;
	out <<"	return 0;"<<endl;
	out << "}" << endl;
	out << "rewind(fp);" << endl; /* ��λ���ļ���ͷ */
	out << "memset(cp,0,flen+1);" << endl;
	out << "fread(cp, sizeof(char), flen, fp);" << endl; /* һ���Զ�ȡȫ���ļ����� */
	out << "cp[flen] = 0; " << endl;/* �ַ���������־ */
	out << "return cp;" << endl;
	out << "}" << endl;

	
	for (int i = 0; i < part4.size(); i++)
	{
		out << part4[i] << endl;
	}

	out.close();
	return 0;
}
void print_array(string name, int size, const int *value,ofstream& out)
{

	const int* array_buf = value;
	out << "static int	" <<name<<"[" << size << "]" << " =" << endl;
	out << "	{	0," << endl;
	for (int i = 1; i < size; i++)
	{
		
		out << setw(4) << left << array_buf[i];
		if (i != size - 1)
		{
			out << ",";
		}
		if (i % 10 == 0)
		{
			out << endl;			
		}
	}
	out << "};" << endl;
}
