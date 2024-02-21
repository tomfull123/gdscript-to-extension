#pragma once

#include "SyntaxNode.h"

class ReturnSyntaxNode : public SyntaxNode
{
public:
	explicit ReturnSyntaxNode(ValueSyntaxNode* value) :
		value_(value)
	{}

	void resolveType() override
	{
		value_->resolveType();
	}

	std::string toCpp(CppData* data) override
	{
		return "return " + value_->toCpp(data);
	}

private:
	ValueSyntaxNode* value_;
};