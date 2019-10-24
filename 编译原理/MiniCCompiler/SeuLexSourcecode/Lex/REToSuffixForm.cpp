#include "stdafx.h"
#include "structs.h"

using std::stack;
using std::vector;
using std::map;
using std::string;
using std::isalnum;
using std::isalpha;
using std::isdigit;
using std::cout;
using std::queue;
using std::stack;
using std::endl;

//中缀转成后缀
void re_to_suffix(vector<Rules>& rules) {
	for (auto &rule :rules ) {
		string pattern = rule.pattern;//对pattern进行操作
		string temp="";//用于临时装载正确pattern
		queue<char> q;	//最终正确的队列
		stack<char> s;	//操作数栈
		for (int i = 0; i < pattern.size();++i) {
			char now = pattern[i];
			//(优先级最低，压栈
			if (now == '(') {
				s.push(now);
			}else if (now == ')') { 
				while (s.top() != '(') {
					q.push(s.top());
					s.pop();
				}
				s.pop();
			}else if (now == '|') {
				if (s.empty()) {
					s.push(now);
				}else if (s.top() == '.' || s.top() == '|'||s.top()=='*') {
					//遇到优先级高的
					do {
						q.push(s.top());
						s.pop();
					} while (!s.empty() && (s.top() == '.' || s.top() == '|'||s.top()=='*'));
					s.push(now);
				}
				else {
					s.push(now);
				}
			}else if (now == '.') { 
				if (s.empty()) {
					s.push(now);
				}else if (s.top() == '.' || s.top() == '*') {
					//遇到优先级高的
					do {
						q.push(s.top());
						s.pop();
					} while (!s.empty() && (s.top() == '.'  || s.top() == '*'));
					s.push(now);
				}else {
					s.push(now);
				}
			}else if (now == '*') {
					q.push(now);
			}//防止数组越界
			else if (now == '`' && (i + 1 == pattern.size())) {
				q.push(now);
			}
			//如果是自己定义的一些转义字符，直接传给正确队列
			else if (now == '`' && (pattern[i + 1] == '(' || pattern[i + 1] == ')'
				|| pattern[i + 1] == '|' || pattern[i + 1] == '.' || pattern[i + 1] == '*' )){
				q.push(now);
				q.push(pattern[i + 1]);
				++i;
			}
			else if (now == '`' && (pattern[i + 1] == '?' || pattern[i + 1] == '+' || pattern[i + 1] == '{' || pattern[i + 1] == '}' || pattern[i + 1] == '[' || pattern[i + 1] == ']')) {//其他的都是字符，建议直接放进正确队列
				q.push(pattern[i + 1]);
				++i;
			}
			else {
				q.push(now);
			}
		}
		//栈里还有，就冲刷出来
		while (!s.empty()) {
			q.push(s.top());
			s.pop();
		}
		//把正确队列转化为字符串
		while (!q.empty()) {
			char abb (q.front());
			string a{abb};
			temp.append(a);
			q.pop();		
		}
		rule.pattern = temp;
	}


}