#pragma once

#include "FunctionPrototypeSyntaxNode.h"
#include "FunctionBodySyntaxNode.h"

class FunctionDefinitionSyntaxNode : public SyntaxNode
{
public:
	FunctionDefinitionSyntaxNode(
		FunctionPrototypeSyntaxNode* prototype,
		FunctionBodySyntaxNode* body
	) :
		prototype_(prototype),
		body_(body)
	{}

	std::string toCpp(CppData* data) override
	{
		return ""
			+ prototype_->toCpp(data) + "\n"
			+ body_->toCpp(data);
	}

private:
	FunctionPrototypeSyntaxNode* prototype_;
	FunctionBodySyntaxNode* body_;
};