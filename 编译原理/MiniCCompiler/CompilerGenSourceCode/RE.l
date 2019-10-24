%{
#include <string>
#include <map>
#include <iostream>
int line = 0;
int numCount = 0;
%}

digit [0~9];
letter [A~Za~z];
id {letter}({letter}|{digit})*;
number {digit}+(.{digit}+)?(E[+-]?{digit}+)?;
%%

int {return "int";}
float {return "float";}
double {return "double";}
void {return "void";}
if  {return "if";}
else  {return "else";}
while {return "while";}
return {return "return";}
static {return "static";}
{id} {return "id";}
{number} {++numCount; return "num";}
"+" {return "+";}
"-" {return "-";}
"*" {return "*";}
"/" {return "/";}
"%" {return "%";}
"+=" {return "+=";}
"-=" {return "-=";}
"*=" {return "*=";}
"/=" {return "/=";}
"%=" {return "%=";}
"&&" {return "&&";}
"||" {return "||";}
"!" {return "!";}
"<=" {return "<=";}
">=" {return ">=";}
"=" {return "=";}
"!=" {return "!=";}
"==" {return "==";}
"<" {return "<";}
">" {return ">";}
"(" {return "(";}
")" {return ")";}
"{" {return "{";}
"}" {return "}";}
"[" {return "[";}
"]" {return "]";}
";" {++line;return ";";}
"," {return ",";}
%%

void calLine() {
	std::cout << "number of semicolon : " << line << std::endl;
}

void getNumCount() {
    std::cout << "count of num : " << numCount << std::endl;
}

void main() {
	calLine();
	getNumCount();
}