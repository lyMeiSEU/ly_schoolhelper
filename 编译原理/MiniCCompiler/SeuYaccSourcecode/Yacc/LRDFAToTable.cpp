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
extern ProducerVec producerVec;//用于编号下表寻址
extern vector<unordered_set<int> > precedenceRulesVec;//移进规约表
extern ProducerVecStr producerVecStr;
extern map<int, int> intsMap;
extern map<string, int> tokensDefineMap;



//最后要返回的vec,自己extern嗷
vector<pair<void*, int> > table_vec;
vector<string> productions_vec;
extern LRDFA lalrdfa;

void lrdfa_to_table() {
	auto & lrdfa = lalrdfa;
	//base表通过状态号得到对应一维数组的索引位置。
	int* base = new int[lrdfa.statesVec.size()+1];
	memset(base,0, lrdfa.statesVec.size() * sizeof(int));
	//next通过base表的值+终结或者非终结符取值得到下一状态,移进大于0，规约小于0
	int* next = new int[(boundNInt + 2)*lrdfa.statesVec.size()];
	memset(next, 0, (boundNInt + 2)*lrdfa.statesVec.size() * sizeof(int));//0表示出错
	//装所有的产生式
	int* producer_data = new int[5*producerVec.size()];
	memset(producer_data, 0, 5 * producerVec.size() * sizeof(int));
	//通过产生式标号获取产生式，[2*产生式标号]=首地址，[2*产生式标号+1]=长度(长度包括左边的那个)，用在producer_data中查找。
	int* index = new int[2* producerVec.size()];
	memset(index, 0, 2 * producerVec.size() * sizeof(int));
	//反向翻译表，索引为production号，值为string。
	string* productions = new string[boundTInt+1];
	//字符到token编号的映射
	int* map_vec = new int[defineCount];
	for (int i = 0; i < defineCount; i++) {
		map_vec[i] = -1;//-1表示没有定义该字符
	}
	


	//遍历存储所有的产生式,构建producer_data、index表
	int count = 0;//记录存储到哪个位置了
	for (int i = 1; i < producerVec.size();++i) {
		producer_data[count] = producerVec[i].first;//记录第一个
		index[2 * i] = count;
		index[2 * i + 1] = producerVec[i].second.size() + 1;
		++count;
		//把右边的给安排上
		for (auto rightIt = producerVec[i].second.begin(); rightIt != producerVec[i].second.end(); rightIt++) {
			producer_data[count++] = *rightIt;
		}

	}
	//构建反向翻译表
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

	//遍历所有状态操作，构建next、base表
	for (int i = 0; i < lrdfa.statesVec.size(); ++i) {
		base[lrdfa.statesVec[i].numberInt+1] = i * (boundNInt + 2);//相当于行号
		//对每个状态处理，相当于对每一行处理

		//先把移进和非终结符的情况安排了，先填入表中
		for (auto j = lrdfa.statesVec[i].edgesMap.begin(); j != lrdfa.statesVec[i].edgesMap.end();++j) {
			if (j->first <= boundTInt) {//终结符
					next[base[lrdfa.statesVec[i].numberInt] + j->first] = j->second;//赋值下一个状态
			}
			else {//非终结符
				next[base[lrdfa.statesVec[i].numberInt] + j->first+1] = j->second;//赋值下一个状态

			}
		}

		//再来安排规约情况,对状态中的每一产生式进行操作
		for (auto it = lrdfa.statesVec[i].LRItemsSet.begin(); it != lrdfa.statesVec[i].LRItemsSet.end();++it) {
			//如果是点在最后的项，则会有规约
			if (it->positionInt == producerVec[it->gramarInt].second.size()) {
				//取出预测分析符，预测分析符都是终结符，因此没有shift_reduce[i]=0的情况
				int predictiveSymbol = it->predictiveSymbol;
				if (predictiveSymbol == -2) { //处理$的情况
					predictiveSymbol = boundTInt + 1;
				}
				//解决移进规约冲突
				if (next[base[lrdfa.statesVec[i].numberInt] + predictiveSymbol] >0) {
					int preLine = -1,lastLine=-1;//preLine记录预测符的行数，lastLine记录每项最后一个终结符的行数。行数越大优先级越高，同一行按照左结合来解决冲突
					//找到产生式中最后一个终结符
					int last;
					for (int k = producerVec[it->gramarInt].second.size() - 1; k >= 0; k--) {
						//小于这个数就是终结符
						if (producerVec[it->gramarInt].second[k] <= boundTInt) {
							last = producerVec[it->gramarInt].second[k];
							break;
						}
					}
						
					//在规则表中定位预测符和last终结符
					for (int k = 0; k < precedenceRulesVec.size(); ++k) {
						for (auto j = precedenceRulesVec[k].begin(); j != precedenceRulesVec[k].end(); ++j) {
							//找到预测符
							if (*j == it->predictiveSymbol) {
								preLine = k;//记录优先级
							}//找到最后一个终结符
							if(last==*j){
								lastLine = k;//记录优先级
							}
						}
					}
					//如果有规则，按照规则来,没有就做移近（啥都不做）
					if (preLine!=-1&&lastLine!=-1) {
						//最后一个的终结符优先级高,改为规约，反之，做移进（做已经什么都不用做）
						if (preLine <= lastLine) {
							next[base[lrdfa.statesVec[i].numberInt] + predictiveSymbol] = -it->gramarInt;
						}
					}
				}//解决规约规约冲突
				else if (next[base[lrdfa.statesVec[i].numberInt] + predictiveSymbol]<0) {
					//比较产生式优先级，选择优先级高（号码比较小的）的做规约
					if ((-next[base[lrdfa.statesVec[i].numberInt] + predictiveSymbol]) > it->gramarInt) {
						next[base[lrdfa.statesVec[i].numberInt] + predictiveSymbol] = -it->gramarInt;
					}
				}
				else {//没有冲突，则把产生式对应的编号赋值。
					next[base[lrdfa.statesVec[i].numberInt] + predictiveSymbol] = -it->gramarInt;
				}		
			}
		}
	}


	//装载vec
	table_vec.push_back(pair<void*, int>(next, (boundNInt + 2)*lrdfa.statesVec.size()));
	table_vec.push_back(pair<void*, int>(base, lrdfa.statesVec.size() + 1));
	table_vec.push_back(pair<void*, int>(producer_data, 5 * producerVec.size()));
	table_vec.push_back(pair<void*, int>(index, 2 * producerVec.size()));
	table_vec.push_back(pair<void*, int>(map_vec, defineCount));


}