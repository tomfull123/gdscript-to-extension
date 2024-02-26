#pragma once

#include "TokenStream.h"
#include <unordered_set>
#include <unordered_map>

const std::unordered_map<std::string, std::string> GDTYPES_TO_CPPTYPES = {
	{"int", "int"},
	{"float", "float"},
	{"String", "String"},
	{"InputMap", "InputMap::get_singleton()"},
	{"Key", "Key"},
	{"MouseButton", "MouseButton"},
	{"absf", "abs"},
	{"absi", "abs"},
};

const std::unordered_map<std::string, std::string> CPPTYPES_TO_INCLUDE_PATH = {
	{"std::string", "<string>"},
	{"RefCounted", "<godot_cpp/classes/ref.hpp>"},
	{"InputMap::get_singleton()", "<godot_cpp/classes/input_map.hpp>"},
	{"InputEventMouseButton", "<godot_cpp/classes/input_event_mouse_button.hpp>"},
	{"InputEventKey", "<godot_cpp/classes/input_event_key.hpp>"},
	{"MouseButton", "<godot_cpp/classes/input_event_mouse_button.hpp>"},
	{"FastNoiseLite", "<godot_cpp/classes/fast_noise_lite.hpp>"},
	{"Signal", "<godot_cpp/classes/ref.hpp>"},
};

struct Type
{
	Type(
		const std::string& name,
		Type* subtype = nullptr
	) :
		name(name),
		subtype(subtype)
	{}

	std::string name;
	Type* subtype;
};

class VariableDefinitionSyntaxNode;

struct CppData
{
	explicit CppData(const std::string& fileName) :
		fileName(fileName)
	{}

	std::string fileName;
	std::unordered_set<std::string> types;
	std::unordered_map<std::string, VariableDefinitionSyntaxNode*> variableDefinitions;

	std::string toCppType(Type* type)
	{
		if (!type) return "auto";

		auto it = GDTYPES_TO_CPPTYPES.find(type->name);

		if (it != GDTYPES_TO_CPPTYPES.end())
		{
			types.emplace(it->second);
			return it->second;
		}

		types.emplace(type->name);

		return "Ref<" + type->name + ">";
	}
};

class SyntaxNode
{
public:

	virtual bool needsSemiColon()
	{
		return true;
	}

	virtual std::string toCpp(CppData* data, const std::string& indents) = 0;
};

class ValueSyntaxNode : public SyntaxNode
{
public:
	virtual bool hasParent() const
	{
		return false;
	}
};