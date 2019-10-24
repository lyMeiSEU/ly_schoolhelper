#ifndef _SUPPORT_FUNCTION_H
#define _SUPPORT_FUNCTION_H

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <iomanip>

extern std::vector<SymbolTable*> symbolTables;
extern SymbolTable* globalTable;
SymbolTable* currentTable;

extern std::vector<Quadruple> middleCode;
int nextInstr = 0;
std::map<int, std::string> labelMap;

typedef const std::string& strType;

// create newline in corresponding symbol table, and fill current variable's information
std::string enter(strType name, strType type, unsigned int offset, bool placeFlag = true) {
	if (name == "#") {
		return currentTable->enter(name, type, offset, false);
	}
	else if (currentTable->_funName == "global") {
		return globalTable->enter(name, type, offset, false);
	}
	else {
		return currentTable->enter(name, type, offset, placeFlag);
	}
}

void createSymbolTable(strType name, unsigned int returnSize) {
	if (globalTable->_endIndex < 0) globalTable->_endIndex = nextInstr;
	currentTable = new SymbolTable(name);
	currentTable->_returnSize = returnSize;
	currentTable->_beginIndex = nextInstr;
}

void returnToGlobalTable() {
	currentTable->_endIndex = nextInstr;
	symbolTables.push_back(currentTable);
	currentTable = globalTable;
}

void addFunLabel(int index, strType label) {
	labelMap.insert(std::make_pair(index, label));
}

void addToSymbolTable(strType itemlist) {

	std::string sep = ",;";
	std::string::size_type pos_begin = itemlist.find_first_not_of(sep);
	std::string::size_type comma_pos = 0;
	std::string strName, strType, strOffset;

	while (pos_begin != std::string::npos) {
		comma_pos = itemlist.find_first_of(sep, pos_begin);
		if (comma_pos != std::string::npos) {
			strName = itemlist.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + 1;
		}
		comma_pos = itemlist.find_first_of(sep, pos_begin);
		if (comma_pos != std::string::npos) {
			strType = itemlist.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + 1;
		}
		comma_pos = itemlist.find_first_of(sep, pos_begin);
		if (comma_pos != std::string::npos) {
			strOffset = itemlist.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + 1;
		}
		currentTable->enterParam(strName, strType, std::atoi(strOffset.c_str()));
		pos_begin = itemlist.find_first_not_of(sep, pos_begin);
	}
}

void setOutLiveVar(strType name) {
	currentTable->at(name)._live = true;
}

void outputSymbolTable(SymbolTable* table, std::ofstream& out) {
	out << std::endl << table->_funName << std::endl;
	out << "return size = " << table->_returnSize << std::endl;
	for (auto& map : table->_field) {
		out << std::setw(6) << map._name
			<< std::setw(8) << map._place
			<< std::setw(6) << map._type
			<< std::setw(6) << map._offset
			<< std::setw(6) << map._space
			<< std::endl;
	}
}

// form newline of parameter information into symbol table
std::string makeParam(strType name, strType type, unsigned int offset) {
	return name + "," + type + "," + std::to_string(offset) + ";";
}

static unsigned int maxTemp = 0;
// generate a new temp variable, return its place
std::string newtemp(std::string var) {
	std::string name = "T" + std::to_string(maxTemp++);
	return enter(name, currentTable->at(var)._type, currentTable->at(var)._space);
}

std::string addNum(std::string str) {
	enter(str,"num", 0, false);
	return str;
}

// look up variable name in symbol table
std::string lookupPlace(strType name) {
	if (name == "#") return name;
	for (auto& it : currentTable->_field) {
		if (it._name == name) {
			return it._place;
		}
	}
	for (auto& it : globalTable->_field) {
		if (it._name == name) {
			return name;
		}
	}
	std::cerr << name << " is not in symbol table " << std::endl;
	return name;
	/*if (name == "#") return name;

	if (currentTable->has(name)) return name;
	else if (globalTable->has(name)) return name;
	else return "";*/
}

static unsigned int maxLabel = 0;
// generator a new label for object code
std::string newlabel() { 
	return "Label_" + std::to_string(maxLabel++);
}

// generator a array
std::string array(int a, strType type) {
	return "array(" + std::to_string(a) + "," + type + ")";
}

std::string gen(int a) {
	return std::to_string(a);
}

void addLeader(int leaderIndex) {
	currentTable->_leaders.insert(leaderIndex);
}

void emit(strType op, strType arg1, strType arg2, strType des) {

	middleCode.push_back(Quadruple(op, arg1, arg2, des));
	++nextInstr;

	if (middleCode.back()._type == 20) {
		addLeader(nextInstr);
		addLeader(nextInstr-1);
	}
	else if (20 < middleCode.back()._type && middleCode.back()._type < 27) {
		addLeader(nextInstr-1);
	}
	
}

void error() {
	exit(1);
}

// create a new list containing only i, an index into the array of instrutions
std::string makelist(int i) {
	return std::to_string(i) + ";";
}

// concatenate the list p1,p2
std::string merge(const std::string& p1, const std::string& p2) {
	return p1 + p2;
}

// insert label as the target label for each of the instructions p
void backpatch(std::string p, std::string label) {
	if (p.empty()) return;

	// "LABEL_" + gen(nextInstr)
	int labelIndex = atoi(label.substr(6).c_str());
	addLeader(labelIndex);

	std::string sep = ";";
	std::string::size_type pos_begin = p.find_first_not_of(sep);
	std::string::size_type comma_pos = 0;
	std::string strIndex;

	while (pos_begin != std::string::npos) {
		comma_pos = p.find_first_of(sep, pos_begin);
		if (comma_pos != std::string::npos) {
			strIndex = p.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + 1;
		}
		middleCode.at(atoi(strIndex.c_str()))._des = label;
		pos_begin = p.find_first_not_of(sep, pos_begin);
	}
	labelMap.insert(std::make_pair(labelIndex, label));
}

// output middle code
void outputMiddleCode() {	
	std::ofstream middleCodeOut("middle_code.txt");
	for (int i = 0; i < middleCode.size(); ++i) {
		auto& code = middleCode.at(i);
		middleCodeOut << std::setw(13) << code._label + " : ";
		middleCodeOut << std::setw(3) << i << ") ";
		middleCodeOut << std::setw(7) << code._op;
		middleCodeOut << std::setw(7) << code._arg1;
		middleCodeOut << std::setw(7) << code._arg2;
		middleCodeOut << std::setw(10) << code._des;
		middleCodeOut << std::endl;
	}
	middleCodeOut.close();
}

#endif // !_SUPPORT_FUNCTION_H