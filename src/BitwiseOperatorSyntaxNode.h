#pragma once

#include "SyntaxNode.h"

class BitwiseOperatorSyntaxNode : public ValueSyntaxNode
{
public:
	BitwiseOperatorSyntaxNode(
		GDToken* operatorToken,
		ValueSyntaxNode* lhs,
		ValueSyntaxNode* rhs
	) :
		operatorToken_(operatorToken),
		lhs_(lhs),
		rhs_(rhs),
		type_(new Type("bool"))
	{
	}

	Type* getType() override
	{
		return type_;
	}

	std::string getName() override
	{
		return "";
	}

	void hoist(CppData* data) override
	{
		lhs_->hoist(data);
		rhs_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		lhs_->resolveDefinitions(data);
		rhs_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		lhs_->resolveTypes(data);
		rhs_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return lhs_->toCpp(data, indents) + " " + operatorToken_->value + " " + rhs_->toCpp(data, "");
	}

private:
	GDToken* operatorToken_;
	ValueSyntaxNode* lhs_;
	ValueSyntaxNode* rhs_;
	Type* type_;
};