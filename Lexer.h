#pragma once

#include <string>

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
