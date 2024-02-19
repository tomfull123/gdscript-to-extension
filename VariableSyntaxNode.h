#pragma once

#include "SyntaxNode.h"

class VariableSyntaxNode : public ValueSyntaxNode
{
public:
	VariableSyntaxNode(Token* name, ValueSyntaxNode* parentInstance) :
		name_(name),
		parentInstance_(parentInstance)
	{}

private:
	Token* name_;
	ValueSyntaxNode* parentInstance_;
};