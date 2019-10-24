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
using std::multimap;
using std::pair;

int number = 0;

//将后缀表达式转化为NFA
void suffix_to_nfa(vector<Rules>& suffixRules,NFA& finalNfa) {
	stack<NFA> nfaStack;//用于构建NFA的工具栈,也是
	for (int i = 0; i < suffixRules.size();++i) {
		string pattern = suffixRules[i].pattern; //已经后缀化后的pattern
		int tempActionInt = -1;//临时终态动作数组
		for (auto it = pattern.cbegin() ; it != pattern.cend(); ++it)
		{
			NFA upNFA,downNFA;
			NFAstate start, end;
			char now = *it;
			switch (now) {
			case'|':
				//取出最上面的NFA进行操作
				upNFA = nfaStack.top();
				nfaStack.pop();
				downNFA = nfaStack.top();
				nfaStack.pop();
				start.number = number;
				++number;
				end.number = number;
				++number;				
				//将down的终态和up的终态都连接到end
				upNFA.statesMap.find(upNFA.endStatesMap.begin()->first)->
					second.exEdgesMultiMap.insert(pair<char, int>('@', end.number));
				downNFA.statesMap.find(downNFA.endStatesMap.begin()->first)->
					second.exEdgesMultiMap.insert(pair<char, int>('@', end.number));
				//将start连接到down和up的初态
				start.exEdgesMultiMap.insert(pair<char, int>('@', upNFA.startState));
				start.exEdgesMultiMap.insert(pair<char, int>('@', downNFA.startState));
				//连接关系定义好后存入map
				downNFA.statesMap.insert(pair<int, NFAstate>(start.number, start));
				downNFA.statesMap.insert(pair<int, NFAstate>(end.number, end));
				//将upNFA的stateMap拷贝到down中
				downNFA.statesMap.insert(upNFA.statesMap.begin(), upNFA.statesMap.end());
				//修改downNFA的初态
				downNFA.startState = start.number;
				//修改downNFA的终态
				downNFA.endStatesMap.clear();
				downNFA.endStatesMap.insert(pair<int, int>(end.number, tempActionInt));
				nfaStack.push(downNFA);		
				break;
			case'*':
				//取出最上面的NFA进行操作
				upNFA = nfaStack.top();
				nfaStack.pop();
				//新建两个状态
				start.number = number;
				++number;
				end.number = number;
				++number;
				//将start和upNFA初态连接
				start.exEdgesMultiMap.insert(pair<char, int>('@', upNFA.startState));
				//将start和end连接
				start.exEdgesMultiMap.insert(pair<char, int>('@', end.number));
				//将upNFA的终态和初态连接
				upNFA.statesMap.find(upNFA.endStatesMap.begin()->first)->
					second.exEdgesMultiMap.insert(pair<char, int>('@', upNFA.startState));
				upNFA.statesMap.find(upNFA.endStatesMap.begin()->first)->
					second.exEdgesMultiMap.insert(pair<char, int>('@', end.number));
				//更改初态
				upNFA.startState = start.number;
				//连接关系定义好后存入map
				upNFA.statesMap.insert(pair<int, NFAstate>(start.number, start));
				upNFA.statesMap.insert(pair<int, NFAstate>(end.number, end));
				//更改终态
				upNFA.endStatesMap.clear();
				upNFA.endStatesMap.insert(pair<int, int >(end.number, tempActionInt));
				nfaStack.push(upNFA);
				break;
			case'.':
				//取出栈顶的两个NFA
				upNFA = nfaStack.top();
				nfaStack.pop();
				downNFA = nfaStack.top();
				nfaStack.pop();
				//取出downNFA的终态
				downNFA.statesMap.find(downNFA.endStatesMap.begin()->first)->second//得到终态
				//将压在下面的（也就是更前面的）NFA用@与后面的进行连接
				.exEdgesMultiMap.insert(pair<char, int>('@', upNFA.startState));
				//更改终态
				downNFA.endStatesMap.clear();
				downNFA.endStatesMap = upNFA.endStatesMap;
				//把upNFA的状态map拷贝到downNFA中，
				downNFA.statesMap.insert(upNFA.statesMap.begin(),upNFA.statesMap.end());
				nfaStack.push(downNFA);
				break;
			default:
				if (now == '`') {
					++it;
					now= *it;
				}
				//遇到字符，构造一个NFA，压栈
				NFA pushNFA;//用于压栈的NFA
				//新建一个起始状态
				start.number = number;
				//更改初态
				pushNFA.startState = start.number;
				++number;
				//新建一个终止状态
				end.number = number;
				++number;	
				//将起始状态和终止状态连接
				start.exEdgesMultiMap.insert(pair<char, int>(now, end.number));
				//连接关系定义好后存入map
				pushNFA.statesMap.insert(pair<int, NFAstate>(start.number, start));
				pushNFA.statesMap.insert(pair<int, NFAstate>(end.number, end));
				//标识当前终态，因此先传入空vector。
				pushNFA.endStatesMap.insert(pair<int, int>(end.number, tempActionInt));
				//压栈
				nfaStack.push(pushNFA);
				break;
			}	
		}
		//将action赋给栈顶的NFA的终态
		nfaStack.top().endStatesMap.begin()->second = i;
	}
	//现在得到了装着所有NFA的nfaStack,合并为一个大的NFA。
	finalNfa = nfaStack.top();
	NFA downNFA;
	nfaStack.pop();
	while (!nfaStack.empty()) {
		NFAstate start;
		//依次，把栈顶NFA与大NFA合并
		downNFA = nfaStack.top();
		nfaStack.pop();
		//新建一个start状态
		start.number = number;
		number++;
		//将start连接到finalNFA和dowNFA的初态
		start.exEdgesMultiMap.insert(pair<char, int>('@', finalNfa.startState));
		start.exEdgesMultiMap.insert(pair<char, int>('@', downNFA.startState));
		//修改finalNFA的起始状态
		finalNfa.startState = start.number;
		finalNfa.statesMap.insert(pair<int, NFAstate>(start.number, start));
		//添加finalNFA的终止状态
		finalNfa.endStatesMap.insert(downNFA.endStatesMap.begin(), downNFA.endStatesMap.end());
		//把downNFA的状态map拷贝到finalNFA中
		finalNfa.statesMap.insert(downNFA.statesMap.begin(), downNFA.statesMap.end());
	}
}