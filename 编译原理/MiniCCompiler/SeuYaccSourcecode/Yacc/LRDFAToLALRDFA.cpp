#include "stdafx.h"
#include "structs.h"
using std::map;
using std::stack;
using std::queue;
using std::cout;
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::pair;
using std::map;
using std::pair;
using std::make_pair;


extern LRDFA lrdfa;
LRDFA lalrdfa;

namespace std {

	template<>
	struct hash<pair<int, int> > {
		inline size_t operator()(const pair<int, int>& s) const {
			return  hash<int>()(s.first+s.second);
		}
	};
	struct {
		bool operator()(const pair<int, int>& p1, const pair<int, int>& p2) const {
			if (p1.first != p2.first) return false;
			if (p1.second != p2.second) return false;
			else return true;
		}
	};

}

//�ϲ�ͬ����
void lrdfa_to_lalrdfa() {
	vector<pair<unordered_set<pair<int, int>>, unordered_set<int>>> core_vec;//ͬ�ļ��ϣ�pairǰ����core����������Ϊǰ���ͬ�ļ�������״̬�ţ�
	//��ÿ��״̬��core��Ȼ��Ƚ�core����core��ͬ��װ��һ��
	for (const auto & state : lrdfa.statesVec) {
		//��״̬��coreŪ����
		unordered_set<pair<int, int>> state_core;
		for (const auto &lritem : state.LRItemsSet) {
			state_core.insert(make_pair(lritem.gramarInt, lritem.positionInt));
		}
		bool isHad = false;//�Ƿ��Ѵ�����core_vec��
		for (auto& core : core_vec) {
			//����Ѿ������ˣ�ֱ�Ӽ���ͬ�ļ�����״̬��Ŵ���
			if (core.first == state_core) {
				isHad = true;
				core.second.insert(state.numberInt);
			}
		}
		//��������ڸ����͵�core����������
		if (!isHad) {
			unordered_set<int> new_type;
			new_type.insert(state.numberInt);
			core_vec.push_back(make_pair(state_core, new_type));
		}
	}
	//�ϲ�

	unordered_map<int, int> index;//�ɣ���״̬��
	int count = 0;
	for (const auto& core : core_vec) {
		LRState newState;
		newState.numberInt = count++;
		newState.edgesMap = lrdfa.statesVec[*core.second.begin()].edgesMap;
		for (const auto& state : core.second) {
			index.emplace(state, newState.numberInt);
			newState.LRItemsSet.insert(lrdfa.statesVec[state].LRItemsSet.cbegin(), lrdfa.statesVec[state].LRItemsSet.cend());
		}
		lalrdfa.statesVec.push_back(newState);
	}
	for (auto& state : lalrdfa.statesVec) {
		for (auto& edge : state.edgesMap) {
			edge.second = index.find(edge.second)->second;
		}
	}

	lalrdfa.startState = index.find(lrdfa.startState)->second;
}