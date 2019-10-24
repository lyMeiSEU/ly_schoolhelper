//	  file : SeuYacc.h
// content : syntax analyze tool class
//	  date : 2017/4/23
//  author : Wangduo
//  @COPYRIGHT  : School of Computer Science & Engineering Southeast University


#ifndef _SEU_YACC_CPP
#define _SEU_YACC_CPP

#include "SeuYacc.h"
#include <functional>
#include <iomanip>

// content : construct function
// input   : grammerFile -> xxx.y
//           parsingFile -> parsing file xxx.cpp generated
SeuYacc::SeuYacc(const std::string grammerFile, const std::string parsingFile) {
	_outputFile.open(parsingFile);
	if (readGrammer(grammerFile)) {
		initTransition();
		initParseTable();
		if (!transformIntoLALR()) {
			// it can not be transform into LALR, clear corresbonding data
			_LALR1parseTable.swap(std::map<ElementType, std::vector<TableItem>>());
			_LALR1stateTransition.swap(std::map<ElementType, TransitionItem>());
		}
		generateParsingProgram();
	}
	_outputFile.close();
}

// content : read grammer file xx.y
bool SeuYacc::readGrammer(const std::string grammerFile) {

	_sourcefile.open(grammerFile, std::ios::in);
	if (_sourcefile.fail()) {
		//error(grammerFile + " cannot be open");
		return false;
	}

	_buffer = new char[100];

	IDType state = 0;
	std::string lexeme = "";
	IDType procHead = 9999;
	bool endFlag = false;
	unsigned int braceMatch = 0;

	_indexSymbol = 0;
	std::list<ElementType> list;
	unsigned int numOfProd = 0;

	// read Priority infomation first
	if (!readPriority()) {
		//error("illegal addition condition declaration", 0);
		return false;
	}

	//read production
	while (!endFlag) {
		switch (state) {
		case 0:
			// read prodction head
			if (!read()) endFlag = true;
			else if (isLegalLetter(_peek)) {
				lexeme.operator+=(_peek);
				state = 0;
			}
			else if (_peek == ':') {
				auto found = _nonterminalTable.find(lexeme);
				if (found != _nonterminalTable.end()) {
					procHead = found->second;
				}
				else {
					// new nontermianl production head
					_nonterminalTable.insert(std::make_pair(lexeme, _indexSymbol));
					_nonterminalNullable.insert(std::make_pair(_indexSymbol, false));
					_word.push_back(lexeme);
					procHead = _indexSymbol;
					++_indexSymbol;
				}
				lexeme = "";
				state = 1;
			} // ignore illegal char
			break;
		case 1:
			read();
			// read production list
			if (isLegalLetter(_peek)) {
				lexeme.operator+=(_peek);
				state = 1;
			}
			else if (_peek == ' ' || _peek == '\t' || _peek == '\n') {
				if (!lexeme.empty()) {
					auto found = _nonterminalTable.find(lexeme);
					if (found != _nonterminalTable.end()) {
						list.push_back(found->second);
					}
					else {
						_nonterminalTable.insert(std::make_pair(lexeme, _indexSymbol));
						_nonterminalNullable.insert(std::make_pair(_indexSymbol, false));
						_word.push_back(lexeme);
						list.push_back(_indexSymbol);
						++_indexSymbol;
					}
				}
				lexeme = "";
				state = 1;
			}
			else if (_peek == '|') {
				// same production head with different body list
				_productionVector.push_back(ProductionItem(procHead, list, list.size(), numOfProd++));
				if (list.empty()) {
					// empty production
					_nonterminalNullable.find(procHead)->second = true;
				}
				list.swap(std::list<ElementType>());
				lexeme = "";
			}
			else if (_peek == '\'' || _peek == ';' || _peek == '{') {
				if (!lexeme.empty()) {
					auto found = _nonterminalTable.find(lexeme);
					if (found != _nonterminalTable.end()) {
						list.push_back(found->second);
					}
					else {
						_nonterminalTable.insert(std::make_pair(lexeme, _indexSymbol));
						_nonterminalNullable.insert(std::make_pair(_indexSymbol, false));
						_word.push_back(lexeme);
						list.push_back(_indexSymbol);
						++_indexSymbol;
					}
				}
				lexeme = "";
				if (_peek == '\'') state = 2;
				else if (_peek == ';') state = 3;
				else if (_peek == '{') { state = 4; ++braceMatch; }
			}
			break;
		case 2:
			// read terminal 'xxx'
			read();
			if (_peek == '\'') {
				auto found = _terminalTable.find(lexeme);
				if (found != _terminalTable.end()) {
					list.push_back(found->second);
				}
				else {
					_terminalTable.insert(std::make_pair(lexeme, _indexSymbol));
					_word.push_back(lexeme);
					list.push_back(_indexSymbol);
					++_indexSymbol;
				}
				lexeme = "";
				state = 1;
			}
			else lexeme.operator+=(_peek);
			break;
		case 3:
			// add production
			_productionVector.push_back(ProductionItem(procHead, list, list.size(), numOfProd++));
			if (list.empty()) {
				// empty production
				_nonterminalNullable.find(procHead)->second = true;
			}
			list.swap(std::list<ElementType>());
			lexeme = "";
			state = 0;
			break;
		case 4:
			// action part
			read();
			if (_peek == '}' && braceMatch == 1) {
				--braceMatch;
				_productionAction.insert(std::make_pair(numOfProd, lexeme));
				lexeme = "";
				state = 1;
			}
			else {
				lexeme.operator+=(_peek);
				if (_peek == '{') ++braceMatch;
				else if (_peek == '}') --braceMatch;
			}
			break;
		default:
			//error("unreachable state");
			return false;
			break;
		}
	}
	translateAction();
	nullableAnalyze();
	_terminalTable.insert(std::make_pair("$", _indexSymbol));
	_word.push_back("$");
	++_indexSymbol;

	if (_buffer != nullptr) delete[] _buffer;
	_sourcefile.close();
	return true;
}

