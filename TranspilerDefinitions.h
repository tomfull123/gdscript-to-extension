#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>

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

// Global methods
const std::unordered_map<std::string, std::string> GDFUNCTIONS_TO_CPPFUNCTIONS = {
	{"absf", "Math::abs"},
	{"absi", "Math::abs"},
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
	{"Vector3::ZERO", "Vector3"},
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
	{"Math::abs", "<godot_cpp/core/math.hpp>"},
	{"Camera3D", "<godot_cpp/classes/camera3d.hpp>"},
};

const std::unordered_map<std::string, std::string> CPPTYPES_TO_FUNCTION = {
	{"Color", "named"},
};

const std::unordered_set<std::string> FUNCTION_TO_MEMBER_METHOD_CALL = {
	"abs",
};

const std::unordered_map<std::string, const std::unordered_map<std::string, std::string>> GDTYPE_PROPERTIES_TO_CPP_PROPERTIES = {
	{
		"Vector3",
		{
			{"ZERO", "Vector3(0, 0, 0)"}
		}
	},
};

const std::unordered_map<std::string, const std::unordered_map<std::string, std::string>> GDTYPE_METHODS_TO_CPP_METHODS = {
	{
		"Array",
		{
			{"append", "push_back"},
			{"is_empty", "empty"}
		}
	},
	{
		"Dictionary",
		{
			{"has", "contains"}
		}
	},
	{
		"MeshInstance3D",
		{
			{"mesh", "get_mesh"}
		}
	},
};