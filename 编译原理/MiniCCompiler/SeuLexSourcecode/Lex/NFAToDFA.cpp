#include "stdafx.h"
#include "structs.h"

using std::cout;
using std::endl;
using std::unordered_map;
using std::map;
using std::unordered_set;
using std::pair;
using std::queue;
using std::stack;


static bool find_actions(const unordered_set<int> &nfaStatesInDFAState, const map<int, int >& endStatesInNFA, int& actions);
static bool subset_construct(const unordered_set<int>& originSet, unordered_set<int>& constructedSet, const char edge, const unordered_map<int, NFAstate>& NFAStatesMap);
static void epsilon_closure(unordered_set<int> &initStatesSet, const unordered_map<int, NFAstate>& NFAStatesMap);


static int numOfNFAState = 0;

void nfa_to_dfa(const NFA &nfa, DFA &dfa) {
	numOfNFAState = nfa.statesMap.size();
	int c = 1;
	const string edgeSet(ALLSET);//设置边的全集
	queue<int> unExamedDFAStates;//存储未处理的DFA状态编号
	int counter = 0;//用于给DFA状态编号
	DFAstate state;//I0
	state.number = counter++;
	state.identitySet.insert(nfa.startState);
	epsilon_closure(state.identitySet, nfa.statesMap);
	dfa.statesVec.push_back(state);
	unExamedDFAStates.push(state.number);
	while (!unExamedDFAStates.empty()) {//还有没有处理的新DFA状态
		int nowStateNum = unExamedDFAStates.front();//取出一个要处理的状态的编号
		unExamedDFAStates.pop();
		//cout  << "POP a state. ("<< c++ <<")" << endl;
		for (const char& c : edgeSet) {
			/*cout << "CHECK edge " << c << " ." << endl;*/
			unordered_set<int> tempSet;
			/*cout << "SUBSET CONSTRUCTION" << endl;*/
			if (subset_construct(dfa.statesVec[nowStateNum].identitySet, tempSet, c, nfa.statesMap)) {
				int toStateNum;//边指向的状态的编号
				epsilon_closure(tempSet, nfa.statesMap);//再epsilon闭包
				bool has = false;//看看是不是已经存在一样的
				for (const auto &s : dfa.statesVec) {
					if (s.identitySet == tempSet) {//存在一样的
						toStateNum = s.number; //指向vect中已有的状态
						has = true;
						/*cout << "EXIST state " << s.number << endl;*/
						break;
					}
				}
				if (!has) {//不存在，要新建一个state
					DFAstate newState;
					newState.number = counter++;//新标号
					toStateNum = newState.number; //指向新建的这个状态
					newState.identitySet = tempSet;
					dfa.statesVec.push_back(newState);
					unExamedDFAStates.push(toStateNum);//新状态尚需要处理
					int actions;	//判断是否是终态并找每个终态对应动作
					if (find_actions(newState.identitySet, nfa.endStatesMap, actions)) { //如果包含一个终态
						dfa.endStatesMap.insert(pair<int, int >(newState.number, actions));//决定这个dfa终态对应的动作
					}
				}
				dfa.statesVec[nowStateNum].exEdgesMap.insert(pair<char, int>(c, toStateNum));//加一条边
			}
		}
	}
}


bool find_actions(const unordered_set<int> &nfaStatesInDFAState, const map<int, int >& endStatesInNFA, int& actions) {
	decltype(endStatesInNFA.find(0)) endStateIt;//要选择的终态
	bool find = false;
	for (const auto & nfaState : nfaStatesInDFAState) {//遍历DFA中所有的NFA状态
		auto it = endStatesInNFA.find(nfaState);//查找一下这个NFA状态是不是一个终态
		if (it != endStatesInNFA.end()) {//发现是终态
			if (find) {//如果有位置更靠前的， 更新
				if (endStateIt->first > it->first)
					endStateIt = it;
			}
			else {
				endStateIt = it;
				find = true;
			}
		}
	}
	if (find) {
		actions = endStateIt->second;
		return true;
	}
	else return false;

}



//void epsilon_closure(unordered_set<NFAstate> &initStatesSet, const unordered_map<int , NFAstate> NFAStatesMap) {
//	unordered_set<NFAstate> tempSet1;
//	unordered_set<NFAstate> tempSet2(initStatesSet);
//	do {
//		tempSet1 = tempSet2;
//		for (const auto& state : tempSet1) {
//			auto itsPair = state.exEdgesMultiMap.equal_range('@');
//			auto beginIt = itsPair.first, endIt = itsPair.second;
//			while (beginIt != endIt) {
//				tempSet2.insert((*NFAStatesMap.find((*beginIt).second)).second);
//				++beginIt;
//			}
//		}
//	} while (tempSet1 != tempSet2);
//	initStatesSet = tempSet2;
//}

void epsilon_closure(unordered_set<int> &initStatesSet, const unordered_map<int, NFAstate>& NFAStatesMap) {

	stack<int> stack;
	vector<bool> handledFlagVec(numOfNFAState);

	//将初始集合中所有状态入栈
	for (const auto& s : initStatesSet) {
		stack.push(s);
		handledFlagVec[s] = true;
	}

	while (!stack.empty()) {
		auto itsPair = NFAStatesMap.find(stack.top())->second.exEdgesMultiMap.equal_range('@');
		stack.pop();
		auto beginIt = itsPair.first;
		while (beginIt != itsPair.second) {
			int unhandledItem = (*beginIt).second;
			if (handledFlagVec[unhandledItem]) {//已经处理过该状态
				continue;
			}
			else {
				stack.push(unhandledItem);
				initStatesSet.emplace(unhandledItem);
				++beginIt;
			}
		}
	}
}


bool subset_construct(const unordered_set<int>& originSet, unordered_set<int>& constructedSet, const char edge, const unordered_map<int, NFAstate>& NFAStatesMap) {
	bool flag = false;//false表示集合中没有延出edge边的状态
	for (const auto& state : originSet) {
		auto itsPair = NFAStatesMap.find(state)->second.exEdgesMultiMap.equal_range(edge);
		auto beginIt = itsPair.first, endIt = itsPair.second;
		if (beginIt == endIt) continue;//判断这个状态有没有发出edge边
		while (beginIt != endIt) {
			constructedSet.insert(beginIt->second);
			++beginIt;
		}
		flag = true;//集合里面有一个状态有就是有！
	}
	return flag;
}
