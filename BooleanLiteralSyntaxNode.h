#pragma once

#include "SyntaxNode.h"

class BooleanLiteralSyntaxNode : public ValueSyntaxNode
{
public:
	BooleanLiteralSyntaxNode(Token* token, bool value) :
		token_(token),
		value_(value),
		type_(new Type("bool"))
	{}

	Type* getType() override
	{
		return type_;
	}

	std::string getName() override
	{
		return "";
	}

	void resolveTypes(CppData* data) override
	{
	}

	void hoist(CppData* data) override
	{
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		if (value_) return "true";
		return "false";
	}

private:
	Token* token_;
	bool value_;
	Type* type_;
};
