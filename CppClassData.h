#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>
#include "GDLexer.h"
#include "Type.h"
#include "CppFunctionData.h"

class VariableDefinitionSyntaxNode;
class FunctionPrototypeSyntaxNode;
class EnumDefinitionSyntaxNode;
class ValueSyntaxNode;
struct CppData;

struct CppClassData
{
	std::unordered_set<std::string> types;
	std::unordered_set<std::string> externalFunctions;
	std::unordered_map<std::string, VariableDefinitionSyntaxNode*> memberVariableDefinitions;
	std::unordered_map<std::string, FunctionPrototypeSyntaxNode*> functionPrototypeDefinitions;
	std::unordered_map<std::string, EnumDefinitionSyntaxNode*> enumDefinitions;
	std::unordered_set<std::string> typeDefinitions;
	std::string currentClassName;
	Type* currentClassType;
	Type* classInheritedType;
	std::unordered_map<std::string, CppFunctionData*> functionData;
	CppFunctionData* currentFunction;
	std::unordered_map<std::string, std::string> constantValueMappings = {};

	VariableDefinitionSyntaxNode* getVariableDefinition(const std::string& variableName)
	{
		if (currentFunction && currentFunction->variableDefinitions.contains(variableName))
			return currentFunction->variableDefinitions[variableName];
		if (memberVariableDefinitions.contains(variableName))
			return memberVariableDefinitions[variableName];
		return nullptr;
	}

	FunctionPrototypeSyntaxNode* getFunctionPrototype(const std::string& name)
	{
		if (functionPrototypeDefinitions.contains(name))
			return functionPrototypeDefinitions[name];
		return nullptr;
	}

	EnumDefinitionSyntaxNode* getEnumDefinition(const std::string& name)
	{
		if (enumDefinitions.contains(name))
			return enumDefinitions[name];
		return nullptr;
	}

	std::string toCppFunction(const std::string& functionName, const std::string& parentType);

	std::string getIncludePath(const std::string& type) const;

	bool isGodotType(const std::string& type) const;

	bool isClassMethod(const std::string& name, const CppData* data) const;

	static bool isGodotTypeMethod(const Type* parentType, const std::string& name, const CppData* data);

	static bool isProperty(ValueSyntaxNode* instance, const GDToken* name, const CppData* data);
};
