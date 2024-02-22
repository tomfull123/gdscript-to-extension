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

	bool isPrivate() const
	{
		return prototype_->isPrivate();
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return ""
			+ prototype_->toCpp(data, indents) + "\n"
			+ body_->toCpp(data, indents);
	}

private:
	FunctionPrototypeSyntaxNode* prototype_;
	FunctionBodySyntaxNode* body_;
};