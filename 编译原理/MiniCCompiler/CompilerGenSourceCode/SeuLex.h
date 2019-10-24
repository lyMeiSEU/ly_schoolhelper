//	  file : SeuLex.h
// content : lexical analyze tool class 
//	  date : 2017/4/23
//  author : Wangduo
//  @COPYRIGHT  : School of Computer Science & Engineering Southeast University

#ifndef _SEU_LEX_H
#define _SEU_LEX_H

// define statement for use hash_map
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#include "Token.h"
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <deque>
#include <stack>
#include <set>
#include <hash_map>

// compare function for string hash_map use
struct string_less : public std::binary_function<const std::string, const std::string, bool> {
public:
	result_type operator()(const first_argument_type& _Left
		, const second_argument_type& _Right) const {
		return _Left.compare(_Right) < 0;
	}
};

class SeuLex {

	typedef unsigned int ElementType;
	typedef unsigned int IDType;
	typedef std::string Action; // updating!!!

	struct State {
		State(IDType id) : _id(id) {}
		void addEdge(ElementType edge, State* next) {
			_transition.push_back(Edge(edge, next));
		}
		struct Edge {
			Edge(ElementType edge, State* next) : _edge(edge), _next(next) {}
			State* _next;
			ElementType _edge;
		};
		std::list<Edge> _transition;
		IDType _id;
	}; // struct State

	typedef typename std::deque<State*> FA;
	typedef typename std::set<State*> DFAState;
	typedef typename std::map<ElementType, IDType> StatesTransfer;
	typedef std::hash_map<std::string, unsigned int, std::hash_compare<std::string, string_less> > StringHash;

public:

	SeuLex(const std::string& reFile, const std::string& parsingFile);
	~SeuLex();

	// generate Lex code
	void generateParsingProgram();
	void outputMinDFA();
	void outputAction();

	//bool parseByminDFA(std::list<Token>* tokenList, const std::string& file);
	
protected:
	// read RE file functions
	bool read();
	void retract();
	void error(const std::string& errorStr);
	unsigned int lookup(const std::string& str);
	bool readHeadInformation();
	bool readRegularDefinition();
	bool readTranslationRule();
	bool readREFile(const std::string& reFile);
	std::list<ElementType> convertPost(std::list<ElementType>& list);

	// NFA create function
	void PUSH(ElementType chInput);
	bool CONCAT();
	bool STAR();
	bool UNION();
	bool QUESTION();
	bool PLUS();
	bool createNFA();

	// DFA create function
	DFAState Closure(DFAState T);
	DFAState Move(DFAState T, ElementType input);
	bool createDFA();

	// min DFA optimized function
	bool optimizeDFA();

private:
	StringHash* _symbolTable;

	std::ofstream outputFile;

	// variable for reading
	char* _buffer;            // reading buffer
	char _peek;               // current char
	unsigned int _line;       // record the line of current _peek
	size_t _offset;              // record the offset of current _peek
	std::ifstream _reFile;
	unsigned int _numSymbol;
	bool _selfDefinedProgramme;

	// regular expressions structure
	const ElementType EPSILON = 0;
	// priority define for RE parsing
	const ElementType RIGHT_BRACE_OP = 1;
	const ElementType LEFT_BRACE_OP = 2;
	const ElementType CONCAT_OP = 3;
	const ElementType UNION_OP = 4;
	const ElementType STAR_OP = 5;
	const ElementType QUESTION_OP = 6;
	const ElementType PLUS_OP = 7;
	unsigned int _reIndex = 7;
	ElementType _minREelement = 8;
	std::map<std::string, std::list<ElementType> > _REs;     // infex REs map
	std::vector<std::list<ElementType> > _postREs;   // index is the order in translation rule (action id)
	std::vector<Action> _actionMap;                  // index is the action id
	std::map<char, ElementType> _inputRE;
	std::map<ElementType, char> _outputRE;
	
	// data to help transfer REs to NFA
	State* _NFA_start;
	IDType _nextStateID = 0;
	std::stack<FA> _operandStack;
	std::set<IDType> _NFAfinalStateSet;
	std::map<IDType, IDType> _NFAAction;         // <NFA state id, action id>

	// DFA data
	std::map<DFAState, DFAState> _ClosureMap;
	std::map<DFAState, IDType> _DFAStateMap;      
	std::set<IDType>  _DFAfinalStateSet;
	std::vector<StatesTransfer> _DFAStateTranfer; // vector index is state id
	std::map<IDType, IDType> _DFAAction;          // <DFA state id, action id>

	// min DFA data
	std::set<IDType> _minDFAfinalStateSet;
	std::vector<StatesTransfer> _minDFAStateTranfer; // vector index is state id
	std::map<IDType, IDType> _minDFAAction;      // <minDFA state id, action id>

}; // class SeuLex

#endif _SEU_LEX_H