#pragma once

#include "FunctionPrototypeSyntaxNode.h"
#include "FunctionBodySyntaxNode.h"

struct FunctionDefinitionSyntaxNode : public SyntaxNode
{
	FunctionDefinitionSyntaxNode(
		FunctionPrototypeSyntaxNode* prototype,
		FunctionBodySyntaxNode* body
	) :
		prototype_(prototype),
		body_(body)
	{}

	std::string toCpp() override
	{
		return "";
	}

	FunctionPrototypeSyntaxNode* prototype_;
	FunctionBodySyntaxNode* body_;
};