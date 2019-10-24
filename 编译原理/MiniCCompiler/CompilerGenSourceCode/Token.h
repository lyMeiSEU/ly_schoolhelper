#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>

struct Token {
	Token(std::string word,
		std::string attr,
		unsigned int innercode,
		unsigned int line,
		unsigned int offset)
		: _lexecal(word),
		_attribute(attr),
		_innerCode(innercode),
		_line(line),
		_offset(offset) {}

	// data member
	std::string _lexecal;
	std::string _attribute;
	unsigned int _innerCode;
	unsigned int _line;
	unsigned int _offset;

}; // struct Token	

#endif _TOKEN_H