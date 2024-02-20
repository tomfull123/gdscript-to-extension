#pragma once

#include "SyntaxNode.h"

struct ReturnSyntaxNode : public SyntaxNode
{
	explicit ReturnSyntaxNode(ValueSyntaxNode* value) :
		value_(value)
	{}

	std::string toCpp() override
	{
		return "return " + value_->toCpp();
	}

	ValueSyntaxNode* value_;
};