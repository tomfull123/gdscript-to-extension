#pragma once

#include "TokenStream.h"
#include <unordered_set>
#include <unordered_map>

const std::unordered_set<std::string> GODOT_LIFECYCLE_METHODS = {
	"_ready",
	"_process",
};

const std::unordered_map<std::string, std::string> GDTYPES_TO_CPPTYPES = {
	{"int", "int"},
	{"float", "float"},
	{"String", "String"},
	{"bool", "bool"},
	{"void", "void"},
	{"InputMap", "InputMap::get_singleton()"},
	{"Key", "Key"},
	{"MouseButton", "MouseButton"},
	{"Vector2", "Vector2"},
	{"Vector2i", "Vector2i"},
	{"Vector3", "Vector3"},
	{"Vector3i", "Vector3i"},
	{"Vector4", "Vector4"},
	{"Vector4i", "Vector4i"},
	{"Color", "Color"},
	{"Dictionary", "std::map"},
	{"Array", "std::vector"},
	{"this", "this"},
	{"Mesh", "Mesh"},
	{"RenderingServer", "RenderingServer::get_singleton()"},
	{"ResourceLoader", "ResourceLoader::get_singleton()"},
	{"Viewport", "Viewport"}
};

const std::unordered_map<std::string, std::string> GDFUNCTIONS_TO_CPPFUNCTIONS = {
	{"absf", "abs"},
	{"absi", "abs"},
	{"minf", "std::fminf"},
	{"mini", "std::min"},
	{"roundi", "std::round"},
	{"roundf", "std::roundf"},
	{"print", "UtilityFunctions::print"},
	{"load", "ResourceLoader::get_singleton()->load"},
	{"get_viewport", "get_viewport"},
	{"append", "push_back"},
};

const std::unordered_set<std::string> CPP_PRIMITIVE_TYPES = {
	"int",
	"float",
	"double",
	"bool",
	"void",
	"this",
};

const std::unordered_map<std::string, std::string> CPPTYPES_TO_INCLUDE_PATH = {
	{"std::string", "<string>"},
	{"std::map", "<map>"},
	{"std::unordered_map", "<unordered_map>"},
	{"std::vector", "<vector>"},
	{"std::min", "<algorithm>"},
	{"std::round", "<cmath>"},
	{"std::roundf", "<cmath>"},
	{"std::abs", "<cmath>"},
};

const std::unordered_map<std::string, std::string> CPPFUNCTION_RETURN_TYPES = {
	{"std::min", "int"},
	{"std::fminf", "float"},
	{"std::round", "int"},
	{"std::roundf", "float"},
	{"ResourceLoader::get_singleton()->load", "PackedScene"},
	{"Color::named", "Color"},
	{"get_viewport", "Viewport"},
};

