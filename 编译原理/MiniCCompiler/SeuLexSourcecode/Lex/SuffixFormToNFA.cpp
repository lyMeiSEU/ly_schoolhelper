#include "stdafx.h"
#include "structs.h"

using std::stack;
using std::vector;
using std::map;
using std::string;
using std::isalnum;
using std::isalpha;
using std::isdigit;
using std::cout;
using std::queue;
using std::stack;
using std::endl;
using std::multimap;
using std::pair;

int number = 0;

//����׺���ʽת��ΪNFA
void suffix_to_nfa(vector<Rules>& suffixRules,NFA& finalNfa) {
	stack<NFA> nfaStack;//���ڹ���NFA�Ĺ���ջ,Ҳ��
	for (int i = 0; i < suffixRules.size();++i) {
		string pattern = suffixRules[i].pattern; //�Ѿ���׺�����pattern
		int tempActionInt = -1;//��ʱ��̬��������
		for (auto it = pattern.cbegin() ; it != pattern.cend(); ++it)
		{
			NFA upNFA,downNFA;
			NFAstate start, end;
			char now = *it;
			switch (now) {
			case'|':
				//ȡ���������NFA���в���
				upNFA = nfaStack.top();
				nfaStack.pop();
				downNFA = nfaStack.top();
				nfaStack.pop();
				start.number = number;
				++number;
				end.number = number;
				++number;				
				//��down����̬��up����̬�����ӵ�end
				upNFA.statesMap.find(upNFA.endStatesMap.begin()->first)->
					second.exEdgesMultiMap.insert(pair<char, int>('@', end.number));
				downNFA.statesMap.find(downNFA.endStatesMap.begin()->first)->
					second.exEdgesMultiMap.insert(pair<char, int>('@', end.number));
				//��start���ӵ�down��up�ĳ�̬
				start.exEdgesMultiMap.insert(pair<char, int>('@', upNFA.startState));
				start.exEdgesMultiMap.insert(pair<char, int>('@', downNFA.startState));
				//���ӹ�ϵ����ú����map
				downNFA.statesMap.insert(pair<int, NFAstate>(start.number, start));
				downNFA.statesMap.insert(pair<int, NFAstate>(end.number, end));
				//��upNFA��stateMap������down��
				downNFA.statesMap.insert(upNFA.statesMap.begin(), upNFA.statesMap.end());
				//�޸�downNFA�ĳ�̬
				downNFA.startState = start.number;
				//�޸�downNFA����̬
				downNFA.endStatesMap.clear();
				downNFA.endStatesMap.insert(pair<int, int>(end.number, tempActionInt));
				nfaStack.push(downNFA);		
				break;
			case'*':
				//ȡ���������NFA���в���
				upNFA = nfaStack.top();
				nfaStack.pop();
				//�½�����״̬
				start.number = number;
				++number;
				end.number = number;
				++number;
				//��start��upNFA��̬����
				start.exEdgesMultiMap.insert(pair<char, int>('@', upNFA.startState));
				//��start��end����
				start.exEdgesMultiMap.insert(pair<char, int>('@', end.number));
				//��upNFA����̬�ͳ�̬����
				upNFA.statesMap.find(upNFA.endStatesMap.begin()->first)->
					second.exEdgesMultiMap.insert(pair<char, int>('@', upNFA.startState));
				upNFA.statesMap.find(upNFA.endStatesMap.begin()->first)->
					second.exEdgesMultiMap.insert(pair<char, int>('@', end.number));
				//���ĳ�̬
				upNFA.startState = start.number;
				//���ӹ�ϵ����ú����map
				upNFA.statesMap.insert(pair<int, NFAstate>(start.number, start));
				upNFA.statesMap.insert(pair<int, NFAstate>(end.number, end));
				//������̬
				upNFA.endStatesMap.clear();
				upNFA.endStatesMap.insert(pair<int, int >(end.number, tempActionInt));
				nfaStack.push(upNFA);
				break;
			case'.':
				//ȡ��ջ��������NFA
				upNFA = nfaStack.top();
				nfaStack.pop();
				downNFA = nfaStack.top();
				nfaStack.pop();
				//ȡ��downNFA����̬
				downNFA.statesMap.find(downNFA.endStatesMap.begin()->first)->second//�õ���̬
				//��ѹ������ģ�Ҳ���Ǹ�ǰ��ģ�NFA��@�����Ľ�������
				.exEdgesMultiMap.insert(pair<char, int>('@', upNFA.startState));
				//������̬
				downNFA.endStatesMap.clear();
				downNFA.endStatesMap = upNFA.endStatesMap;
				//��upNFA��״̬map������downNFA�У�
				downNFA.statesMap.insert(upNFA.statesMap.begin(),upNFA.statesMap.end());
				nfaStack.push(downNFA);
				break;
			default:
				if (now == '`') {
					++it;
					now= *it;
				}
				//�����ַ�������һ��NFA��ѹջ
				NFA pushNFA;//����ѹջ��NFA
				//�½�һ����ʼ״̬
				start.number = number;
				//���ĳ�̬
				pushNFA.startState = start.number;
				++number;
				//�½�һ����ֹ״̬
				end.number = number;
				++number;	
				//����ʼ״̬����ֹ״̬����
				start.exEdgesMultiMap.insert(pair<char, int>(now, end.number));
				//���ӹ�ϵ����ú����map
				pushNFA.statesMap.insert(pair<int, NFAstate>(start.number, start));
				pushNFA.statesMap.insert(pair<int, NFAstate>(end.number, end));
				//��ʶ��ǰ��̬������ȴ����vector��
				pushNFA.endStatesMap.insert(pair<int, int>(end.number, tempActionInt));
				//ѹջ
				nfaStack.push(pushNFA);
				break;
			}	
		}
		//��action����ջ����NFA����̬
		nfaStack.top().endStatesMap.begin()->second = i;
	}
	//���ڵõ���װ������NFA��nfaStack,�ϲ�Ϊһ�����NFA��
	finalNfa = nfaStack.top();
	NFA downNFA;
	nfaStack.pop();
	while (!nfaStack.empty()) {
		NFAstate start;
		//���Σ���ջ��NFA���NFA�ϲ�
		downNFA = nfaStack.top();
		nfaStack.pop();
		//�½�һ��start״̬
		start.number = number;
		number++;
		//��start���ӵ�finalNFA��dowNFA�ĳ�̬
		start.exEdgesMultiMap.insert(pair<char, int>('@', finalNfa.startState));
		start.exEdgesMultiMap.insert(pair<char, int>('@', downNFA.startState));
		//�޸�finalNFA����ʼ״̬
		finalNfa.startState = start.number;
		finalNfa.statesMap.insert(pair<int, NFAstate>(start.number, start));
		//���finalNFA����ֹ״̬
		finalNfa.endStatesMap.insert(downNFA.endStatesMap.begin(), downNFA.endStatesMap.end());
		//��downNFA��״̬map������finalNFA��
		finalNfa.statesMap.insert(downNFA.statesMap.begin(), downNFA.statesMap.end());
	}
}