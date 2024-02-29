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

	std::string getName() override
	{
		return "";
	}

	void resolveTypes(CppData* data) override
	{
		lhs_->resolveTypes(data);
		rhs_->resolveTypes(data);
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
	Type* type_;
};