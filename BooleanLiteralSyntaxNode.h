#pragma once

#include "SyntaxNode.h"

class BooleanLiteralSyntaxNode : public ValueSyntaxNode
{
public:
	BooleanLiteralSyntaxNode(Token* token, bool value) :
		token_(token),
		value_(value)
	{}

	std::string toCpp(CppData* data, const std::string& indents)
	{
		if (value_) return "true";
		return "false";
	}

private:
	Token* token_;
	bool value_;
};
