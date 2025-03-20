#pragma once

#include "SyntaxNode.h"

class EnumValueSyntaxNode : public SyntaxNode
{
public:
	explicit EnumValueSyntaxNode(GDToken* name) :
		name_(name)
	{
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
		return name_->value;
	}

private:
	GDToken* name_;
};
