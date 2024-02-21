#pragma once

#include "SyntaxNode.h"

class VariableSyntaxNode : public ValueSyntaxNode
{
public:
	VariableSyntaxNode(Token* name, ValueSyntaxNode* parentInstance) :
		name_(name),
		parentInstance_(parentInstance)
	{}

	std::string toCpp(CppData* data) override
	{
		return name_->value;
	}

private:
	Token* name_;
	ValueSyntaxNode* parentInstance_;
};