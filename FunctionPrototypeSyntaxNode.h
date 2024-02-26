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
		if (!returnType_) returnType_ = new Type("void");
	}

	std::string getName() const
	{
		return name_->value;
	}

	std::vector<VariableDefinitionSyntaxNode*> getArgDefs() const
	{
		return argDefs_;
	}

	bool isPrivate() const
	{
		return name_->value[0] == '_';
	}

	std::string toCpp(CppData* data, const std::string& indents)
	{
		std::string argsString;

		for (int a = 0; a < argDefs_.size(); a++)
		{
			auto arg = argDefs_[a];
			argsString += arg->toCpp(data, "");
			if (a < argDefs_.size() - 1) argsString += ", ";
		}

		return indents + data->toCppType(returnType_) + " " + name_->value + "(" + argsString + ")";
	}

private:
	Token* name_;
	std::vector<VariableDefinitionSyntaxNode*> argDefs_;
	Type* returnType_;
	bool isStatic_;
};