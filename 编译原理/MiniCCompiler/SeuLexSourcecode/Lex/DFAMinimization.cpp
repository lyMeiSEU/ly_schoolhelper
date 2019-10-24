#include "stdafx.h"
#include "structs.h"

using std::vector;
using std::pair;
using std::unordered_map;

static bool scan(const vector<DFAstate> &);
static void split_to_sets(const DFA & dfa);

static unordered_map<int,int> statesSetsMap;
static vector<unordered_set<int> > statesSetsVec;
static int count = 0;

void dfa_minimize(const DFA& origindfa, DFA& newdfa) {
	split_to_sets(origindfa);
	newdfa.startState = statesSetsMap.find(origindfa.startState)->second;//处理初态
	//处理终态
	for (const auto &p : origindfa.endStatesMap) {
		newdfa.endStatesMap.emplace(statesSetsMap.find(p.first)->second, p.second);
	}
	//处理其他状态
	for (int k = 0; k < statesSetsVec.size(); ++k) {
		auto & pivotState = origindfa.statesVec[*statesSetsVec[k].cbegin()];
		DFAstate newState;
		newState.number = k;
		for (const auto &p : pivotState.exEdgesMap) {
			newState.exEdgesMap.emplace(p.first, statesSetsMap.find(p.second)->second);
		}
		newdfa.statesVec.push_back(newState);
	}
}


void split_to_sets(const DFA &dfa) {
	auto & endStatesMap = dfa.endStatesMap;
	auto & statesVec = dfa.statesVec;
	for (const auto &p : endStatesMap) {
		statesSetsVec.push_back(unordered_set<int>{p.first});
		statesSetsMap.emplace(p.first,count++);
	}//终态先都分开

	unordered_set<int> tempSet;
	for (const auto &e : statesVec) {
		if (endStatesMap.find(e.number) == endStatesMap.end()) {//非终态
			tempSet.insert(e.number);
			statesSetsMap.emplace(e.number, count);
		}
	}//非终态都在一个集合里
	statesSetsVec.push_back(tempSet);
	++count;

	while (scan(statesVec));//不停的扫描直到不再变化为止

}


bool scan(const vector<DFAstate> & statesVec) {//一次只分裂一个集合！
	bool flag = false;
	int splitSetNumber = 0;
	unordered_set<int> newSet;
	for (int k = 0; k < statesSetsVec.size();++k) {//找到一个需要处理的集合
		auto &s = statesSetsVec[k];
		if (s.size() == 1) continue;
		else {
			auto &standard = statesVec[*(s.begin())];//找一个作为基准
			for (const auto &c : ALLSET) {//遍历基准状态的所有边
				for (const auto &i : s) {//检查集合每一个状态
					const auto & state = statesVec[i];
					//先看看边数一不一样多
					auto findStateIt = state.exEdgesMap.find(c);//找一找这条边
					auto findStandardIt = standard.exEdgesMap.find(c);//找一找这条边
					if (findStateIt == state.exEdgesMap.end() && findStandardIt != standard.exEdgesMap.end()) {//有一个找不到
						flag = true;
						newSet.insert(i);
					}
					else if ((findStateIt != state.exEdgesMap.end()) && (findStandardIt == standard.exEdgesMap.end())) {//有一个找不到
						flag = true;
						newSet.insert(i);
					}
					else if (*(statesSetsMap.find(findStandardIt->second)) != *(statesSetsMap.find(findStateIt->second))) {//都找到了但是目的状态不同
						flag = true;
						newSet.insert(i);
					}
				}
				if (flag) break; //发现一条边有不同就停
			}
			if (flag) {//分开一个状态就停
				splitSetNumber = k;
				break; 
			}
		}
	}
	for (const auto & s : newSet) {
		statesSetsVec[splitSetNumber].erase(s);
		statesSetsMap.insert_or_assign(s, count);
	}

	statesSetsVec.push_back(newSet);
	++count;

	return flag;
}