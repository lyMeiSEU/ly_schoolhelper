//	  file : SeuLex.cpp
// content : lexical analyze tool class    
//	  date : 2017/4/23
//  author : Wangduo
//  @COPYRIGHT  : School of Computer Science & Engineering Southeast University

#ifndef _SEU_LAX_CPP
#define _SEU_LAX_CPP

#include "SeuLex.h"
#include <queue>
#include <iostream>
#include <ctime>

struct Token;

#define MAX_CHAR_PER_LINE 100

// content : construct function
// input   : reFile -> xxx.l
//           parsingFile -> parsing file xxx.cpp generated
SeuLex::SeuLex(const std::string& reFile, const std::string& parsingFile) {
	_symbolTable = new StringHash();
	outputFile.open(parsingFile);	
	if (readREFile(reFile)) {
		_NFA_start = new State(0);
		createNFA();
		createDFA();
		optimizeDFA();
		generateParsingProgram();
	}
	outputFile.close();
};

// content : descontruct function
SeuLex::~SeuLex() {
	if (_symbolTable != nullptr) {
		delete _symbolTable;
	}
	_symbolTable = nullptr;

	// release source
	std::queue<State*> queue;
	queue.push(_NFA_start);
	while (!queue.empty()) {
		State* cur = queue.front();
		queue.pop();
		if (cur != nullptr) {
			if (!cur->_transition.empty()) {
				for (auto tran : cur->_transition) {
					if (cur->_id < tran._next->_id)
						queue.push(tran._next);
				}
			}
			if (cur != nullptr) delete cur; 
			cur = nullptr;
		}
	}
}


