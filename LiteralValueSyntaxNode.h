#pragma once

#include "SyntaxNode.h"

class LiteralValueSyntaxNode : public ValueSyntaxNode
{
public:
	LiteralValueSyntaxNode(
		Token* value,
		Type* type
	) :
		value_(value),
		type_(type)
	{}

	Type* getType() override
	{
		return type_;
	}

	void resolveType() override
	{
	}

	std::string toCpp(CppData* data) override
	{
		return "";
	}

private:
	Token* value_;
	Type* type_;
};