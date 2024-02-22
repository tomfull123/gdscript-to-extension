#pragma once

#include "SyntaxNode.h"

class VariableSyntaxNode : public ValueSyntaxNode
{
public:
	VariableSyntaxNode(Token* name, ValueSyntaxNode* parentInstance) :
		name_(name),
		parentInstance_(parentInstance)
	{}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string code;

		if (parentInstance_) code += parentInstance_->toCpp(data, indents) + ".";

		return code + name_->value;
	}

private:
	Token* name_;
	ValueSyntaxNode* parentInstance_;
};