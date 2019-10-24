#include"stdafx.h"
#include"structs.h"
using std::map;


// �ȴ������е�token, ��ת�����������������
// char 0-255
// tokens 256-n
// ���ս�� n-m

extern int boundTInt, boundNInt, startInt;
extern ProducerVec producerVec;
extern map<int, pair<int, int> > indexMap;
extern vector<unordered_set<int> > precedenceRulesVec;
map<string, int> tokensMap;//<token,number>
map<string, int> tokensDefineMap;//<token,number> ����define
map<int, int> intsMap;

int defineCount = 256;

void translate_expression(const string &startItemStr, const vector<string> &tokensVec, const ProducerVecStr & producerVecStr,  const vector<unordered_set<string>>&left) {
	

	int count = 0;

	for (const auto &producer : producerVecStr) {
		//�����ַ�
		for (const auto & item : producer.second) {
			if (item.size() == 3 && item[0] == '\'' && item[2] == '\'') {
				if (tokensMap.find(item) != tokensMap.end()) continue;
				intsMap.emplace(item[1], count);
				tokensMap.emplace(item, count++);
			}
		}
	}


	for (const auto &token : tokensVec) {
		tokensDefineMap.emplace(token, defineCount);
		intsMap.emplace(defineCount++, count);
		tokensMap.emplace(token, count++);
	}

	

	boundTInt = count - 1;
	for (const auto &producer : producerVecStr) {
		auto result = tokensMap.try_emplace(producer.first, count);
		if (result.second)++count;
	}
	boundNInt = count - 1;

	for (const auto & s : left) {
		unordered_set<int> newSet;
		for (const auto & e : s) {
				newSet.insert(tokensMap[e]);		
		}
		precedenceRulesVec.push_back(newSet);
	}


	startInt = tokensMap[startItemStr];

	producerVec.push_back(pair<int, vector<int> >(0, vector<int>()));

	producerVec.push_back(pair<int, vector<int> >(0, vector<int>({ startInt })));

	vector<int > tempRightVec;
	int preleftInt = boundTInt + 1, counter = 0, temp;
	for (const auto &producer : producerVecStr) {
		tempRightVec.clear();
		//�����Ҳ�����ʽ
		for (const auto & item : producer.second) {
			tempRightVec.push_back(tokensMap[item]);
		}
		//�����󲿲���ʽ������
		temp = tokensMap[producer.first];
		if (temp != preleftInt) {
			indexMap.try_emplace(preleftInt, pair<int,int>(producerVec.size()- counter,counter));
			counter = 0;
			preleftInt = temp;
		}
		producerVec.push_back(pair<int,vector<int> >(temp, tempRightVec));
		++counter;
		
	}
	indexMap.try_emplace(preleftInt, pair<int, int>(producerVec.size() - counter, counter));
	
}