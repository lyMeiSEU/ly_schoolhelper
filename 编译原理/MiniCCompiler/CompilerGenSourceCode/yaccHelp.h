#ifndef _YACC_HELP_H
#define _YACC_HELP_H

#include "Token.h"
#include "actionYacc.h"
#include "tableYacc.h"
#include "structDefine.h"
#include "CodeGenerate.h"
#include <string>
#include <list>
#include <deque>
#include <map>
#include <iostream>
#include <fstream>


// use deque to simulate stack (for action convenience)
std::deque<StackItem> stack;
std::map<unsigned int, std::map<std::string, TableItem> > _parseTable;

std::vector<SymbolTable*> symbolTables;
SymbolTable* globalTable;
std::stack<std::string> paramStack;

std::vector<Quadruple> middleCode;

void errorReduce(std::list<Token>::iterator token) {
	std::cout << "line " << token->_line << ",offset " << token->_offset << " is wrong for grammar\n";
	exit(1);
}

bool yyreduce(std::list<Token> _tokenList) {

	std::ofstream sequence("reduce_sequence.txt");

	initTable(_parseTable);
	globalTable = new SymbolTable("global");
	globalTable->_beginIndex = nextInstr;
	currentTable = globalTable;

	_tokenList.push_back(Token("$", "$", 0, 0, 0));
	stack.push_back(StackItem(0, "$")); // init stack
	auto token = _tokenList.begin(), tokenEnd = _tokenList.end();
	while (token != tokenEnd) {
		TableItem tableItem = _parseTable.find(stack.back()._state)->second.find(token->_attribute)->second;

		if (tableItem._action == SHIFT) {
			// push t onto the stack
			std::map<std::string, std::string> map;
			map["lexeme"] = token->_lexecal;
			stack.push_back(StackItem(tableItem._index, token->_attribute, map));
			// let a be the next input symbol
			++token;
		}
		else if (tableItem._action == REDUCTION) {
			// ACTION[s, a] = reduce A->beta
			std::map<std::string, std::string> reduceHead;
			// pair <length of production, production head>
			std::pair<unsigned int, std::string> item = performAction(tableItem._index, reduceHead);
			// pop |beta| symbols off the stack
			for (size_t i = 0; i < item.first; ++i) stack.pop_back();
			// let state t now be on top of stack
			const auto& tableitem = _parseTable.find(stack.back()._state)->second;
			auto found = tableitem.find(item.second);
			if (found == tableitem.end()) errorReduce(token);
			unsigned int goto_state = found->second._index;
			// push GOTO[t, A] onto stack
			stack.push_back(StackItem(goto_state, item.second, reduceHead));
			// output the production A->beta
			sequence << getProduction(tableItem._index) << std::endl;
		}
		else if (tableItem._action == ACCEPT) {
			// parsing is done
			sequence << getProduction(tableItem._index) << std::endl;
			break;
		}
		else if (tableItem._action == ERROR) {
			// M[state,a] is an error entry
			errorReduce(token);
			//error("no such item in parsing table M[" + std::to_string(topItem.curState) + ',' + word.at(terminalIndex) + ']');
			return false;
			//break;
		}
		else {
			errorReduce(token);
			return false;
			//error("impossible parsing table action");
			break;
		}
	}
	sequence.close();

	// put global symbol table into structure
	symbolTables.push_back(globalTable);

	for (const auto& map : labelMap) {
		middleCode.at(map.first)._label = map.second;
	}
	outputMiddleCode();

	tranlateIntoAssembly();

	// release table pointers
	std::ofstream symbolFile("symbol_table_file.txt");
	for (auto& table : symbolTables) {
		outputSymbolTable(table, symbolFile);
		if (table != nullptr) delete table;
		table = nullptr;
	}
	symbolFile.close();
	//system("pause");
	return true;
}

#endif // !_YACC_HELP_H