#include "stdafx.h"
#include "structs.h"
#include "GlobalData.hpp"

using std::stack;
using std::vector;
using std::map;
using std::set;
using std::string;
using std::isalnum;
using std::isalpha;
using std::isdigit;
using std::cout;

static void handle_escape(string& exp, bool in);
static void replace_brace(string& exp, const map<string, string>& reMap);
static void construct_char_set(set<char> &s, const string &content, bool n);
static void replace_square_brace(string& exp);
static void handle_quote(string& exp);
static void replace_dot(string &exp);
static void replace_question_and_add(string& exp);
static void add_dot(string &exp);


//�滻{X}
void replace_brace(string& exp, const map<string, string>& reMap) {
	string rename;
	vector<char> charVec;
	bool inBrace = false;
	auto expIt = exp.begin();
	while (expIt != exp.end()) {
		if ((*expIt) == '{'&&((expIt) == exp.begin()|| *(expIt-1) != '`')) {
			inBrace = true;
			rename = "";
			++expIt;
			continue;
		}
		else if ((*expIt) == '}' && ((expIt) == exp.begin() || *(expIt - 1) != '`') &&inBrace) {
			inBrace = false;
			auto findIt = reMap.find(rename);
			if (findIt != reMap.end()) {//�ҵ��ˣ���ԭ���ĳ�ջ���ҵ�����ջ
				for (const char& c : findIt->second) {
					charVec.push_back(c);
				}
			}
			else {
				throw(""); //UNDONE: �����Ҳ���ǰ���name
			}
			++expIt;
			continue;
		}
		else if (inBrace) {
			rename += *expIt;
			++expIt;
			continue;
		}
		charVec.push_back(*expIt);
		++expIt;
	}

	//���vector������
	exp = "";
	for (const auto &e : charVec) {
		exp += e;
	}
	//cout << "REPLACE BRACE:"<<exp << "\n";
}

// �滻[X]
void construct_char_set(set<char> &s, const string &content, bool n) {
	string stemp(content);
	handle_escape(stemp, true);
	//����[a-z]
	auto it = stemp.cbegin();
	set<char> temp;
	while (it != stemp.cend()) {
		if (*it == '-' && ((it) != stemp.cbegin()) && ((it + 1) != stemp.cend()) && isalnum(*(it - 1)) && isalnum(*(it + 1))) {
			//��ǰ�ַ���-��ǰ�����ַ����Ҷ�����ĸ������
			auto sit = ALLSET.find(*(it - 1)) + 1;
			auto eit = ALLSET.find(*(it + 1));
			if (eit >= sit) {
				// UNDONE: ����[]�ڲ���ʽ����
			}
			while (sit != eit) {
				temp.insert(ALLSET[sit]);
				++sit;
			}
			++it;
		}
		else {
			temp.insert(*it);//�����������
			++it;
		}
	}
	if (n) {//����ȡ��
		for (const char &c : ALLSET) {
			if (temp.find(c) == temp.end()) {
				s.insert(c);
			}
		}
	}
	else {
		s = temp;
	}
}

void replace_square_brace(string& exp) {
	string sbcontent;
	vector<char> charVec;
	bool inSquareBrackes = false;
	auto expIt = exp.begin();
	while (expIt != exp.end()) {
		if ((*expIt) == '[' && ((expIt) == exp.begin() || *(expIt - 1) != '`')) {
			inSquareBrackes = true;
			sbcontent = "";
			++expIt;
			continue;
		}
		else if ((*expIt) == ']' &&inSquareBrackes && ((expIt) == exp.begin() || *(expIt - 1) != '`')) {
			inSquareBrackes = false;
			charVec.push_back('(');
			set<char> s;//����ת�������ַ�
			if (sbcontent[0] == '^') { // �ǲ���
				construct_char_set(s, sbcontent.substr(1, sbcontent.size() - 1), true);
			}
			else {
				construct_char_set(s, sbcontent, false);
			}
			for (const auto &c : s) {
				if(ESCAPEDCHARS.find(c)!= ESCAPEDCHARS.cend())//ת��
					charVec.push_back('`');
				charVec.push_back(c);
				charVec.push_back('|');
			}
			charVec.pop_back();
			charVec.push_back(')');
			++expIt;
			continue;
		}
		else if (inSquareBrackes) {
			sbcontent += *expIt;
			++expIt;
			continue;
		}
		
		charVec.push_back(*expIt);
		++expIt;
	}
	//���vector������

	exp = "";
	for (const auto &e : charVec) {
		exp += e;
	}
	//cout << "REPLACE SB:"<<exp << "\n";
}


