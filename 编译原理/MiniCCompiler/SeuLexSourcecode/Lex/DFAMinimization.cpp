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
	newdfa.startState = statesSetsMap.find(origindfa.startState)->second;//�����̬
	//������̬
	for (const auto &p : origindfa.endStatesMap) {
		newdfa.endStatesMap.emplace(statesSetsMap.find(p.first)->second, p.second);
	}
	//��������״̬
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
	}//��̬�ȶ��ֿ�

	unordered_set<int> tempSet;
	for (const auto &e : statesVec) {
		if (endStatesMap.find(e.number) == endStatesMap.end()) {//����̬
			tempSet.insert(e.number);
			statesSetsMap.emplace(e.number, count);
		}
	}//����̬����һ��������
	statesSetsVec.push_back(tempSet);
	++count;

	while (scan(statesVec));//��ͣ��ɨ��ֱ�����ٱ仯Ϊֹ

}


bool scan(const vector<DFAstate> & statesVec) {//һ��ֻ����һ�����ϣ�
	bool flag = false;
	int splitSetNumber = 0;
	unordered_set<int> newSet;
	for (int k = 0; k < statesSetsVec.size();++k) {//�ҵ�һ����Ҫ����ļ���
		auto &s = statesSetsVec[k];
		if (s.size() == 1) continue;
		else {
			auto &standard = statesVec[*(s.begin())];//��һ����Ϊ��׼
			for (const auto &c : ALLSET) {//������׼״̬�����б�
				for (const auto &i : s) {//��鼯��ÿһ��״̬
					const auto & state = statesVec[i];
					//�ȿ�������һ��һ����
					auto findStateIt = state.exEdgesMap.find(c);//��һ��������
					auto findStandardIt = standard.exEdgesMap.find(c);//��һ��������
					if (findStateIt == state.exEdgesMap.end() && findStandardIt != standard.exEdgesMap.end()) {//��һ���Ҳ���
						flag = true;
						newSet.insert(i);
					}
					else if ((findStateIt != state.exEdgesMap.end()) && (findStandardIt == standard.exEdgesMap.end())) {//��һ���Ҳ���
						flag = true;
						newSet.insert(i);
					}
					else if (*(statesSetsMap.find(findStandardIt->second)) != *(statesSetsMap.find(findStateIt->second))) {//���ҵ��˵���Ŀ��״̬��ͬ
						flag = true;
						newSet.insert(i);
					}
				}
				if (flag) break; //����һ�����в�ͬ��ͣ
			}
			if (flag) {//�ֿ�һ��״̬��ͣ
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