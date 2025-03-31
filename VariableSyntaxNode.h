#pragma once

#include "SyntaxNode.h"
#include "EnumDefinitionSyntaxNode.h"

class VariableSyntaxNode : public ValueSyntaxNode
{
public:
	VariableSyntaxNode(GDToken* name, ValueSyntaxNode* parentInstance, bool asValue) :
		name_(name),
		parentInstance_(parentInstance),
		asValue_(asValue)
	{
	}

	bool hasParent() const override
	{
		if (parentInstance_) return true;
		return false;
	}

	virtual ValueSyntaxNode* getParent() const override
	{
		return parentInstance_;
	}

	bool isFunction(CppData* data) const override
	{
		if (CppClassData::isProperty(parentInstance_, name_, data)) return false;

		if (parentInstance_ && !asValue_ && parentInstance_->getName() != "self")
		{
			return true;
		}
		return false;
	}

	Type* getType() override
	{
		return type_;
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
		if (!parentInstance_)
		{
			variableDefinition_ = data->currentClass->getVariableDefinition(name_->value);
		}
		else
		{
			parentInstance_->resolveDefinitions(data);
			if (!variableDefinition_) enumDefinition_ = data->currentClass->getEnumDefinition(parentInstance_->getName());
		}

		if (variableDefinition_) variableDefinition_->resolveDefinitions(data);
		if (enumDefinition_) enumDefinition_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		if (parentInstance_) parentInstance_->resolveTypes(data);

		if (variableDefinition_)
		{
			variableDefinition_->resolveTypes(data);
			type_ = variableDefinition_->getType();
		}
		else if (enumDefinition_)
		{
			enumDefinition_->resolveTypes(data);
			type_ = new Type(enumDefinition_->getName());
		}

		if (!type_ && parentInstance_)
		{
			auto parentName = parentInstance_->getName();
			if (CPPTYPES_TO_FUNCTION.contains(parentName))
			{
				auto it = CPPTYPES_TO_FUNCTION.find(parentName);
				auto functionName = parentName + "::" + it->second;

				if (CPPFUNCTION_RETURN_TYPES.contains(functionName))
				{
					type_ = new Type(CPPFUNCTION_RETURN_TYPES.find(functionName)->second);
					return;
				}
			}
			auto functionName = parentName + "::" + name_->value;
			if (CPPFUNCTION_RETURN_TYPES.contains(functionName))
			{
				type_ = new Type(CPPFUNCTION_RETURN_TYPES.find(functionName)->second);
			}
		}
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string code;

		if (parentInstance_)
		{
			code += parentInstance_->toCpp(data, indents);

			auto parentName = parentInstance_->getName();
			if (data->currentClass->enumDefinitions.contains(parentName) || GDTYPES_TO_CPPTYPES.contains(parentName))
				code += "::";
			else if (!CPPTYPES_TO_FUNCTION.contains(parentName))
			{
				auto parentType = parentInstance_->getType();
				if (CPPFUNCTION_RETURN_TYPES.contains(parentName))
					code += "->";
				else if (parentType && GDTYPES_TO_CPPTYPES.contains(parentType->name))
					code += ".";
				else
					code += "->";
			}
		}

		auto varDef = data->currentClass->getVariableDefinition(name_->value);

		if (name_->value == "self")
		{
			code += "this";
		}
		// static call
		else if (!parentInstance_ && !varDef && GDTYPES_TO_CPPTYPES.contains(name_->value))
			code += data->toCppType(new Type(name_->value));
		else if (data->currentClass->isClassMethod(name_->value, data))
			code += data->currentClass->toCppFunction(name_->value, data->currentClass->classInheritedType->name) + "()";
		else
		{
			if (parentInstance_)
			{
				auto parentName = parentInstance_->getName();

				if (CPPTYPES_TO_FUNCTION.contains(parentName) || data->currentClass->enumDefinitions.contains(parentName)
					|| CppClassData::isProperty(parentInstance_, name_, data) || GDTYPES_TO_CPPTYPES.contains(parentName)
					|| parentName == "self")
				{
					code += data->toWrappedCppFunction(parentInstance_, name_);
				}
				else
				{
					if (asValue_) // getter
					{
						code += "get_" + name_->value + "()";
					}
					else // setter
					{
						code += "set_" + name_->value;
					}
				}
			}
			else
			{
				code += data->toWrappedCppFunction(parentInstance_, name_);
			}
		}

		return code;
	}

private:
	GDToken* name_;
	ValueSyntaxNode* parentInstance_;
	bool asValue_;
	Type* type_ = nullptr;
	VariableDefinitionSyntaxNode* variableDefinition_ = nullptr;
	EnumDefinitionSyntaxNode* enumDefinition_ = nullptr;
};