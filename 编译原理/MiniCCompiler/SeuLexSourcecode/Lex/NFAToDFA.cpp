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
	const string edgeSet(ALLSET);//���ñߵ�ȫ��
	queue<int> unExamedDFAStates;//�洢δ�����DFA״̬���
	int counter = 0;//���ڸ�DFA״̬���
	DFAstate state;//I0
	state.number = counter++;
	state.identitySet.insert(nfa.startState);
	epsilon_closure(state.identitySet, nfa.statesMap);
	dfa.statesVec.push_back(state);
	unExamedDFAStates.push(state.number);
	while (!unExamedDFAStates.empty()) {//����û�д������DFA״̬
		int nowStateNum = unExamedDFAStates.front();//ȡ��һ��Ҫ�����״̬�ı��
		unExamedDFAStates.pop();
		//cout  << "POP a state. ("<< c++ <<")" << endl;
		for (const char& c : edgeSet) {
			/*cout << "CHECK edge " << c << " ." << endl;*/
			unordered_set<int> tempSet;
			/*cout << "SUBSET CONSTRUCTION" << endl;*/
			if (subset_construct(dfa.statesVec[nowStateNum].identitySet, tempSet, c, nfa.statesMap)) {
				int toStateNum;//��ָ���״̬�ı��
				epsilon_closure(tempSet, nfa.statesMap);//��epsilon�հ�
				bool has = false;//�����ǲ����Ѿ�����һ����
				for (const auto &s : dfa.statesVec) {
					if (s.identitySet == tempSet) {//����һ����
						toStateNum = s.number; //ָ��vect�����е�״̬
						has = true;
						/*cout << "EXIST state " << s.number << endl;*/
						break;
					}
				}
				if (!has) {//�����ڣ�Ҫ�½�һ��state
					DFAstate newState;
					newState.number = counter++;//�±��
					toStateNum = newState.number; //ָ���½������״̬
					newState.identitySet = tempSet;
					dfa.statesVec.push_back(newState);
					unExamedDFAStates.push(toStateNum);//��״̬����Ҫ����
					int actions;	//�ж��Ƿ�����̬����ÿ����̬��Ӧ����
					if (find_actions(newState.identitySet, nfa.endStatesMap, actions)) { //�������һ����̬
						dfa.endStatesMap.insert(pair<int, int >(newState.number, actions));//�������dfa��̬��Ӧ�Ķ���
					}
				}
				dfa.statesVec[nowStateNum].exEdgesMap.insert(pair<char, int>(c, toStateNum));//��һ����
			}
		}
	}
}


bool find_actions(const unordered_set<int> &nfaStatesInDFAState, const map<int, int >& endStatesInNFA, int& actions) {
	decltype(endStatesInNFA.find(0)) endStateIt;//Ҫѡ�����̬
	bool find = false;
	for (const auto & nfaState : nfaStatesInDFAState) {//����DFA�����е�NFA״̬
		auto it = endStatesInNFA.find(nfaState);//����һ�����NFA״̬�ǲ���һ����̬
		if (it != endStatesInNFA.end()) {//��������̬
			if (find) {//�����λ�ø���ǰ�ģ� ����
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

	//����ʼ����������״̬��ջ
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
			if (handledFlagVec[unhandledItem]) {//�Ѿ��������״̬
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
	bool flag = false;//false��ʾ������û���ӳ�edge�ߵ�״̬
	for (const auto& state : originSet) {
		auto itsPair = NFAStatesMap.find(state)->second.exEdgesMultiMap.equal_range(edge);
		auto beginIt = itsPair.first, endIt = itsPair.second;
		if (beginIt == endIt) continue;//�ж����״̬��û�з���edge��
		while (beginIt != endIt) {
			constructedSet.insert(beginIt->second);
			++beginIt;
		}
		flag = true;//����������һ��״̬�о����У�
	}
	return flag;
}