// read addition conditions from file
bool SeuYacc::readPriority() {
	std::string lexeme = "";
	unsigned int curPriority = 0;
	read();
	while (true) {
		if (_peek != '%') { retract(); return true; }
		++curPriority;
		read();
		if (_peek == 'l') {
			read(); if (_peek != 'e') return false;
			read(); if (_peek != 'f') return false;
			read(); if (_peek != 't') return false;
			read();
			while (_peek != '%') {
				if (_peek == ' ' || _peek == '\t') {}
				else if (_peek == '\'') {
					lexeme = "";
					read();
					while (_peek != '\'') { lexeme.operator+=(_peek); read(); }
					_terminalTable.insert(std::make_pair(lexeme, _indexSymbol));
					_priority.insert(std::make_pair(_indexSymbol, curPriority));
					_leftAssociative.insert(std::make_pair(_indexSymbol, true));
					_word.push_back(lexeme);
					++_indexSymbol;
				}
				read();
			}
		}
		else if (_peek == 'r') {
			read(); if (_peek != 'i') return false;
			read(); if (_peek != 'g') return false;
			read(); if (_peek != 'h') return false;
			read(); if (_peek != 't') return false;
			read();
			while (_peek != '%') {
				if (_peek == ' ' || _peek == '\t') {}
				else if (_peek == '\'') {
					lexeme = "";
					read();
					while (_peek != '\'') { lexeme.operator+=(_peek); read(); }
					_terminalTable.insert(std::make_pair(lexeme, _indexSymbol));
					_priority.insert(std::make_pair(_indexSymbol, curPriority));
					_leftAssociative.insert(std::make_pair(_indexSymbol, false));
					_word.push_back(lexeme);
					++_indexSymbol;
				}
				read();
			}
		}
		else if (_peek == '%') {
			return true;
		}
	}
}

// read a char from sourse file
bool SeuYacc::read() {
	if (_offset == -1) {
		if (!_sourcefile.eof()) {
			_sourcefile.getline(_buffer, 100);
			++_line;
		}
		else
			return false;
	}
	++_offset;
	if (_offset == strlen(_buffer)) {
		_offset = -1;
		read();
	}
	else {
		_peek = _buffer[_offset];
		//if (_peek == ' ' || _peek == '\t' || _peek == '\n') read();
		if (_peek == '\n') read();
	}
	return true;
}

// retract the char back to stream
void SeuYacc::retract() {
	//_sourcefile.putback(_peek);
	if (_offset == -1) _offset = strlen(_buffer);
	_offset -= 1;
}

bool SeuYacc::isLetterOrDigit(char ch) {
	return ('A' <= ch && ch <= 'Z')
		|| ('a' <= ch && ch <= 'z')
		|| ('0' <= ch && ch <= '9');
}

bool SeuYacc::isLegalLetter(char ch) {
	return (isLetterOrDigit(ch) || ch == '_');
}

// content : _nonterminalTable construct
// effect  : alter _nonterminalNullable value
void SeuYacc::nullableAnalyze() {
	auto termEnd = _nonterminalNullable.end();
	bool continueFlag = true;
	while (continueFlag) {
		continueFlag = false;
		for (auto& X : _nonterminalNullable) {
			if (X.second == true) continue;
			else {
				// check all production X->...
				for (auto& prod : _productionVector) {
					if (prod._head == X.first) {
						// check X->ABC..., where A,B,C,.. are all nullable
						auto bodyIT = prod._bodyList.begin(), bodyEnd = prod._bodyList.end();
						for (; bodyIT != bodyEnd; ++bodyIT) {
							auto found = _nonterminalNullable.find(*bodyIT);
							if (found == termEnd || found->second == false) break;
						}
						// X is nullable, new one
						if (bodyIT == bodyEnd) {
							X.second = true;
							continueFlag = true;
						}
					}
				} // end for each in _productionVector
			}
		} // end for each in _nonterminalNullable
	} // end while
}

