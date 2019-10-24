#include "stdafx.h"
#include "structs.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::pair;


extern vector<Rules> rules;

void dfa_to_array(const DFA& dfa,vector<pair<int*,int> >& arrays, vector<Rules>& endVec) {

	string allset(ALLSET);
	const int set_size = allset.size();


	//�����ĸ���1��ec������char��ascii�룬ֵ�Ƕ�Ӧ������
	int* ec=new int[256];
	memset(ec, 0, 256*sizeof(int));
	//���±�Ϊ1��ʼ��char��ֵ��0����Ƿ��ַ�
	for (int i = 0; i < set_size; ++i) {
		ec[int(allset[i])] = i + 1;
	}
	arrays.push_back(pair<int*,int>(ec, 256));

	//2��base��������״̬���У�ֵ������*��ȡ�
	const int DFA_size = dfa.statesVec.size();
	int* base = new int[DFA_size];
	memset(base, 0, DFA_size*sizeof(int));
	//3��next��������base+ec��ֵ����һ����ת״̬����С��base*ec
	int sizeOfNext = DFA_size * (set_size + 1);
	int* next = new int[sizeOfNext];//set��1��ʼ����
	//memset(next, 0, sizeOfNext * sizeof(int));
	//��ʼ��-1�ɲ�����memset
	for (int i = 0; i < sizeOfNext; i++)
	{

		next[i] = -1;
	}

	for (int i = 0; i < dfa.statesVec.size(); i++) {
		base[dfa.statesVec[i].number] = i * (set_size+1);
		for (auto it = dfa.statesVec[i].exEdgesMap.begin(); it != dfa.statesVec[i].exEdgesMap.end(); it++) {
			next[base[dfa.statesVec[i].number] + ec[int(it->first)]] = it->second;
		}	
	}
	arrays.push_back(pair<int*, int>(base, DFA_size));
	arrays.push_back(pair<int*, int>(next, sizeOfNext));
	//4��accept����������̬״̬�ţ�ֵ�Ƕ�Ӧ�����кš�
	int* accept = new int[DFA_size];
	memset(accept, 0, DFA_size*sizeof(int));
	
	
	int numOfend = 0;
	for (auto it = dfa.endStatesMap.begin(); it != dfa.endStatesMap.end(); ++it) {
		accept[it->first] = numOfend+1;
		endVec.push_back(rules[it->second]);
		++numOfend;
	}

	arrays.push_back(pair<int*, int>(accept, DFA_size));
	
}