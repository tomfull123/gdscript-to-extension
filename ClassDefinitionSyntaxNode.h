#pragma once

#include "FunctionDefinitionSyntaxNode.h"
#include "VariableDefinitionSyntaxNode.h"

class ClassDefinitionSyntaxNode
{
public:
	ClassDefinitionSyntaxNode(
		Token* name,
		const std::vector<FunctionDefinitionSyntaxNode*>& memberFunctionDefinitions,
		const std::vector<VariableDefinitionSyntaxNode*>& memberVariableDefinitions
	) :
		name_(name),
		memberFunctionDefinitions_(memberFunctionDefinitions),
		memberVariableDefinitions_(memberVariableDefinitions)
	{}

	std::string toCpp()
	{
		std::string className = name_->value;

		std::string memberVariableDefinitionString;

		for (auto v : memberVariableDefinitions_)
			memberVariableDefinitionString += v->toCpp();

		return ""
			"class " + className + "\n"
			"{\n"
			"public:\n"
			"private:\n"
			"protected:\n"
			"\tstatic void _bind_methods()\n"
			"\t{\n"
			"\t"
			"\t}\n"
			"}\n";
	}

private:
	Token* name_;
	std::vector<FunctionDefinitionSyntaxNode*> memberFunctionDefinitions_;
	std::vector<VariableDefinitionSyntaxNode*> memberVariableDefinitions_;
};