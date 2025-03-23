#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>

struct CppClassData;

struct CppData
{
	std::unordered_map<std::string, CppClassData*> classData;
	CppClassData* currentClass;
	std::unordered_map<std::string, std::string> inheritTypes = {};

	/*CppClassData* getClassDefinition(const std::string& className) const
	{
		if (classData.contains(className))
		{
			return classData.find(className)->second;
		}

		return nullptr;
	}

	VariableDefinitionSyntaxNode* getVariableDefinition(const std::string& typeName, const std::string& variableName) const
	{
		const auto typeClass = getClassDefinition(typeName);

		if (typeClass)
		{
			if (typeClass->variableDefinitions.contains(variableName))
			{
				return typeClass->variableDefinitions.find(variableName)->second;
			}
		}

		return nullptr;
	}

	FunctionPrototypeSyntaxNode* getFunctionPrototype(const std::string& typeName, const std::string& functionName) const
	{
		const auto typeClass = getClassDefinition(typeName);

		if (typeClass)
		{
			if (typeClass->functionPrototypeDefinitions.contains(functionName))
			{
				return typeClass->functionPrototypeDefinitions.find(functionName)->second;
			}
		}

		return nullptr;
	}*/
};