// ��������
void handle_quote(string& exp) {
	bool inQuote = false;
	const auto sexpIt = exp.begin();
	auto expIt = sexpIt;
	vector<char> charVec;
	while (expIt != exp.end()) {
		if (*expIt == '\"'&&( (expIt != sexpIt && *(expIt - 1) != '\\')|| (expIt == sexpIt))) {// ��",�Ҳ���\"
			if (!inQuote) {
				inQuote = true;
				++expIt;
				continue;
			}
			else {
				inQuote = false;
				++expIt;
				continue;
			}
		}
		else if(inQuote && ESCAPEDCHARS.find(*expIt) != ESCAPEDCHARS.cend()){ //ת��������߿��ܵĲ�������Ҫ��ת��
			charVec.push_back('`');
		}
		charVec.push_back(*expIt);
		++expIt;
	}
	exp = "";
	for (const auto &c : charVec) {
		exp += c;
	}
	//cout << "HANDLE QUOTE:"<<exp << "\n";
}

//����\ת���ַ�
void handle_escape(string& exp,bool in) {
	string stemp;
	bool flag = false;
	for (auto &c : exp) {
		if (in) {//ת��[]�ڵ����п����ַ�
			if (flag) {
				switch (c) {
				case 'n':
					stemp += '\n';
					break;
				case 't':
					stemp += '\t';
					break;
				case 'v':
					stemp += '\v';
					break;
				case 'f':
					stemp += '\f';
					break;
				case '\\':
					stemp += '\\';
					break;
				}
				flag = false;
				continue;
			}
			if (c == '\\') {
				flag = true;
				continue;
			}
		}
		else {/*��[]�⣬ֻת�� \"��\\*/
			if (flag) {
				flag = false;
				if (c == '\"' || c == '\\') stemp = stemp.substr(0, stemp.size() - 1);
			}else if (c == '\\') {
				flag = true;
			}
		}
		stemp += c;
	}
	exp = stemp;
	//cout << "HANDLE ESCAPE:" << exp << "\n";
}

//����. (ƥ�����\n֮�����е����ַ�)
void replace_dot(string &exp) {
	vector<char> charVec;
	auto expIt = exp.begin();
	while (expIt != exp.end()) {
		if (*expIt == '.' && (expIt== exp.begin() || (expIt != exp.begin() && *(expIt-1)!='`'))){
			charVec.push_back('(');
			set<char> s;//����ת�������ַ�
			construct_char_set(s, "\n", true);
			for (const auto &c : s) {
				if (ESCAPEDCHARS.find(c) != ESCAPEDCHARS.cend())//ת��
					charVec.push_back('`');
				charVec.push_back(c);
				charVec.push_back('|');
			}
			charVec.pop_back();
			charVec.push_back(')');
			++expIt;
			continue;
		}
		charVec.push_back(*expIt);
		++expIt;
	}
	//���vector������

	exp = "";
	for (const auto &e : charVec) {
		exp += e;
	}
	//cout << "REPLACE DOT:" << exp << "\n";
}

