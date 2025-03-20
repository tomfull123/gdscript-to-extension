#pragma once

#include "SyntaxNode.h"

class IsOperatorSyntaxNode : public ValueSyntaxNode
{
public:
	IsOperatorSyntaxNode(
		GDToken* operatorToken,
		ValueSyntaxNode* lhs,
		Type* rhs
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
		return "is";
	}

	void hoist(CppData* data) override
	{
		lhs_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		lhs_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		lhs_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return "std::is_same<decltype(" +
			lhs_->toCpp(data, indents) +
			"), " +
			data->currentClass->toCppType(rhs_) +
			"> == true";
	}

private:
	GDToken* operatorToken_;
	ValueSyntaxNode* lhs_;
	Type* rhs_;
	Type* type_;
};
