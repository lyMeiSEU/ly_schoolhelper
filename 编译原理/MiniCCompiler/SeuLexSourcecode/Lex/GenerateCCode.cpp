#include"stdafx.h"
#include"structs.h"

using namespace std;

/*打印数组，name为数组名，size为数组大小，value为数组值，out为写入的文件流*/
static void print_array(string name, int size, const int *value, ofstream& out);
/*pl(),单行输出函数*/



/*生成.c文件,arrays为包含多个相关数组的容器，endVec为终态对应的动作*/
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
	/*首先判断size的大小是否为4*/
	if (arrays.size() != 4)
	{
		return -1;
	}
	/*主函数的开始*/
	out << "#define _CRT_SECURE_NO_WARNINGS" << endl; 
	out << "#include\"stdio.h\"" << endl;
	out << "#include\"stdlib.h\"" << endl;

	out << "#include<string.h>" << endl;
	out << "#define START_STATE " << startState << endl;
	/*依次输出P1和P4*/
	for (int i = 0; i < part1.size(); i++)
	{
		out << part1[i] << endl;
	}

	out << "char* getCharPtr(char* fileName);" << endl;
	out << "int findAction(int action);" << endl; /*函数声明*/
	//out << "void addToken(int token);" << endl; /*将token加入Token序列*/
	out << "void comment();" << endl; /*comment函数，啥都不做*/
	


	/*依次输出ec表,base表,next表,accept表*/
	vector<string> array_name;
	array_name.push_back(string("yy_ec"));
	array_name.push_back(string("yy_base"));
	array_name.push_back(string("yy_next"));
	array_name.push_back(string("yy_accept"));

	for (int i = 0; i < 4; i++)
	{
		print_array(array_name[i], arrays[i].second, arrays[i].first, out);

	}

	/*定义若干变量*/
	out << " int yy_current_state = START_STATE;" << endl;
	out << " int yy_last_accepting_state = -1;" << endl;
	out << " char *yy_cp = NULL;" << endl;
	out << " char *yy_last_accepting_cpos = NULL;" << endl;
	out << " int yy_act = 0;" << endl;
	out << " int isEnd=0;" << endl;
	out << " int yy_c=-1;" << endl;
	out << "int correct=1;" << endl;


	/*一些初始化动作*/
	out << "void lex_init(char* fileName)" << endl;
	out << "{" << endl;
	/*调用char* getCharPtr(char* fileName)得到文件字符指针*/
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
	out << "}" << endl;/*lex_mian函数结束*/
  
	/*int findAction(int action)函数*/
	out << "int findAction(int action)" << endl;
	out << "{" << endl;
	out << "switch (action) " << endl;/*根据endVec打印switch语句*/
	out << "{" << endl;
	out << "case 0:" << endl;
	/*...此处省略了一些东西*/
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
	out << "}" << endl; /*int findAction(int state）函数的下括号*/
	out << "return -1;" << endl;
	out << "}" << endl;/*int findAction(int action)函数的下括号*/
	/*char* getCharPtr(char* fileName)函数*/
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
	out << "int flen = ftell(fp);" << endl; /* 得到文件大小 */
	out << "cp = (char *)malloc(flen + 1);" << endl; /* 根据文件大小动态分配内存空间 */
	out << "if (cp == NULL)" << endl;
	out << "{" << endl;
	out << "fclose(fp);" << endl;
	out <<"	return 0;"<<endl;
	out << "}" << endl;
	out << "rewind(fp);" << endl; /* 定位到文件开头 */
	out << "memset(cp,0,flen+1);" << endl;
	out << "fread(cp, sizeof(char), flen, fp);" << endl; /* 一次性读取全部文件内容 */
	out << "cp[flen] = 0; " << endl;/* 字符串结束标志 */
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
