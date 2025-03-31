#pragma once

#include "SyntaxNode.h"
#include "VariableDefinitionSyntaxNode.h"

class FunctionPrototypeSyntaxNode : public SyntaxNode
{
public:
	FunctionPrototypeSyntaxNode(
		Token* name,
		const std::vector<VariableDefinitionSyntaxNode*>& argDefs,
		Type* returnType,
		bool _isStatic
	) :
		name_(name),
		argDefs_(argDefs),
		returnType_(returnType),
		isStatic_(_isStatic)
	{
	}

	bool isStatic() const
	{
		return isStatic_;
	}

	Token* getToken() const
	{
		return name_;
	}

	std::string getName() const
	{
		if (name_->value == "_init") return "init";
		return name_->value;
	}

	Type* getReturnType() const
	{
		return returnType_;
	}

	std::vector<VariableDefinitionSyntaxNode*> getArgDefs() const
	{
		return argDefs_;
	}

	bool isPrivate() const
	{
		auto name = getName();
		if (GODOT_LIFECYCLE_METHODS.contains(name)) return false;
		return name[0] == '_';
	}

	void hoist(CppData* data) override
	{
		data->currentClass->functionPrototypeDefinitions[name_->value] = this;
		data->currentClass->currentFunction = new CppFunctionData();
		data->currentClass->functionData[name_->value] = data->currentClass->currentFunction;
		for (auto a : argDefs_) a->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		setCurrentFunction(data);
		for (auto a : argDefs_) a->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		setCurrentFunction(data);
		if (name_->value == "_init") returnType_ = new Type(data->currentClass->currentClassName);
		else if (!returnType_) returnType_ = new Type("void");

		for (auto a : argDefs_) a->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		setCurrentFunction(data);
		std::string argsString;

		bool eachArgOnNewLine = argDefs_.size() >= 5;

		if (eachArgOnNewLine) argsString += "\n\t" + indents;

		for (int a = 0; a < argDefs_.size(); a++)
		{
			auto arg = argDefs_[a];
			argsString += arg->toCpp(data, "");
			bool islastArg = a >= (argDefs_.size() - 1);
			if (!islastArg) argsString += ",";
			if (eachArgOnNewLine)
			{
				argsString += "\n" + indents;
				if (!islastArg) argsString += "\t";
			}
			else if (!islastArg) argsString += " ";
		}

		std::string code = indents;

		if (isStatic_) code += "static ";

		std::string name = getName();

		auto typeCode = data->toCppType(returnType_);

		return code + typeCode + " " + name + "(" + argsString + ")";
	}

private:
	Token* name_;
	std::vector<VariableDefinitionSyntaxNode*> argDefs_;
	Type* returnType_;
	bool isStatic_;

	void setCurrentFunction(CppData* data) const
	{
		data->currentClass->currentFunction = data->currentClass->functionData[name_->value];
	}
};