#ifndef _CODE_GENERATE_H
#define _CODE_GENERATE_H

#define REGISTER_NUM 6

#include "structDefine.h"
#include <vector>
#include <set>
#include <stack>
#include <algorithm>
#include <iomanip>
#include <iterator>

// reg descripter table
extern std::vector<SymbolTable*> symbolTables;
SymbolTable* table;
std::vector<std::set<std::string> >* RValue;
std::vector<int>* RNextUse;

std::vector<Assembly> assemblyCode;
std::map<size_t, std::string> AssemblyLabelMap;

typedef std::set<std::string> VarSetType;

// live_nextuse algorithm
void fillVarState(int beginIndex, int endIndex, const VarSetType& outLiveVar, VarSetType& inLiveVar) {

	// table init with outLiveVar
	for (auto& var : outLiveVar) {
		table->at(var)._live = true;
		table->at(var)._nextUse = endIndex;
	}
	
	for (int i = endIndex - 1; beginIndex <= i; --i) {
		auto& code = middleCode.at(i);
		if (code._type == 7) continue; // call N funName
		if (code._arg1 == "#") {
			code._liveDes = table->at(code._des)._live;
			code._nextDes = table->at(code._des)._nextUse;
			table->at(code._des)._live = false;
			table->at(code._des)._nextUse = -1;
			continue;
		}
		bool desExsitFlag = !(code._des.empty() || code._op[0] == 'j');
		bool arg1ExsitFlag = !code._arg1.empty();
		bool arg2ExsitFlag = !code._arg2.empty();

		// fill code var state live from symbol table
		if (desExsitFlag) code._liveDes = table->at(code._des)._live;
		if (arg1ExsitFlag) code._liveArg1 = table->at(code._arg1)._live;
		if (arg2ExsitFlag) code._liveArg2 = table->at(code._arg2)._live;
		// fill code var state next use from symbol table
		if (desExsitFlag) code._nextDes = table->at(code._des)._nextUse;
		if (arg1ExsitFlag) code._nextArg1 = table->at(code._arg1)._nextUse;
		if (arg2ExsitFlag) code._nextArg2 = table->at(code._arg2)._nextUse;

		// erase des state in symbol table
		if (desExsitFlag) {
			table->at(code._des)._live = false;
			table->at(code._des)._nextUse = -1;
		}

		// set arg1,arg2 state in symbol table
		if (arg1ExsitFlag) {
			table->at(code._arg1)._live = true;
			table->at(code._arg1)._nextUse = i;
		}
		if (arg2ExsitFlag) {
			table->at(code._arg2)._live = true;
			table->at(code._arg2)._nextUse = i;
		}
	}

	// generate inLivaVar
	for (auto& var : table->_field) {
		if (var._live == true && var._space != 0) {
			inLiveVar.insert(var._place);
		}
		// clear table
		var._live = false;
		var._nextUse = -1;
	}
}

int getEmptyReg() {
	auto& availReg = std::find(RValue->begin(), RValue->end(), std::set<std::string>());
	if (availReg != RValue->end()) {
		return availReg - RValue->begin();
	}
	else return -1;
}

int storeToGetReg() {
	// find one available register
	int furthestUseCode = -1;
	int regReturn = -1;
	for (int Reg = 0; Reg < REGISTER_NUM; ++Reg) {
		// find one reg whose var has copy in memory and will be uesd furthest
		int varNotInM = 0;
		for (const auto& var : RValue->at(Reg)) {
			if (table->at(var)._inM == false)
				++varNotInM;
		}
		if (0 < varNotInM && furthestUseCode < RNextUse->at(Reg)) {
			regReturn = Reg;
			furthestUseCode = RNextUse->at(Reg);
		}
	}
	if (regReturn < 0) {
		// when all var not in memoty
		// need optimize
		regReturn = 0;
	}

	// store var not in memory
	for (auto& var : RValue->at(regReturn)) {
		if (table->at(var)._inM == false) {
			assemblyCode.push_back(Assembly("MOV", regReturn, var));
		}
		table->at(var)._inR = -1;
		table->at(var)._inM = true;
	}
	return regReturn;
}

