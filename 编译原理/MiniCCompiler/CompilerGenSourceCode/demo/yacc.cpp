#include "lex.h"
#include "yaccHelp.h"
int main(int argc, char *argv[]) {
if (argc < 2) {
std::cout << "please input the target source file name" << std::endl;
return 1;
}
std::list<Token> tokenlist;
yyparse(argv[1], tokenlist);
yyreduce(tokenlist);
std::cout << "compile success" << std::endl;
return 0;
}