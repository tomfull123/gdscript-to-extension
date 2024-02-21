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

	std::string toCpp(CppData* data) override
	{
		return "(" + lhs_->toCpp(data) + " " + operatorToken_->value + " " + rhs_->toCpp(data) + ")";
	}

private:
	Token* operatorToken_;
	ValueSyntaxNode* lhs_;
	ValueSyntaxNode* rhs_;
};