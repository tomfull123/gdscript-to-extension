#pragma once

#include "SyntaxNode.h"

struct NotOperatorSyntaxNode : public ValueSyntaxNode
{
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

	Token* token_;
	ValueSyntaxNode* value_;
};
