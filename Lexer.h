#pragma once

#include <string>
#include "InputStream.h"

struct Token
{
	std::string value;
	int lineNumber = -1;
	int columnNumber = -1;
	std::string filename = "";
	int indentDepth = -1;
	Token() = default;
	explicit Token(const std::string& newValue) :
		value(newValue) {
	}
};

class Lexer
{
public:
	explicit Lexer(const std::string& input) :
		inputStream_(input)
	{
	}

	bool end() const
	{
		return inputStream_.eof();
	}

protected:
	InputStream inputStream_;
};
