#include "TranspileTest.h"

TEST_F(TranspileTest, LocalVariableVarInt)
{
	std::string input = R"(
		func doStuff():
			var x := 1
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = 1;\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, LocalVariableVarIntNegative)
{
	std::string input = R"(
		func doStuff():
			var x := -1
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tauto x = -1;\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, LocalVariableVarFloat)
{
	std::string input = R"(
		func doStuff():
			var x := 2.0
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tfloat x = 2.0f;\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, LocalVariableVarFloatNegative)
{
	std::string input = R"(
		func doStuff():
			var x := -2.0
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tauto x = -2.0f;\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, LocalVariableVarStaticFunctionCall)
{
	std::string input = R"(
		class Hello:
			static func getStuff() -> int:
				return 1
		func doStuff():
			var x := Hello.getStuff()
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Hello : public RefCounted\n\t{\n\t\tGDCLASS(Hello, RefCounted)\n\tpublic:\n\t\tstatic int getStuff()\n\t\t{\n\t\t\treturn 1;\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_static_method(\"Hello\", D_METHOD(\"getStuff\"), &Hello::getStuff);\n\t\t}\n\t};\n\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = Hello::getStuff();\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarDictionaryEmpty)
{
	std::string input = R"(
		var x: Dictionary = {
		}
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_dictionary.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tTypedDictionary get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(TypedDictionary newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tTypedDictionary x = {};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarDictionaryWithSubtypesEmpty)
{
	std::string input = R"(
		var x: Dictionary[int, string] = {
		}
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include \"string.h\"\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_dictionary.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tTypedDictionary<int, string> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(TypedDictionary<int, string> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tTypedDictionary<int, string> x = {};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarDictionaryIntToInt)
{
	std::string input = R"(
		var x: Dictionary = {
			1: 2
		}
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_dictionary.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tTypedDictionary<int, int> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(TypedDictionary<int, int> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tTypedDictionary<int, int> x = {\n\t\t\t{1,2},\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableConstInt)
{
	std::string input = R"(
		const x := 1
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tconst int x = 1;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarVector3)
{
	std::string input = R"(
		var x := Vector3(1, 2, 3)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/vector3.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tVector3 get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(Vector3 newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tVector3 x = Vector3(1, 2, 3);\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarVector3DividedByInt)
{
	std::string input = R"(
		var x := Vector3(1, 2, 3) / 10
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/vector3.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tVector3 get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(Vector3 newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tVector3 x = (Vector3(1, 2, 3) / 10);\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarVector3DividedByVar)
{
	std::string input = R"(
		var y := 10
		var x := Vector3(1, 2, 3) / y
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/vector3.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint get_y()\n\t\t{\n\t\t\treturn y;\n\t\t}\n\n\t\tvoid set_y(int newy)\n\t\t{\n\t\t\ty = newy;\n\t\t}\n\n\t\tVector3 get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(Vector3 newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint y = 10;\n\t\tVector3 x = (Vector3(1, 2, 3) / y);\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y\"), &Test::get_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y\", \"newy\"), &Test::set_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarVector3PlusVector3XYZ)
{
	std::string input = R"(
		var x := Vector3(1, 2, 3)
		var y := Vector3(x.x + 1, x.y + 2, x.z + 3)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/vector3.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tVector3 get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(Vector3 newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tVector3 get_y()\n\t\t{\n\t\t\treturn y;\n\t\t}\n\n\t\tvoid set_y(Vector3 newy)\n\t\t{\n\t\t\ty = newy;\n\t\t}\n\n\t\tVector3 x = Vector3(1, 2, 3);\n\t\tVector3 y = Vector3((x.x + 1), (x.y + 2), (x.z + 3));\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y\"), &Test::get_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y\", \"newy\"), &Test::set_y);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarVectori3PlusVectori3XYZ)
{
	std::string input = R"(
		var x := Vector3i(1, 2, 3)
		var y := Vector3i(x.x + 1, x.y + 2, x.z + 3)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/vector3i.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tVector3i get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(Vector3i newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tVector3i get_y()\n\t\t{\n\t\t\treturn y;\n\t\t}\n\n\t\tvoid set_y(Vector3i newy)\n\t\t{\n\t\t\ty = newy;\n\t\t}\n\n\t\tVector3i x = Vector3i(1, 2, 3);\n\t\tVector3i y = Vector3i((x.x + 1), (x.y + 2), (x.z + 3));\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y\"), &Test::get_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y\", \"newy\"), &Test::set_y);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarVector2PlusVector3XY)
{
	std::string input = R"(
		var x := Vector2(1, 2)
		var y := Vector3(x.x + 1, 0, x.y + 3)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/vector2.hpp>\n#include <godot_cpp/variant/vector3.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tVector2 get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(Vector2 newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tVector3 get_y()\n\t\t{\n\t\t\treturn y;\n\t\t}\n\n\t\tvoid set_y(Vector3 newy)\n\t\t{\n\t\t\ty = newy;\n\t\t}\n\n\t\tVector2 x = Vector2(1, 2);\n\t\tVector3 y = Vector3((x.x + 1), 0, (x.y + 3));\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y\"), &Test::get_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y\", \"newy\"), &Test::set_y);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarVectori2PlusVectori3XY)
{
	std::string input = R"(
		var x := Vector2i(1, 2)
		var y := Vector3i(x.x + 1, 0, x.y + 3)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/vector2i.hpp>\n#include <godot_cpp/variant/vector3i.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tVector2i get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(Vector2i newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tVector3i get_y()\n\t\t{\n\t\t\treturn y;\n\t\t}\n\n\t\tvoid set_y(Vector3i newy)\n\t\t{\n\t\t\ty = newy;\n\t\t}\n\n\t\tVector2i x = Vector2i(1, 2);\n\t\tVector3i y = Vector3i((x.x + 1), 0, (x.y + 3));\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y\"), &Test::get_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y\", \"newy\"), &Test::set_y);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarArrayEmpty)
{
	std::string input = R"(
		var x: Array = [
		]
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tTypedArray get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(TypedArray newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tTypedArray x = {};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarArrayWithElements)
{
	std::string input = R"(
		var x: Array = [
			1, 3, 4
		]
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tTypedArray<int> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(TypedArray<int> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tTypedArray<int> x = {\n\t\t\t1,3,4,\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarArrayWithSubarraysElements)
{
	std::string input = R"(
		var x: Array = [
			[ 1, 3, 4 ]
		]
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tTypedArray<TypedArray<int>> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(TypedArray<TypedArray<int>> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tTypedArray<TypedArray<int>> x = {{\n\t\t\t\t1,3,4,\n\t\t\t},\n};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarArrayWithSubarraysElementsEmptyFirst)
{
	std::string input = R"(
		var x: Array = [
			[],
			[ 1, 3, 4 ]
		]
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tTypedArray<TypedArray<int>> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(TypedArray<TypedArray<int>> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tTypedArray<TypedArray<int>> x = {{},{\n\t\t\t\t1,3,4,\n\t\t\t},\n};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarArrayWithSubarraysElementsEmptyFirstAndLast)
{
	std::string input = R"(
		var x: Array = [
			[],
			[ 1, 3, 4 ],
			[]
		]
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tTypedArray<TypedArray<int>> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(TypedArray<TypedArray<int>> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tTypedArray<TypedArray<int>> x = {\n\t\t\t{},{\n\t\t\t\t1,3,4,\n\t\t\t},{},\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, LocalVariableVarClassInstance)
{
	std::string input = R"(
		class MyClass:

		func doStuff():
			var x := MyClass.new()
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass MyClass : public RefCounted\n\t{\n\t\tGDCLASS(MyClass, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tRef<MyClass> x = memnew(MyClass());\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, LocalVariableVarClassInstanceUnderscore)
{
	std::string input = R"(
		class _MyClass:

		func doStuff():
			var x := _MyClass.new()
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass MyClass : public RefCounted\n\t{\n\t\tGDCLASS(MyClass, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tRef<MyClass> x = memnew(MyClass());\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableColour)
{
	std::string input = R"(
		var colour := Color.BLACK
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/color.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tColor get_colour()\n\t\t{\n\t\t\treturn colour;\n\t\t}\n\n\t\tvoid set_colour(Color newcolour)\n\t\t{\n\t\t\tcolour = newcolour;\n\t\t}\n\n\t\tColor colour = Color::named(\"BLACK\");\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_colour\"), &Test::get_colour);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_colour\", \"newcolour\"), &Test::set_colour);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, LocalVariableCalledRange)
{
	std::string input = R"(
		static func _getValue(upper: float, lower: float) -> float:
			var range : = upper - lower
			var halfRange : = range / 2.0
			return halfRange
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\tprivate:\n\n\t\tstatic float _getValue(float upper, float lower)\n\t\t{\n\t\t\tfloat range = (upper - lower);\n\t\t\tfloat halfRange = (range / 2.0f);\n\t\t\treturn halfRange;\n\t\t}\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, Node3DMemberVariableGetter)
{
	std::string input = R"(
		extends Node3D
		func doStuff() -> void:
			var raycast := $RayCast3D as RayCast3D
			var pos := raycast.global_position
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/node3d.hpp>\n#include <godot_cpp/classes/ray_cast3d.hpp>\n\nnamespace godot\n{\n\tclass Test : public Node3D\n\t{\n\t\tGDCLASS(Test, Node3D)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tRayCast3D* raycast = (RayCast3D*)find_child(\"RayCast3D\");\n\t\t\tauto pos = raycast->get_global_position();\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, Node3DMemberVariableSetter)
{
	std::string input = R"(
		extends Node3D
		func doStuff() -> void:
			var raycast := $RayCast3D as RayCast3D
			raycast.global_position = Vector3(0, 0, 0)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/node3d.hpp>\n#include <godot_cpp/classes/ray_cast3d.hpp>\n\nnamespace godot\n{\n\tclass Test : public Node3D\n\t{\n\t\tGDCLASS(Test, Node3D)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tRayCast3D* raycast = (RayCast3D*)find_child(\"RayCast3D\");\n\t\t\traycast->set_global_position(Vector3(0, 0, 0));\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, CastVariableMismatchObjectType)
{
	std::string input = R"(
		var resource := load("")
		var node: Node3D = resource.instantiate()
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/node3d.hpp>\n#include <godot_cpp/classes/packed_scene.hpp>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tRef<PackedScene> get_resource()\n\t\t{\n\t\t\treturn resource;\n\t\t}\n\n\t\tvoid set_resource(Ref<PackedScene> newresource)\n\t\t{\n\t\t\tresource = newresource;\n\t\t}\n\n\t\tNode3D* get_node()\n\t\t{\n\t\t\treturn node;\n\t\t}\n\n\t\tvoid set_node(Node3D* newnode)\n\t\t{\n\t\t\tnode = newnode;\n\t\t}\n\n\t\tRef<PackedScene> resource = ResourceLoader::get_singleton()->load(\"\");\n\t\tNode3D* node = (Node3D*)resource->instantiate();\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_resource\"), &Test::get_resource);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_resource\", \"newresource\"), &Test::set_resource);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_node\"), &Test::get_node);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_node\", \"newnode\"), &Test::set_node);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarGetter)
{
	std::string input = R"(
		var x: Vector3 : get = _getX
		func _getX() -> Vector3:
			return x
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/vector3.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tVector3 x;\n\tprivate:\n\n\t\tVector3 _getX()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tVector3 _getX()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarSetter)
{
	std::string input = R"(
		var x: Vector3 : set = _setX
		func _setX(newX: Vector3) -> void:
			x = newX
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/vector3.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tVector3 x;\n\tprivate:\n\n\t\tvoid _setX(Vector3 newX)\n\t\t{\n\t\t\tx = newX;\n\t\t}\n\n\t\tvoid _setX(Vector3 newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarGetterSetter)
{
	std::string input = R"(
		var x: Vector3 : get = _getX, set = _setX
		func _getX() -> Vector3:
			return x
		func _setX(newX: Vector3) -> void:
			x = newX
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/vector3.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tVector3 x;\n\tprivate:\n\n\t\tVector3 _getX()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid _setX(Vector3 newX)\n\t\t{\n\t\t\tx = newX;\n\t\t}\n\n\t\tVector3 _getX()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid _setX(Vector3 newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarSetterGetter)
{
	std::string input = R"(
		var x: Vector3 : set = _setX, get = _getX
		func _setX(newX: Vector3) -> void:
			x = newX
		func _getX() -> Vector3:
			return x
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/vector3.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tVector3 x;\n\tprivate:\n\n\t\tvoid _setX(Vector3 newX)\n\t\t{\n\t\t\tx = newX;\n\t\t}\n\n\t\tVector3 _getX()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tVector3 _getX()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid _setX(Vector3 newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}