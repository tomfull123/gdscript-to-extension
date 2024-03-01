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
	{}

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
		if (instance_) instance_->resolveDefinitions(data);
		for (auto a : args_) a->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data) override
	{
		if (instance_) instance_->resolveTypes(data);
		for (auto a : args_) a->resolveTypes(data);

		bool isConstructorCall = name_->value == "new";

		if (isConstructorCall && instance_) type_ = new Type(instance_->getName());
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string code;

		bool isConstructorCall = name_->value == "new";

		if (instance_)
		{
			code += instance_->toCpp(data, indents);

			if (!isConstructorCall) code += "->";
		}

		std::string argsString;

		for (int a = 0; a < args_.size(); a++)
		{
			auto arg = args_[a];
			argsString += arg->toCpp(data, "");
			if (a < args_.size() - 1) argsString += ", ";
		}

		if (!isConstructorCall)
		{
			code += data->toCppFunction(name_->value);
		}

		code += "(" + argsString + ")";

		return code;
	}
private:
	ValueSyntaxNode* instance_;
	Token* name_;
	std::vector<ValueSyntaxNode*> args_;
	Type* type_ = nullptr;
};
