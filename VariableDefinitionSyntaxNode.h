#pragma once

#include "SyntaxNode.h"

struct VariableDefinitionSyntaxNode : public SyntaxNode
{
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

	Token* name_;
	Type* dataType_;
};