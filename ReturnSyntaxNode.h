#pragma once

#include "SyntaxNode.h"

class ReturnSyntaxNode : public SyntaxNode
{
public:
	explicit ReturnSyntaxNode(ValueSyntaxNode* value) :
		value_(value)
	{}

	std::string toCpp(CppData* data) override
	{
		return "return " + value_->toCpp(data);
	}

private:
	ValueSyntaxNode* value_;
};