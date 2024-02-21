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
		rhs_(rhs),
		type_(new Type("bool"))
	{}

	Type* getType() override
	{
		return type_;
	}

	void resolveType() override
	{
		lhs_->resolveType();
		rhs_->resolveType();
	}

	std::string toCpp(CppData* data) override
	{
		return "(" + lhs_->toCpp(data) + " " + operatorToken_->value + " " + rhs_->toCpp(data) + ")";
	}

private:
	Token* operatorToken_;
	ValueSyntaxNode* lhs_;
	ValueSyntaxNode* rhs_;
	Type* type_;
};