// code : A = B op C
void Assembly_A_BopC(const Quadruple& code) {

	int RegForC = table->at(code._arg2)._inR;
	int RegForB = table->at(code._arg1)._inR;
	int RegForA = table->at(code._des)._inR;

	if (code._typeArg1) {
		if (RegForC < 0) {
			if (RegForA < 0) {
				RegForA = getEmptyReg();
				if (RegForA < 0) RegForA = storeToGetReg();
				std::set<std::string> set; set.insert(code._des);
				RValue->at(RegForA) = set;
				RNextUse->at(RegForA) = code._nextDes;
				table->at(code._des)._inR = RegForA;
				table->at(code._des)._inM = false;
			}
			assemblyCode.push_back(Assembly("MOV", RegForA, code._arg2));
			assemblyCode.push_back(Assembly(code._op, RegForA, code._arg1));
		}
		else {
			if (RegForA < 0) {
				// A in memory and C in reg
				assemblyCode.push_back(Assembly("MOV", code._des, RegForC));
				assemblyCode.push_back(Assembly(code._op, code._des, code._arg1));
			}
			else {
				// A in reg and C in reg
				assemblyCode.push_back(Assembly("MOV", RegForA, RegForC));
				assemblyCode.push_back(Assembly(code._op, RegForA, code._arg1));
				std::set<std::string> set; set.insert(code._des);
				RValue->at(RegForA) = set;
				RNextUse->at(RegForA) = code._nextDes;
				table->at(code._des)._inR = RegForA;
				table->at(code._des)._inM = false;
			}
		}
	}
	else if (code._typeArg2) {
		if (RegForB < 0) {
			if (RegForA < 0) {
				RegForA = getEmptyReg();
				if (RegForA < 0) RegForA = storeToGetReg();
				std::set<std::string> set; set.insert(code._des);
				RValue->at(RegForA) = set;
				RNextUse->at(RegForA) = code._nextDes;
				table->at(code._des)._inR = RegForA;
				table->at(code._des)._inM = false;
			}
			assemblyCode.push_back(Assembly("MOV", RegForA, code._arg2));
			assemblyCode.push_back(Assembly(code._op, RegForA, code._arg1));
		}
		else {
			if (RegForA < 0) {
				// A in memory and B in reg
				assemblyCode.push_back(Assembly("MOV", code._des, RegForB));
				assemblyCode.push_back(Assembly(code._op, code._des, code._arg2));
			}
			else {
				// A in reg and B in reg
				assemblyCode.push_back(Assembly("MOV", RegForA, RegForB));
				assemblyCode.push_back(Assembly(code._op, RegForA, code._arg2));
				std::set<std::string> set; set.insert(code._des);
				RValue->at(RegForA) = set;
				RNextUse->at(RegForA) = code._nextDes;
				table->at(code._des)._inR = RegForA;
				table->at(code._des)._inM = false;
			}
		}
	}
	else {
		// B and C is variable, not number
		if (RegForB < 0) {
			// IN_R[B] != Ri
			RegForB = getEmptyReg();
			if (RegForB < 0) RegForB = storeToGetReg();
			// generate targer code
			assemblyCode.push_back(Assembly("MOV", RegForB, code._arg1));
			// RValue[Ri] = {B}, RNextUse[Ri] = B.NextUse
			std::set<std::string> set; set.insert(code._arg1);
			RValue->at(RegForB) = set;
			RNextUse->at(RegForB) = code._nextArg1;
			table->at(code._arg1)._inR = RegForB;
			table->at(code._arg1)._inM = true;
		}

		// select reg for A
		RegForA = RegForB;
		if (*(RValue->at(RegForA).begin()) == code._arg1 && RValue->at(RegForA).size() == 1
			&& (code._liveArg1 == false || code._arg1 == code._des)) {
			// select reg of B to store A
			// [Ri] = {B} && (B.live == false || A = A op C)
			if (-1 < RegForC)
				assemblyCode.push_back(Assembly(code._op, RegForA, RegForC));
			else
				assemblyCode.push_back(Assembly(code._op, RegForA, code._arg2));
			table->at(code._arg1)._inR = -1;
		}
		else {
			// select another reg for A
			RegForA = getEmptyReg();
			if (-1 < RegForA) {
				assemblyCode.push_back(Assembly("MOV", RegForA, RegForB));
				int regC = table->at(code._arg2)._inR;
				if (-1 < regC)
					assemblyCode.push_back(Assembly(code._op, RegForA, regC));
				else
					assemblyCode.push_back(Assembly(code._op, RegForA, code._arg2));
			}
			else {
				RegForA = storeToGetReg();
				assemblyCode.push_back(Assembly("MOV", RegForA, RegForB));
				assemblyCode.push_back(Assembly(code._op, RegForA, code._arg2));
			}
		}
		// update reg descripter table RValue[RegA] = {A}, RNextUse[RegA] = A.next
		std::set<std::string> set; set.insert(code._des);
		RValue->at(RegForA) = set;
		RNextUse->at(RegForA) = code._nextDes;
		table->at(code._des)._inR = RegForA;
		table->at(code._des)._inM = false;

		// variable in C deal
		if (code._liveArg2 == false && -1 < RegForC) {
			RValue->at(RegForC).erase(code._arg2);
			table->at(code._arg2)._inR = -1;
			std::set<std::string> set = RValue->at(RegForC);
			for (const auto& var : set) {
				auto& it = table->at(var);
				if (it._live == false) {
					it._inR = -1;
					RValue->at(RegForC).erase(var);
				}
			}
			int nextUse = INT_MAX;
			for (auto& var : RValue->at(RegForC)) {
				nextUse = std::min(nextUse, table->at(var)._nextUse);
			}
		}
	}
}

