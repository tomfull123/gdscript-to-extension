#pragma once

#include "SyntaxNode.h"

class ArrayValueSyntaxNode : public ValueSyntaxNode
{
public:
	explicit ArrayValueSyntaxNode(
		const std::vector<ValueSyntaxNode*>& expressions
	) :
		expressions_(expressions)
	{}

	std::string getName() override
	{
		return "";
	}

	void hoist(CppData* data) override
	{
		for (auto e : expressions_) e->hoist(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string expressionsString;

		for (auto e : expressions_)
		{
			expressionsString += indents + e->toCpp(data, indents) + "\n";
		}

		return "{" + expressionsString + "}";
	}

private:
	std::vector<ValueSyntaxNode*> expressions_;
};