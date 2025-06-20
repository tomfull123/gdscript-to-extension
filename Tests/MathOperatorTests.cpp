#include "TranspileTest.h"

TEST_F(TranspileTest, MathOperatorPlusIntLiterals)
{
	std::string input = R"(
		var x := 1 + 2
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint x = (1 + 2);\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MathOperatorPlusIntVariables)
{
	std::string input = R"(
		var y := 1
		var z := 2
		var x := y + z
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_y()\n\t\t{\n\t\t\treturn y;\n\t\t}\n\n\t\tvoid set_y(int newy)\n\t\t{\n\t\t\ty = newy;\n\t\t}\n\n\t\tint get_z()\n\t\t{\n\t\t\treturn z;\n\t\t}\n\n\t\tvoid set_z(int newz)\n\t\t{\n\t\t\tz = newz;\n\t\t}\n\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint y = 1;\n\t\tint z = 2;\n\t\tint x = (y + z);\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y\"), &Test::get_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y\", \"newy\"), &Test::set_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_z\"), &Test::get_z);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_z\", \"newz\"), &Test::set_z);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"y\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_y\", \"get_y\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"z\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_z\", \"get_z\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MathOperatorPlusIntLiteralAndVariable)
{
	std::string input = R"(
		var y := 1
		var x := y + 2
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_y()\n\t\t{\n\t\t\treturn y;\n\t\t}\n\n\t\tvoid set_y(int newy)\n\t\t{\n\t\t\ty = newy;\n\t\t}\n\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint y = 1;\n\t\tint x = (y + 2);\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y\"), &Test::get_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y\", \"newy\"), &Test::set_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"y\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_y\", \"get_y\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MathOperatorMinusIntLiterals)
{
	std::string input = R"(
		var x := 1 - 2
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint x = (1 - 2);\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MathOperatorMultiplyIntLiterals)
{
	std::string input = R"(
		var x := 1 * 2
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint x = (1 * 2);\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MathOperatorDivideIntLiterals)
{
	std::string input = R"(
		var x := 1 / 2
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint x = (1 / 2);\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MathOperatorMinusPrefixWithBrackets)
{
	std::string input = R"(
		var y := 10
		var x := -(y + 1)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_y()\n\t\t{\n\t\t\treturn y;\n\t\t}\n\n\t\tvoid set_y(int newy)\n\t\t{\n\t\t\ty = newy;\n\t\t}\n\n\t\tauto get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(Variant newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint y = 10;\n\t\tauto x = -(y + 1);\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y\"), &Test::get_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y\", \"newy\"), &Test::set_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"y\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_y\", \"get_y\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::OBJECT, \"x\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}