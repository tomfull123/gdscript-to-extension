#include "CppData.h"
#include "CppClassData.h"
#include "TranspilerDefinitions.h"
#include "SyntaxNode.h"
#include "EnumDefinitionSyntaxNode.h"

CppClassData* CppData::getClassDefinition(const std::string& className) const
{
	if (classData.contains(className))
	{
		return classData.find(className)->second;
	}

	return nullptr;
}

VariableDefinitionSyntaxNode* CppData::getCurrentClassVariableDefinition(const std::string& variableName) const
{
	if (!currentClass) return nullptr;

	const std::string& className = currentClass->currentClassName;

	auto classDef = getClassDefinition(className);

	auto localVar = classDef->getLocalVariableDefinition(variableName);
	if (localVar) return localVar;

	return getVariableDefinition(className, variableName);
}

VariableDefinitionSyntaxNode* CppData::getVariableDefinition(const std::string& className, const std::string& variableName) const
{
	std::string classTypeName = className;

	while (true)
	{
		if (classTypeName[0] == '_') classTypeName.erase(0, 1);
		if (classData.contains(classTypeName))
		{
			const auto typeClass = classData.find(classTypeName)->second;

			if (typeClass->memberVariableDefinitions.contains(variableName))
			{
				return typeClass->memberVariableDefinitions.find(variableName)->second;
			}
		}

		if (inheritTypes.contains(classTypeName))
			classTypeName = inheritTypes.find(classTypeName)->second;
		else
			return nullptr;
	}

	return nullptr;
}

FunctionPrototypeSyntaxNode* CppData::getFunctionPrototype(const std::string& className, const std::string& functionName) const
{
	std::string classTypeName = className;

	while (true)
	{
		if (classTypeName[0] == '_') classTypeName.erase(0, 1);
		if (classData.contains(classTypeName))
		{
			const auto typeClass = classData.find(classTypeName)->second;

			if (typeClass->functionPrototypeDefinitions.contains(functionName))
			{
				return typeClass->functionPrototypeDefinitions.find(functionName)->second;
			}
		}

		if (inheritTypes.contains(classTypeName))
			classTypeName = inheritTypes.find(classTypeName)->second;
		else
			return nullptr;
	}

	return nullptr;
}

std::string CppData::toWrappedCppFunction(ValueSyntaxNode* parentInstance, const Token* nameToken) const
{
	std::string name = nameToken->value;

	if (name[0] == '_')
	{
		name.erase(0, 1);
		if (!classData.contains(name))
			name = "_" + name;
	}

	if (parentInstance)
	{
		auto parentName = parentInstance->getName();
		auto it = CPPTYPES_TO_FUNCTION.find(parentName);

		if (it != CPPTYPES_TO_FUNCTION.end())
		{
			return it->second + "(\"" + name + "\")";
		}

		if (classData.contains(parentName))
		{
			const auto& classDef = classData.find(parentName)->second;
			const auto& constantValueMappings = classDef->constantValueMappings;
			if (constantValueMappings.contains(name)) return constantValueMappings.find(name)->second;
		}
	}

	return name;
}

std::string CppData::toCppType(const Type* type, const Type* parentType)
{
	if (!type) return "auto";

	std::string typeName = type->getName();
	auto enumDef = getEnumDefinition(typeName);
	if (enumDef) return typeName;

	auto it = GDTYPES_TO_CPPTYPES.find(typeName);

	if (it != GDTYPES_TO_CPPTYPES.end())
	{
		currentClass->types.emplace(it->second);

		std::string subtypesString;

		if (!type->subtypes.empty())
		{
			auto lastIndex = type->subtypes.size() - 1;

			for (int i = 0; i < type->subtypes.size(); i++)
			{
				const Type* subtype = type->subtypes[i];
				subtypesString += toCppType(subtype, type);

				if (i < lastIndex) subtypesString += ", ";
			}

			subtypesString = "<" + subtypesString + ">";
		}

		return it->second + subtypesString;
	}

	if (typeName[0] == '_') typeName.erase(0, 1);

	currentClass->types.emplace(typeName);

	if (currentClass->typeDefinitions.contains(typeName)) return typeName;

	if (isRefType(typeName)) return "Ref<" + typeName + ">";

	std::string parentTypeName;

	if (parentType) parentTypeName = parentType->getName();

	if (parentTypeName != "Array") // currently not supported
	{
		if (isObjectType(typeName)) return typeName + "*";
	}

	return typeName;
}

bool CppData::isRefType(const std::string& type) const
{
	std::string currentType = type;

	while (true)
	{
		if (currentType == "RefCounted") return true;

		if (inheritTypes.contains(currentType))
			currentType = inheritTypes.find(currentType)->second;
		else
			return false;
	}

	return false;
}

bool CppData::isObjectType(const std::string& type) const
{
	std::string currentType = type;

	while (true)
	{
		if (currentType == "Object") return true;

		if (inheritTypes.contains(currentType))
			currentType = inheritTypes.find(currentType)->second;
		else
			return false;
	}

	return false;
}

bool CppData::isSingletonType(const std::string& type) const
{
	return GDSINGLETON_TYPES.contains(type);
}