// �滻����+ (ͬʱ��������)
void replace_question_and_add(string& exp){
	vector<char> charVec;
	auto expIt = exp.begin();
	while (expIt != exp.end()) {
		if ((*expIt == '+' || *expIt == '?') && (expIt == exp.begin() || (expIt != exp.begin() && *(expIt - 1) != '`'))) {
			int counter = 0;
			if ((expIt) != exp.begin() && *(expIt - 1) == ')' && (((expIt-1) != exp.begin()&& *(expIt - 2)!='`')|| (expIt - 1) == exp.begin())){//���ǰ����)�Ҳ���`�����ҵ��������ŵĲ���
				const auto preExpIt = expIt;//��¼֮ǰ+/?��λ��	
				--expIt;
				charVec.pop_back();
				++counter;
				do{
					charVec.pop_back();
					--expIt;
					if (*expIt == '(' && (((expIt) != exp.begin() && *(expIt - 1) != '`') || (expIt) == exp.begin())) {//��(�Ҳ���`(
						--counter;
					}
					else if (*expIt == ')' && (((expIt) != exp.begin() && *(expIt - 1) != '`') || (expIt) == exp.begin())) {//��)�Ҳ���`)
						++counter;
					}
				} while (counter > 0);
				if (*preExpIt == '?') { //a? -> (@|a)
					charVec.push_back('(');
					charVec.push_back('@');
					charVec.push_back('|');
					while (expIt != preExpIt) {
						charVec.push_back(*expIt);
						++expIt;
					}
					charVec.push_back(')');
				}
				else { //a+ -> aa*
					const auto startIt = expIt;
					for (int i = 0; i < 2; i++) {
						expIt = startIt;
						while (expIt != preExpIt) {
							charVec.push_back(*expIt);
							++expIt;
						}
					}
					charVec.push_back('*');
				}
			}
			else {
				char c = charVec.back();
				if (*expIt == '?') { //a? -> (@|a)
					charVec.pop_back();
					charVec.push_back('(');
					charVec.push_back('@');
					charVec.push_back('|');
					charVec.push_back(c);
					charVec.push_back(')');
				}
				else {//a+ -> aa*
					charVec.push_back(c);
					charVec.push_back('*');
				}
			}
			++expIt;
			while (expIt != exp.end()) {
				charVec.push_back(*expIt);//�����ֱ�ӽ���ȥ
				++expIt;
			}
			exp = "";
			for (const auto &e : charVec) {
				exp += e;
			}
			charVec.clear();
			expIt = exp.begin();//��ͷ��ʼ
			continue;
		}
		charVec.push_back(*expIt);
		++expIt;
	}
	//���vector������

	exp = "";
	for (const auto &e : charVec) {
		exp += e;
	}
	//cout << "REPLACE QS&A:" << exp << "\n";
}


// �ӵ�
void  add_dot(string &exp) {
	string oldExp = exp;
	string dotedExp;
	for (auto oldExpIt = oldExp.cbegin(); oldExpIt != oldExp.cend(); ++oldExpIt) {
		dotedExp += (*oldExpIt);
		if (*oldExpIt == '`')continue;// ��ת���ַ���ֱ������
		if ((*oldExpIt == '(' || *oldExpIt == '|' )&& (oldExpIt == oldExp.cbegin() || *(oldExpIt - 1) != '`'))
			continue;// |��(,��ǰ�治��`
		if ((oldExpIt + 1) == oldExp.cend()) continue; //���һ���ַ������ӵ�
		if (*(oldExpIt + 1) == '|' || *(oldExpIt + 1) == '*' ||  *(oldExpIt + 1) == ')') continue; //��һ���ַ��Ѿ��ǲ������������ţ����ӵ�
		dotedExp += '.';//�ӵ�
	}
	exp = dotedExp;
	//cout << "ADD DOT:" << exp << "\n";
}

void re_standardize(vector<Rules>& reVec, map<string, string>& reMap) {
	//�ȴ���map�����
	for (auto & p : reMap) {
		handle_quote(p.second);
		replace_brace(p.second, reMap);
	}

	//�ٴ���Vector�����
	for (auto & e : reVec) {
		handle_quote(e.pattern);
		replace_brace(e.pattern, reMap);
		replace_square_brace(e.pattern);
		replace_dot(e.pattern);
		replace_question_and_add(e.pattern);
		handle_escape(e.pattern, false);
		add_dot(e.pattern);
	}
	
}