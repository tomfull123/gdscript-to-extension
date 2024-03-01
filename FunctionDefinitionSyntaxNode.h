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

	std::string getName() const
	{
		return prototype_->getName();
	}

	std::vector<VariableDefinitionSyntaxNode*> getArgDefs() const
	{
		return prototype_->getArgDefs();
	}

	bool isPrivate() const
	{
		return prototype_->isPrivate();
	}

	void hoist(CppData* data) override
	{
		prototype_->hoist(data);
		body_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		prototype_->resolveDefinitions(data);
		body_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data) override
	{
		prototype_->resolveTypes(data);
		body_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string prototypeString = prototype_->toCpp(data, indents) + "\n";
		std::string bodyString = body_->toCpp(data, indents);
		return ""
			+ prototypeString
			+ bodyString;
	}

private:
	FunctionPrototypeSyntaxNode* prototype_;
	FunctionBodySyntaxNode* body_;
};