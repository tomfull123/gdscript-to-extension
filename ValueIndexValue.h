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

	std::string getName() override
	{
		return "";
	}

	void hoist(CppData* data) override
	{
		variable_->hoist(data);
		index_->hoist(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return variable_->toCpp(data, indents) + "[" + index_->toCpp(data, "") + "]";
	}

private:
	ValueSyntaxNode* variable_;
	ValueSyntaxNode* index_;
};
