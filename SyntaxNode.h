#pragma once

#include "TokenStream.h"
#include <unordered_set>
#include <unordered_map>

const std::unordered_map<std::string, std::string> GDTYPES_TO_CPPTYPES = {
	{"int", "int"},
	{"float", "float"},
	{"String", "String"},
	{"bool", "bool"},
	{"void", "void"},
	{"InputMap", "InputMap::get_singleton()"},
	{"Key", "Key"},
	{"MouseButton", "MouseButton"},
	{"Color", "Color"},
	{"Dictionary", "std::map"},
};

const std::unordered_map<std::string, std::string> GDFUNCTIONS_TO_CPPFUNCTIONS = {
	{"absf", "abs"},
	{"absi", "abs"},
	{"minf", "std::min"},
	{"mini", "std::min"},
};

const std::unordered_map<std::string, std::string> CPPTYPES_TO_INCLUDE_PATH = {
	{"std::string", "<string>"},
	{"std::map", "<map>"},
	{"std::unordered_map", "<unordered_map>"},
};

const std::unordered_map<std::string, std::string> GODOTTYPES_TO_INCLUDE_PATH = {
	{"RefCounted", "<godot_cpp/classes/ref.hpp>"},
	{"InputMap::get_singleton()", "<godot_cpp/classes/input_map.hpp>"},
	{"InputEventMouseButton", "<godot_cpp/classes/input_event_mouse_button.hpp>"},
	{"InputEventKey", "<godot_cpp/classes/input_event_key.hpp>"},
	{"MouseButton", "<godot_cpp/classes/input_event_mouse_button.hpp>"},
	{"FastNoiseLite", "<godot_cpp/classes/fast_noise_lite.hpp>"},
	{"Signal", "<godot_cpp/classes/ref.hpp>"},
};

const std::unordered_map<std::string, std::string> CPPTYPES_TO_FUNCTION = {
	{"Color", "::named"},
};

struct Type
{
	Type(
		const std::string& name,
		const std::vector<Type*>& subtypes = {}
	) :
		name(name),
		subtypes(subtypes)
	{}

	Type* subtype(int index)
	{
		if (index >= subtypes.size()) return nullptr;
		return subtypes[index];
	}

	std::string name;
	std::vector<Type*> subtypes;
};

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

	virtual void resolveTypes(CppData* data) = 0;

	virtual std::string toCpp(CppData* data, const std::string& indents) = 0;
};

class ValueSyntaxNode : public SyntaxNode
{
public:
	virtual bool hasParent() const
	{
		return false;
	}

	virtual Type* getType() = 0;

	virtual std::string getName() = 0;
};

class VariableDefinitionSyntaxNode;
class EnumDefinitionSyntaxNode;

struct CppData
{
	explicit CppData(const std::string& fileName) :
		fileName(fileName)
	{}

	std::string fileName;
	std::unordered_set<std::string> types;
	std::unordered_map<std::string, VariableDefinitionSyntaxNode*> variableDefinitions;
	std::unordered_map<std::string, EnumDefinitionSyntaxNode*> enumDefinitions;

	std::string toCppType(Type* type)
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
					subtypesString += subtype->name;

					if (i < lastIndex) subtypesString += ", ";
				}

				subtypesString = "<" + subtypesString + ">";
			}

			return it->second + subtypesString;
		}

		types.emplace(type->name);

		if (isGodotType(type->name)) return "Ref<" + type->name + ">";
		return type->name;
	}

	std::string toCppFunction(const std::string& functionName)
	{
		auto it = GDFUNCTIONS_TO_CPPFUNCTIONS.find(functionName);

		if (it != GDFUNCTIONS_TO_CPPFUNCTIONS.end())
		{
			types.emplace(it->second);
			return it->second;
		}

		types.emplace(functionName);

		return functionName;
	}

	std::string toWrappedCppFunction(ValueSyntaxNode* parentInstance, Token* nameToken) const
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
		auto it = GODOTTYPES_TO_INCLUDE_PATH.find(type);
		return (it != GODOTTYPES_TO_INCLUDE_PATH.end());
	}
};
