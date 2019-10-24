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
			// 检查是否存在相同的状态
			for (const auto &s : lrdfa.statesVec) {
				if (s.LRItemsSet == p.second) {
					edgeToInt = s.numberInt;
					break;
				}
			}
			if (edgeToInt == -1)//不存在要新建状态了
			{
				//cout << "NEW STATE: "<<stateNumCounter << "\n";
				LRState newState;
				lrdfa.statesVec.push_back(newState);
				edgeToInt = stateNumCounter;
				unhandledStates.push(stateNumCounter);
				lrdfa.statesVec.back().numberInt = stateNumCounter++;
				lrdfa.statesVec.back().LRItemsSet = p.second;
			}
			lrdfa.statesVec[top].edgesMap.emplace(p.first, edgeToInt);//连上边
		}
	}
}

void epsilon_clousure(unordered_set<LRItem> &lrStateSet) {
	// TODO 优化
	queue<LRItem> queue;
	for (const auto &lrItem : lrStateSet) {//产生式先都入栈
		queue.push(lrItem);
	}
	pair<int, vector<int> > producer;
	int position, symbol;
	int predictiveSymbol;
	unordered_set<int> predictiveSymbolSet;
	while (!queue.empty()) {//栈不为空时
		producer = producerVec[queue.front().gramarInt];//要处理的产生式
		position = queue.front().positionInt;//要处理的符号的位置
		if (producer.second.size() == position) {//点在末尾，处理下一个产生式
			queue.pop();
			continue;
		}else {
			symbol = producer.second[position];//要处理的符号
			if (symbol <= boundTInt) {//是终结符，没有闭包，处理下一个
				queue.pop();
				continue;
			}
			//先找到所有对应的产生式
			auto index = indexMap.find(symbol)->second;
			predictiveSymbol = queue.front().predictiveSymbol;
			queue.pop();
			for (int i = 0; i < index.second; ++i) {//对于所有产生式，新建LRItem
				LRItem newItem;
				newItem.gramarInt = index.first + i;
				//求预测符
				vector<int> nextSymbolsVec;
				for (int i = position+1; i < producer.second.size(); ++i) {
					nextSymbolsVec.push_back(producer.second[i]);
				}
				predictiveSymbolSet.clear();
				first_string(predictiveSymbolSet, nextSymbolsVec);
				if (predictiveSymbolSet.find(-1)!= predictiveSymbolSet.end()) {//有epsilon
					predictiveSymbolSet.erase(-1);
					predictiveSymbolSet.insert(predictiveSymbol);
				}
				for (const auto & p : predictiveSymbolSet) {//对于first中的每一个预测符
					//TODO 一样的产生式没有合并
					newItem.predictiveSymbol = p;
					auto findIt = lrStateSet.find(newItem);
					if (findIt != lrStateSet.end()) {//存在一样的了， 就不再处理
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
	for (const auto &lrItem : lrStateSet) {//对于每一个产生式
		producer = producerVec[lrItem.gramarInt];
		if (producer.second.size() == lrItem.positionInt)//点在末尾，处理下一个产生式
			continue;
		edge = producer.second[lrItem.positionInt];
		newItem = lrItem;
		newItem.positionInt++;
		auto findIt = newStateMap.find(edge);
		if (findIt == newStateMap.end()) {//没有遍历过该边，新建一个集合先
			unordered_set<LRItem> newState;
			newState.insert(newItem);//把这个新的加到集合里
			newStateMap.emplace(edge, newState);
		}else {//如果遍历过该边，把新的lritem加到已有的集合中
			findIt->second.insert(newItem);
		}
	}
}