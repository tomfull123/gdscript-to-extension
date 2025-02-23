#pragma once

#include "SyntaxNode.h"

class CallSyntaxNode : public ValueSyntaxNode
{
public:
	CallSyntaxNode(
		ValueSyntaxNode* instance,
		Token* name,
		const std::vector<ValueSyntaxNode*>& args
	) :
		instance_(instance),
		name_(name),
		args_(args)
	{
	}

	Type* getType() override
	{
		if (type_) return type_;
		return nullptr;
	}

	std::string getName() override
	{
		return name_->value;
	}

	void hoist(CppData* data) override
	{
		if (instance_) instance_->hoist(data);
		for (auto a : args_) a->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		prototype_ = data->functionPrototypeDefinitions[getName()];

		if (instance_) instance_->resolveDefinitions(data);
		for (auto a : args_) a->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		if (instance_) instance_->resolveTypes(data);
		for (auto a : args_) a->resolveTypes(data);

		bool isConstructorCall = name_->value == "new";

		if (isConstructorCall && instance_) type_ = new Type(instance_->getName());
		else if (!instance_ && GDTYPES_TO_CPPTYPES.contains(name_->value)) type_ = new Type(name_->value);
		else if (prototype_) type_ = prototype_->getReturnType();
		else if (GDFUNCTIONS_TO_CPPFUNCTIONS.contains(name_->value))
		{
			auto& cppFunction = GDFUNCTIONS_TO_CPPFUNCTIONS.find(name_->value)->second;
			if (CPPFUNCTION_RETURN_TYPES.contains(cppFunction)) type_ = new Type(CPPFUNCTION_RETURN_TYPES.find(cppFunction)->second);
		}
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string code;

		bool isConstructorCall = name_->value == "new";

		if (instance_)
		{
			auto instanceName = instance_->getName();
			if (instanceName[0] == '_') instanceName.erase(0, 1);
			if (isConstructorCall) code += "memnew(" + instanceName;
			else
			{
				code += instance_->toCpp(data, indents);

				if (!isConstructorCall)
				{
					auto parentType = instance_->getType();
					auto varDef = data->variableDefinitions[instanceName];

					if ((parentType && data->isGodotType(parentType->name)) || (data->isGodotType(instanceName) || GDTYPES_TO_CPPTYPES.contains(instanceName)))
						code += "->";
					else if (parentType && !data->isGodotType(parentType->name))
						code += ".";
					else if (data->isClassMethod(instance_->getName()))
						code += "->";
					else if (!parentType && !varDef && !instance_->hasParent()) // static method call
					{
						data->types.emplace(instanceName);
						code += "::";
					}
					else
						code += ".";
				}
			}
		}

		std::string argsString;

		for (int a = 0; a < args_.size(); a++)
		{
			auto arg = args_[a];
			argsString += arg->toCpp(data, "");
			if (a < args_.size() - 1) argsString += ", ";
		}

		if (FUNCTION_TO_MEMBER_METHOD_CALL.contains(name_->value))
		{
			code += "(" + argsString + ")" + "." + name_->value + "()";
		}
		else
		{
			if (!isConstructorCall)
			{
				std::string instanceTypeString = "";
				if (instance_)
				{
					const auto* instanceType = instance_->getType();
					if (instanceType) instanceTypeString = instanceType->name;
				}
				code += data->toCppFunction(name_->value, instanceTypeString);
			}

			code += "(" + argsString + ")";
		}

		if (isConstructorCall) code += ")";

		return code;
	}
private:
	ValueSyntaxNode* instance_;
	Token* name_;
	std::vector<ValueSyntaxNode*> args_;
	Type* type_ = nullptr;
	FunctionPrototypeSyntaxNode* prototype_ = nullptr;
};