// content : auto construct state transition
void SeuYacc::initTransition() {
	// add the first state H
	LR1State H;
	std::set<ElementType> predSet;
	predSet.insert(_terminalTable.find("$")->second);
	H.addItem(LR1Item(&*_productionVector.begin(), predSet));
	_LR1stateTransition.push_back(TransitionItem(Closure(H), 0));

	// inner-state-extension and between-state-extension
	IDType maxState = 1;
	for (IDType k = 0; k < maxState; ++k) {
		// check all symbols whether current state has a out-edge
		for (IDType edge = 0; edge < _indexSymbol; ++edge) {
			LR1State newState = GOTO(_LR1stateTransition.at(k)._lr1state, edge);
			// transition construct
			if (!newState._itemList.empty()) {
				bool newStateFlag = true;
				for (auto& m : _LR1stateTransition) {
					if (m._lr1state.equal(newState)) {
						// the state has existed, insert out-edge
						_LR1stateTransition.at(k)._transMap.insert(std::make_pair(edge, m._index));
						newStateFlag = false;
						break;
					}
				}
				if (newStateFlag) {
					// create a new state in transition table
					_LR1stateTransition.push_back(TransitionItem(newState, maxState));
					_LR1stateTransition.at(k)._transMap.insert(std::make_pair(edge, maxState));
					++maxState;
				}
			}
		}
	}
}

// auto construct _LR1parseTable
void SeuYacc::initParseTable() {
	for (IDType state = 0, maxState = _LR1stateTransition.size(); state < maxState; ++state) {
		std::vector<TableItem> temp(_indexSymbol);
		// for end state, make reduction action
		for (auto& p : _LR1stateTransition.at(state)._lr1state._itemList) {
			if (p._dot == p._prod->_bodyList.end()) {
				for (auto& pred : p._predSet) {
					TableItem &curItem = temp.at(pred);
					switch (curItem._action) {
					case ERROR:
						// no table item now
						curItem = TableItem(REDUCTION, p._prod->_index);// terminal, action reduction
						if (p._prod->_index == 0)
							curItem = TableItem(ACCEPT, p._prod->_index);// terminal, action reduction
						break;
					default:
						//error("warning : state " + std::to_string(state) + " occurs reduce-reduce conflict");
						//error("state transition table should be initialed by error state");
						break;
					}
				}
			}
		}
		// shift action
		for (auto& tran : _LR1stateTransition.at(state)._transMap) {
			TableItem &curItem = temp.at(tran.first);
			if (curItem._action == ERROR) {
				// no table item now
				if (_nonterminalNullable.find(tran.first) == _nonterminalNullable.end())
					curItem = TableItem(SHIFT, tran.second);// terminal, action
				else
					curItem = TableItem(GOTO_STATE, tran.second);// nonterminal, goto
			}
			else if (curItem._action == REDUCTION) {
				// solving shift-reduce conflixt
				// reduce action is done before
				unsigned int reduceOP = _indexSymbol;
				// find the last terminial as reduceOP
				auto list = _productionVector.at(curItem._index)._bodyList;
				for (auto it = list.rbegin(); it != list.rend(); ++it) {
					if (_nonterminalNullable.find(*it) == _nonterminalNullable.end()) {
						reduceOP = *it;
						break;
					}
				}
				if (reduceOP != _indexSymbol && conflictSolve(tran.first, reduceOP) == SHIFT) {
					curItem = TableItem(SHIFT, tran.second);
				}
			}
			else if (curItem._action == SHIFT) {
				//error("no possible for shift occur before");
			}
			else {
				//error("illegal state transition table item action");
			}
		}
		_LR1parseTable.push_back(temp);
	}
}

