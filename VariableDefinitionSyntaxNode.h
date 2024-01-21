#pragma once

#include "SyntaxNode.h"

class VariableDefinitionSyntaxNode
{
public:
	VariableDefinitionSyntaxNode(
		Token* name,
		Type* dataType
	);
private:
	Token* name_;
	Type* dataType_;
};