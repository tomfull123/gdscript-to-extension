#include "TranspileTest.h"

TEST_F(TranspileTest, ClassName)
{
	std::string input = R"(
		class_name Hello
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Hello : public RefCounted\n\t{\n\t\tGDCLASS(Hello, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ClassNameWithUnderscore)
{
	std::string input = R"(
		class_name _Hello
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Hello : public RefCounted\n\t{\n\t\tGDCLASS(Hello, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ExtendsNode3D)
{
	std::string input = R"(
		extends Node3D
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/node3d.hpp>\n\nnamespace godot\n{\n\tclass Test : public Node3D\n\t{\n\t\tGDCLASS(Test, Node3D)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ToolAnnotation)
{
	std::string input = R"(
		@tool
		class_name Hello
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Hello : public RefCounted\n\t{\n\t\tGDCLASS(Hello, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, Preload)
{
	std::string input = R"(
		var Resource := preload("res://x/y/z.tscn")
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/packed_scene.hpp>\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/classes/resource_loader.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tRef<PackedScene> Resource = ResourceLoader::get_singleton()->load(\"res://x/y/z.tscn\");\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, InnerClass)
{
	std::string input = R"(
		class Hello:
			var x: int
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Hello : public RefCounted\n\t{\n\t\tGDCLASS(Hello, RefCounted)\n\tpublic:\n\t\tint x;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, StaticVariableDefinition)
{
	std::string input = R"(
		static var x := 12
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tstatic int x;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n\tint Test::x = 12;\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, StaticVariableDefinitionDictionaryFunction)
{
	std::string input = R"(
		static func color8(r: float, g: float, b: float, a: float = 255.0) -> Color:
			return Color(r / 255.0, g / 255.0, b / 255.0, a / 255.0)

		static var x: Dictionary = {
			1: color8(1.0, 2.0, 4.0),
		}
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/color.hpp>\n#include <map>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tstatic Color color8(float r, float g, float b, float a = 255.0f)\n\t\t{\n\t\t\treturn Color((r / 255.0f), (g / 255.0f), (b / 255.0f), (a / 255.0f));\n\t\t}\n\n\t\tstatic std::map<int, Color> x;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_static_method(\"Test\", D_METHOD(\"color8\", \"r\", \"g\", \"b\", \"a\"), &Test::color8);\n\t\t}\n\t};\n\tstd::map<int, Color> Test::x = {\n\t\t{1,color8(1.0f, 2.0f, 4.0f)},\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}