// content : transfer LR(1) TO LALR(1)
// return  : true if succeed; false if fail
bool SeuYacc::transformIntoLALR() {
	std::map<IDType, IDType> mergeMap;
	// find same core state from last to one
	for (IDType checkState = _LR1parseTable.size() - 1; 0 < checkState; --checkState) {
		for (IDType it2State = 0; it2State < checkState; ++it2State) {
			auto& finalState = _LR1stateTransition.at(it2State)._lr1state
				, tomergeState = _LR1stateTransition.at(checkState)._lr1state;
			if (finalState.equalSameCore(tomergeState)) {
				mergeMap.insert(std::make_pair(checkState, it2State));
				break;
			}
		}
	}
	// make LALR_state_transition
	for (IDType index = 0; index < _LR1stateTransition.size(); ++index) {
		if (mergeMap.find(index) == mergeMap.end())
			// it is not a merge_need state,preserve it
			_LALR1stateTransition.insert(std::make_pair(index, _LR1stateTransition[index]));
	}
	// add the merged item list to the merge final one
	for (auto it : mergeMap) {
		auto& mergeFinalState = _LALR1stateTransition.find(it.second)->second._lr1state;
		for (auto& itemListIt : _LR1stateTransition.at(it.first)._lr1state._itemList) {
			mergeFinalState.addItem(itemListIt);
		}
	}

	// construct LALR_parsing table
	for (IDType curstate = 0; curstate != _LR1parseTable.size(); ++curstate) {
		std::vector<TableItem> newLine(_LR1parseTable[curstate]);
		// update _index to after-merge state id
		for (auto& it4 : newLine) {
			if (it4._action == SHIFT || it4._action == GOTO_STATE) {
				auto found = mergeMap.find(it4._index);
				if (found != mergeMap.end()) {
					it4._index = found->second;
				}
			}
		}
		auto found = mergeMap.find(curstate);
		if (found == mergeMap.end()) {
			// it is not a merge-need state
			_LALR1parseTable.insert(std::make_pair(curstate, newLine));
		}
		else {
			// merge same core states
			auto& mergeFinalTranItr = _LALR1parseTable.at(found->second).begin();
			for (auto& toMergeTran : newLine) {
				if (toMergeTran._action == REDUCTION || toMergeTran._action == SHIFT || toMergeTran._action == GOTO_STATE) {
					if (toMergeTran._action == mergeFinalTranItr->_action && toMergeTran._index != mergeFinalTranItr->_index) {
						return false;
					}
					*mergeFinalTranItr = toMergeTran;
				}
				++mergeFinalTranItr;
			}
		}
	}
	return true;
}

// content : Closure(T) is T with extended layer FAs
// return  : Clousure(T)
SeuYacc::LR1State SeuYacc::Closure(LR1State T) {
	for (auto& A : T._itemList) {
		// dot at the end
		if (A._dot == A._prod->_bodyList.end()) {
			continue;
		}
		for (auto& it2 : _productionVector) {
			if (it2._head == *(A._dot)) {
				auto next = A._dot; ++next;
				// A->B, add B->xxx to T
				if (next == A._prod->_bodyList.end()) {
					T.addItem(LR1Item(&it2, A._predSet));
				}
				else {
					auto found = _nonterminalNullable.find(*next);
					if (found == _nonterminalNullable.end()) {
						// A->a.Bb b, b is terminal
						std::set<ElementType> set; set.insert(*next);
						T.addItem(LR1Item(&it2, set));
					}
					else if (found->second) {
						// A->a.BCDE.., C can be empty
						std::set<ElementType> firstC = First(*next);
						firstC.erase(find(firstC.begin(), firstC.end(), EPLISON)); // erase empty
						firstC.insert(A._predSet.begin(), A._predSet.end()); // copy preSet to first(C)

						// add first(symbol DE...) to firstC
						auto next2 = next; ++next2;
						while (next2 != A._prod->_bodyList.end()) {
							auto found2 = _nonterminalNullable.find(*next2);
							if (found2 == _nonterminalNullable.end()) {
								// A->a.BCDE..., next2 D is terminal
								std::set<ElementType> set; set.insert(*next);
								firstC.insert(*next2);
								break;
							}
							// A->a.BCDE..., next2 D can be empty
							else if (found2->second) {
								std::set<ElementType> firstD = First(*next2);
								firstD.erase(find(firstD.begin(), firstD.end(), EPLISON)); // erase empty
								firstC.insert(firstD.begin(), firstD.end());
								++next2;
							}
							// A->a.BCDE..., next2 D can not be empty
							else {
								std::set<ElementType> firstD = First(*next2);
								firstC.insert(firstD.begin(), firstD.end());
								break;
							}
						}
						T.addItem(LR1Item(&it2, firstC));
					}
					else {
						// A->a.BCD, C can not be empty
						T.addItem(LR1Item(&it2, First(*next)));
					}

				}
			}
		} // end for each in _productionVector
	} // end for each in T._itemList
	return T;
}

// content : h with edge X to GOTO(T)
// return  : GOTO(T)
SeuYacc::LR1State SeuYacc::GOTO(LR1State h, ElementType edge) {
	LR1State result;
	for (auto& it : h._itemList) {
		if (it._dot != it._prod->_bodyList.end() && *(it._dot) == edge) {
			// shift, reserve same predictive symbol
			LR1Item item(it);
			++(item._dot);
			result.addItem(item);
		}
	}
	if (!result._itemList.empty())
		return Closure(result);
	else
		return result; // no such transition edge
}

