#pragma once

#include "SyntaxNode.h"

class EnumValueSyntaxNode : public SyntaxNode
{
public:
	explicit EnumValueSyntaxNode(Token* name) :
		name_(name)
	{}

	void hoist(CppData* data) override
	{
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return name_->value;
	}

private:
	Token* name_;
};
