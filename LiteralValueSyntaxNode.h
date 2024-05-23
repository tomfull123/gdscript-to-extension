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

	std::string getName() override
	{
		return "";
	}

	void hoist(CppData* data) override
	{
	}

	void resolveDefinitions(CppData* data) override
	{
	}

	void resolveTypes(CppData* data) override
	{
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		switch (value_->type)
		{
		case TokenType::FloatLiteral:
			return value_->value + "f";
		case TokenType::StringLiteral:
			return "\"" + value_->value + "\"";
		}

		return value_->value;
	}

private:
	Token* value_;
	Type* type_;
};