// code : A = op B
void Assembly_A_opB(const Quadruple& code) {

	int RegForB = table->at(code._arg1)._inR;
	if (RegForB < 0) {
		// IN_R[B] != Ri
		RegForB = getEmptyReg();
		if (RegForB < 0) RegForB = storeToGetReg();
		// generate targer code
		assemblyCode.push_back(Assembly("MOV", RegForB, code._arg1));
		// RValue[Ri] = {B}, RNextUse[Ri] = B.NextUse
		std::set<std::string> set; set.insert(code._arg1);
		RValue->at(RegForB) = set;
		RNextUse->at(RegForB) = code._nextArg1;
		table->at(code._arg1)._inR = RegForB;
		table->at(code._arg1)._inM = true;
	}

	// select reg for A
	int RegForA = table->at(code._arg1)._inR;
	if (*(RValue->at(RegForA).begin()) == code._arg1 && RValue->at(RegForA).size() == 1
		&& (code._liveArg1 == false || code._arg1 == code._des)) {
		// select reg of B to store A
		// [Ri] = {B} && (B.live == false || A = op A)
		assemblyCode.push_back(Assembly(code._op, RegForA, ""));
		table->at(code._arg1)._inR = -1;
	}
	else {
		// select another reg for A
		RegForA = getEmptyReg();
		if (-1 < RegForA) {
			assemblyCode.push_back(Assembly("MOV", RegForA, RegForB));
			assemblyCode.push_back(Assembly(code._op, RegForA, ""));
		}
		else {
			RegForA = storeToGetReg();
			assemblyCode.push_back(Assembly("MOV", RegForA, RegForB));
			assemblyCode.push_back(Assembly(code._op, RegForA, code._arg2));
		}
	}
	// update reg descripter table RValue[RegA] = {A}, RNextUse[RegA] = A.next
	std::set<std::string> set; set.insert(code._des);
	RValue->at(RegForA) = set;
	RNextUse->at(RegForA) = code._nextDes;
	table->at(code._des)._inR = RegForA;
	table->at(code._des)._inM = false;
}

