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

		if (parentInstance_) code += parentInstance_->toCpp(data, indents) + "->";

		auto varDef = data->variableDefinitions[name_->value];

		// static call
		if (!parentInstance_ && !varDef) code += data->toCppType(new Type(name_->value));
		else code += name_->value;

		return code;
	}

private:
	Token* name_;
	ValueSyntaxNode* parentInstance_;
};