#pragma once

#include "SyntaxNode.h"
#include "BodySyntaxNode.h"

class ForSyntaxNode : public SyntaxNode
{
public:
	ForSyntaxNode(
		Token* variableToken,
		ValueSyntaxNode* array,
		BodySyntaxNode* body
	) :
		variableToken_(variableToken),
		array_(array),
		body_(body)
	{
	}

	bool needsSemiColon() override
	{
		return false;
	}

	void hoist(CppData* data) override
	{
		array_->hoist(data);
		body_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		array_->resolveDefinitions(data);
		body_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data) override
	{
		array_->resolveTypes(data);
		body_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return "for (auto " + variableToken_->value + " : " + array_->toCpp(data, "") + ")\n"
			+ body_->toCpp(data, indents);
	}

private:
	Token* variableToken_;
	ValueSyntaxNode* array_;
	BodySyntaxNode* body_;
};
