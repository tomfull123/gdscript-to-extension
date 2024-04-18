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

	void resolveTypes(CppData* data) override
	{
		lhs_->resolveTypes(data);
		rhs_->resolveTypes(data);

		if (!type_) resolveOperatorType();
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return "(" + lhs_->toCpp(data, "") + " " + operatorToken_->value + " " + rhs_->toCpp(data, "") + ")";
	}

private:
	Token* operatorToken_;
	ValueSyntaxNode* lhs_;
	ValueSyntaxNode* rhs_;
	Type* type_ = nullptr;

	void resolveOperatorType()
	{
		const auto& o = operatorToken_->value;

		if (o == "==" || o == "!=" || o == ">" || o == ">=" || o == "<" || o == "<=")
		{
			type_ = new Type("bool");
			return;
		}

		auto leftType = lhs_->getType();
		auto rightType = rhs_->getType();

		if (leftType && rightType && leftType->name == "int" && rightType->name == "int")
		{
			type_ = leftType;
			return;
		}
	}
};
