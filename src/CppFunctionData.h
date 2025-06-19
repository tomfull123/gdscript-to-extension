#pragma once

#include <unordered_map>
#include <string>

class VariableDefinitionSyntaxNode;

struct CppFunctionData
{
	std::unordered_map<std::string, VariableDefinitionSyntaxNode*> variableDefinitions;
};