// code : A = B
void Assembly_A_B(const Quadruple& code) {

	if (code._typeArg1) {
		assemblyCode.push_back(Assembly("MOV", code._des, code._arg1));
		return;
	}

	if (code._arg1 == "#") {
		assemblyCode.push_back(Assembly("MOV", code._des, "AX"));

		/*int RegForA = table->at(code._des)._inR;
		if (RegForA < 0) RegForA = getEmptyReg();
		if (RegForA < 0) RegForA = storeToGetReg();
		assemblyCode.push_back(Assembly("POP", RegForA,""));*/
		return;
	}

	int RegForB = table->at(code._arg1)._inR;
	if (RegForB < 0) {
		RegForB = getEmptyReg();
		if (RegForB < 0) RegForB = storeToGetReg();
		assemblyCode.push_back(Assembly("MOV", RegForB, code._arg1));
	}
	RValue->at(RegForB).insert(code._des);
	table->at(code._des)._inR = RegForB;
	table->at(code._des)._inM = false;
	if (code._liveArg1 == false) {
		RValue->at(RegForB).erase(code._arg1);
		table->at(code._arg1)._inR = -1;
	}
	//assemblyCode.push_back(Assembly("JMP", "$+2",""));
}

// code : j LABEL_xxx
void Assembly_j(const Quadruple& code) {
	assemblyCode.push_back(Assembly("JMP", code._des, ""));
}

// code : j rop B C LABEL_xxx
void Assembly_jrop(const Quadruple& code) {

	if (code._typeArg1) {
		int regC = table->at(code._arg2)._inR;
		if (regC < 0) {
			assemblyCode.push_back(Assembly("CMP", code._arg1, code._arg2));
		}
		else {
			assemblyCode.push_back(Assembly("CMP", code._arg1, regC));
		}
	}
	else if (code._typeArg2) {
		int regB = table->at(code._arg1)._inR;
		if (regB < 0) {
			assemblyCode.push_back(Assembly("CMP", code._arg1, code._arg2));
		}
		else {
			assemblyCode.push_back(Assembly("CMP", regB, code._arg2));
		}
	}
	else {
		int RegForB = table->at(code._arg1)._inR;
		if (RegForB < 0) {
			// IN_R[B] != Ri
			RegForB = getEmptyReg();
			if (RegForB < 0) RegForB = storeToGetReg();
			assemblyCode.push_back(Assembly("MOV", RegForB, code._arg1));
			// RValue[Ri] = {B}, RNextUse[Ri] = B.NextUse
			std::set<std::string> set; set.insert(code._arg1);
			RValue->at(RegForB) = set;
			RNextUse->at(RegForB) = code._nextArg1;
			table->at(code._arg1)._inR = RegForB;
			table->at(code._arg1)._inM = true;
		}
		assemblyCode.push_back(Assembly("CMP", RegForB, code._arg2));
	}

	std::string jrop = "";
	if (code._op == "j==") jrop = "JE";
	else if (code._op == "j!=") jrop = "JNE";
	else if (code._op == "j<") {
		if (table->at(code._arg1)._unsigned) jrop = "JB";
		else jrop = "JL";
	}
	else if (code._op == "j<=") {
		if (table->at(code._arg1)._unsigned) jrop = "JNA";
		else jrop = "JLE";
	}
	else if (code._op == "j>") {
		if (table->at(code._arg1)._unsigned) jrop = "JA";
		else jrop = "JG";
	}
	else if (code._op == "j>=") {
		if (table->at(code._arg1)._unsigned) jrop = "JNB";
		else jrop = "JGE";
	}
	assemblyCode.push_back(Assembly(jrop, code._des, ""));
}

// code : param p
void Assembly_param(const Quadruple& code) {
	int regP = table->at(code._arg1)._inR;
	if (regP < 0) {
		regP = getEmptyReg();
		if (regP < 0) regP = storeToGetReg();
		assemblyCode.push_back(Assembly("MOV", regP, code._arg1));
		std::set<std::string> set; set.insert(code._arg1);
		RValue->at(regP) = set;
		RNextUse->at(regP) = code._nextArg1;
		table->at(code._arg1)._inR = regP;
		table->at(code._arg1)._inM = true;
	}
	assemblyCode.push_back(Assembly("PUSH", regP, ""));
}

