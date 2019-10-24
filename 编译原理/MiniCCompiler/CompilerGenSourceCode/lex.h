 #include <string>
 #include <map>
 #include <iostream>
 int line = 0;
 int numCount = 0;
 
void calLine() {
	std::cout << "number of semicolon : " << line << std::endl;
}

void getNumCount() {
    std::cout << "count of num : " << numCount << std::endl;
}

void main2() {
	calLine();
	getNumCount();
}

#include "lexHelp.h"
int yyparse(char* file, std::list<Token>& tokenList) {
yyparseFct(file,tokenList);
main2();
return 0;
}