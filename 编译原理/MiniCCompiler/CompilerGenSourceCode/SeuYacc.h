#ifndef _SEU_YACC_H
#define _SEU_YACC_H

#include <string>
#include <fstream>
#include <algorithm>
#include <set>
#include <list>
#include <vector>
#include <deque>
#include <stack>
#include <map>

#include "Token.h"

class SeuYacc {
public:
	typedef unsigned int ElementType;
	typedef unsigned int IDType;

	struct ProductionItem {
		ProductionItem(ElementType head, std::list<ElementType> str, size_t length, IDType index)
			: _head(head), _bodyLength(length), _index(index)
		{
			_bodyList.assign(str.begin(), str.end());
			_op = 999999;  // init with no op
		};
		ProductionItem(ElementType head, std::list<ElementType> str, size_t length, IDType index, ElementType opr)
			: _head(head), _bodyLength(length), _index(index)
		{
				_bodyList.assign(str.begin(), str.end());
				_op = opr;  // init with no op
		};
		void setOP(ElementType opr) {
			_op = opr;
		}
		bool equals(ProductionItem item) {
			return item._index == this->_index;
		}
		ElementType _head;  // nonterminal for production head
		std::list<ElementType> _bodyList;// nonterminal and terminal for production body
		size_t _bodyLength;
		IDType _index;
		ElementType _op;
	};

	struct LR1Item {
		LR1Item(ProductionItem* prod, std::set<ElementType> predSet)
		: _prod(prod), _predSet(predSet)
		{
			_dot = _prod->_bodyList.begin();
		}
		bool equals(LR1Item item) {
			return (item._prod == _prod && item._dot == _dot && item._predSet == _predSet);
		}
		bool sameCoreEqual(LR1Item item) {
			return (item._prod == _prod && item._dot == _dot);
		}
		ProductionItem* _prod;
		std::list<ElementType>::iterator _dot; // dot position pointer to the latter (non)terminal
		std::set<ElementType> _predSet;
	};

	struct LR1State {
		std::list<LR1Item> _itemList;
		bool addItem(LR1Item& item) {
			for (auto& it : _itemList) {
				if (it.equals(item)) {
					return false;
				}
				else if (it.sameCoreEqual(item)) {
					it._predSet.insert(item._predSet.begin(), item._predSet.end());
					return true;
				}
			}
			_itemList.push_back(item);
			return true;
		}

		bool equal(LR1State& other) {
			if (this->_itemList.size() != other._itemList.size()) return false;
			for (auto it : other._itemList) {
				auto it2 = _itemList.begin(), it2End = _itemList.end();
				for (; it2 != it2End; ++it2) {
					if (it2->equals(it)) {
						break;
					}
				}
				if (it2 == it2End)
					return false;
			}
			for (auto it : _itemList) {
				auto it2 = other._itemList.begin(), it2End = other._itemList.end();
				for (; it2 != it2End; ++it2) {
					if (it2->equals(it)) {
						break;
					}
				}
				if (it2 == it2End)
					return false;
			}
			return true;
		}

		bool equalSameCore(LR1State& other) {
			if (this->_itemList.size() != other._itemList.size()) return false;
			for (auto it : other._itemList) {
				auto it2 = _itemList.begin(), it2End = _itemList.end();
				for (; it2 != it2End; ++it2) {
					if (it2->sameCoreEqual(it)) {
						break;
					}
				}
				if (it2 == it2End)
					return false;
			}
			for (auto it : _itemList) {
				auto it2 = other._itemList.begin(), it2End = other._itemList.end();
				for (; it2 != it2End; ++it2) {
					if (it2->sameCoreEqual(it)) {
						break;
					}
				}
				if (it2 == it2End)
					return false;
			}
			return true;
		}
	};

	struct TransitionItem {
		TransitionItem(LR1State& state, IDType index)
		:_lr1state(state), _index(index){}
		void addEdge(ElementType edge, IDType next) {
			_transMap.insert(std::make_pair(edge, next));
		}
		bool operator==(TransitionItem& it) {
			return (it._lr1state.equal(this->_lr1state) && it._transMap == _transMap);
		}
		LR1State _lr1state;
		std::map<ElementType, IDType> _transMap;
		IDType _index;
	};

	enum ACTION_TYPE { SHIFT = 1, REDUCTION, GOTO_STATE, ERROR, ACCEPT };

	struct TableItem {
		TableItem() : _action(ERROR), _index(0) {};
		TableItem(ACTION_TYPE action, IDType index) : _action(action), _index(index) {};
		ACTION_TYPE _action;
		IDType _index;   // index of production or goto state id
	};

	struct StackItem {
		StackItem(IDType state, ElementType symbol) : _state(state), _symbol(symbol) {};
		IDType _state;
		ElementType _symbol;
		std::map<std::string, std::string> _definitionMap;
	};

public:
	SeuYacc(const std::string grammerFile, const std::string parsingFile);

	//bool reduce(std::list<Token>* _tokenList, std::list<ProductionItem>* _reduceSequence);
	//void outputReduceSequence(const std::string& filename, std::list<ProductionItem>* _reduceSequence);
	
	// generate Lex code
	void generateParsingProgram();
	void outputLRStateSet();
	void outputLRParsingTable();
	void outputTable();
	void outputAction();
	

protected:
	// read part
	bool readGrammer(const std::string grammerFile);
	bool readPriority();
	bool read();
	void retract();
	bool isLetterOrDigit(char ch);
	bool isLegalLetter(char ch);

	// help function
	void nullableAnalyze();
	LR1State Closure(LR1State result);
	LR1State GOTO(LR1State h, ElementType edge);
	std::set<ElementType> First(ElementType X);
	std::set<ElementType> Follow(ElementType X);

	// transition graph
	void initTransition();

	// parsing table
	void initParseTable();
	ACTION_TYPE conflictSolve(ElementType shiftOP, ElementType reduceOP);

	bool transformIntoLALR();

	void translateAction();

private:

	std::ifstream _sourcefile;
	std::ofstream _outputFile;

	char* _buffer;
	char _peek;          // current char
	unsigned int _line = 0;  // record the line of current _peek
	int _offset = -1;// record the offset of current _peek

	ElementType _indexSymbol = 0; // max value stands for empty
	ElementType EPLISON = 100;

	std::vector<std::string> _word; // output word

	std::map<std::string, ElementType> _nonterminalTable;
	std::map<std::string, ElementType> _terminalTable;
	std::map<ElementType, bool> _nonterminalNullable;
	std::map<ElementType, bool> _leftAssociative;
	std::map<ElementType, unsigned int> _priority;
	std::map<ElementType, std::set<ElementType> > _firstMap;

	std::deque<ProductionItem> _productionVector;
	std::map<unsigned int, std::string> _productionAction;

	// LR(1) data
	std::vector<std::vector<TableItem> > _LR1parseTable;
	std::vector<TransitionItem> _LR1stateTransition;

	// LALR(1) data
	std::map<IDType, std::vector<TableItem> > _LALR1parseTable;
	std::map<IDType, TransitionItem> _LALR1stateTransition;

}; // class SeuYacc

#endif _SEU_YACC_H