// code : call N funName
void Assembly_call(const Quadruple& code) {
	// reserve AX as return value register
	for (const auto& var : *(RValue->begin())) {
		assemblyCode.push_back(Assembly("MOV", var, "AX"));
		table->at(var)._inR = -1;
		table->at(var)._inM = true;
	}
	*(RNextUse->begin()) = -1;
	RValue->begin()->clear();

	assemblyCode.push_back(Assembly("CALL", code._arg2, ""));
	//assemblyCode.push_back(Assembly("SUB", "SP", table->_returnSize));
	//assemblyCode.push_back(Assembly("MOV", "[SP]", "(DWORD)0")); // keep a space for return value
	//assemblyCode.push_back(Assembly("CALL", code._arg2, ""));

	//assemblyCode.push_back(Assembly("MOV", "1[BP]", "SP"));
	//assemblyCode.push_back(Assembly("MOV", "3[BP]", code._arg1));
	//assemblyCode.push_back(Assembly("CALL", code._arg2, ""));
}

// code : function begin
void Assembly_function_prework(const Quadruple& code) {
	//assemblyCode.push_back(Assembly("MOV", "SP", "1[BP]"));
	//assemblyCode.push_back(Assembly("ADD", "SP", "1"));
	//assemblyCode.push_back(Assembly("MOV", "1[SP]", "PC"));
	////std::find_if(symbolTables.begin(), symbolTables.end(), [&](auto& it) {it->_funName == code._label});
	//for (auto& table : symbolTables) {
	//	if (table->_funName == code._label) {
	//		assemblyCode.push_back(Assembly("ADD", "TOP", std::to_string(table->_maxOffset)));
	//		break;
	//	}
	//}
	assemblyCode.push_back(Assembly("PUSH", "BP", ""));
	for (unsigned int reg = 1; reg < REGISTER_NUM; ++reg) {
		assemblyCode.push_back(Assembly("PUSH", reg, ""));
	}
	assemblyCode.push_back(Assembly("MOV", "BP", "SP"));
	for (const auto& table : symbolTables) {
		if (table->_funName == code._label) {
			assemblyCode.push_back(Assembly("SUB", "SP", std::to_string(table->_variableOffset)));
			break;
		}
	}
}

// code : return B
void Assembly_return(const Quadruple& code) {
	//int RegForB = table->at(code._arg1)._inR;
	//if (RegForB < 0) RegForB = getEmptyReg();
	//if (RegForB < 0) RegForB = storeToGetReg();
	
	if (table->_returnSize <= 2) {
		int RegForB = table->at(code._arg1)._inR;
		if (-1 < RegForB) { 
			if (0 < RegForB) {
				// Reg for B is not AX
				assemblyCode.push_back(Assembly("MOV", "AX", RegForB));
			}
			// Reg for B is AX, do nothing
		}
		else {
			assemblyCode.push_back(Assembly("MOV", "AX", code._arg1));
		}
	}
	assemblyCode.push_back(Assembly("MOV", "SP", "BP"));
	// reserve AX as return value register  __stdcall calling convention
	for (unsigned int reg = REGISTER_NUM -1 ; 0 < reg; --reg) {
		assemblyCode.push_back(Assembly("POP", reg, ""));
	}
	assemblyCode.push_back(Assembly("POP", "BP", ""));
	assemblyCode.push_back(Assembly("RET", std::to_string(table->_paramOffset), ""));

	//assemblyCode.push_back(Assembly("MOV", "TOP", "SP"));
	//assemblyCode.push_back(Assembly("SUB", "TOP", "1"));
	//assemblyCode.push_back(Assembly("MOV", "SP", "0[SP]"));
	//assemblyCode.push_back(Assembly("ret", "", ""));
	//assemblyCode.push_back(Assembly("MOV", "X", "1[SP]"));
	/*assemblyCode.push_back(Assembly("MOV", "X", "1[SP]"));
	assemblyCode.push_back(Assembly("J", "0[X]", ""));*/
}

