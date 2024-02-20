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

	std::string toCpp() override
	{
		return "";
	}

private:
	Token* name_;
	Type* dataType_;
};