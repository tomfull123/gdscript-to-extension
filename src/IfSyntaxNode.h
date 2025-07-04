#pragma once

#include "SyntaxNode.h"
#include "BodySyntaxNode.h"
#include <vector>

class IfSyntaxNode : public SyntaxNode
{
public:
	IfSyntaxNode(
		ValueSyntaxNode* condition,
		BodySyntaxNode* thenBody,
		SyntaxNode* elseBody
	) :
		condition_(condition),
		thenBody_(thenBody),
		elseBody_(elseBody)
	{
		conditionType_ = new Type("bool");
	}

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

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		condition_->resolveTypes(data, conditionType_);
		thenBody_->resolveTypes(data);
		if (elseBody_) elseBody_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string thenString = thenBody_->toCpp(data, indents);

		std::string elseString;

		if (elseBody_)
		{
			elseString = elseBody_->toCpp(data, indents);
		}

		return "if (" + condition_->toCpp(data, "") + ")\n"
			+ thenString
			+ elseString;
	}

private:
	Type* conditionType_;
	ValueSyntaxNode* condition_;
	BodySyntaxNode* thenBody_;
	SyntaxNode* elseBody_;
};
