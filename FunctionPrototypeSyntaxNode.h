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
		bool isStatic
	) :
		name_(name),
		argDefs_(argDefs),
		returnType_(returnType),
		isStatic_(isStatic)
	{
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

	std::vector<VariableDefinitionSyntaxNode*> getArgDefs() const
	{
		return argDefs_;
	}

	bool isPrivate() const
	{
		return getName()[0] == '_';
	}

	void hoist(CppData* data) override
	{
		for (auto a : argDefs_) a->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		for (auto a : argDefs_) a->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data) override
	{
		if (name_->value == "_init")
		{
			returnType_ = new Type(data->currentClassName);
		}
		else if (!returnType_) returnType_ = new Type("void");

		for (auto a : argDefs_) a->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string argsString;

		for (int a = 0; a < argDefs_.size(); a++)
		{
			auto arg = argDefs_[a];
			argsString += arg->toCpp(data, "");
			if (a < argDefs_.size() - 1) argsString += ", ";
		}

		std::string code = indents;

		if (isStatic_) code += "static ";

		std::string name = getName();

		return code + data->toCppType(returnType_) + " " + name + "(" + argsString + ")";
	}

private:
	Token* name_;
	std::vector<VariableDefinitionSyntaxNode*> argDefs_;
	Type* returnType_;
	bool isStatic_;
};