#pragma once

#include "SyntaxNode.h"
#include "VariableSyntaxNode.h"

class ValueIndexValue : public ValueSyntaxNode
{
public:
	ValueIndexValue(
		ValueSyntaxNode* variable,
		ValueSyntaxNode* index
	) :
		variable_(variable),
		index_(index)
	{}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return variable_->toCpp(data, indents) + "[" + index_->toCpp(data, "") + "]";
	}

private:
	ValueSyntaxNode* variable_;
	ValueSyntaxNode* index_;
};
