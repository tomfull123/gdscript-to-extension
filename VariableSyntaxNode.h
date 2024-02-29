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

	Type* getType() override
	{
		return nullptr;
	}

	std::string getName() override
	{
		return name_->value;
	}

	void resolveTypes(CppData* data) override
	{
		if (parentInstance_) parentInstance_->resolveTypes(data);
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
			code += parentInstance_->toCpp(data, indents);

			auto parentName = parentInstance_->getName();
			auto parentEnumDef = data->enumDefinitions[parentName];
			if (parentEnumDef)
				code += "::";
			else if (CPPTYPES_TO_FUNCTION.find(parentName) == CPPTYPES_TO_FUNCTION.end())
				code += "->set_";
		}

		auto varDef = data->variableDefinitions[name_->value];

		// static call
		if (!parentInstance_ && !varDef) code += data->toCppType(new Type(name_->value));
		else
		{
			code += data->toWrappedCppFunction(parentInstance_, name_);
		}

		return code;
	}

private:
	Token* name_;
	ValueSyntaxNode* parentInstance_;
};