#pragma once

#include "SyntaxNode.h"
#include "MatchCaseSyntaxNode.h"

class MatchSyntaxNode : public SyntaxNode
{
public:
	MatchSyntaxNode(
		ValueSyntaxNode* condition,
		const std::vector<MatchCaseSyntaxNode*>& cases
	) :
		condition_(condition),
		cases_(cases)
	{
	}

	void hoist(CppData* data) override
	{
		condition_->hoist(data);
		for (auto c : cases_) c->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		condition_->resolveDefinitions(data);
		for (auto c : cases_) c->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		condition_->resolveTypes(data);
		for (auto c : cases_) c->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string code;

		code += "switch (" + condition_->toCpp(data, "") + ")\n";
		code += indents + "{";

		for (auto c : cases_)
		{
			code += c->toCpp(data, indents);
		}

		code += indents + "}";

		return code;
	}

private:
	ValueSyntaxNode* condition_;
	std::vector<MatchCaseSyntaxNode*> cases_;
};