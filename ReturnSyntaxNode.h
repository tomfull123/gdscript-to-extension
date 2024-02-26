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

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return indents + "return " + value_->toCpp(data, "");
	}

private:
	ValueSyntaxNode* value_;
};