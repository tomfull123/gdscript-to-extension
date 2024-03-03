#pragma once

#include "SyntaxNode.h"
#include "BodySyntaxNode.h"
#include <vector>

class IfSyntaxNode : public SyntaxNode
{
public:
	IfSyntaxNode(
		SyntaxNode* condition,
		BodySyntaxNode* thenBody,
		BodySyntaxNode* elseBody
	) :
		condition_(condition),
		thenBody_(thenBody),
		elseBody_(elseBody)
	{}

	bool needsSemiColon() override
	{
		return false;
	}

	void hoist(CppData* data) override
	{
		condition_->hoist(data);
		thenBody_->hoist(data);
		if (elseBody_) elseBody_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		condition_->resolveDefinitions(data);
		thenBody_->resolveDefinitions(data);
		if (elseBody_) elseBody_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data) override
	{
		condition_->resolveTypes(data);
		thenBody_->resolveTypes(data);
		if (elseBody_) elseBody_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string thenString = thenBody_->toCpp(data, indents);

		std::string elseString;

		if (elseBody_)
		{
			elseString = indents + "else\n"
				+ elseBody_->toCpp(data, indents);
		}

		return "if (" + condition_->toCpp(data, "") + ")\n"
			+ thenString
			+ elseString;
	}

private:
	SyntaxNode* condition_;
	BodySyntaxNode* thenBody_;
	BodySyntaxNode* elseBody_;
};
