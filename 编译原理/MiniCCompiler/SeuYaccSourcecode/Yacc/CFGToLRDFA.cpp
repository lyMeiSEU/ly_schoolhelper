#include "stdafx.h"
#include "structs.h"
using std::map;
using std::stack;
using std::queue;
using std::cout;


static void epsilon_clousure(unordered_set<LRItem> &lrStateSet);
static void subset_construct(const unordered_set<LRItem> &lrStateSet, map<int, unordered_set<LRItem> >& newStateMap);

void first_string(unordered_set<int>& inputSet, const vector<int>& symbols);

extern int boundTInt, boundNInt;
extern map<int, pair<int, int> > indexMap;
extern ProducerVec producerVec;
extern LRDFA lrdfa;


void cfg_to_lrdfa() {
	LRState I0;
	LRItem initProducer;
	int stateNumCounter = 0;
	initProducer.predictiveSymbol=-2;//-2 stands for $R
	initProducer.gramarInt = 1;
	I0.LRItemsSet.insert(initProducer);
	epsilon_clousure(I0.LRItemsSet);
	I0.numberInt = stateNumCounter++;
	lrdfa.statesVec.push_back(I0);
	queue<int> unhandledStates;
	unhandledStates.push(0);
	map<int, unordered_set<LRItem> > newStateMap;
	while (!unhandledStates.empty()) {
		//cout << "remain"<< unhandledStates .size()<<"\n";
		int top = unhandledStates.front();
		unhandledStates.pop();
		newStateMap.clear();
		//cout << "SUBSET\n";
		subset_construct(lrdfa.statesVec[top].LRItemsSet, newStateMap);
		//cout << "EPSILON\n";
		for (auto & p : newStateMap) {
			epsilon_clousure(p.second);
			int edgeToInt = -1;
			// ����Ƿ������ͬ��״̬
			for (const auto &s : lrdfa.statesVec) {
				if (s.LRItemsSet == p.second) {
					edgeToInt = s.numberInt;
					break;
				}
			}
			if (edgeToInt == -1)//������Ҫ�½�״̬��
			{
				//cout << "NEW STATE: "<<stateNumCounter << "\n";
				LRState newState;
				lrdfa.statesVec.push_back(newState);
				edgeToInt = stateNumCounter;
				unhandledStates.push(stateNumCounter);
				lrdfa.statesVec.back().numberInt = stateNumCounter++;
				lrdfa.statesVec.back().LRItemsSet = p.second;
			}
			lrdfa.statesVec[top].edgesMap.emplace(p.first, edgeToInt);//���ϱ�
		}
	}
}

void epsilon_clousure(unordered_set<LRItem> &lrStateSet) {
	// TODO �Ż�
	queue<LRItem> queue;
	for (const auto &lrItem : lrStateSet) {//����ʽ�ȶ���ջ
		queue.push(lrItem);
	}
	pair<int, vector<int> > producer;
	int position, symbol;
	int predictiveSymbol;
	unordered_set<int> predictiveSymbolSet;
	while (!queue.empty()) {//ջ��Ϊ��ʱ
		producer = producerVec[queue.front().gramarInt];//Ҫ����Ĳ���ʽ
		position = queue.front().positionInt;//Ҫ����ķ��ŵ�λ��
		if (producer.second.size() == position) {//����ĩβ��������һ������ʽ
			queue.pop();
			continue;
		}else {
			symbol = producer.second[position];//Ҫ����ķ���
			if (symbol <= boundTInt) {//���ս����û�бհ���������һ��
				queue.pop();
				continue;
			}
			//���ҵ����ж�Ӧ�Ĳ���ʽ
			auto index = indexMap.find(symbol)->second;
			predictiveSymbol = queue.front().predictiveSymbol;
			queue.pop();
			for (int i = 0; i < index.second; ++i) {//�������в���ʽ���½�LRItem
				LRItem newItem;
				newItem.gramarInt = index.first + i;
				//��Ԥ���
				vector<int> nextSymbolsVec;
				for (int i = position+1; i < producer.second.size(); ++i) {
					nextSymbolsVec.push_back(producer.second[i]);
				}
				predictiveSymbolSet.clear();
				first_string(predictiveSymbolSet, nextSymbolsVec);
				if (predictiveSymbolSet.find(-1)!= predictiveSymbolSet.end()) {//��epsilon
					predictiveSymbolSet.erase(-1);
					predictiveSymbolSet.insert(predictiveSymbol);
				}
				for (const auto & p : predictiveSymbolSet) {//����first�е�ÿһ��Ԥ���
					//TODO һ���Ĳ���ʽû�кϲ�
					newItem.predictiveSymbol = p;
					auto findIt = lrStateSet.find(newItem);
					if (findIt != lrStateSet.end()) {//����һ�����ˣ� �Ͳ��ٴ���
						continue;
					}
					queue.push(newItem);
					lrStateSet.insert(newItem);
				}

			}
		}
	}
}


void subset_construct(const unordered_set<LRItem> &lrStateSet, map<int, unordered_set<LRItem> >& newStateMap) {
	
	pair<int, vector<int> > producer;
	LRItem newItem;
	int edge;
	for (const auto &lrItem : lrStateSet) {//����ÿһ������ʽ
		producer = producerVec[lrItem.gramarInt];
		if (producer.second.size() == lrItem.positionInt)//����ĩβ��������һ������ʽ
			continue;
		edge = producer.second[lrItem.positionInt];
		newItem = lrItem;
		newItem.positionInt++;
		auto findIt = newStateMap.find(edge);
		if (findIt == newStateMap.end()) {//û�б������ñߣ��½�һ��������
			unordered_set<LRItem> newState;
			newState.insert(newItem);//������µļӵ�������
			newStateMap.emplace(edge, newState);
		}else {//����������ñߣ����µ�lritem�ӵ����еļ�����
			findIt->second.insert(newItem);
		}
	}
}