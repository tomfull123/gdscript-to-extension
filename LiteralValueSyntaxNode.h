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

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		switch (value_->type)
		{
		case TokenType::FloatLiteral:
			return value_->value + "f";
		}

		return value_->value;
	}

private:
	Token* value_;
	Type* type_;
};