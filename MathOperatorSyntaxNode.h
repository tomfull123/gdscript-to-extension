#pragma once

#include "SyntaxNode.h"

class MathOperatorSyntaxNode : public ValueSyntaxNode
{
public:
	MathOperatorSyntaxNode(
		Token* operatorToken,
		ValueSyntaxNode* lhs,
		ValueSyntaxNode* rhs
	) :
		operatorToken_(operatorToken),
		lhs_(lhs),
		rhs_(rhs)
	{}

	std::string getName() override
	{
		return "";
	}

	void hoist(CppData* data) override
	{
		lhs_->hoist(data);
		rhs_->hoist(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return "(" + lhs_->toCpp(data, "") + " " + operatorToken_->value + " " + rhs_->toCpp(data, "") + ")";
	}

private:
	Token* operatorToken_;
	ValueSyntaxNode* lhs_;
	ValueSyntaxNode* rhs_;
};
