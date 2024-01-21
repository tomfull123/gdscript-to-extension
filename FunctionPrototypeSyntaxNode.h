#pragma once

#include "SyntaxNode.h"
#include "VariableDefinitionSyntaxNode.h"

class FunctionPrototypeSyntaxNode
{
public:
	FunctionPrototypeSyntaxNode(
		Token* name,
		const std::vector<VariableDefinitionSyntaxNode*>& argDefs,
		Type* returnType,
		bool isStatic
	) :
		name_(name),
		argDefs_(argDefs),
		returnType_(returnType),
		isStatic_(isStatic)
	{}

private:
	Token* name_;
	std::vector<VariableDefinitionSyntaxNode*> argDefs_;
	Type* returnType_;
	bool isStatic_;
};