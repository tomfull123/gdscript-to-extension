#pragma once

#include "SyntaxNode.h"

class VariableDefinitionSyntaxNode : public SyntaxNode
{
public:
	VariableDefinitionSyntaxNode(
		Token* name,
		Type* dataType
	) :
		name_(name),
		dataType_(dataType)
	{
	}

private:
	Token* name_;
	Type* dataType_;
};