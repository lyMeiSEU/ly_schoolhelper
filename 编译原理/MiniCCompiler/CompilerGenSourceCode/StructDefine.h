#ifndef _STRUCT_DEFINE_H
#define _STRUCT_DEFINE_H

#include <string>
#include <sstream>  
#include <map>
#include <vector>
#include <set>

#define SHIFT 1
#define REDUCTION 2
#define GOTO_STATE 3
#define ERROR 4
#define ACCEPT 5

#define REGISTER_NUM 6

//typedef unsigned int ACTION_TYPE;
struct TableItem {
	TableItem() : _action(ERROR), _index(0) {};
	TableItem(unsigned int action, unsigned int index) : _action(action), _index(index) {};
	unsigned int _action;
	unsigned int _index;   // index of production or goto state id
};

struct StackItem {
	StackItem(unsigned int state, std::string symbol) : _state(state), _symbol(symbol) {};
	StackItem(unsigned int state, std::string symbol, std::map<std::string, std::string> map) 
		: _state(state), _symbol(symbol) ,_map(map){};
	unsigned int _state;
	std::string _symbol;
	std::map<std::string, std::string> _map;
};

struct SymbolTable {
	// data
	std::string _funName;
	//unsigned int _maxOffset = 0;
	unsigned int _returnSize = 0;
	unsigned int _variableOffset = 0;
	unsigned int _paramOffset = 0;
	int _beginIndex = -1;
	int _endIndex = -1;
	struct varState {
		bool _live;
		int _nextUse;
		bool _inM;
		int _inR;
		bool _unsigned;
		unsigned int _offset;
		unsigned int _space;
		std::string _name;
		std::string _type;
		std::string _place;
		varState(const std::string& name, const std::string& type
			, unsigned int offset,unsigned int space)
			: _name(name),_type(type), _live(false), _nextUse(-1),
			_inM(false), _inR(-1), _offset(offset), _space(space){
			std::string::size_type pos = _type.find("unsigned", 0); 
			if (pos != std::string::npos) {
				_unsigned = true;
			}
		};
	};
	std::vector<varState> _field;
	std::set<int> _leaders;
	//std::vector<std::set<std::string> > RValue;
	//std::vector<int> RNextUse;

	// function
	SymbolTable(const std::string& name): _funName(name) {
		//RValue.resize(REGISTER_NUM);
		//RNextUse.resize(REGISTER_NUM);
		//std::fill_n(RNextUse.begin(), REGISTER_NUM, -1);
	}
	// add local variable into symbol table
	std::string enter(const std::string& name, const std::string& type, unsigned int space, bool placeFlag = true) {
		for (const auto& it : _field) {
			if (it._name == name) return it._place;
		}
		_variableOffset += space;
		varState state(name, type, _variableOffset, space);
		if (placeFlag) {
			state._place = "[BP-" + std::to_string(_variableOffset) + "]";
		}
		else {
			state._place = name;
		}
		_field.push_back(state);
		return state._place;
	}
	// add param into symbol table
	std::string enterParam(const std::string& name, const std::string& type, unsigned int space) {
		for (const auto& it : _field) {
			if (it._name == name) return false;
		}
		_paramOffset += space;
		varState state(name, type, _paramOffset, space);
		state._place = "[BP+" + std::to_string((REGISTER_NUM-1+2-1) * 2 + _paramOffset) + "]";
		_field.push_back(state);
		return state._place;
	}

	//bool has(const std::string& name) {
	//	for (const auto& it : _field) {
	//		if (it._name == name) return true;
	//	}
	//	return false;
	//}
	varState& at(const std::string& place) {
		for (auto& it : _field) {
			if (it._place == place) {
				return it;
			}
		}
		std::cerr << place << " is not in symbol table " << _funName << std::endl;
		// error handle
		exit(1);
	}
};

struct BasicBlock {
	int _begin;
	int _end;
	std::vector<int> _predecessors;
	std::vector<int> _successors;
	std::set<std::string> _inLiveVar;
	std::set<std::string> _outLiveVar;

	BasicBlock(int index = -1) {
		_begin = index;
	}
}; 

bool isNum(std::string str) {
	std::stringstream sin(str);
	int d;
	char c;
	if (!(sin >> d))
		return false;
	if (sin >> c)
		return false;
	return true;
}

struct  Quadruple {
	int _type;
	std::string _label;
	std::string _op;
	std::string _des, _arg1, _arg2;
	// false mean it is a variable, true mean it is a number
	bool _typeDes = false, _typeArg1 = false, _typeArg2 = false;
	bool _liveDes, _liveArg1, _liveArg2;
	int _nextDes, _nextArg1, _nextArg2;
	
	Quadruple(const std::string& op, const std::string& arg1, const std::string& arg2, const std::string& des)
		: _op(op), _des(des), _arg1(arg1), _arg2(arg2) {

		if (isNum(arg1)) _typeArg1 = true;
		if (isNum(arg2)) _typeArg2 = true;
		//_typeDes = _typeArg1 & _typeArg2;

		if (_op == "ADD") {
			_type = 10;
		}
		else if (_op == "SUB") {
			_type = 11;
		}
		else if (_op == "MUL") {
			_type = 12;
		}
		else if (_op == "DIV") {
			_type = 13;
		}
		else if (_op == "NEG") {
			_type = 2; // A = op B
		}
		else if (_op.empty()) {
			_type = 3; // A = B
		}
		else if (_op[0] == 'j') {
			if (_op.size() == 1) {
				_type = 20; // j LABEL_xxx
			}
			else {
				// j rop B C LABEL_xxx
				if (_op == "j<") {
					_type = 21; 
				} else if(_op == "j<=") {
					_type = 22;
				}
				else if (_op == "j>") {
					_type = 23;
				}
				else if (_op == "j>=") {
					_type = 24;
				}
				else if (_op == "j==") {
					_type = 25;
				}
				else if (_op == "j!=") {
					_type = 26;
				}
			}
		}
		else if (_op == "param") {
			_type = 6; // param p
		}
		else if (_op == "call") {
			_type = 7;// call N funName
		}
		else if (_op == "return") {
			_type = 8; // return a
		}
	}
};

struct Assembly {
	std::string _op;
	std::string _des;
	std::string _arg;
	std::string _label;
	Assembly(const std::string& op, const std::string& des, const std::string& arg)
		: _op(op), _des(des), _arg(arg){}
	Assembly(const std::string& op, int des, const std::string& arg)
		: _op(op), _arg(arg) {
		_des = transformToRegName(des);
	}
	Assembly(const std::string& op, int des, int arg)
		: _op(op) {
		_des = transformToRegName(des);
		_arg = transformToRegName(arg);
	}
	Assembly(const std::string& op, const std::string& des, int arg)
		: _op(op) {
		_des = des;
		_arg = transformToRegName(arg);
	}
	std::string transformToRegName(int reg) {
		if (reg == 0) return "AX";
		else if (reg == 1) return "BX";
		else if (reg == 2) return "CX";
		else if (reg == 3) return "DX";
		else if (reg == 4) return "SI";
		else if (reg == 5) return "DI";
		else return "AX";
	}
};

#endif //_STRUCT_DEFINE_H