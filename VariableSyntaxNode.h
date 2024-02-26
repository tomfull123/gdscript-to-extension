#pragma once

#include "SyntaxNode.h"
#include "EnumDefinitionSyntaxNode.h"

class VariableSyntaxNode : public ValueSyntaxNode
{
public:
	VariableSyntaxNode(Token* name, ValueSyntaxNode* parentInstance) :
		name_(name),
		parentInstance_(parentInstance)
	{}

	bool hasParent() const override
	{
		if (parentInstance_) return true;
		return false;
	}

	void hoist(CppData* data) override
	{
		if (parentInstance_) parentInstance_->hoist(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string code;

		if (parentInstance_) code += parentInstance_->toCpp(data, indents) + "->" + "set_";

		auto varDef = data->variableDefinitions[name_->value];
		auto enumDef = data->enumDefinitions[name_->value];

		// static call
		if (!parentInstance_ && !varDef && !enumDef) code += data->toCppType(new Type(name_->value));
		else code += name_->value;

		return code;
	}

private:
	Token* name_;
	ValueSyntaxNode* parentInstance_;
};