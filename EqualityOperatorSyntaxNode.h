#pragma once

#include "SyntaxNode.h"

class EqualityOperatorSyntaxNode : public ValueSyntaxNode
{
public:
	EqualityOperatorSyntaxNode(
		Token* operatorToken,
		ValueSyntaxNode* lhs,
		ValueSyntaxNode* rhs
	) :
		operatorToken_(operatorToken),
		lhs_(lhs),
		rhs_(rhs)
	{}

private:
	Token* operatorToken_;
	ValueSyntaxNode* lhs_;
	ValueSyntaxNode* rhs_;
};