// generate condition choose
void GenerateAssembly(const Quadruple& code) {
	
	if (!code._label.empty()) {
		
		if (code._label.substr(0, 6) != "LABEL_" && code._label != "main") {
			Assembly_function_prework(code);
		}
		else if (code._label == "main") {
			// prework for function entrance
			assemblyCode.push_back(Assembly("MOV", "BP", "SP"));
			for (const auto& table : symbolTables) {
				if (table->_funName == code._label) {
					assemblyCode.push_back(Assembly("SUB", "SP", std::to_string(table->_variableOffset)));
					break;
				}
			}
		}
		
	}

	// different handle by type of code
	if (9 < code._type && code._type < 14) {
		Assembly_A_BopC(code);
	}
	else if (code._type == 2) {
		Assembly_A_opB(code);
	}
	else if (code._type == 3) {
		Assembly_A_B(code);
	}
	else if (code._type == 20) {
		Assembly_j(code);
	}
	else if (20 < code._type && code._type < 27) {
		Assembly_jrop(code);
	}
	else if (code._type == 6) {
		Assembly_param(code);
	}
	else if (code._type == 7) {
		Assembly_call(code);
	}
	else if (code._type == 8) {
		Assembly_return(code);
	}
}

// output assembly codes
void outputAssemblyCode() {
	std::ofstream out("code.asm");
	//out << ".386" << std::endl;
	out << ".MODEL SMALL" << std::endl;
	out << ".STACK" << std::endl;

	// output static data assemble code
	out << ".DATA" << std::endl;
	for (auto& var : globalTable->_field) {
		out << std::setw(5) << var._name;
		// init value
		if (var._type == "int") {
			out << std::setw(5) << "DD ";
			out << std::setw(5) << "00000000H";
		}
		else if (var._type == "float") {
			out << std::setw(5) << "DD ";
			out << std::setw(5) << "00000000H";
		}
		else if (var._type == "double") {
			out << std::setw(5) << "DW ";
			out << std::setw(5) << "0000000000000000H";
		}
		out << std::endl;
	}

	// output code part
	out << ".CODE" << std::endl;
	unsigned int index = 0;
	for (const auto& code : assemblyCode) {
		if (!code._label.empty() 
			&& (code._label.substr(0, 6) == "LABEL_" || code._label == "main")) {
			out << std::setw(13) << code._label + " : ";
		}
		else {
			out << std::setw(13) << " ";
		}
		//out << std::setw(3) << index++ << ") ";
		out << code._op << " " << code._des;
		if (!code._arg.empty())
			out << "," << code._arg << std::endl;
		else 
			out << std::endl;
	}
	out.close();
}

