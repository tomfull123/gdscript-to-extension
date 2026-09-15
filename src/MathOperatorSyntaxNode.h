#pragma once

#include "SyntaxNode.h"

class MathOperatorSyntaxNode : public ValueSyntaxNode
{
public:
	MathOperatorSyntaxNode(
		GDToken* operatorToken,
		ValueSyntaxNode* lhs,
		ValueSyntaxNode* rhs
	) :
		operatorToken_(operatorToken),
		lhs_(lhs),
		rhs_(rhs)
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

		if (!type_) resolveOperatorType();
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		auto operatorValue = operatorToken_->value;
		auto leftValue = lhs_->toCpp(data, "");
		auto rightValue = rhs_->toCpp(data, "");

		if (operatorValue == "%" && lhs_->getType()->getName() == "String")
		{
			return "godot::String(" + leftValue + ").format(" + rightValue + ")";
		}
		return "(" + leftValue + " " + operatorValue + " " + rightValue + ")";
	}

private:
	GDToken* operatorToken_;
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

		if (leftType && rightType)
		{
			type_ = leftType;
			return;
		}
	}
};
