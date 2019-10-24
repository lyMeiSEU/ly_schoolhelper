#include"stdafx.h"
#include"structs.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::pair;
using std::map;
using std::pair;

extern int boundNInt;
extern int boundTInt;
extern int defineCount;
extern ProducerVec producerVec;//���ڱ���±�Ѱַ
extern vector<unordered_set<int> > precedenceRulesVec;//�ƽ���Լ��
extern ProducerVecStr producerVecStr;
extern map<int, int> intsMap;
extern map<string, int> tokensDefineMap;



//���Ҫ���ص�vec,�Լ�extern�
vector<pair<void*, int> > table_vec;
vector<string> productions_vec;
extern LRDFA lalrdfa;

void lrdfa_to_table() {
	auto & lrdfa = lalrdfa;
	//base��ͨ��״̬�ŵõ���Ӧһά���������λ�á�
	int* base = new int[lrdfa.statesVec.size()+1];
	memset(base,0, lrdfa.statesVec.size() * sizeof(int));
	//nextͨ��base���ֵ+�ս���߷��ս��ȡֵ�õ���һ״̬,�ƽ�����0����ԼС��0
	int* next = new int[(boundNInt + 2)*lrdfa.statesVec.size()];
	memset(next, 0, (boundNInt + 2)*lrdfa.statesVec.size() * sizeof(int));//0��ʾ����
	//װ���еĲ���ʽ
	int* producer_data = new int[5*producerVec.size()];
	memset(producer_data, 0, 5 * producerVec.size() * sizeof(int));
	//ͨ������ʽ��Ż�ȡ����ʽ��[2*����ʽ���]=�׵�ַ��[2*����ʽ���+1]=����(���Ȱ�����ߵ��Ǹ�)������producer_data�в��ҡ�
	int* index = new int[2* producerVec.size()];
	memset(index, 0, 2 * producerVec.size() * sizeof(int));
	//�����������Ϊproduction�ţ�ֵΪstring��
	string* productions = new string[boundTInt+1];
	//�ַ���token��ŵ�ӳ��
	int* map_vec = new int[defineCount];
	for (int i = 0; i < defineCount; i++) {
		map_vec[i] = -1;//-1��ʾû�ж�����ַ�
	}
	


	//�����洢���еĲ���ʽ,����producer_data��index��
	int count = 0;//��¼�洢���ĸ�λ����
	for (int i = 1; i < producerVec.size();++i) {
		producer_data[count] = producerVec[i].first;//��¼��һ��
		index[2 * i] = count;
		index[2 * i + 1] = producerVec[i].second.size() + 1;
		++count;
		//���ұߵĸ�������
		for (auto rightIt = producerVec[i].second.begin(); rightIt != producerVec[i].second.end(); rightIt++) {
			producer_data[count++] = *rightIt;
		}

	}
	//�����������
	for (int i = 0; i < producerVecStr.size(); ++i) {
		string temp_str= producerVecStr[i].first + " ->";
		//productions[i] = 
		for (auto it = producerVecStr[i].second.begin(); it != producerVecStr[i].second.end(); it++) {
			temp_str += " ";
			temp_str += *it;
		}
		productions_vec.push_back(temp_str);
	}
	

	for (auto& it : intsMap) {
			map_vec[it.first] = it.second;
	}

	//��������״̬����������next��base��
	for (int i = 0; i < lrdfa.statesVec.size(); ++i) {
		base[lrdfa.statesVec[i].numberInt+1] = i * (boundNInt + 2);//�൱���к�
		//��ÿ��״̬�����൱�ڶ�ÿһ�д���

		//�Ȱ��ƽ��ͷ��ս������������ˣ����������
		for (auto j = lrdfa.statesVec[i].edgesMap.begin(); j != lrdfa.statesVec[i].edgesMap.end();++j) {
			if (j->first <= boundTInt) {//�ս��
					next[base[lrdfa.statesVec[i].numberInt] + j->first] = j->second;//��ֵ��һ��״̬
			}
			else {//���ս��
				next[base[lrdfa.statesVec[i].numberInt] + j->first+1] = j->second;//��ֵ��һ��״̬

			}
		}

		//�������Ź�Լ���,��״̬�е�ÿһ����ʽ���в���
		for (auto it = lrdfa.statesVec[i].LRItemsSet.begin(); it != lrdfa.statesVec[i].LRItemsSet.end();++it) {
			//����ǵ������������й�Լ
			if (it->positionInt == producerVec[it->gramarInt].second.size()) {
				//ȡ��Ԥ���������Ԥ������������ս�������û��shift_reduce[i]=0�����
				int predictiveSymbol = it->predictiveSymbol;
				if (predictiveSymbol == -2) { //����$�����
					predictiveSymbol = boundTInt + 1;
				}
				//����ƽ���Լ��ͻ
				if (next[base[lrdfa.statesVec[i].numberInt] + predictiveSymbol] >0) {
					int preLine = -1,lastLine=-1;//preLine��¼Ԥ�����������lastLine��¼ÿ�����һ���ս��������������Խ�����ȼ�Խ�ߣ�ͬһ�а��������������ͻ
					//�ҵ�����ʽ�����һ���ս��
					int last;
					for (int k = producerVec[it->gramarInt].second.size() - 1; k >= 0; k--) {
						//С������������ս��
						if (producerVec[it->gramarInt].second[k] <= boundTInt) {
							last = producerVec[it->gramarInt].second[k];
							break;
						}
					}
						
					//�ڹ�����ж�λԤ�����last�ս��
					for (int k = 0; k < precedenceRulesVec.size(); ++k) {
						for (auto j = precedenceRulesVec[k].begin(); j != precedenceRulesVec[k].end(); ++j) {
							//�ҵ�Ԥ���
							if (*j == it->predictiveSymbol) {
								preLine = k;//��¼���ȼ�
							}//�ҵ����һ���ս��
							if(last==*j){
								lastLine = k;//��¼���ȼ�
							}
						}
					}
					//����й��򣬰��չ�����,û�о����ƽ���ɶ��������
					if (preLine!=-1&&lastLine!=-1) {
						//���һ�����ս�����ȼ���,��Ϊ��Լ����֮�����ƽ������Ѿ�ʲô����������
						if (preLine <= lastLine) {
							next[base[lrdfa.statesVec[i].numberInt] + predictiveSymbol] = -it->gramarInt;
						}
					}
				}//�����Լ��Լ��ͻ
				else if (next[base[lrdfa.statesVec[i].numberInt] + predictiveSymbol]<0) {
					//�Ƚϲ���ʽ���ȼ���ѡ�����ȼ��ߣ�����Ƚ�С�ģ�������Լ
					if ((-next[base[lrdfa.statesVec[i].numberInt] + predictiveSymbol]) > it->gramarInt) {
						next[base[lrdfa.statesVec[i].numberInt] + predictiveSymbol] = -it->gramarInt;
					}
				}
				else {//û�г�ͻ����Ѳ���ʽ��Ӧ�ı�Ÿ�ֵ��
					next[base[lrdfa.statesVec[i].numberInt] + predictiveSymbol] = -it->gramarInt;
				}		
			}
		}
	}


	//װ��vec
	table_vec.push_back(pair<void*, int>(next, (boundNInt + 2)*lrdfa.statesVec.size()));
	table_vec.push_back(pair<void*, int>(base, lrdfa.statesVec.size() + 1));
	table_vec.push_back(pair<void*, int>(producer_data, 5 * producerVec.size()));
	table_vec.push_back(pair<void*, int>(index, 2 * producerVec.size()));
	table_vec.push_back(pair<void*, int>(map_vec, defineCount));


}