// content : First(X) is all terminials X begins with
// return  : First(X)
std::set<SeuYacc::ElementType> SeuYacc::First(ElementType X) {
	// check whether First(X) has done
	auto foundFirstSet = _firstMap.find(X);
	if (foundFirstSet != _firstMap.end()) return foundFirstSet->second;
	// calculate First(X)
	std::set<ElementType> result;
	auto foundX = _nonterminalNullable.find(X);
	if (foundX == _nonterminalNullable.end()) {
		// X is terminal, First(X) = {X}
		result.insert(X);
	}
	else {
		if (foundX->second == true) {
			result.insert(EPLISON); // X can be empty
		}
		// find all productions with X as head
		for (auto& it : _productionVector) {
			if (it._head == X) {
				if (it._bodyList.empty()) continue; // X->empty
				if (*it._bodyList.begin() == X) continue; // X->X...
				for (auto it2 : it._bodyList) {
					//X->AYB
					auto foundA = _nonterminalNullable.find(it2);
					if (foundA == _nonterminalNullable.end()) {
						// A is terminal, add A to First(X)
						result.insert(it2);
						break;
					}
					else if (foundA->second == false) {
						// A is terminal and A is not nullable, add First(A) to First(X)
						std::set<ElementType> firstIT2 = First(it2);
						result.insert(firstIT2.begin(), firstIT2.end());
						break;
					}
					else {
						// A is terminal and A is nullable, add {First(A)-EPLISON} to First(X) 
						std::set<ElementType> firstIT2 = First(it2);
						firstIT2.erase(find(firstIT2.begin(), firstIT2.end(), EPLISON)); // erase empty
						result.insert(firstIT2.begin(), firstIT2.end());
					}
				}
			}
		}
	}
	_firstMap.insert(std::make_pair(X, result));
	return result;
}

// content : Follow(X) is all terminials X being front of
// return  : Follow(X)
std::set<SeuYacc::ElementType> SeuYacc::Follow(ElementType X) {
	std::set<ElementType> result;
	if (_productionVector.begin()->_head == X)  // X is start symbol
		result.insert(_terminalTable.find("$")->second);
	for (auto& it :_productionVector) {
		for (auto it2 = it._bodyList.begin(), it2End = it._bodyList.end(); it2 != it2End; ++it2) {
			if (*it2 == X) {
				// A->abcdXBCD..., next is B 
				auto next = it2; ++next;
				if (next == it2End) {
					if (it._head == X) break; // X->XB
					// A->abcdX, add first(A) to Follow(X)
					std::set<ElementType> firstIT2 = First(it._head);
					result.insert(firstIT2.begin(), firstIT2.end());
					break;
				}
				else {
					if (_nonterminalNullable.find(*next)->second == false) {
						// A->abcdXB..., B is not nullable, add first(B) to Follow(X)
						std::set<ElementType> firstIT2 = First(*next);
						result.insert(firstIT2.begin(), firstIT2.end());
						break;
					}
					else {
						// A->abcdXB..., B is nullable, add {first(B)-EPLISON} to Follow(X)
						std::set<ElementType> firstIT2 = First(*next);
						firstIT2.erase(find(firstIT2.begin(), firstIT2.end(), EPLISON)); // erase EPLISON

						// A->abcdXBCDE..., add first(symbol DE...) to firstC
						auto next2 = next; ++next2;
						while (next2 != it2End) {
							auto found2 = _nonterminalNullable.find(*next2);
							if (found2 == _nonterminalNullable.end()) {
								//A->aXBCDE..., next2 is terminal, add it to First(it2)
								firstIT2.insert(*next2);
								break;
							}
							else if (found2->second) {
								//A->aXBCDE..., next2 can be empty, add {First(D)-EPLISON} to First(C)
								std::set<ElementType> firstD = First(*next2);
								firstD.erase(find(firstD.begin(), firstD.end(), EPLISON)); // erase empty
								firstIT2.insert(firstD.begin(), firstD.end());
								//++next2; // ?? need checking
							}
							else {
								//A->aXBCDE..., next2 can not be empty, add First(D) to First(C)
								std::set<ElementType> firstD = First(*next2);
								firstIT2.insert(firstD.begin(), firstD.end());
								break;
							}
							++next2;
						}
						// add first(C) to Follow(X)
						result.insert(firstIT2.begin(), firstIT2.end());
						break;
					}
				}
			}
		}
	} // end for each in _productionVector
	return result;
}

// content : Follow(X) is all terminials X being front of
// return SHIFT or REDUCTION based on the adddtional condition
SeuYacc::ACTION_TYPE SeuYacc::conflictSolve(ElementType shiftOP, ElementType reduceOP) {
	auto sOp = _priority.find(shiftOP), rOp = _priority.find(reduceOP);

	if (sOp == _priority.end()) return REDUCTION;
	else if (rOp == _priority.end()) return SHIFT;
	else {
		// both priority is declared in file
		if (sOp->second < rOp->second) return REDUCTION;
		else if (rOp->second < sOp->second) return SHIFT;
		else {
			// equal priority, then judge left associatity
			return _leftAssociative.at(shiftOP) ? REDUCTION : SHIFT;
		}
	}
}

