#pragma once

#include "SyntaxNode.h"

class PrefixOperator : public ValueSyntaxNode
{
public:
	PrefixOperator(
		GDToken* operatorToken,
		ValueSyntaxNode* value
	) :
		operatorToken_(operatorToken),
		value_(value)
	{
	}

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

	void resolveDefinitions(CppData* data) override
	{
		value_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		value_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return operatorToken_->value + value_->toCpp(data, "");
	}

private:
	GDToken* operatorToken_;
	ValueSyntaxNode* value_;
};