// content : read a char from sourse file
// return : true if succeed; false if fail
bool SeuLex::read() {
	if (_offset == -1) {
		if (!_reFile.eof()) {
			_reFile.getline(_buffer, MAX_CHAR_PER_LINE);
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
void SeuLex::retract() {
	//_sourcefile.putback(_peek);
	if (_offset == -1) _offset = strlen(_buffer);
	_offset -= 1;
}

// return : innerCode in _symbolTable
// if it is not in, return _numSymbol
unsigned int SeuLex::lookup(const std::string& str) {
	auto found = _symbolTable->find(str);
	if (found != _symbolTable->end()) return found->second;
	else return _numSymbol;
}

// content : output error message to screen
void SeuLex::error(const std::string& errorStr) {
	//string str = string(' ', _offset) + '\0';
	std::cout << "line  " << _line << " : " << _buffer << std::endl;
	//std::cout << str.c_str() << '^' << std::endl;
	std::cout << errorStr.c_str() << std::endl;
}

// content : read include and variable declation
bool SeuLex::readHeadInformation() {
	read();
	if (_peek != '%') {
		retract();
		return true; // no head part
	}
	read();
	read(); // skip {
	while (_peek != '%') {
		outputFile << _peek;
		read();
		if (_offset == -1)outputFile << '\n';
	}
	read();// skip }
	return true;
}

// content : read regular definitions from file, store them in REs
// return : true if succeed; false if fail
bool SeuLex::readRegularDefinition() {

	std::list<ElementType> relist;
	std::string head = "";

	std::stack<bool> firstFlagStack;

	while (true) {
		// read head string
		head = "";
		read();
		while (_offset != 0) { read(); }
		if (_peek == '%') { read(); break; }
		while (_peek != ' ') {
			head.operator+=(_peek);
			read();
		}
		// read the body
		relist.swap(std::list<ElementType>());
		read();
		char endChar = _peek;
		char lastChar = _peek;
		bool firstFlag = true;
		while (_peek != ';') {
			// solve [x~s] format
			if (_peek == '[') {
				if (!firstFlag) relist.push_back(CONCAT_OP);
				firstFlag = false;
				firstFlagStack.push(firstFlag);
				firstFlag = true;
				relist.push_back(LEFT_BRACE_OP);
				while (_peek != ']') {
					lastChar = _peek;
					read();
					if (_peek == ']') break;
					if (_peek == '~') {
						read(); endChar = _peek + 1; ++lastChar;
						while (lastChar != endChar) {
							auto found = _inputRE.find(lastChar);
							if (found == _inputRE.end()) {
								relist.push_back(UNION_OP);
								relist.push_back(++_reIndex);
								_inputRE.insert(std::make_pair(lastChar, _reIndex));
								_outputRE.insert(std::make_pair(_reIndex, lastChar));
							}
							else {
								relist.push_back(UNION_OP);
								relist.push_back(found->second);
							}
							++lastChar;
						}
						read();
						if (_peek != ']') retract();
					}
					else {
						auto found = _inputRE.find(_peek);
						if (found == _inputRE.end()) {
							if (!firstFlag) relist.push_back(UNION_OP);
							firstFlag = false;
							relist.push_back(++_reIndex);
							_inputRE.insert(std::make_pair(_peek, _reIndex));
							_outputRE.insert(std::make_pair(_reIndex, _peek));
						}
						else {
							relist.push_back(found->second);
						}
					}
				}
				relist.push_back(RIGHT_BRACE_OP);
				firstFlag = firstFlagStack.top();
				firstFlagStack.pop();
				read();
				continue;
			}
			// solve {nonterminal}
			else if (_peek == '{') {
				std::string nonterminal = "";
				if (!firstFlag) relist.push_back(CONCAT_OP);
				firstFlag = false;
				read();
				while (_peek != '}') {
					nonterminal.operator+=(_peek);
					read();
				}
				auto found = _REs.find(nonterminal);
				if (found != _REs.end()) {
					firstFlag = false;
					relist.push_back(LEFT_BRACE_OP);
					for (auto each : found->second)
						relist.push_back(each);
					relist.push_back(RIGHT_BRACE_OP);
				}
				else {
					error("no such nonterminal definition above");
					return false;
				}
				read();
				continue;
			}
			else if (_peek == '|') {
				relist.push_back(UNION_OP);
				firstFlag = true;
				read();
				continue;
			}
			else if (_peek == '*') {
				relist.push_back(STAR_OP);
				read();
				continue;
			}
			else if (_peek == '(') {
				relist.push_back(CONCAT_OP);
				relist.push_back(LEFT_BRACE_OP);
				firstFlag = true;
				read();
				continue;
			}
			else if (_peek == ')') {
				relist.push_back(RIGHT_BRACE_OP);
				firstFlag = false;
				read();
				continue;
			}
			else if (_peek == '?') {
				relist.push_back(QUESTION_OP);
				read();
				continue;
			}
			else if (_peek == '+') {
				relist.push_back(PLUS_OP);
				read();
				continue;
			}
			else {
				auto found = _inputRE.find(_peek);
				if (!firstFlag) relist.push_back(CONCAT_OP);
				firstFlag = false;
				if (found == _inputRE.end()) {
					relist.push_back(++_reIndex);
					_inputRE.insert(std::make_pair(_peek, _reIndex));
					_outputRE.insert(std::make_pair(_reIndex, _peek));
				}
				else {
					relist.push_back(found->second);
				}
				read();
				continue;
			}
		}
		_REs.insert(std::make_pair(head, relist));
	}

	return true;
}

// content : convert REs to postfix format
// return : true if succeed; false if fail
bool  SeuLex::readTranslationRule() {
	bool firstFlag = true;
	std::string head = "";
	std::string actionStr = "";
	std::list<ElementType> relist;
	while (true) {
		while (_offset != 0) read();
		if (_peek == '%') break;

		head = "";
		actionStr = "";
		relist.swap(std::list<ElementType>());
		firstFlag = true;

		// word in {xxxx}
		if (_peek == '{') {
			read();
			while (_peek != '}') {
				head.operator+=(_peek);
				read();
			}
			while (_peek != '{') read();
			if (_peek == '{') {
				read();
				// read action part
				while (_peek != '}') {
					actionStr.operator+=(_peek);
					read();
				}
				if (actionStr.back() == '\"') {
					actionStr.operator+=(_peek);
					read();
					while (_peek != '}') {
						actionStr.operator+=(_peek);
						read();
					}
				}
			}

			auto found = _REs.find(head);
			if (found == _REs.end()) {
				error("no such regular definition up");
				return false;
			}
			relist = found->second;
		}
		// word in "xxxx"
		else if (_peek == '"') {
			read();
			while (_peek != '"') {
				head.operator+=(_peek);
				auto found = _inputRE.find(_peek);
				if (!firstFlag) relist.push_back(CONCAT_OP);
				firstFlag = false;
				if (found == _inputRE.end()) {
					relist.push_back(++_reIndex);
					_inputRE.insert(std::make_pair(_peek, _reIndex));
					_outputRE.insert(std::make_pair(_reIndex, _peek));
				}
				else {
					relist.push_back(found->second);
				}
				read();
			}
			while (_peek != '{') read();
			read(); // skip {
			// read action part
			while (_peek != '}') {
				actionStr.operator+=(_peek);
				read();
			}
			if (actionStr.back() == '\"') {
				actionStr.operator+=(_peek);
				read();
				while (_peek != '}') {
					actionStr.operator+=(_peek);
					read();
				}
			}
		}
		// reserved word
		else {
			while (_peek != ' ') {
				head.operator+=(_peek);
				auto found = _inputRE.find(_peek);
				if (!firstFlag) relist.push_back(CONCAT_OP);
				firstFlag = false;
				if (found == _inputRE.end()) {
					relist.push_back(++_reIndex);
					_inputRE.insert(std::make_pair(_peek, _reIndex));
					_outputRE.insert(std::make_pair(_reIndex, _peek));
				}
				else {
					relist.push_back(found->second);
				}
				read();
			}
			while (_peek != '{') read();
			read();
			while (_peek != '}') {
				actionStr.operator+=(_peek);
				read();
			}
			if (actionStr.back() == '\"') {
				actionStr.operator+=(_peek);
				read();
				while (_peek != '}') {
					actionStr.operator+=(_peek);
					read();
				}
			}
		}

		// convert to Post format and add it to map
		_postREs.push_back(convertPost(relist));
		_actionMap.push_back(actionStr);
	}

	return true;
}

// content : read RE file content
// input   : reFile -> xxx.l
// return  : true when succeed; false when error
bool SeuLex::readREFile(const std::string& reFile) {
	_reFile.open(reFile, std::ios::in);
	if (_reFile.fail()) {
		std::cout << reFile.c_str() << " cannot be open\n";
		return false;
	}

	_buffer = new char[MAX_CHAR_PER_LINE];
	_line = 0;
	_offset = -1;

	bool failFlag = false;
	if (failFlag || !readHeadInformation()) failFlag = true;
	if (failFlag || !readRegularDefinition()) failFlag = true;
	if (failFlag || !readTranslationRule()) failFlag = true;

	_selfDefinedProgramme = false;
	while (!_reFile.eof()) {
		_reFile.getline(_buffer, MAX_CHAR_PER_LINE);
		int d = 0;
		while (_buffer[d] == ' ' || _buffer[d] == '\t') ++d;
		if (isalpha(_buffer[d])) {
			while (isalpha(_buffer[d])) ++d;
			while (_buffer[d] == ' ' || _buffer[d] == '\t') ++d;
			if (_buffer[d] == 'm' && _buffer[d + 1] == 'a' && _buffer[d + 2] == 'i'&& _buffer[d + 3] == 'n') {
				d = d + 4;
				while (_buffer[d] == ' ' || _buffer[d] == '\t') ++d;
				if (_buffer[d] == '(') {
					failFlag = false;
					_selfDefinedProgramme = true;
					d = 0;
					while (_buffer[d] == ' ' || _buffer[d] == '\t') ++d;
					while (isalpha(_buffer[d])) ++d;
					while (_buffer[d] == ' ' || _buffer[d] == '\t') ++d;
					d = d + 3;
					for (int i = 0; i <= d; i++) { outputFile << _buffer[i]; }
					outputFile << "2";
					++d;
					while (_buffer[d] != '\0') {
						outputFile << _buffer[d];
						++d;
					}
					outputFile << std::endl;
					continue;
				}
				else { failFlag = true; }
			}
		}

		outputFile << _buffer << std::endl;
	}

	if (failFlag == true) {
		error("User defined programs are wrong in RE!");
		return !failFlag;
	}

	// treat blank as a legal input
	++_reIndex;
	_inputRE.insert(std::make_pair(' ', _reIndex));
	++_reIndex;

	_reFile.close();
	delete[] _buffer;
	_buffer = nullptr;

	return !failFlag;
}

// content : convert to postfix format
// input   : list -> tht infix RE body list
// return  : the postfix RE body list
std::list<SeuLex::ElementType> SeuLex::convertPost(std::list<ElementType>& list) {
	std::list<ElementType> postlist;
	std::stack<ElementType> stack;
	for (auto k : list) {
		if (k == UNION_OP) {
			while (!stack.empty() && k <= stack.top()) {
				postlist.push_back(stack.top());
				stack.pop();
			}
			stack.push(k);
		}
		else if (k == CONCAT_OP) {
			while (!stack.empty() && k <= stack.top()) {
				postlist.push_back(stack.top());
				stack.pop();
			}
			stack.push(k);
		}
		else if (k == STAR_OP) {
			postlist.push_back(k);
		}
		else if (k == PLUS_OP) {
			postlist.push_back(k);
		}
		else if (k == QUESTION_OP) {
			postlist.push_back(k);
		}
		else if (k == LEFT_BRACE_OP) {
			stack.push(k);
		}
		else if (k == RIGHT_BRACE_OP) {
			while (stack.top() != LEFT_BRACE_OP) {
				postlist.push_back(stack.top());
				stack.pop();
			}
			stack.pop();  //pop LEFT_BRACE_OP
		}
		else {
			postlist.push_back(k);
		}
	}
	// pop rest element
	while (!stack.empty()) {
		postlist.push_back(stack.top());
		stack.pop();
	}
	return postlist;
}

// content : make a NFA with an edge
// input   : chInput -> edge element
// effect  : the NFA is added into _operandStack
void SeuLex::PUSH(ElementType chInput) {
	// create 2 new states on the stack

	State* s0 = new State(++_nextStateID);
	State* s1 = new State(++_nextStateID);
	s0->addEdge(chInput, s1);

	// create a NFA from these 2 states
	FA nfa;
	nfa.push_back(s0);
	nfa.push_back(s1);

	// push it onto the operand stack
	_operandStack.push(nfa);
}

// content : make a NFA by concat with two NFAs
// input   : 
// effect  : pop two NFAs and the new NFA is added into _operandStack
bool SeuLex::CONCAT() {
	//pop 2 elements
	FA A, B;
	if (_operandStack.empty()) return false;
	B = _operandStack.top(); _operandStack.pop();
	if (_operandStack.empty()) return false;
	A = _operandStack.top(); _operandStack.pop();

	// evaluate AB
	// take the last state from A
	// add an epsilon transition to the first state of B
	// store the result into new NFA and push
	A.back()->addEdge(EPSILON, B.front());
	A.push_back(B.back());
	_operandStack.push(A);
	return true;
}

// content : make a NFA by star the top one, zero or more turn
// input   : 
// effect  : the new NFA is added into _operandStack
bool SeuLex::STAR() {
	// pop 1 element
	FA A;
	if (_operandStack.empty()) return false;
	A = _operandStack.top(); _operandStack.pop();

	// evaluate A*
	// create 2 new states which will be inserted at each end of deque
	// take A and make a epsilon transition 
	// from last to the first state in the deque
	// add epsilon transition from begin to end of new NFA
	State* start = new State(++_nextStateID);
	State* end = new State(++_nextStateID);
	start->addEdge(EPSILON, A.front());
	start->addEdge(EPSILON, end);
	A.back()->addEdge(EPSILON, end);
	A.back()->addEdge(EPSILON, A.front());
	A.push_back(end);
	A.push_front(start);
	_operandStack.push(A);
	return true;
}

// content : make a NFA by union with two NFAs
// input   : 
// effect  : pop two NFAs and the new NFA is added into _operandStack
bool SeuLex::UNION() {
	// pop 2 elements
	FA A, B;
	if (_operandStack.empty()) return false;
	B = _operandStack.top(); _operandStack.pop();
	if (_operandStack.empty()) return false;
	A = _operandStack.top(); _operandStack.pop();

	// elaluate A|B
	// create 2 new states as start and end of new NFA
	// create epsilon transitions from start state to the start states of A,B
	// create epsilon transition from end states of A,B to new end state
	State* start = new State(++_nextStateID);
	State* end = new State(++_nextStateID);
	start->addEdge(EPSILON, A.front());
	start->addEdge(EPSILON, B.front());
	A.back()->addEdge(EPSILON, end);
	B.back()->addEdge(EPSILON, end);

	A.push_front(start);
	A.push_back(end);
	_operandStack.push(A);
	return true;
}

// content : make a NFA by question the top one, zero or one turn
// input   : 
// effect  : the new NFA is added into _operandStack
bool SeuLex::QUESTION() {
	// pop 1 element
	FA A;
	if (_operandStack.empty()) return false;
	A = _operandStack.top(); _operandStack.pop();

	// evaluate A?
	// create 2 new states which will be inserted at each end of deque
	// take A and make a epsilon transition 
	// from the first state to the end in the deque
	State* start = new State(++_nextStateID);
	State* end = new State(++_nextStateID);
	start->addEdge(EPSILON, A.front());
	A.back()->addEdge(EPSILON, end);
	start->addEdge(EPSILON, end);
	A.push_back(end);
	A.push_front(start);
	_operandStack.push(A);
	return true;
}

// content : make a NFA by plus the top one, one or more turn
// input   : 
// effect  : the new NFA is added into _operandStack
bool SeuLex::PLUS() {
	// pop 1 element
	FA A;
	if (_operandStack.empty()) return false;
	A = _operandStack.top(); _operandStack.pop();

	// evaluate A+
	// create 2 new states which will be inserted at each end of deque
	// take A and make a epsilon transition 
	// from last to the first state in the deque
	State* start = new State(++_nextStateID);
	State* end = new State(++_nextStateID);
	start->addEdge(EPSILON, A.front());
	A.back()->addEdge(EPSILON, end);
	A.back()->addEdge(EPSILON, A.front());
	A.push_back(end);
	A.push_front(start);
	_operandStack.push(A);
	return true;
}

// content : make NFA by merge each two NFAs into one
// input   : 
// effect  : _NFAfinalStateSet filled 
// return  : true if succeed; false if fail
bool SeuLex::createNFA() {
	for (IDType re = 0, reEnd = _postREs.size(); re != reEnd; ++re) {

		for (auto i : _postREs.at(re)) {
			if (i == CONCAT_OP) {
				CONCAT();
			}
			else if (i == UNION_OP) {
				UNION();
			}
			else if (i == STAR_OP) {
				STAR();
			}
			else if (i == QUESTION_OP) {
				QUESTION();
			}
			else if (i == PLUS_OP) {
				PLUS();
			}
			else {
				PUSH(i);
			}
		}
		FA A = _operandStack.top(); _operandStack.pop();
		_NFA_start->addEdge(EPSILON, A.front());
		_NFAfinalStateSet.insert(A.back()->_id);
		_NFAAction.insert(std::make_pair(A.back()->_id, re));
	}

	return true;
}

// content : include all EPSILON edges to make closure(T)
//           using stack to prevent deal cycle
// input   : T -> the kernel of DFA state
// return  : Closure(T) DFA state
SeuLex::DFAState SeuLex::Closure(DFAState T) {
	
	auto& found = _ClosureMap.find(T);
	if (found != _ClosureMap.end()) return found->second;

	DFAState result(T);// init result to T
	std::stack<State*> stack;
	// push all states in T onto the stack
	for (auto state : T) stack.push(state);
	while (!stack.empty()) {
		State* u = stack.top(); stack.pop();
		// for each epsilon edge
		for (auto tran : u->_transition) {
			if (tran._edge == EPSILON) {
				// if the next state didnot occur, add it to stack
				if (result.insert(tran._next).second == true) {
					stack.push(tran._next);
				}
			}
		}
	}
	_ClosureMap.insert(std::make_pair(T, result));
	return result;
}

// content : Move(T) from DFA state T at edge input
// input   : T -> the kernel of DFA state
//         : inpnut -> transition edge
// return  : Move(T) DFA state 
SeuLex::DFAState SeuLex::Move(DFAState T, ElementType input) {
	DFAState result;
	for (auto& state : T) {
		for (auto& tran : state->_transition) {
			if (tran._edge == input) {
				result.insert(tran._next);
			}
		}
	}
	return result;
}

// content : Closure each equal-value NFA state to a DFA state
// input   : 
// effect  : fill _DFAStateMap , _DFAfinalStateSet, _DFAStateTranfer
// return  : true if succeed; false if fail
bool SeuLex::createDFA() {

	// init with the start state 
	DFAState set; set.insert(_NFA_start);
	DFAState start(Closure(set));
	_DFAStateMap.insert(std::make_pair(start, 0));

	// check whether start state is a final state
	for (auto state : start) {
		if (_NFAfinalStateSet.find(state->_id) != _NFAfinalStateSet.end()) {
			// start state is a final DFA state
			_DFAfinalStateSet.insert(0); // 0 is the start state id
			break;
		}
	}

	// work on rest NFA states
	std::queue<DFAState> unmarkedDFAState;
	unmarkedDFAState.push(start);
	IDType maxDFAstateIndex = 1;

	while (!unmarkedDFAState.empty()) {
		StatesTransfer tran;
		DFAState oldstate = unmarkedDFAState.front(); unmarkedDFAState.pop();
		// input charactor edge from _minREelement to _maxREelement
		for (ElementType edge = _minREelement; edge != _reIndex; ++edge) {
			DFAState newState = Closure(Move(oldstate, edge));
			if (newState.empty()) continue; // no such transition on the edge
			auto found = _DFAStateMap.find(newState);
			if (found != _DFAStateMap.end()) {
				// newState has existed, add transition edge to it
				tran.insert(std::make_pair(edge, found->second));
			} else {
				// new DFA state
				unmarkedDFAState.push(newState);
				_DFAStateMap.insert(std::make_pair(newState, maxDFAstateIndex));
				tran.insert(std::make_pair(edge, maxDFAstateIndex));

				IDType minID = UINT_MAX;

				// check whether it is a final state, action define by first occurred one
				for (auto& state : newState) {
					if (_NFAfinalStateSet.find(state->_id) != _NFAfinalStateSet.end()) {
						_DFAfinalStateSet.insert(maxDFAstateIndex);
						
						auto& found = _DFAAction.find(maxDFAstateIndex);
						if (found == _DFAAction.end()) {
							_DFAAction.insert(std::make_pair(maxDFAstateIndex, _NFAAction.at(state->_id)));
							minID = state->_id;
						}
						else if (state->_id < minID) {
							found->second = _NFAAction.at(state->_id);
							minID = state->_id;
						}
					}
				}
				// increase state index
				++maxDFAstateIndex;
			}
		}
		_DFAStateTranfer.push_back(tran);
	}
	return true;
}

// content : merge same-core DFA states to single one
// input   : 
// effect  : fill _DFAStateMap , _DFAfinalStateSet, _DFAStateTranfer
// return  : true if succeed; false if fail
bool SeuLex::optimizeDFA() {

	// init same-core group
	std::set<std::set<IDType> > group;
	std::set<IDType> groupNotAccept;
	for (IDType i = 0, DFAstateNumber = _DFAStateTranfer.size(); i < DFAstateNumber; ++i) {
		if (_DFAfinalStateSet.find(i) == _DFAfinalStateSet.end()) {
			groupNotAccept.insert(i);
		}
	}
	group.insert(groupNotAccept);
	for (auto& it : _DFAfinalStateSet) {
		std::set<IDType> g;
		for (auto& it2 : _DFAfinalStateSet) {
			if (_DFAAction.find(it)->second == _DFAAction.find(it2)->second) {
				g.insert(it2);
			}
		}
		group.insert(g);
	}
	//group.insert(_DFAfinalStateSet);

	// break no same-core group from top to down
	std::set<std::set<IDType> > groupTwo = group;
	bool continueFlag = true;
	while (continueFlag) {
		group = groupTwo;
		continueFlag = false;
		for (auto G = group.begin(), GEnd = group.end(); G != GEnd; ++G) {
			if (++(G->begin()) == G->end()) continue; // G.size() == 1, skip
			groupTwo.erase(*G);
			IDType firstState = *(G->begin());       // seperate G by the first state in it
			std::set<IDType> newGroup, newGroup2;
			newGroup2.insert(firstState);
			for (auto state = ++(G->begin()); state != G->end(); ++state) {
				bool sameGroupFlag = true;
				if (_DFAStateTranfer.at(firstState).empty() && _DFAStateTranfer.at(*state).empty()) {
					// both have no edge out
					sameGroupFlag = _DFAAction.at(firstState) == _DFAAction.at(*state);
				}
				else if (_DFAStateTranfer.at(firstState).size() != _DFAStateTranfer.at(*state).size()) {
					// the number of out edges is not equal
					sameGroupFlag = false;
				}
				else {
					// judge whether these two states' all destination states in same group
					for (ElementType edge = _minREelement; edge != _reIndex; ++edge) {
						auto next1 = _DFAStateTranfer.at(firstState).find(edge);
						auto next2 = _DFAStateTranfer.at(*state).find(edge);

						if (next1 != _DFAStateTranfer.at(firstState).end()) {
							if (next2 != _DFAStateTranfer.at(*state).end()) {
								// both has input-edge, judge whether they are in the same group
								bool eachDestinateInSameGroupFlag = true;
								for (auto& p : group) {
									if (p.find(next1->second) != p.end()) {
										if (p.find(next2->second) != p.end()) {
											break;// next1 and next2 both not in group p
										}
										else {
											// next1 in p but next2 not in p
											eachDestinateInSameGroupFlag = false;
											break;
										}
									}
									else {
										if (p.find(next2->second) != p.end()) {
											// next1 not in p but next2 in p
											eachDestinateInSameGroupFlag = false;
											break;
										}
										else {
											// next1 and next2 both in p
											continue;
										}
									}
								}
								if (eachDestinateInSameGroupFlag) {
									sameGroupFlag = true;
								}
								else {
									sameGroupFlag = false;
									break;
								}
							}
							else {
								// next1 has edge but next2 doesn't
								sameGroupFlag = false;
								break; 
							}
						}
						else {
							if (next2 != _DFAStateTranfer.at(*state).end()) {
								// next2 has edge but next1 doesn't
								sameGroupFlag = false;
								break; 
							}
							else {
								// neither has edge
								continue; 
							}
						}
					}
				}

				if (sameGroupFlag) {
					newGroup2.insert(*state);
				}
				else {
					newGroup.insert(*state);
				}
			}
			if (!newGroup.empty()) {
				// sepatate into two groups
				groupTwo.insert(newGroup);
				groupTwo.insert(newGroup2);
				continueFlag = true;
			}
			else {
				groupTwo.insert(newGroup2);
			}
		}
	}

	// construct mergeMap
	std::map<IDType, IDType> mergeMap;
	IDType maxID = 0;
	for (auto& g : group) {
		for (auto& state : g) {
			mergeMap.insert(std::make_pair(state, maxID));
		}
		auto& found = _DFAAction.find(*g.begin());
		if (found != _DFAAction.end())
			_minDFAAction.insert(std::make_pair(maxID, found->second));
		++maxID;
	}

	// construct _minDFAStateTranfer and _minDFAfinalStateSet
	for (auto& g : group) {
		StatesTransfer newTran;
		if (_DFAfinalStateSet.find(*g.begin()) != _DFAfinalStateSet.end())
			_minDFAfinalStateSet.insert(mergeMap.find(*g.begin())->second);
		for (auto& edge : _DFAStateTranfer.at(*g.begin())) {
			newTran.insert(std::make_pair(edge.first, mergeMap.find(edge.second)->second));
		}
		_minDFAStateTranfer.push_back(newTran);
	}

	return true;
}

// content : generate parsing code
// input   : parsingFile -> xxx.cpp
// effect  : 
void SeuLex::generateParsingProgram() {

	outputFile << "\n#include \"lexHelp.h\"\n";

	outputMinDFA();
	outputAction();

	outputFile << "int yyparse(char* file, std::list<Token>& tokenList) {\n";
	outputFile << "yyparseFct(file,tokenList);\n";
	if (_selfDefinedProgramme == true)
	{
		outputFile << "main2();\n";
	}
	outputFile << "return 0;\n";
	outputFile << "}";
}

void SeuLex::outputAction() {
	std::ofstream out("actionLex.h");
	out << "#ifndef _ACTION_LEX_H\n";
	out << "#define _ACTION_LEX_H\n";
	out << "#include <set>\n";
	out << "void initFinalSet(std::set<unsigned int>& finalSet) {\n";
	for (auto& state : _minDFAfinalStateSet)
		out << "finalSet.insert(" << state << ");\n";
	out << "}\n\n";
	
	out << "std::string performAction(unsigned int state) {\n";
	out << "switch(state) {\n";
	for (unsigned int index = 0; index < _minDFAStateTranfer.size(); ++index) {
		auto& found = _minDFAAction.find(index);
		if (found != _minDFAAction.end()) {
			out << "case " << index << " :";
			out << _actionMap.at(found->second) << std::endl;
		}
	}
	out << "default: return \"\";\n";
	out << "}\n";
	out << "}// end function\n";
	out << "#endif //_ACTION_LEX_H\n";
	out.close();
}

// content : output minDFA to output
// input   : output -> ofstream handler
// effect  : 
void SeuLex::outputMinDFA() {
	std::ofstream out("tableLex.h");
	out << "#ifndef _TABLE_LEX_H\n";
	out << "#define _TABLE_LEX_H\n";
	out << "#include <vector>\n";
	out << "#include <map>\n";
	out << "void initMinDFAStateTranfer(std::vector<std::map<char, unsigned int> >* _minDFAStateTranfer) {\n";
	out << "std::map<char, unsigned int> tran;\n";
	unsigned int stateIndex = 0;
	for (auto& tran : _minDFAStateTranfer) {
		out << "// state " << stateIndex++ << ";\n";
		for (auto& edge : tran) {
			out << "tran.insert(std::make_pair(\'" << _outputRE.find(edge.first)->second << "\'," << edge.second << "));\n";
		}
		out << "_minDFAStateTranfer->push_back(tran);\n";
		out << "tran.swap(std::map<char, unsigned int>());\n\n";
	}
	out << "}\n";
	out << "#endif //_TABLE_LEX_H\n";
	out.close();
}

// content : parse by minDFA to get the token list
// input   : file -> ofstream handler
// effect  : 
//bool SeuLex::parseByminDFA(std::list<Token>* tokenList, const std::string& file) {
//
//	_buffer = new char[MAX_CHAR_PER_LINE];
//	_line = 0;
//	_offset = -1;
//	IDType state = 0;
//	std::string lexeme = "";
//	unsigned int innerCode;  // inner code for value attribute of token
//
//	_reFile.open(file, std::ios::in);
//	if (_reFile.fail()) {
//		std::cout << file.c_str() << " cannot be open\n";
//		return false;
//	}
//
//	while (true) {
//		if (!read()) break;
//		auto indexFound = _inputRE.find(_peek);
//		if (indexFound == _inputRE.end()) {
//			error("illegal input character");
//			break;
//		}
//		StatesTransfer& tran = _minDFAStateTranfer.at(state);
//		auto found = tran.find(indexFound->second);
//		if (found != tran.end()) {
//			// state transition
//			state = found->second;
//			lexeme.operator+=(_peek);
//		}
//		else  {
//			// check whether it is a final state
//			if (!lexeme.empty() && _minDFAfinalStateSet.find(state) != _minDFAfinalStateSet.end()) {
//
//				innerCode = lookup(lexeme);
//				if (innerCode == _numSymbol) {
//					// new identifier
//					(*_symbolTable)[lexeme] = _numSymbol++;
//				}
//				tokenList->push_back(Token(lexeme, _actionMap.at(_minDFAAction.at(state)), innerCode, _line, _offset));
//
//				state = 0;
//				lexeme = "";
//				if (_peek != ' ') retract();
//			}
//			else {
//				if (_peek == ' ') continue;
//				else {
//					error("no such transition in DFA");
//					break;
//				}
//			}
//		}
//	};
//
//	delete[] _buffer; 
//	_buffer = nullptr;
//	_reFile.close();
//	return true;
//}


#endif _SEU_LAX_CPP