#pragma once

#include "GDTokenStream.h"
#include <unordered_set>
#include <unordered_map>
#include "Type.h"
#include "TranspilerDefinitions.h"
#include "CppClassData.h"

struct CppData;

class SyntaxNode
{
public:
	virtual bool needsSemiColon()
	{
		return true;
	}

	virtual void hoist(CppData* data) = 0;

	virtual void resolveDefinitions(CppData* data) = 0;

	virtual void resolveTypes(CppData* data, Type* otherType = nullptr) = 0;

	virtual std::string toCpp(CppData* data, const std::string& indents) = 0;
};

class ValueSyntaxNode : public SyntaxNode
{
public:
	virtual bool hasParent() const
	{
		return false;
	}

	virtual ValueSyntaxNode* getParent() const
	{
		return nullptr;
	}

	virtual bool isFunction(CppData* data) const
	{
		return false;
	}

	virtual Type* getType() = 0;

	virtual std::string getName() = 0;
};

class VariableDefinitionSyntaxNode;
class FunctionPrototypeSyntaxNode;
class EnumDefinitionSyntaxNode;
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
