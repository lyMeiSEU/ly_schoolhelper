#include "stdafx.h"
#include "structs.h"
using std::map;
using std::cout;
using std::endl;



extern map<int, unordered_set<int> > firstMap;
extern int boundTInt, boundNInt;
extern map<int, pair<int, int> > indexMap;
extern ProducerVec producerVec;

static void first_symbol(unordered_set<int>& inputSet, const int& symbol, unordered_set<int >& handlingSymbol);
static void intersection(unordered_set<int>& leftSet, const unordered_set<int>& rightSet);

void first_operation();
void first_string(unordered_set<int>& inputSet, const vector<int>& symbols);



void first_symbol(unordered_set<int>& inputSet, const int& symbol, unordered_set<int >& handlingSymbol) {
	if (firstMap.find(symbol) != firstMap.end()) {//֮ǰ�����
		intersection(inputSet, firstMap[symbol]);
		return;
	}
	
	if (symbol == -1) return;
	if (symbol <= boundTInt) {//symol���ս��
		inputSet.insert(symbol);
		return;
	}
	else{//symbol�Ƿ��ս��
		handlingSymbol.insert(symbol);
		auto indexPair = indexMap[symbol];
		for (int i = indexPair.first; i < indexPair.first + indexPair.second; ++i) {//��������symbol����ߵĲ���ʽ
			auto producer = producerVec[i];
			if (producer.second.size() == 1 && producer.second[0] == -1) {//symbol->epsilon
				inputSet.insert(-1);
				continue;
			}
			//��������ʽ�ұ�������
			int j = 1;
			unordered_set<int> tempSet;
			while (j <= producer.second.size()) {//�������һ���������epsilon
				tempSet.clear();
				if (handlingSymbol.find(producer.second[j - 1]) != handlingSymbol.end()) {//��ֹ��ݹ�
					break;
				}
				first_symbol(tempSet, producer.second[j - 1], handlingSymbol);
				if (tempSet.find(-1) == tempSet.end()) {
					intersection(inputSet, tempSet);
					break;
				}
				if (j == producer.second.size()) {//���һ���epsilon
					inputSet.insert(-1);
					break;
				}
				tempSet.erase(-1);
				intersection(inputSet, tempSet);
				++j;
			}
		}
	}
}


void intersection(unordered_set<int>& leftSet, const unordered_set<int>& rightSet) {
	leftSet.insert(rightSet.cbegin(), rightSet.cend());
}

void first_operation() {
	unordered_set<int> firstSet, stackSet;

	for (int symbol = -1; symbol <= boundNInt; ++symbol) {
		firstSet.clear();
		stackSet.clear();
		first_symbol(firstSet, symbol, stackSet);
		firstMap.emplace(symbol, firstSet);
	}
}

void first_string(unordered_set<int>& inputSet, const vector<int>& symbolsVec) {
	int i = 1;
	if (symbolsVec.size() == 0) {
		inputSet.insert(-1);
		return;
	}
	while (i <= symbolsVec.size()) {//�������һ���������epsilon
		auto & tempRef = firstMap[symbolsVec[i - 1]];
		if (tempRef.find(-1) == tempRef.end()) {
			intersection(inputSet, tempRef);
			break;
		}
		if (i == symbolsVec.size()) {//���һ���epsilon
			inputSet.insert(-1);
			break;
		}
		tempRef.erase(-1);
		intersection(inputSet, tempRef);
		++i;
	}
}


