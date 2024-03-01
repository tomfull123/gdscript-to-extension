#pragma once

#include "SyntaxNode.h"

class ReturnSyntaxNode : public SyntaxNode
{
public:
	explicit ReturnSyntaxNode(ValueSyntaxNode* value) :
		value_(value)
	{}

	void hoist(CppData* data) override
	{
		value_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		value_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data) override
	{
		value_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return "return " + value_->toCpp(data, "");
	}

private:
	ValueSyntaxNode* value_;
};