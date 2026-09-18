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

TEST_F(TranspileTest, ClassNameUnicode)
{
	std::string input = R"(
		class_name ExamplePrzykład
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass ExamplePrzyk\xC5\x82" "ad : public RefCounted\n\t{\n\t\tGDCLASS(ExamplePrzyk\xC5\x82" "ad, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/packed_scene.hpp>\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/classes/resource_loader.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tRef<PackedScene> get_Resource()\n\t\t{\n\t\t\treturn Resource;\n\t\t}\n\n\t\tvoid set_Resource(Ref<PackedScene> newResource)\n\t\t{\n\t\t\tResource = newResource;\n\t\t}\n\n\t\tRef<PackedScene> Resource = ResourceLoader::get_singleton()->load(\"res://x/y/z.tscn\");\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_Resource\"), &Test::get_Resource);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_Resource\", \"newResource\"), &Test::set_Resource);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::OBJECT, \"Resource\", PROPERTY_HINT_RESOURCE_TYPE, \"PackedScene\", PROPERTY_USAGE_NONE), \"set_Resource\", \"get_Resource\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, InnerClass)
{
	std::string input = R"(
		class Hello:
			var x: int
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Hello : public RefCounted\n\t{\n\t\tGDCLASS(Hello, RefCounted)\n\tpublic:\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint x;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Hello::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Hello::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, StaticVariableDefinition)
{
	std::string input = R"(
		static var x := 12
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tstatic int& x()\n\t\t{\n\t\t\tstatic int x = 12;\n\t\t\treturn x;\n\t\t}\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/color.hpp>\n#include <godot_cpp/variant/typed_dictionary.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tstatic Color color8(float r, float g, float b, float a = 255.0f)\n\t\t{\n\t\t\treturn Color((r / 255.0f), (g / 255.0f), (b / 255.0f), (a / 255.0f));\n\t\t}\n\n\t\tstatic TypedDictionary<int, Color>& x()\n\t\t{\n\t\t\tstatic TypedDictionary<int, Color> x = {\n\t\t\t\t{1,color8(1.0f, 2.0f, 4.0f)},\n\t\t\t};\n\t\t\treturn x;\n\t\t}\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_static_method(\"Test\", D_METHOD(\"color8\", \"r\", \"g\", \"b\", \"a\"), &Test::color8);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, StaticVariableDefinitionWithUnderscoreInOtherClass)
{
	std::string input = R"(
		class _Stuff:
			static var x := 12
		func doStuff() -> void:
			var x := _Stuff.x
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Stuff : public RefCounted\n\t{\n\t\tGDCLASS(Stuff, RefCounted)\n\tpublic:\n\t\tstatic int& x()\n\t\t{\n\t\t\tstatic int x = 12;\n\t\t\treturn x;\n\t\t}\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = Stuff::x();\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, InnerClassInlineExtends)
{
	std::string input = R"(
		class Stuff:
			pass
		class MyClass extends Stuff:
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Stuff : public RefCounted\n\t{\n\t\tGDCLASS(Stuff, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n\n\tclass MyClass : public Stuff\n\t{\n\t\tGDCLASS(MyClass, Stuff)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ClassExtendsSelf)
{
	std::string input = R"(
		class_name Hello
		extends Hello
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n\nnamespace godot\n{\n\tclass Hello : public Hello\n\t{\n\t\tGDCLASS(Hello, Hello)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ClassAbstract)
{
	std::string input = R"(
		@abstract
		class_name Hello
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Hello : public RefCounted\n\t{\n\t\tGDCLASS(Hello, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ClassExportGroup)
{
	std::string input = R"(
		@export_group("General")

		@export var x: int = 1
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint x = 1;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tClassDB::add_property_group(\"Test\", \"General\", \"\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x\", PROPERTY_HINT_NONE, \"\"), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ClassExportGroupWithPrefix)
{
	std::string input = R"(
		@export_group("General", "general")

		@export var x: int = 1
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint x = 1;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tClassDB::add_property_group(\"Test\", \"General\", \"general\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x\", PROPERTY_HINT_NONE, \"\"), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ClassExportSubgroup)
{
	std::string input = R"(
		@export_subgroup("General")

		@export var x: int = 1
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint x = 1;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tClassDB::add_property_subgroup(\"Test\", \"General\", \"\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x\", PROPERTY_HINT_NONE, \"\"), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ClassExportSubgroupWithPrefix)
{
	std::string input = R"(
		@export_subgroup("General", "general")

		@export var x: int = 1
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint x = 1;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tClassDB::add_property_subgroup(\"Test\", \"General\", \"general\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x\", PROPERTY_HINT_NONE, \"\"), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ClassExportGroupWithMultipleSubgroups)
{
	std::string input = R"(
		@export_group("General")
		@export_subgroup("Subgroup1")

		@export var x1: int = 1
		@export var y1: int = 1

		@export_subgroup("Subgroup2")

		@export var x2: int = 1
		@export var y2: int = 1
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_x1()\n\t\t{\n\t\t\treturn x1;\n\t\t}\n\n\t\tvoid set_x1(int newx1)\n\t\t{\n\t\t\tx1 = newx1;\n\t\t}\n\n\t\tint get_y1()\n\t\t{\n\t\t\treturn y1;\n\t\t}\n\n\t\tvoid set_y1(int newy1)\n\t\t{\n\t\t\ty1 = newy1;\n\t\t}\n\n\t\tint get_x2()\n\t\t{\n\t\t\treturn x2;\n\t\t}\n\n\t\tvoid set_x2(int newx2)\n\t\t{\n\t\t\tx2 = newx2;\n\t\t}\n\n\t\tint get_y2()\n\t\t{\n\t\t\treturn y2;\n\t\t}\n\n\t\tvoid set_y2(int newy2)\n\t\t{\n\t\t\ty2 = newy2;\n\t\t}\n\n\t\tint x1 = 1;\n\t\tint y1 = 1;\n\t\tint x2 = 1;\n\t\tint y2 = 1;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x1\"), &Test::get_x1);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x1\", \"newx1\"), &Test::set_x1);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y1\"), &Test::get_y1);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y1\", \"newy1\"), &Test::set_y1);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x2\"), &Test::get_x2);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x2\", \"newx2\"), &Test::set_x2);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y2\"), &Test::get_y2);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y2\", \"newy2\"), &Test::set_y2);\n\t\t\tClassDB::add_property_group(\"Test\", \"General\", \"\");\n\t\t\tClassDB::add_property_subgroup(\"Test\", \"Subgroup1\", \"\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x1\", PROPERTY_HINT_NONE, \"\"), \"set_x1\", \"get_x1\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"y1\", PROPERTY_HINT_NONE, \"\"), \"set_y1\", \"get_y1\");\n\t\t\tClassDB::add_property_subgroup(\"Test\", \"Subgroup2\", \"\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x2\", PROPERTY_HINT_NONE, \"\"), \"set_x2\", \"get_x2\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"y2\", PROPERTY_HINT_NONE, \"\"), \"set_y2\", \"get_y2\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ClassExportMultipleGroupWithMultipleSubgroupsAndUngroupedProperties)
{
	std::string input = R"(
		@export var x: int = 1
		@export var y: int = 1

		@export_group("Group1")
		@export_subgroup("Subgroup1")

		@export var x1: int = 1
		@export var y1: int = 1

		@export_subgroup("Subgroup2")

		@export var x2: int = 1
		@export var y2: int = 1

		@export_group("Group2")
		@export_subgroup("Subgroup1")

		@export var x3: int = 1
		@export var y3: int = 1

		@export_subgroup("Subgroup2")

		@export var x4: int = 1
		@export var y4: int = 1
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint get_y()\n\t\t{\n\t\t\treturn y;\n\t\t}\n\n\t\tvoid set_y(int newy)\n\t\t{\n\t\t\ty = newy;\n\t\t}\n\n\t\tint get_x1()\n\t\t{\n\t\t\treturn x1;\n\t\t}\n\n\t\tvoid set_x1(int newx1)\n\t\t{\n\t\t\tx1 = newx1;\n\t\t}\n\n\t\tint get_y1()\n\t\t{\n\t\t\treturn y1;\n\t\t}\n\n\t\tvoid set_y1(int newy1)\n\t\t{\n\t\t\ty1 = newy1;\n\t\t}\n\n\t\tint get_x2()\n\t\t{\n\t\t\treturn x2;\n\t\t}\n\n\t\tvoid set_x2(int newx2)\n\t\t{\n\t\t\tx2 = newx2;\n\t\t}\n\n\t\tint get_y2()\n\t\t{\n\t\t\treturn y2;\n\t\t}\n\n\t\tvoid set_y2(int newy2)\n\t\t{\n\t\t\ty2 = newy2;\n\t\t}\n\n\t\tint get_x3()\n\t\t{\n\t\t\treturn x3;\n\t\t}\n\n\t\tvoid set_x3(int newx3)\n\t\t{\n\t\t\tx3 = newx3;\n\t\t}\n\n\t\tint get_y3()\n\t\t{\n\t\t\treturn y3;\n\t\t}\n\n\t\tvoid set_y3(int newy3)\n\t\t{\n\t\t\ty3 = newy3;\n\t\t}\n\n\t\tint get_x4()\n\t\t{\n\t\t\treturn x4;\n\t\t}\n\n\t\tvoid set_x4(int newx4)\n\t\t{\n\t\t\tx4 = newx4;\n\t\t}\n\n\t\tint get_y4()\n\t\t{\n\t\t\treturn y4;\n\t\t}\n\n\t\tvoid set_y4(int newy4)\n\t\t{\n\t\t\ty4 = newy4;\n\t\t}\n\n\t\tint x = 1;\n\t\tint y = 1;\n\t\tint x1 = 1;\n\t\tint y1 = 1;\n\t\tint x2 = 1;\n\t\tint y2 = 1;\n\t\tint x3 = 1;\n\t\tint y3 = 1;\n\t\tint x4 = 1;\n\t\tint y4 = 1;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y\"), &Test::get_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y\", \"newy\"), &Test::set_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x1\"), &Test::get_x1);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x1\", \"newx1\"), &Test::set_x1);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y1\"), &Test::get_y1);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y1\", \"newy1\"), &Test::set_y1);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x2\"), &Test::get_x2);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x2\", \"newx2\"), &Test::set_x2);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y2\"), &Test::get_y2);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y2\", \"newy2\"), &Test::set_y2);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x3\"), &Test::get_x3);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x3\", \"newx3\"), &Test::set_x3);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y3\"), &Test::get_y3);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y3\", \"newy3\"), &Test::set_y3);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x4\"), &Test::get_x4);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x4\", \"newx4\"), &Test::set_x4);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y4\"), &Test::get_y4);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y4\", \"newy4\"), &Test::set_y4);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x\", PROPERTY_HINT_NONE, \"\"), \"set_x\", \"get_x\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"y\", PROPERTY_HINT_NONE, \"\"), \"set_y\", \"get_y\");\n\t\t\tClassDB::add_property_group(\"Test\", \"Group1\", \"\");\n\t\t\tClassDB::add_property_subgroup(\"Test\", \"Subgroup1\", \"\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x1\", PROPERTY_HINT_NONE, \"\"), \"set_x1\", \"get_x1\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"y1\", PROPERTY_HINT_NONE, \"\"), \"set_y1\", \"get_y1\");\n\t\t\tClassDB::add_property_subgroup(\"Test\", \"Subgroup2\", \"\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x2\", PROPERTY_HINT_NONE, \"\"), \"set_x2\", \"get_x2\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"y2\", PROPERTY_HINT_NONE, \"\"), \"set_y2\", \"get_y2\");\n\t\t\tClassDB::add_property_group(\"Test\", \"Group2\", \"\");\n\t\t\tClassDB::add_property_subgroup(\"Test\", \"Subgroup1\", \"\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x3\", PROPERTY_HINT_NONE, \"\"), \"set_x3\", \"get_x3\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"y3\", PROPERTY_HINT_NONE, \"\"), \"set_y3\", \"get_y3\");\n\t\t\tClassDB::add_property_subgroup(\"Test\", \"Subgroup2\", \"\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x4\", PROPERTY_HINT_NONE, \"\"), \"set_x4\", \"get_x4\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"y4\", PROPERTY_HINT_NONE, \"\"), \"set_y4\", \"get_y4\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ExportRangeInt)
{
	std::string input = R"(
		@export_range(10, 20) var x: int = 10
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint x = 10;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x\", PROPERTY_HINT_RANGE, \"10,20\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ExportRangeIntWithStep)
{
	std::string input = R"(
		@export_range(10, 20, 5) var x: int = 10
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint x = 10;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x\", PROPERTY_HINT_RANGE, \"10,20,5\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ExportRangeFloat)
{
	std::string input = R"(
		@export_range(100.0, 2000.0) var x: float = 200.0
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tfloat get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(float newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tfloat x = 200.0f;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::FLOAT, \"x\", PROPERTY_HINT_RANGE, \"100.0f,2000.0f\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ExportRangeFloatWithStep)
{
	std::string input = R"(
		@export_range(100.0, 2000.0, 10.0) var x: float = 200.0
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tfloat get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(float newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tfloat x = 200.0f;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::FLOAT, \"x\", PROPERTY_HINT_RANGE, \"100.0f,2000.0f,10.0f\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}