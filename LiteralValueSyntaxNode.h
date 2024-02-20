#pragma once

#include "SyntaxNode.h"

struct LiteralValueSyntaxNode : public ValueSyntaxNode
{
	LiteralValueSyntaxNode(
		Token* value,
		Type* type
	) :
		value_(value),
		type_(type)
	{}

	std::string toCpp() override
	{
		return "";
	}

	Token* value_;
	Type* type_;
};