// content : reduce procedure, LR-parsing program
// input   : parsingFile -> xxx.cpp
// effect  :  
//bool SeuYacc::reduce(std::list<Token>* _tokenList, std::list<ProductionItem>* _reduceSequence) {
//
//	// use deque to simulate stack (for action convenience)
//	std::deque<StackItem> stack;
//
//	bool LALR_flag = !_LALR1parseTable.empty();
//
//	_tokenList->push_back(Token("$", "$", 0, 0, 0));
//	stack.push_back(StackItem(0, _terminalTable.find("$")->second)); // init stack
//	StackItem topItem(0, _terminalTable.find("$")->second);
//	ElementType terminalIndex = 99999;
//	auto ip = _tokenList->begin(), ipEnd = _tokenList->end();
//	while (ip != ipEnd) {
//		topItem = stack.back();
//		std::string terminal = ip->_attribute;
//		//if (ip->_attribute != "id" && ip->_attribute != "num") terminal = ip->_lexecal; // treat other token as terminal
//		terminalIndex = _terminalTable.find(terminal)->second;
//		TableItem tableItem;
//		if (LALR_flag) {
//			tableItem = _LALR1parseTable.find(topItem._state)->second.at(terminalIndex);
//		}
//		else {
//			tableItem = _LR1parseTable.at(topItem._state).at(terminalIndex);
//		}
//
//		if (tableItem._action == SHIFT) {
//			// push t onto the stack
//			stack.push_back(StackItem(tableItem._index, terminalIndex));
//			// let a be the next input symbol
//			++ip;
//		}
//		else if (tableItem._action == REDUCTION) {
//			// ACTION[s, a] = reduce A->beta
//			ProductionItem production = _productionVector.at(tableItem._index);
//			// pop |beta| symbols off the stack
//			for (size_t i = 0, bodyLength = production._bodyLength; i < bodyLength; ++i) stack.pop_back();
//			// let state t now be on top of stack
//			topItem = stack.back();
//			IDType goto_state = 0;
//			if (LALR_flag) {
//				goto_state = _LALR1parseTable.find(topItem._state)->second.at(production._head)._index;
//			}
//			else {
//				goto_state = _LR1parseTable.at(topItem._state).at(production._head)._index;
//			}
//			// push GOTO[t, A] onto stack
//			stack.push_back(StackItem(goto_state, production._head));
//			// output the production A->beta
//			_reduceSequence->push_back(_productionVector.at(tableItem._index));
//		}
//		else if (tableItem._action == ACCEPT) {
//			// parsing is done
//			_reduceSequence->push_back(_productionVector.at(0));
//			break;
//		}
//		else if (tableItem._action == ERROR) {
//			// M[state,a] is an error entry
//			//error("no such item in parsing table M[" + std::to_string(topItem.curState) + ',' + word.at(terminalIndex) + ']');
//			return false;
//			//break;
//		}
//		else {
//			return false;
//			//error("impossible parsing table action");
//			break;
//		}
//	}
//	return true;
//}

// output reduce sequence into filename
//void SeuYacc::outputReduceSequence(const std::string& filename, std::list<ProductionItem>* _reduceSequence) {
//
//	std::ofstream file;
//	file.open("reduce_sequence.txt", std::ios::out);
//
//	for (auto& it : *_reduceSequence) {
//		file << _word.at(it._head).c_str() << "->";
//		for (auto m : it._bodyList) {
//			file << _word.at(m).c_str() << ' ';
//		}
//		file << std::endl;
//	}
//
//	file.close();
//}

void SeuYacc::generateParsingProgram() {
	_outputFile << "#include \"lex.h\"\n";
	_outputFile << "#include \"yaccHelp.h\"\n";

	outputTable();
	outputAction();

	_outputFile << "int main(int argc, char *argv[]) {\n";
	_outputFile << "if (argc < 2) {\n";
	_outputFile << "std::cout << \"please input the target source file name\" << std::endl;\n";
	_outputFile << "return 1;\n";
	_outputFile << "}\n";
	_outputFile << "std::list<Token> tokenlist;\n";
	_outputFile << "yyparse(argv[1], tokenlist);\n";
	_outputFile << "yyreduce(tokenlist);\n";
	_outputFile << "std::cout << \"compile success\" << std::endl;\n";
	_outputFile << "return 0;\n";
	_outputFile << "}";
}

