#include "CppData.h"
#include "CppClassData.h"

CppClassData* CppData::getClassDefinition(const std::string& className) const
{
	if (classData.contains(className))
	{
		return classData.find(className)->second;
	}

	return nullptr;
}

VariableDefinitionSyntaxNode* CppData::getVariableDefinition(const std::string& typeName, const std::string& variableName) const
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

FunctionPrototypeSyntaxNode* CppData::getFunctionPrototype(const std::string& typeName, const std::string& functionName) const
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
}