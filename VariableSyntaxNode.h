#pragma once

#include "SyntaxNode.h"

struct VariableSyntaxNode : public ValueSyntaxNode
{
	VariableSyntaxNode(Token* name, ValueSyntaxNode* parentInstance) :
		name_(name),
		parentInstance_(parentInstance)
	{}

	std::string toCpp() override
	{
		return "";
	}

	Token* name_;
	ValueSyntaxNode* parentInstance_;
};