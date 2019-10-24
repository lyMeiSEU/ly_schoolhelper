#pragma once
#ifndef  _STRUCTS_H
#define  _STRUCTS_H

#define LEX_TEST 0
#define YACC_TEST 1
#include "stdafx.h" 
using std::vector;
using std::string;
using std::map;
using std::unordered_map;
using std::unordered_set;
using std::unordered_multimap;
using std::set;


//���ڹ���Ľṹ��
typedef struct {
	string pattern; //RE
	vector<string> actions;//��Ӧ��������䣩
} Rules;

//����NFA״̬�Ľṹ��
typedef struct NFAstate{
	int number = 0;//״̬���
	unordered_multimap<char, int> exEdgesMultiMap;//������,��Ϊ���ϵ�ֵ��ֵΪ��һ��״̬���
}NFAstate;//NFA�ڲ�״̬


typedef struct {
	int startState = 0;//��ʼ״̬���
	map<int, int> endStatesMap;//�洢��̬�Ͷ�Ӧ�Ķ���
	unordered_map<int,NFAstate> statesMap;//�洢��Ŷ�Ӧ״̬
}NFA;//NFA

typedef struct  {
	int number = 0;//״̬���
	unordered_set <int> identitySet;//���ֲ�ͬ״̬�õļ���
	unordered_map <char, int> exEdgesMap;//������,��Ϊ���ϵ�ֵ��ֵΪ��һ��״̬���
}DFAstate;

typedef struct {
	unordered_map< int, int > endStatesMap;//�洢��̬�Ͷ�Ӧ�Ķ���
	int startState = 0;//��ʼ״̬���
	vector<DFAstate> statesVec;//�洢��Ŷ�Ӧ״̬
}DFA;

#endif 