#ifndef  _LEX_HELP_H
#define _LEX_HELP_H

// define statement for use hash_map
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#include "Token.h"
#include "actionLex.h"
#include "tableLex.h"
#include <hash_map>
#include <string>
#include <list>
#include <set>
#include <iostream>
#include <fstream>

struct string_less : public std::binary_function<const std::string, const std::string, bool> {
public:
	result_type operator()(const first_argument_type& _Left
		, const second_argument_type& _Right) const {
		return _Left.compare(_Right) < 0;
	}
};

// variable for reading
std::ifstream _reFile;
static char* _buffer;            // reading buffer
static char _peek;               // current char
static unsigned int _line;       // record the line of current _peek
static int _offset;              // record the offset of current _peek
static unsigned int _numSymbol = 0;

static std::hash_map<std::string, unsigned int, std::hash_compare<std::string, string_less> >* _symbolTable;

bool read() {
	if (_offset == -1) {
		if (!_reFile.eof()) {
			_reFile.getline(_buffer, 100);
			++_line;
		}
		else
			return false;
	}
	++_offset;
	if (_offset == strlen(_buffer)) {
		_offset = -1;
		_peek = ' ';
	}
	else {
		_peek = _buffer[_offset];
		if (_peek == '\t') _peek = ' ';
	}
	return true;
}

// content : retract the char back to stream
void retract() {
	if (_offset == -1) _offset = strlen(_buffer);
	_offset -= 1;
}

// return : innerCode in _symbolTable
// if it is not in, return _numSymbol
unsigned int lookup(const std::string& str) {
	auto found = _symbolTable->find(str);
	if (found != _symbolTable->end()) return found->second;
	else return _numSymbol;
}

void outputTokenList(std::list<Token>& tokenList) {
	std::ofstream result;
	result.open("token_list.txt", std::ios::out);
	for (auto token : tokenList) {
		result << "< " << token._lexecal.c_str() << " , " << token._attribute.c_str() << " , " << token._innerCode << " >\n";
	}
	result.close();
}

bool yyparseFct(const std::string& file, std::list<Token>& tokenList) {
	
	std::vector<std::map<char, unsigned int> >* _minDFAStateTranfer = new std::vector<std::map<char, unsigned int> >();
	initMinDFAStateTranfer(_minDFAStateTranfer);

	std::set<unsigned int> _minDFAfinalStateSet;
	initFinalSet(_minDFAfinalStateSet);
	
	_symbolTable = new std::hash_map<std::string, unsigned int, std::hash_compare<std::string, string_less> >();
	
	_buffer = new char[100];
	_line = 0;
	_offset = -1;
	
	unsigned int _state = 0;
	std::string _lexeme = "";

	_reFile.open(file, std::ios::in);
	if (_reFile.fail()) {
		std::cout << file.c_str() << " cannot be open\n";
		return false;
	}

	while (true) {
		if (!read()) break;
		std::map<char, unsigned int>& tran = _minDFAStateTranfer->at(_state);
		auto found = tran.find(_peek);
		if (found != tran.end()) {
			// state transition
			_state = found->second;
			_lexeme.operator+=(_peek);
		}
		else  {
			// check whether it is a final state
			if (!_lexeme.empty() && _minDFAfinalStateSet.find(_state) != _minDFAfinalStateSet.end()) {

				// judge the _lexeme is a keyword
				unsigned int _innerCode = lookup(_lexeme);
				if (_innerCode == _numSymbol) {
					// new identifier
					(*_symbolTable)[_lexeme] = _numSymbol++;
				}
				tokenList.push_back(Token(_lexeme, performAction(_state), _innerCode, _line, _offset));

				_state = 0;
				_lexeme = "";
				if (_peek != ' ') retract();
			}
			else {
				if (_peek == ' ') continue;
				else {
					//error("no such transition in DFA");
					break;
				}
			}
		}
	};

	outputTokenList(tokenList);

	delete _symbolTable; _symbolTable = nullptr;
	delete _minDFAStateTranfer;_minDFAStateTranfer = nullptr;
	delete[] _buffer; _buffer = nullptr;

	_reFile.close();
	return true;
}

#endif // ! _LEX_HELP_H