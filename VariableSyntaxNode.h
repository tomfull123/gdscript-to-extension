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

		code += name_->value;

		if (!parentInstance_)
		{
			auto varDef = data->variableDefinitions[name_->value];
			if (!varDef) // static call
			{
				data->toCppType(new Type(name_->value));
			}
		}

		return code;
	}

private:
	Token* name_;
	ValueSyntaxNode* parentInstance_;
};