#include"stdafx.h"
#include"structs.h"
#include<fstream>
#include<iomanip>

using namespace std;

extern vector<pair<void*, int> > table_vec;
extern vector<string> productions_vec;
extern map<string, int> tokensDefineMap;
extern int boundTInt;
extern vector<string> funcVec;
extern LRDFA lalrdfa;

void print_array(string name, int size, void *value, ofstream& out);
void generate_files() {
	/*第一部分y.tab.h*/
	ofstream out;
	out.open("y.tab.h", ios::out);
	out << "#ifndef Y_TAB_H" << endl;
	out << "#define Y_TAB_H" << endl;

	for (const auto &p:tokensDefineMap)
	{

		out << "#define  " << p.first <<"    "<< p.second << endl;

	}
	
	out << "#endif" << endl;
	out.close();



	/*第二部分y.tab.c*/
	
	out.open("y.tab.c", ios::out);

	if (!out)
	{
		cout << "ERROR: can't open file y.tab.c !" << endl;
	}
	out << "#include <stdio.h>" << endl;
	out << "#include <stdlib.h>" << endl;
	out << "#include <assert.h>" << endl;

	out << "#define TERMINATED_SYMBOL "<< boundTInt << endl;


	for (const auto &s : funcVec) {
		out << s<<endl;
	}


	/*声明函数*/
	//out << "int getOneToken(int& token);" << endl;
	//out << "int nextToken();" << endl;
	//out << "int tokenIndex=0;" << endl;/*token数组的索引*/
	//out << "int *tokens=NULL;" << endl;/*token数组*/
	//out << "int tokenLength=0;" << endl; /*token的长度*/
	out << "void lex_init(char* fileName);" << endl;
	out << "int yy_lex();" << endl;/*yy_lex外部函数定义*/
	//out << "void getTokens(unsigned num, int* _tokens);" << endl;


	/*栈定义部分*/
	out << "struct Node" << endl;
	out << "{" << endl;
	out << "	int data;" << endl;
	out << "	struct Node* next;" << endl;
	out << "};" << endl;

	out << "struct Stack" << endl;
	out << "{" << endl;
	out << "struct Node* head;" << endl;
	out << "int size;" << endl;
	out << "};" << endl;

	out << "void StackInit(struct Stack* stack)" << endl;
	out << "{" << endl;
	out << "	stack->head = NULL;" << endl;
	out << "	stack->size = 0;" << endl;
	out << "}" << endl;

	out << "void StackPush(struct Stack* stack, const int data)//进栈 " << endl;
	out << "{" << endl;
	out << "struct Node* node;" << endl;
	out << "node = (struct Node*)malloc(sizeof(struct Node));" << endl;
	out << "assert(node != NULL);" << endl;
	out << "node->data = data;" << endl;
	out << "node->next = stack->head;" << endl;
	out << "stack->head = node;" << endl;
	out << "++stack->size;" << endl;
	out << "}" << endl;

	out << "int StackEmpty(struct Stack* stack)" << endl;
	out << "{" << endl;
	out << "return (stack->size == 0);" << endl;
	out << "}" << endl;

	out << "int StackPop(struct Stack* stack, int* data)//出栈  " << endl;
	out << "{" << endl;
	out << "if (StackEmpty(stack))" << endl;
	out << "{" << endl;
	out << "	return 0;" << endl;
	out << "}" << endl;

	out << "	struct Node* tmp = stack->head;" << endl;
	out << "	*data = stack->head->data;" << endl;
	out << "	stack->head = stack->head->next;" << endl;
	out << "	free(tmp);" << endl;
	out << "--stack->size;" << endl;

	out << "	return 1;" << endl;
	out << "}" << endl;
	out << "int StackTop(struct Stack* stack, int* data)" << endl;
	out << "{" << endl;

	out << "	if (StackEmpty(stack))" << endl;
	out << "	{" << endl;
	out << "		return 0;" << endl;
	out << "	}" << endl;
	out << "	*data = stack->head->data;" << endl;
	out << "	return 1;" << endl;
	out << "}" << endl;
	out << "void StackCleanup(struct Stack* stack)" << endl;
	out << "{" << endl;
	out << "struct Node* tmp;" << endl;
	out << "while (stack->head)" << endl;
	out << "{" << endl;
	out << "		tmp = stack->head;" << endl;
	out << "		stack->head = stack->head->next;" << endl;
	out << "		free(tmp);" << endl;
	out << "	}" << endl;

	out << "	stack->size = 0;" << endl;
	out << "}" << endl;


	
	/*table_vec.push_back(pair<void*, int>(next, (boundNInt + 2)*lrdfa.statesVec.size()));
	table_vec.push_back(pair<void*, int>(base, lrdfa.statesVec.size() + 1));
	table_vec.push_back(pair<void*, int>(producer_data, 5 * producerVec.size()));
	table_vec.push_back(pair<void*, int>(index, 2 * producerVec.size()));
	table_vec.push_back(pair<void*, int>(productions, boundTInt + 1));
	table_vec.push_back(pair<void*, int>(map_vec, defineCount));
	table_vec.push_back(pair<void*, int>(tokens, tokensDefineMap.size()));
	*/
	print_array("yy_next", table_vec[0].second, table_vec[0].first, out);
	print_array("yy_base", table_vec[1].second, table_vec[1].first, out);
	print_array("yy_producer_data", table_vec[2].second, table_vec[2].first, out);
	print_array("yy_index", table_vec[3].second, table_vec[3].first, out);
	print_array("yy_map_vec", table_vec[4].second, table_vec[4].first, out);
	
	/*对字符串类型数组单独处理*/
	
	out << "static char* yy_productions[]=" << endl;
	out << "{" << endl;

	int i = 0;
	for (const auto & e: productions_vec)
	{

		out << "\"" << e << "\"" << ", ";
		++i;
		if (i % 5 == 0)
		{
			out << endl;
		}
		if (i == productions_vec.size() - 1) break;
	}
	
	/*最后一个元素没得逗号*/

	out << "\"" << productions_vec[i] << "\"" << endl << "};" << endl;



	/*main()函数部分*/

	out << "int main(int argc,char** argv)" << endl;
	out << "{" << endl;
	out << "	if(argc==2)" << endl;
	out << "	{" << endl;
	out << "		lex_init(argv[1]);" << endl;
	out << "	}" << endl;
	out << "	else{" << endl;
	out << "		printf(\"ERROR: invalid argument!\\n\");" << endl;
	out << "		return -1;" << endl;
	out << "	}" << endl;


	/*调用lex_main*/
	//out << "	lex_init(\"q.txt\");" << endl;



	/*创建两个栈，状态栈和符号栈*/

	out << "	struct Stack stateStack;" << endl;
	out << "	struct Stack symbolStack;" << endl;
	out << "	StackInit(&stateStack);" << endl;
	out << "	StackInit(&symbolStack);" << endl;

	/*将初始状态和终结符压到两个栈中*/
	out << "	StackPush(&stateStack,"<<(lalrdfa.startState+1)<<");" << endl;
	out << "	StackPush(&symbolStack,TERMINATED_SYMBOL+1);" << endl;/*符号栈压入$*/
	out << "	int token=0;" << endl; /*存放每次读取的token*/
	out << "	int item=0;" << endl;  /*存放转移表的表项*/
	out << "	int vt=0;" << endl;  /*存放终结符编号*/
	out << "	int producerStart=0;" << endl; /*存放产生式开始坐标*/
	out << "	int producerLength=0;" << endl; /*存放产生式的长度*/
	/*token读取函数yy_lex(); */
	out << "	token=yy_lex();" << endl;
	out << "	if (token < 0) {" << endl;
	out << "		printf(\"ERROR: cant find token in the file.\\n\");" << endl;
	out << "		return -1;" << endl;
	out << "	}	else {" << endl;
	out << "		vt = yy_map_vec[token];" << endl;
	out << "	}" << endl;

	/*接下来就重复作一些事情就完事了*/

	out << "	do{" << endl;
	/*第一步，查表，看看移进还是归约*/
	out << "		int top;" << endl;
	out << "		StackTop(&stateStack,&top);" << endl;

	out << "		item=yy_next[yy_base[top]+ vt];" << endl;
    
	/*如果是移进*/
	out << "		if(item>0)" << endl;
	out << "		{" << endl;
	out << "			StackPush(&stateStack,item);" << endl;
	out << "			StackPush(&symbolStack,vt);" << endl;
	/*token指针指向下一个*/
	out << "			token = yy_lex();" << endl;
	out << "			if (token == -2) {" << endl;
	out << "				printf(\"ERROR: reached the end of file.\\n\");" << endl;
	out << "				break;" << endl;
	out << "			}" << endl;
	out << "			else if (token == -1) {//$" << endl;
	out << "				vt = TERMINATED_SYMBOL + 1;" << endl;
	out << "			}" << endl;
	out << "			else {" << endl;
	out << "				vt = yy_map_vec[token];" << endl;
	out << "			}" << endl;
	out << "			continue;" << endl;
	out << "		}" << endl;

	/*如果是归约*/
	out << "		else if(item<0)" << endl;
	out << "		{" << endl;
	out << "			if(item == -1){" << endl;
	out << "				printf(\"finish!\\n\");" << endl;
	out << "				break;" << endl;
	out << "			}" << endl;
	out << "			producerStart=yy_index[2*(-item)];" << endl;
	out << "			producerLength=yy_index[2*(-item)+1];" << endl;
	out << "			int reverseIndex=producerStart+producerLength-1;" << endl;/*反向索引*/
	
	out << "			int stackTopItem=0;" << endl;
	/*弹出产生式右边个数量的状态和符号*/
	out << "			for(int i=reverseIndex;i>producerStart;i--)" << endl;
	out << "			{" << endl;
	out << "				if(StackPop(&stateStack,&stackTopItem)==0)" << endl;
	out << "				{" << endl;
	
	out << "					printf(\"ERROR:STACK EMPTY!\");" << endl;
	out << "					return -1;" << endl;
	out << "				}" << endl;

	out << "				if(StackPop(&symbolStack,&stackTopItem)==0)" << endl;
	out << "				{" << endl;
	out << "					printf(\"ERROR:STACK EMPTY!\");" << endl;
	out << "					return -1;" << endl;
	out << "				}" << endl;

	out << "				if(yy_producer_data[i]!=stackTopItem)" << endl;
	out << "				{" << endl;
	out << "					printf(\"ERROR:SYNTAX ERROR!\");" << endl;
	out << "					return -1;" << endl;
	out << "				}" << endl;

	out << "			}" << endl;

	/*将产生式左侧的符号压入符号栈以及GOTO(当前栈顶状态，产生式左侧符号)压入状态栈*/
	out << "			printf(\"%s\\n\",yy_productions[-(item+2)]);" << endl;
	out << "			StackPush(&symbolStack,yy_producer_data[producerStart]);" << endl;
	out << "			int top;" << endl;
	out << "			StackTop(&stateStack, &top);" << endl;
	out << "			item = yy_next[yy_base[top] + yy_producer_data[producerStart]+1];" << endl;
	out << "			StackPush(&stateStack,item);" << endl;
	out << "		}" << endl;
	out << "		else{" << endl;
	out << "			printf(\"ERROR:SYNTAX ERROR!\");" << endl;
	out << "			return -1;" << endl;
	out << "		}" << endl;
	out << "	}while(1);" << endl;
	out << "	return 0;" << endl;
	out << "}" << endl;
	/*
	out << "int getOneToken(int& token)" << endl;
	out << "{" << endl;
	out << "if(tokenLength<=0)" << endl;
	out << "return 0;" << endl;
	out << "token=tokens[tokenIndex];" << endl;
	out << "return 1;" << endl;
	out << "}" << endl;

	out << "int nextToken()" << endl;
	out << "{" << endl;
	out << "if(tokenLength>0)" << endl;
	out << "{" << endl;
	out << "++tokenIndex;" << endl;
	out << "--tokenLength;" << endl;
	out << "return 1;" << endl;
	out << "}" << endl;
	out << "return 0;" << endl;
	out << "}" << endl;
	*/
	out.close();

}

void print_array(string name, int size, void *value, ofstream& out)
{

	int* array_buf = (int*) value;
	out << "static int	" << name << "[" << size << "]" << " =" << endl;
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