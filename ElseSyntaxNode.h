#pragma once

#include "BodySyntaxNode.h"

class ElseSyntaxNode : public SyntaxNode
{
public:
	explicit ElseSyntaxNode(
		BodySyntaxNode* body
	) :
		body_(body)
	{
	}

	bool needsSemiColon() override
	{
		return false;
	}

	void hoist(CppData* data) override
	{
		body_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		body_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		body_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return indents + "else\n" + body_->toCpp(data, indents);
	}

private:
	BodySyntaxNode* body_;
};
