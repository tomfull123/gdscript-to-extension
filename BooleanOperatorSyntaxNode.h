#pragma once

#include "SyntaxNode.h"

class BooleanOperatorSyntaxNode : public ValueSyntaxNode
{
public:
	BooleanOperatorSyntaxNode(
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
		return lhs_->toCpp(data, indents) + " " + toCPPOperator() + " " + rhs_->toCpp(data, "");
	}

private:
	Token* operatorToken_;
	ValueSyntaxNode* lhs_;
	ValueSyntaxNode* rhs_;
	Type* type_;

	std::string toCPPOperator() const
	{
		const TokenType type = operatorToken_->type;

		switch (type) {
		case TokenType::OrOperator:
			return "||";
		case TokenType::AndOperator:
			return "&&";
		}
		return "";
	}
};