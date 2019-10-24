//	  file : main_function.cpp
// content : main function for lexical and syntax analyzer
//	  date : 2017/4/25
//  author : Wangduo
//  @COPYRIGHT  : School of Computer Science & Engineering Southeast University

#include "SeuLex.h"
#include "SeuYacc.h"

int main(int argc, char* argv[]) {
	if (argc < 3) return 1;
	SeuLex lex(argv[1] ,"lex.h");
	SeuYacc yacc(argv[2], "yacc.cpp");
	return 0;
}