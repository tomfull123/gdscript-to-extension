#pragma once

#include "SyntaxNode.h"

class ReturnSyntaxNode : public SyntaxNode
{
public:
	explicit ReturnSyntaxNode(ValueSyntaxNode* value) :
		value_(value)
	{}

	std::string toCpp() override
	{
		return "return " + value_->toCpp();
	}

private:
	ValueSyntaxNode* value_;
};