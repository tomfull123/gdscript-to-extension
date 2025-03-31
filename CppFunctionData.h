#pragma once

#include <unordered_map>
#include <string>

class VariableDefinitionSyntaxNode;

struct CppFunctionData
{
	explicit CppFunctionData(const std::string& Name) :
		name(Name)
	{
	}
	std::string name;
	std::unordered_map<std::string, VariableDefinitionSyntaxNode*> variableDefinitions;
};