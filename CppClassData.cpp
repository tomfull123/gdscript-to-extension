#include "CppClassData.h"
#include "TranspilerDefinitions.h"
#include "SyntaxNode.h"
#include "VariableDefinitionSyntaxNode.h"

std::string CppClassData::toCppFunction(const std::string& functionName, const std::string& parentType)
{
	auto it = GDFUNCTIONS_TO_CPPFUNCTIONS.find(functionName);

	if (it != GDFUNCTIONS_TO_CPPFUNCTIONS.end())
	{
		externalFunctions.emplace(it->second);
		return it->second;
	}

	if (parentType != "")
	{
		auto methods = GDTYPE_METHODS_TO_CPP_METHODS.find(parentType);

		if (methods != GDTYPE_METHODS_TO_CPP_METHODS.end())
		{
			auto method = methods->second.find(functionName);

			if (method != methods->second.end())
			{
				return method->second;
			}
		}
	}

	return functionName;
}

std::string CppClassData::getIncludePath(const std::string& type) const
{
	auto it = CPPTYPES_TO_INCLUDE_PATH.find(type);

	if (it != CPPTYPES_TO_INCLUDE_PATH.end()) return it->second;

	auto git = GODOTTYPES_TO_INCLUDE_PATH.find(type);

	if (git != GODOTTYPES_TO_INCLUDE_PATH.end()) return git->second;

	return "";
}

bool CppClassData::isGodotType(const std::string& type) const
{
	return GODOTTYPES_TO_INCLUDE_PATH.contains(type);
}

bool CppClassData::isClassMethod(const std::string& name, const CppData* data) const
{
	return isGodotTypeMethod(classInheritedType, name, data);
}

bool CppClassData::isGodotTypeMethod(const Type* parentType, const std::string& name, const CppData* data)
{
	if (!parentType) return false;

	std::string parentTypeName = parentType->name;

	while (true)
	{
		if (data->classData.contains(parentTypeName))
		{
			const auto typeClass = data->classData.find(parentTypeName)->second;

			if (typeClass->functionPrototypeDefinitions.contains(name)) return true;
			if (typeClass->variableDefinitions.contains(name)) return true;
		}

		if (data->inheritTypes.contains(parentTypeName))
			parentTypeName = data->inheritTypes.find(parentTypeName)->second;
		else
			return false;
	}

	return false;
}

bool CppClassData::isProperty(ValueSyntaxNode* parentInstance, const GDToken* name, const CppData* data)
{
	if (!parentInstance) return false;

	auto parentType = parentInstance->getType();
	if (parentType)
	{
		std::string parentTypeName = parentType->name;
		if (parentTypeName[0] == '_') parentTypeName.erase(0, 1);
		if (data->classData.contains(parentTypeName))
		{
			const auto typeClass = data->classData.find(parentTypeName)->second;

			if (typeClass->variableDefinitions.contains(name->value))
			{
				const auto* variableDefinition = typeClass->variableDefinitions.find(name->value)->second;
				return variableDefinition->getGetterName() == nullptr && variableDefinition->getSetterName() == nullptr;
			}
		}
	}

	return false;
}