void SeuYacc::outputLRStateSet() {
	std::ofstream file;
	file.open("LR(1)_state_sets.txt", std::ios::out);

	if (!_LALR1stateTransition.empty()) {
		file << "number of states :" << _LALR1stateTransition.size() << std::endl;
		for (auto& stateIt : _LALR1stateTransition) {
			file << stateIt.first << ":" << std::endl;
			for (auto& lr1Item : stateIt.second._lr1state._itemList) {
				// output production with shift dot
				ProductionItem* prod = lr1Item._prod;
				auto it3 = prod->_bodyList.begin(), it3End = prod->_bodyList.end();
				file << "(" << _word.at(prod->_head).c_str() << "->";
				while (it3 != lr1Item._dot) {
					file << _word.at(*it3).c_str();
					++it3;
				}
				file << ".";
				while (it3 != it3End) {
					file << _word.at(*it3).c_str();
					++it3;
				}
				file << ",";
				// output predictives
				auto it4 = lr1Item._predSet.begin(), it4End = lr1Item._predSet.end();
				if (it4 != it4End) {
					file << _word.at(*it4).c_str(); ++it4;
					for (; it4 != it4End; ++it4) {
						file << '|' << _word.at(*it4).c_str();
					}
				}
				file << ')' << std::endl;
			}
			file << std::endl;
		}
	}
	else {
		file << "number of states :" << _LR1stateTransition.size() << std::endl;
		int stateIndex = 0;
		for (auto& stateIt : _LR1stateTransition) {
			file << stateIndex++ << ":" << std::endl;
			for (auto& lr1Item : stateIt._lr1state._itemList) {
				// output production with shift dot
				ProductionItem* prod = lr1Item._prod;
				auto it3 = prod->_bodyList.begin(), it3End = prod->_bodyList.end();
				file << "(" << _word.at(prod->_head).c_str() << "->";
				while (it3 != lr1Item._dot) {
					file << _word.at(*it3).c_str();
					++it3;
				}
				file << ".";
				while (it3 != it3End) {
					file << _word.at(*it3).c_str();
					++it3;
				}
				file << ",";
				// output predictives
				auto it4 = lr1Item._predSet.begin(), it4End = lr1Item._predSet.end();
				if (it4 != it4End) {
					file << _word.at(*it4).c_str(); ++it4;
					for (; it4 != it4End; ++it4) {
						file << '|' << _word.at(*it4).c_str();
					}
				}
				file << ')' << std::endl;
			}
			file << std::endl;
		}
	}

	file.close();
}


void SeuYacc::outputLRParsingTable() {
	std::ofstream file;
	file.open("LR(1)_parsing_table.txt", std::ios::out);

	if (!_LALR1parseTable.empty()) {
		for (auto& item : _LALR1parseTable) {
			file << "state " << item.first << std::endl;
			for (ElementType edge = 0; edge < _indexSymbol; ++edge) {
				switch (item.second[edge]._action) {
				case SHIFT:
					file << _word.at(edge) << " S " << item.second[edge]._index << std::endl; break;
				case REDUCTION:
					file << _word.at(edge) << " R " << item.second[edge]._index << std::endl; break;
				case ACCEPT:
					file << _word.at(edge) << " accept" << std::endl; break;
				case GOTO_STATE:
					file << _word.at(edge) << " G " << item.second[edge]._index << std::endl; break;
				}
			}
			file << std::endl;
		}
	}
	else {
		int stateIndex = 0;
		for (auto& item : _LR1parseTable) {
			file << "state " << stateIndex++ << std::endl;
			for (ElementType edge = 0; edge < _indexSymbol; ++edge) {
				switch (item[edge]._action) {
				case SHIFT:
					file << _word.at(edge) << " S " << item[edge]._index << std::endl; break;
				case REDUCTION:
					file << _word.at(edge) << " R " << item[edge]._index << std::endl; break;
				case ACCEPT:
					file << _word.at(edge) << " accept" << std::endl; break;
				case GOTO_STATE:
					file << _word.at(edge) << " G " << item[edge]._index << std::endl; break;
				}
			}
			file << std::endl;
		}
	}
	
	file.close();
}


void SeuYacc::outputTable() {
	std::ofstream out("tableYacc.h");
	out << "#ifndef _TABLE_YACC_H\n";
	out << "#define _TABLE_YACC_H\n";
	out << "#include \"StructDefine.h\"\n";
	out << "#include <vector>\n";
	out << "#include <map>\n";
	out << "void initTable(std::map<unsigned int, std::map<std::string, TableItem> >& _parseTable) {\n";
	out << "std::map<std::string, TableItem> tran;\n";
	if (!_LALR1parseTable.empty()) {
		for (auto& item : _LALR1parseTable) {
			out << "// state " << item.first << std::endl;
			for (ElementType edge = 0; edge < _indexSymbol; ++edge) {
				if (item.second[edge]._action != ERROR) {
					out << "tran.insert(std::make_pair(\"" << _word.at(edge) << "\", TableItem("
						<< item.second[edge]._action << "," << item.second[edge]._index << ")));\n";
				}
			}
			out << "_parseTable.insert(std::make_pair(" << item.first << ",tran));\n";
			out << "tran.swap(std::map<std::string, TableItem>());\n\n";
			out << std::endl;
		}
	}
	else {
		for (int i = 0; i < _LR1parseTable.size(); ++i) {
			out << "// state " << i << std::endl;
			auto& item = _LR1parseTable.at(i);
			for (ElementType edge = 0; edge < _indexSymbol; ++edge) {
				if (item.at(edge)._action != ERROR) {
					out << "tran.insert(std::make_pair(\"" << _word.at(edge) << "\", TableItem("
						<< item.at(edge)._action << "," << item.at(edge)._index << ")));\n";
				}
			}
			out << "_parseTable.insert(std::make_pair(" << i << ",tran));\n";
			out << "tran.swap(std::map<std::string, TableItem>());\n\n";
			out << std::endl;
		}
	}

	out << "}\n";
	out << "#endif //_TABLE_YACC_H\n";
	out.close();
}

