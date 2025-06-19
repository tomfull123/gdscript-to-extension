#pragma once

#include "SyntaxNode.h"
#include "BodySyntaxNode.h"

class WhileSyntaxNode : public SyntaxNode
{
public:
	WhileSyntaxNode(
		ValueSyntaxNode* condition,
		BodySyntaxNode* body
	) :
		condition_(condition),
		body_(body)
	{
	}

	bool needsSemiColon() override
	{
		return false;
	}

	void hoist(CppData* data) override
	{
		condition_->hoist(data);
		body_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		condition_->resolveDefinitions(data);
		body_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		condition_->resolveTypes(data);
		body_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return "while (" + condition_->toCpp(data, "") + ")\n"
			+ body_->toCpp(data, indents);
	}

private:
	ValueSyntaxNode* condition_;
	BodySyntaxNode* body_;
};
