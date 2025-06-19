#pragma once

#include "IfSyntaxNode.h"

class ElseIfSyntaxNode : public SyntaxNode
{
public:
	explicit ElseIfSyntaxNode(
		IfSyntaxNode* ifSyntaxNode
	) :
		ifSyntaxNode_(ifSyntaxNode)
	{
	}

	bool needsSemiColon() override
	{
		return false;
	}

	void hoist(CppData* data) override
	{
		ifSyntaxNode_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		ifSyntaxNode_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		ifSyntaxNode_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return indents + "else " + ifSyntaxNode_->toCpp(data, indents);
	}

private:
	IfSyntaxNode* ifSyntaxNode_;
};
