#pragma once

#include "SyntaxNode.h"

struct EqualityOperatorSyntaxNode : public ValueSyntaxNode
{
	EqualityOperatorSyntaxNode(
		Token* operatorToken,
		ValueSyntaxNode* lhs,
		ValueSyntaxNode* rhs
	) :
		operatorToken_(operatorToken),
		lhs_(lhs),
		rhs_(rhs)
	{}

	std::string toCpp() override
	{
		return "(" + lhs_->toCpp() + " " + operatorToken_->value + " " + rhs_->toCpp() + ")";
	}

	Token* operatorToken_;
	ValueSyntaxNode* lhs_;
	ValueSyntaxNode* rhs_;
};