const std::unordered_map<std::string, std::string> GODOTTYPES_TO_INCLUDE_PATH = {
	{"RefCounted", "<godot_cpp/classes/ref.hpp>"},
	{"InputMap::get_singleton()", "<godot_cpp/classes/input_map.hpp>"},
	{"InputEventMouseButton", "<godot_cpp/classes/input_event_mouse_button.hpp>"},
	{"InputEventKey", "<godot_cpp/classes/input_event_key.hpp>"},
	{"MouseButton", "<godot_cpp/classes/input_event_mouse_button.hpp>"},
	{"FastNoiseLite", "<godot_cpp/classes/fast_noise_lite.hpp>"},
	{"Signal", "<godot_cpp/classes/ref.hpp>"},
	{"ArrayMesh", "<godot_cpp/classes/array_mesh.hpp>"},
	{"SurfaceTool", "<godot_cpp/classes/surface_tool.hpp>"},
	{"Curve", "<godot_cpp/classes/curve.hpp>"},
	{"RenderingServer::get_singleton()", "<godot_cpp/classes/rendering_server.hpp>"},
	{"Node3D", "<godot_cpp/classes/node3d.hpp>"},
	{"get_viewport", "<godot_cpp/classes/viewport.hpp>"},
	{"ResourceLoader::get_singleton()", "<godot_cpp/classes/resource_loader.hpp>"},
	{"Vector2", "<godot_cpp/variant/vector2.hpp>"},
	{"Vector2i", "<godot_cpp/variant/vector2i.hpp>"},
	{"Vector3", "<godot_cpp/variant/vector3.hpp>"},
	{"Vector3i", "<godot_cpp/variant/vector3i.hpp>"},
	{"Vector4", "<godot_cpp/variant/vector4.hpp>"},
	{"Vector4i", "<godot_cpp/variant/vector4i.hpp>"},
	{"Color", "<godot_cpp/variant/color.hpp>"},
	{"Mesh", "<godot_cpp/classes/mesh.hpp>"},
	{"Resource", "<godot_cpp/classes/resource.hpp>"},
	{"Thread", "<godot_cpp/classes/thread.hpp>"},
	{"Viewport", "<godot_cpp/classes/viewport.hpp>"},
	{"String", "<godot_cpp/variant/string.hpp>"},
	{"UtilityFunctions::print", "<godot_cpp/variant/utility_functions.hpp>"},
	{"Material", "<godot_cpp/classes/material.hpp>"},
	{"ShaderMaterial", "<godot_cpp/classes/shader_material.hpp>"},
	{"MeshInstance3D", "<godot_cpp/classes/mesh_instance3d.hpp>"},
	{"TypedArray", "<godot_cpp/variant/typed_array.hpp>"},
	{"PackedScene", "<godot_cpp/classes/packed_scene.hpp>"},
	{"RayCast3D", "<godot_cpp/classes/ray_cast3d.hpp>"},
};

const std::unordered_map<std::string, std::string> CPPTYPES_TO_FUNCTION = {
	{"Color", "named"},
};

const std::unordered_map<std::string, std::unordered_set<std::string>> GDTYPE_PROPERTIES = {
	{
		"Vector3",
		{
			"x",
			"y",
			"z",
		}
	},
	{
		"Vector3i",
		{
			"x",
			"y",
			"z",
		}
	},
};

const std::unordered_map<std::string, const std::unordered_map<std::string, std::string>> GDTYPE_METHODS_TO_CPP_METHODS = {
	{
		"Array",
		{
			{"append", "push_back"}
		}
	},
	{
		"Dictionary",
		{
			{"has", "contains"}
		}
	}
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

	virtual bool isFunction() const
	{
		return false;
	}

	virtual Type* getType() = 0;

	virtual std::string getName() = 0;
};

class VariableDefinitionSyntaxNode;
class FunctionPrototypeSyntaxNode;
class EnumDefinitionSyntaxNode;

struct CppData
{
	explicit CppData(const std::string& fileName) :
		fileName(fileName)
	{}

	std::string fileName;
	std::unordered_set<std::string> types;
	std::unordered_set<std::string> externalFunctions;
	std::unordered_map<std::string, VariableDefinitionSyntaxNode*> variableDefinitions;
	std::unordered_map<std::string, FunctionPrototypeSyntaxNode*> functionPrototypeDefinitions;
	std::unordered_map<std::string, EnumDefinitionSyntaxNode*> enumDefinitions;
	std::string currentClassName;

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

	std::string toWrappedCppFunction(ValueSyntaxNode* parentInstance, const Token* nameToken) const
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
		return GODOTTYPES_TO_INCLUDE_PATH.contains(type);
	}

	static bool isProperty(ValueSyntaxNode* parentInstance, const Token* name)
	{
		if (!parentInstance) return false;

		auto parentType = parentInstance->getType();
		if (parentType && GDTYPE_PROPERTIES.contains(parentType->name))
		{
			auto& properties = GDTYPE_PROPERTIES.find(parentType->name)->second;
			return properties.contains(name->value);
		}

		return false;
	}
};
