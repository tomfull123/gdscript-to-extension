#pragma once

#include "SyntaxNode.h"

class MatchCaseSyntaxNode : public SyntaxNode
{
public:
	MatchCaseSyntaxNode(
		const std::vector<ValueSyntaxNode*>& values,
		BodySyntaxNode* body
	) :
		values_(values),
		body_(body)
	{
	}

	void hoist(CppData* data) override
	{
		for (auto v : values_) v->hoist(data);
		body_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		for (auto v : values_) v->resolveDefinitions(data);
		body_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		for (auto v : values_) v->resolveTypes(data);
		body_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents, bool asValue) override
	{
		std::string code;

		for (auto v : values_)
		{
			code += indents + "case" + v->toCpp(data, "", true) + ":\n";
		}

		code += body_->toCpp(data, indents, false);

		if (!body_->returnsValue()) code += indents + "break;\n";

		return code;
	}

private:
	std::vector<ValueSyntaxNode*> values_;
	BodySyntaxNode* body_;
};