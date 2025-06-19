#pragma once

#include "SyntaxNode.h"

class LiteralValueSyntaxNode : public ValueSyntaxNode
{
public:
	LiteralValueSyntaxNode(
		GDToken* value,
		Type* type
	) :
		value_(value),
		type_(type)
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
	}

	void resolveDefinitions(CppData* data) override
	{
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		switch (value_->type)
		{
		case GDTokenType::FloatLiteral:
			return value_->value + "f";
		case GDTokenType::StringLiteral:
			return "\"" + value_->value + "\"";
		}

		return value_->value;
	}

private:
	GDToken* value_;
	Type* type_;
};