void tranlateIntoAssembly() {

	RValue = new std::vector<std::set<std::string>>(REGISTER_NUM);
	RNextUse = new std::vector<int>(REGISTER_NUM);

	for (auto& tab : symbolTables) {

		table = tab;
		
		if (tab->_funName != "global" && tab->_funName != "main")
			assemblyCode.push_back(Assembly(tab->_funName + " PROC", "", ""));

		// set DS to DATA segment
		if (tab->_funName == "main") {
			AssemblyLabelMap.insert(std::make_pair(assemblyCode.size(), tab->_funName));
			assemblyCode.push_back(Assembly("MOV","AX", "@DATA"));
			assemblyCode.push_back(Assembly("MOV","DS", "AX"));
		}

		std::map<int, BasicBlock> flowGragh;
		
		std::deque<int> leaders(table->_leaders.begin(), table->_leaders.end());
		leaders.push_back(table->_endIndex);

		for (auto& leader : leaders) {
			// use the beginIndex as block index
			flowGragh.insert(std::make_pair(leader, BasicBlock(leader)));
		}

		int beginIndex = leaders.front(); 
		auto& cur = leaders.begin(); ++cur;
		int endIndex = beginIndex;
		while (cur != leaders.end()) {

			endIndex = *cur; 
			
			auto& block = flowGragh.at(beginIndex);
			block._end = endIndex;

			auto& code = middleCode.at(endIndex - 1);

			if (code._type == 20) {
				// j LABEL_i
				int d = atoi(code._des.substr(6).c_str());
				block._successors.push_back(d);
				flowGragh.at(d)._predecessors.push_back(beginIndex);

			} else if (20 < code._type && code._type < 27) {
				// jop A B LABEL_i
				int d = atoi(code._des.substr(6).c_str());
				block._successors.push_back(d);
				flowGragh.at(d)._predecessors.push_back(beginIndex);

				// execute in order
				block._successors.push_back(endIndex);
				flowGragh.at(endIndex)._predecessors.push_back(beginIndex);
			}
			else {
				// execute in order
				block._successors.push_back(endIndex);
				flowGragh.at(endIndex)._predecessors.push_back(beginIndex);
			}

			beginIndex = endIndex;
			++cur;
		}

		// fill outLiveVar and inLiveVar info
		for (auto& blockIndex = leaders.rbegin() + 1; blockIndex != leaders.rend(); ++blockIndex) {

			auto& block = flowGragh.at(*blockIndex);
			VarSetType inLiveVar;
			fillVarState(block._begin, block._end, block._outLiveVar, inLiveVar);
			for (auto& predecessor : block._predecessors) {
				auto vars = flowGragh.at(predecessor)._outLiveVar;
				VarSetType newVars;
				std::set_union(inLiveVar.begin(), inLiveVar.end(), vars.begin(), vars.end(),
					std::inserter(newVars, newVars.begin()));
				flowGragh.at(predecessor)._outLiveVar = newVars;
			}
		}

		// generate assembly codes for each basic block
		for (auto& blockIndex = leaders.begin(); blockIndex != leaders.end()-1; ++blockIndex) {

			auto& block = flowGragh.at(*blockIndex);
			fillVarState(block._begin, block._end, block._outLiveVar, block._inLiveVar);
			std::fill_n(RValue->begin(), REGISTER_NUM, std::set<std::string>());
			std::fill_n(RNextUse->begin(), REGISTER_NUM, -1);

			std::string label = middleCode.at(block._begin)._label;
			if (!label.empty() && label != "main") {
				AssemblyLabelMap.insert(std::make_pair(assemblyCode.size(), label));
			}

			for (int i = block._begin; i < block._end; ++i)
				GenerateAssembly(middleCode.at(i));

			// save live variables
			for (int reg = 0; reg < REGISTER_NUM; ++reg) {
				for (auto& var : RValue->at(reg)) {
					if (block._outLiveVar.find(var) != block._outLiveVar.end()
						&& table->at(var)._inM == false) {
						assemblyCode.push_back(Assembly("MOV", var, reg));
						table->at(var)._inM = true;
					}
					table->at(var)._inR = -1;
				}
			}
			// reserve label
			//if (block._end < middleCode.size() && !middleCode.at(block._end)._label.empty()) {
			//	AssemblyLabelMap.insert(std::make_pair(assemblyCode.size(), middleCode.at(block._end)._label));
			//}
			
		}

		// return to dos
		if (tab->_funName == "main") {
			assemblyCode.push_back(Assembly("MOV", "AH", "4CH"));
			assemblyCode.push_back(Assembly("INT", "21H", ""));
		}

		if (tab->_funName != "global" && tab->_funName != "main")
			assemblyCode.push_back(Assembly(tab->_funName + " ENDP", "", ""));
	}


	assemblyCode.push_back(Assembly("END main", "", ""));
	// add label
	for (const auto& map : AssemblyLabelMap) {
		assemblyCode.at(map.first)._label = map.second;
	}
	outputAssemblyCode();


	if (RValue != nullptr) delete RValue;
	if (RNextUse != nullptr) delete RNextUse;
}

#endif
