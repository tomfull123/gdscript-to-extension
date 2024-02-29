#pragma once

#include "SyntaxNode.h"

class PrefixOperator : public ValueSyntaxNode
{
public:
	PrefixOperator(
		Token* operatorToken,
		ValueSyntaxNode* value
	) :
		operatorToken_(operatorToken),
		value_(value)
	{}

	Type* getType() override
	{
		return nullptr;
	}

	std::string getName() override
	{
		return "";
	}

	void hoist(CppData* data) override
	{
		value_->hoist(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return operatorToken_->value + value_->toCpp(data, "");
	}

private:
	Token* operatorToken_;
	ValueSyntaxNode* value_;
};
