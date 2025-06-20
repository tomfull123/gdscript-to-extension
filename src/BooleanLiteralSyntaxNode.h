#pragma once

#include "SyntaxNode.h"

class BooleanLiteralSyntaxNode : public ValueSyntaxNode
{
public:
	BooleanLiteralSyntaxNode(GDToken* token, bool value) :
		token_(token),
		value_(value),
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
	}

	void resolveDefinitions(CppData* data) override
	{
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		if (value_) return "true";
		return "false";
	}

private:
	GDToken* token_;
	bool value_;
	Type* type_;
};
