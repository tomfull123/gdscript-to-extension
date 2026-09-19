#include "TranspileTest.h"

TEST_F(TranspileTest, EmptyArray)
{
	std::string input = R"(
		var x: Array[int] = []
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tTypedArray<int> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(TypedArray<int> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tTypedArray<int> x = {};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::ARRAY, \"x\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ArrayType)
{
	std::string input = R"(
		var x: Array[int]
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tTypedArray<int> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(TypedArray<int> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tTypedArray<int> x;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::ARRAY, \"x\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ArrayWithValuesInt)
{
	std::string input = R"(
		var x: Array[int] = [1, 2, 3]
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tTypedArray<int> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(TypedArray<int> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tTypedArray<int> x = {\n\t\t\t1,2,3,\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::ARRAY, \"x\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ArrayWithValuesIntNewLines)
{
	std::string input = R"(
		var x: Array[int] = [
			1,
			2,
			3
		]
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tTypedArray<int> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(TypedArray<int> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tTypedArray<int> x = {\n\t\t\t1,2,3,\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::ARRAY, \"x\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ArrayWithValuesIntNewLinesSpaces)
{
	std::string input = R"(
		var x: Array[int] = [
		    1,
		    2,
		    3
		]
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tTypedArray<int> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(TypedArray<int> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tTypedArray<int> x = {\n\t\t\t1,2,3,\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::ARRAY, \"x\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, IndexArray)
{
	std::string input = R"(
		var x: Array[int] = [1, 2, 3]
		var y := x[1]
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tTypedArray<int> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(TypedArray<int> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint get_y()\n\t\t{\n\t\t\treturn y;\n\t\t}\n\n\t\tvoid set_y(int newy)\n\t\t{\n\t\t\ty = newy;\n\t\t}\n\n\t\tTypedArray<int> x = {\n\t\t\t1,2,3,\n\t\t};\n\t\tint y = ((int)x[1]);\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y\"), &Test::get_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y\", \"newy\"), &Test::set_y);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::ARRAY, \"x\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"y\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_y\", \"get_y\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, AssignIndexedArray)
{
	std::string input = R"(
		class Stuff:
			var items: Array[int] = [1]

		func doStuff() -> void:
			var x := Stuff.new()
			x.items[0] = 2
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Stuff : public RefCounted\n\t{\n\t\tGDCLASS(Stuff, RefCounted)\n\tpublic:\n\t\tTypedArray<int> get_items()\n\t\t{\n\t\t\treturn items;\n\t\t}\n\n\t\tvoid set_items(TypedArray<int> newitems)\n\t\t{\n\t\t\titems = newitems;\n\t\t}\n\n\t\tTypedArray<int> items = { 1,\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_items\"), &Stuff::get_items);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_items\", \"newitems\"), &Stuff::set_items);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::ARRAY, \"items\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_items\", \"get_items\");\n\t\t}\n\t};\n\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tRef<Stuff> x = Ref(memnew(Stuff));\n\t\t\tx->get_items()[0] = 2;\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}