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

	bool isFunction() const override
	{
		return false;
	}

	Type* getType() override
	{
		if (type_) return type_;
		if (variableDefinition_) return type_ = variableDefinition_->getType();
		if (enumDefinition_) return type_ = new Type(enumDefinition_->getName());
		return nullptr;
	}

	std::string getName() override
	{
		return name_->value;
	}

	void hoist(CppData* data) override
	{
		if (parentInstance_) parentInstance_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		variableDefinition_ = data->variableDefinitions[name_->value];

		if (parentInstance_)
		{
			parentInstance_->resolveDefinitions(data);
			if (!variableDefinition_) enumDefinition_ = data->enumDefinitions[parentInstance_->getName()];
		}
	}

	void resolveTypes(CppData* data) override
	{
		if (parentInstance_) parentInstance_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string code;

		if (parentInstance_)
		{
			code += parentInstance_->toCpp(data, indents);

			auto parentName = parentInstance_->getName();
			if (data->enumDefinitions[parentName])
				code += "::";
			else if (GDTYPES_TO_CPPTYPES.find(parentName) != GDTYPES_TO_CPPTYPES.end())
				code += "::";
			else if (CPPTYPES_TO_FUNCTION.find(parentName) == CPPTYPES_TO_FUNCTION.end())
				code += "->";
		}

		auto varDef = data->variableDefinitions[name_->value];

		if (name_->value == "self")
		{
			code += "this";
		}
		// static call
		else if (!parentInstance_ && !varDef) code += data->toCppType(new Type(name_->value));
		else
		{
			code += data->toWrappedCppFunction(parentInstance_, name_);
		}

		return code;
	}

private:
	Token* name_;
	ValueSyntaxNode* parentInstance_;
	Type* type_ = nullptr;
	VariableDefinitionSyntaxNode* variableDefinition_ = nullptr;
	EnumDefinitionSyntaxNode* enumDefinition_ = nullptr;
};