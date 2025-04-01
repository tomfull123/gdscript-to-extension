#pragma once

#include "SyntaxNode.h"

class CallSyntaxNode : public ValueSyntaxNode
{
public:
	CallSyntaxNode(
		ValueSyntaxNode* instance,
		GDToken* name,
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
		prototype_ = data->currentClass->getFunctionPrototype(getName());

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
		else if (instance_)
		{
			auto instanceType = instance_->getType();

			if (instanceType)
			{
				auto instanceTypeClass = data->classData[instanceType->name];

				if (instanceTypeClass)
				{
					auto prototype = instanceTypeClass->getFunctionPrototype(name_->value);

					if (prototype) type_ = prototype->getReturnType();
				}
			}
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
			if (isConstructorCall)
			{
				code += "memnew(" + instanceName;
				if (!args_.empty()) code += "())->init";
			}
			else
			{
				code += instance_->toCpp(data, indents);

				if (!isConstructorCall)
				{
					auto parentType = instance_->getType();
					auto instanceVarDef = data->currentClass->getVariableDefinition(instanceName);
					auto instanceFunctionDef = data->currentClass->getFunctionPrototype(instanceName);
					bool isParentRefOrObject = false;
					std::string parentTypeName = "";
					FunctionPrototypeSyntaxNode* staticFunctionDef = data->getFunctionPrototype(instanceName, name_->value);

					if (parentType)
					{
						parentTypeName = parentType->name;
						if (parentTypeName[0] == '_') parentTypeName.erase(0, 1);
						isParentRefOrObject = data->isRefType(parentTypeName) || data->isObjectType(parentTypeName);
					}

					if (instanceName == "new")
						code += "->";
					else if (isParentRefOrObject || data->isSingletonType(instanceName))
						code += "->";
					else if (parentType && !isParentRefOrObject)
						code += ".";
					else if (data->currentClass->isClassMethod(instance_->getName(), data))
						code += "->";
					else if (staticFunctionDef && staticFunctionDef->isStatic()) // static method call
					{
						data->currentClass->types.emplace(instanceName);
						code += "::";
					}
					else
						code += ".";
				}
			}
		}

		std::string argsString = getArgsString(data);

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
				code += data->currentClass->toCppFunction(name_->value, instanceTypeString);
			}

			code += "(" + argsString + ")";
		}

		if (isConstructorCall && args_.empty()) code += ")";

		return code;
	}
private:
	ValueSyntaxNode* instance_;
	GDToken* name_;
	std::vector<ValueSyntaxNode*> args_;
	Type* type_ = nullptr;
	FunctionPrototypeSyntaxNode* prototype_ = nullptr;

	std::string getArgsString(CppData* data)
	{
		std::string argsString;
		for (int a = 0; a < args_.size(); a++)
		{
			auto arg = args_[a];
			argsString += arg->toCpp(data, "");
			if (a < args_.size() - 1) argsString += ", ";
		}
		return argsString;
	}
};
