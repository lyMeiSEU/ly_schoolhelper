#pragma once
#ifndef STRUCT_H
#define STRUCT_H

#include "stdafx.h"
using std::vector;
using std::multimap;
using std::string;
using std::pair;
using std::set;
using std::unordered_set;
using std::unordered_map;
using std::unordered_multimap;

typedef vector<pair<string, vector<string> > > ProducerVecStr;//×Ö·û°æ±¾£¬ÓÃ×öÏÔÊ¾<×ó£¬ÓÒ>

typedef vector<pair<int, vector<int> > > ProducerVec;//<×ó£¬ÓÒ>


typedef struct LRItem {
	int positionInt = 0;
	int gramarInt = -1;
	int predictiveSymbol;
	inline bool operator==(const LRItem& rLRItem) const {
		if (positionInt != rLRItem.positionInt) return false;
		if (gramarInt != rLRItem.gramarInt) return false;
		if (predictiveSymbol != rLRItem.predictiveSymbol) return false;
		else return true;
	}
}LRItem;


namespace std {

	template<>
	struct hash<LRItem> {
		inline size_t operator()(const LRItem& s) const {
			return  hash<int>()(s.gramarInt);
		}
	};
}


typedef struct {
	int numberInt = -1;//×´Ì¬ºÅ
	unordered_map<int, int> edgesMap;//<ÖÕ½á·û£¬×´Ì¬ºÅ>
	unordered_set<LRItem> LRItemsSet;
}LRState;


typedef struct {
	int startState = 0;
	vector<LRState > statesVec;//´æ´¢ËùÓÐµÄLRState
}LRDFA;

#endif // !STRUCT_H