// translate action string to runnable one
void SeuYacc::translateAction() {
	for (auto& mapAction : _productionAction) {
		std::string action = mapAction.second;
		std::string actionTranslate;
		size_t index = 0, indexEnd = action.length();
		while (index != indexEnd) {
			if (action[index] == '$') {
				++index;
				if (action[index] == '$') {
					++index; ++index;// skip dot
					std::string strAttr;
					while (isalpha(action[index])) {
						strAttr += action[index];
						++index;
					}
					actionTranslate += "reduceHead[\"" + strAttr + "\"]";
				}
				else if (isdigit(action[index])) {
					int pos = action[index] - '0';
					++index;
					while (isdigit(action[index])) {
						pos = pos * 10 + (action[index] - '0');
						++index;
					}
					++index; // skip dot
					std::string strAttr;
					while (isalpha(action[index])) {
						strAttr += action[index];
						++index;
					}
					if (strAttr == "width" || strAttr == "lexval") {
						actionTranslate += "atoi(stack[stackSize - " + std::to_string(_productionVector[mapAction.first]._bodyLength) 
							+ " + " + std::to_string(pos) + "]._map[\"" + strAttr + "\"].c_str())";
					}
					else {
						actionTranslate += "stack[stackSize - " + std::to_string(_productionVector[mapAction.first]._bodyLength) + " + " + std::to_string(pos) + "]._map[\"" + strAttr + "\"]";
					}
					
				}
				continue;
			}
			else if (action[index] == ';') {
				actionTranslate += ";\n";
				++index;
			}
			else if (action[index] == '|' && action[index+1] == '|') {
				actionTranslate += " + ";
				++index;
				++index;
			}
			else {
				actionTranslate += action[index];
				++index;
			}
		}
		mapAction.second = actionTranslate;
	}
}

// output productions and their actions
void SeuYacc::outputAction() {
	std::ofstream out("actionYacc.h");
	out << "#ifndef _ACTION_YACC_H\n";
	out << "#define _ACTION_YACC_H\n";
	out << "#include \"structDefine.h\"\n";
	out << "#include \"supportFunction.h\"\n";
	out << "#include <string>\n";
	out << "#include <stack>\n";
	out << "#include <stdlib.h>\n";

	out << "extern unsigned int offset;\n";
	out << "std::string p;\n";
	out << "extern std::deque<StackItem> stack;\n";
	out << "extern std::stack<std::string> paramStack;\n";

	out << "std::pair<unsigned int, std::string> performAction(unsigned int index, std::map<std::string, std::string>& reduceHead) {\n";
	out << "size_t stackSize = stack.size() - 1;\n";
	out << "switch(index) {\n";
	for (unsigned int index = 0; index < _productionAction.size(); ++index) {

		out << "case " << index << " :";
		// output production for view
		out << "//" << _word.at(_productionVector[index]._head).c_str() << "->";
		for (auto& it3 : _productionVector[index]._bodyList) {
			out << _word.at(it3).c_str() << " ";
		}
		out << std::endl << _productionAction[index] << std::endl;
		out << "return std::pair<unsigned int, std::string>(" 
			<< _productionVector[index]._bodyLength << ",\"" 
			<< _word[_productionVector[index]._head] << "\");\n\n";
	}
	out << "default: return std::pair<unsigned int, std::string>(0,\"\");\n";
	out << "}\n";
	out << "}// end function\n";

	out << "std::string getProduction(unsigned int index) {\n";
	out << "switch(index) {\n";
	for (unsigned int index = 0; index < _productionAction.size(); ++index) {
		out << "case " << index << " :";
		out << "return \"" << _word.at(_productionVector[index]._head).c_str() << "->";
		for (auto& it3 : _productionVector[index]._bodyList) {
			out << _word.at(it3).c_str() << " ";
		}
		out << "\";\n";
	}
	out << "default: return \"\";\n";
	out << "}\n";
	out << "}\n";
	out << "#endif //_ACTION_YACC_H\n";
	out.close();
}

#endif //_SEU_YACC_CPP
