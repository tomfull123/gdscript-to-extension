#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>
#include "Type.h"

struct CppClassData;
class VariableDefinitionSyntaxNode;
class FunctionPrototypeSyntaxNode;
class EnumDefinitionSyntaxNode;
class ValueSyntaxNode;
class Type;
struct Token;

struct CppData
{
	std::unordered_map<std::string, CppClassData*> classData;
	CppClassData* currentClass;
	std::unordered_map<std::string, std::string> inheritTypes;
	std::unordered_map<std::string, EnumDefinitionSyntaxNode*> enumDefinitions;

	CppClassData* getClassDefinition(const std::string& className) const;

	VariableDefinitionSyntaxNode* getCurrentClassVariableDefinition(const std::string& variableName) const;

	VariableDefinitionSyntaxNode* getVariableDefinition(const std::string& className, const std::string& variableName) const;

	FunctionPrototypeSyntaxNode* getFunctionPrototype(const std::string& className, const std::string& functionName) const;

	EnumDefinitionSyntaxNode* getEnumDefinition(const std::string& name)
	{
		if (enumDefinitions.contains(name))
			return enumDefinitions[name];
		return nullptr;
	}

	std::string toWrappedCppFunction(ValueSyntaxNode* parentInstance, const Token* nameToken) const;

	std::string toCppType(const Type* type, const Type* parentType = nullptr);

	bool isRefType(const std::string& type) const;

	bool isObjectType(const std::string& type) const;

	bool isSingletonType(const std::string& type) const;

	bool isResourceType(const std::string& type) const;

	std::string castValue(Type* targetType, Type* sourceType)
	{
		bool shouldCast = false;

		if (targetType && sourceType)
		{
			if (isObjectType(targetType->getName()))
				shouldCast = targetType->toString() != sourceType->toString();
		}

		if (shouldCast)
		{
			return "(" + toCppType(targetType) + ")";
		}
		return "";
	}

private:
	bool isSubtypeType(const std::string& type, const std::string& targetType) const;
};