#pragma once

#include "SyntaxNode.h"

class NotOperatorSyntaxNode : public ValueSyntaxNode
{
public:
	NotOperatorSyntaxNode(
		Token* token,
		ValueSyntaxNode* value
	) :
		token_(token),
		value_(value),
		type_(new Type("bool"))
	{
	}

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
		value_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		value_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data) override
	{
		value_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return "!" + value_->toCpp(data, "");
	}

private:
	Token* token_;
	ValueSyntaxNode* value_;
	Type* type_;
};
