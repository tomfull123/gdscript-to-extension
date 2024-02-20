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

	std::string toCpp() override
	{
		return "(" + lhs_->toCpp() + " " + operatorToken_->value + " " + rhs_->toCpp() + ")";
	}

private:
	Token* operatorToken_;
	ValueSyntaxNode* lhs_;
	ValueSyntaxNode* rhs_;
};