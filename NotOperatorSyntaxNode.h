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
		value_(value)
	{}

	std::string toCpp() override
	{
		return "!" + value_->toCpp();
	}

private:
	Token* token_;
	ValueSyntaxNode* value_;
};
