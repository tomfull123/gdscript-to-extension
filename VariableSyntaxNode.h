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

	std::string getName() override
	{
		return name_->value;
	}

	void hoist(CppData* data) override
	{
		if (parentInstance_) parentInstance_->hoist(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string code;

		if (parentInstance_)
		{
			auto parentEnumDef = data->enumDefinitions[parentInstance_->getName()];
			if (parentEnumDef)
				code += parentInstance_->toCpp(data, indents) + "::";
			else
				code += parentInstance_->toCpp(data, indents) + "->set_";
		}

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