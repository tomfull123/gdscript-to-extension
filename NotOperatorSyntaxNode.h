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
private:
	Token* token_;
	ValueSyntaxNode* value_;
};
