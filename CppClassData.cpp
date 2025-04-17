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

bool CppClassData::isGodotTypeMethod(const Type* classType, const std::string& name, const CppData* data)
{
	if (!classType) return false;

	std::string classTypeName = classType->getName();

	while (true)
	{
		if (data->classData.contains(classTypeName))
		{
			const auto typeClass = data->classData.find(classTypeName)->second;

			if (typeClass->functionPrototypeDefinitions.contains(name)) return true;
			if (typeClass->memberVariableDefinitions.contains(name))
			{
				auto variableDefinition = typeClass->memberVariableDefinitions[name];
				if (variableDefinition->getGetterName() != nullptr || variableDefinition->getSetterName() != nullptr)
					return true;
			}
		}

		if (data->inheritTypes.contains(classTypeName))
			classTypeName = data->inheritTypes.find(classTypeName)->second;
		else
			return false;
	}

	return false;
}

bool CppClassData::isProperty(ValueSyntaxNode* instance, const GDToken* name, const CppData* data)
{
	if (!instance) return false;

	auto type = instance->getType();
	if (type)
	{
		std::string typeName = type->getName();
		if (typeName[0] == '_') typeName.erase(0, 1);
		if (data->classData.contains(typeName))
		{
			const auto typeClass = data->classData.find(typeName)->second;

			if (typeClass->memberVariableDefinitions.contains(name->value))
			{
				const auto* variableDefinition = typeClass->memberVariableDefinitions.find(name->value)->second;
				return variableDefinition->getGetterName() == nullptr && variableDefinition->getSetterName() == nullptr;
			}
		}
	}

	return false;
}