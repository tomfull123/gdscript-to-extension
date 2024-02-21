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
	{}

	Type* getType() override
	{
		return type_;
	}

	void resolveType() override
	{
		value_->resolveType();
	}

	std::string toCpp(CppData* data) override
	{
		return "!" + value_->toCpp(data);
	}

private:
	Token* token_;
	ValueSyntaxNode* value_;
	Type* type_;
};
