#pragma once

#include "GDTokenStream.h"
#include <unordered_set>
#include <unordered_map>
#include "Type.h"
#include "TranspilerDefinitions.h"

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

struct CppClassData
{
	std::unordered_set<std::string> types;
	std::unordered_set<std::string> externalFunctions;
	std::unordered_map<std::string, VariableDefinitionSyntaxNode*> variableDefinitions;
	std::unordered_map<std::string, FunctionPrototypeSyntaxNode*> functionPrototypeDefinitions;
	std::unordered_map<std::string, EnumDefinitionSyntaxNode*> enumDefinitions;
	std::unordered_set<std::string> typeDefinitions;
	std::string currentClassName;
	Type* classInheritedType;

	std::string toCppType(const Type* type)
	{
		if (!type) return "auto";

		auto enumDef = enumDefinitions[type->name];
		if (enumDef) return type->name;

		auto it = GDTYPES_TO_CPPTYPES.find(type->name);

		if (it != GDTYPES_TO_CPPTYPES.end())
		{
			types.emplace(it->second);

			std::string subtypesString;

			if (!type->subtypes.empty())
			{
				auto lastIndex = type->subtypes.size() - 1;

				for (int i = 0; i < type->subtypes.size(); i++)
				{
					const Type* subtype = type->subtypes[i];
					subtypesString += toCppType(subtype);

					if (i < lastIndex) subtypesString += ", ";
				}

				subtypesString = "<" + subtypesString + ">";
			}

			return it->second + subtypesString;
		}

		std::string typeName = type->name;

		if (typeName[0] == '_') typeName.erase(0, 1);

		types.emplace(typeName);

		if (typeDefinitions.contains(typeName)) return typeName;

		if (isGodotType(typeName) || GDTYPES_TO_CPPTYPES.contains(typeName)) return "Ref<" + typeName + ">";
		return "Ref<" + typeName + ">";
	}

	std::string toCppFunction(const std::string& functionName, const std::string& parentType)
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

	std::string toWrappedCppFunction(ValueSyntaxNode* parentInstance, const GDToken* nameToken) const
	{
		std::string name = nameToken->value;

		if (parentInstance)
		{
			auto parentName = parentInstance->getName();
			auto it = CPPTYPES_TO_FUNCTION.find(parentName);

			if (it != CPPTYPES_TO_FUNCTION.end())
			{
				return it->second + "(\"" + name + "\")";
			}

			if (GDTYPE_PROPERTIES_TO_CPP_PROPERTIES.contains(parentName))
			{
				const auto& methods = GDTYPE_PROPERTIES_TO_CPP_PROPERTIES.find(parentName)->second;
				if (methods.contains(name)) return methods.find(name)->second;
			}
		}

		return name;
	}

	std::string getIncludePath(const std::string& type) const
	{
		auto it = CPPTYPES_TO_INCLUDE_PATH.find(type);

		if (it != CPPTYPES_TO_INCLUDE_PATH.end()) return it->second;

		auto git = GODOTTYPES_TO_INCLUDE_PATH.find(type);

		if (git != GODOTTYPES_TO_INCLUDE_PATH.end()) return git->second;

		return "";
	}

	bool isGodotType(const std::string& type) const
	{
		return GODOTTYPES_TO_INCLUDE_PATH.contains(type);
	}

	bool isClassMethod(const std::string& name, const CppData* data) const
	{
		return isGodotTypeMethod(classInheritedType, name, data);
	}

	static bool isGodotTypeMethod(const Type* parentType, const std::string& name, const CppData* data)
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

	static bool isProperty(ValueSyntaxNode* parentInstance, const GDToken* name, const CppData* data)
	{
		if (!parentInstance) return false;

		auto parentType = parentInstance->getType();
		if (parentType && data->classData.contains(parentType->name))
		{
			const auto typeClass = data->classData.find(parentType->name)->second;

			if (typeClass->variableDefinitions.contains(name->value)) return true;
		}

		return false;
	}
};
