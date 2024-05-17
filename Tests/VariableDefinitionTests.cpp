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
		func doStuff():
			var x := Hello.getStuff()
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include \"Hello.h\"\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tauto x = Hello::getStuff();\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarDictionaryEmpty)
{
	std::string input = R"(
		var x: Dictionary = {
		}
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <map>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tstd::map x = {\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <map>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tstd::map<int, int> x = {\n\t\t\t{1,2},\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include <godot_cpp/variant/vector3.hpp>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tVector3 x = Vector3(1, 2, 3);\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarVector3DividedByInt)
{
	std::string input = R"(
		var x := Vector3(1, 2, 3) / 10
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/variant/vector3.hpp>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tVector3 x = (Vector3(1, 2, 3) / 10);\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarVector3DividedByVar)
{
	std::string input = R"(
		var y := 10
		var x := Vector3(1, 2, 3) / y
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/variant/vector3.hpp>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint y = 10;\n\t\tVector3 x = (Vector3(1, 2, 3) / y);\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarVector3PlusVector3XYZ)
{
	std::string input = R"(
		var x := Vector3(1, 2, 3)
		var y := Vector3(x.x + 1, x.y + 2, x.z + 3)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/variant/vector3.hpp>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tVector3 x = Vector3(1, 2, 3);\n\t\tVector3 y = Vector3((x.x + 1), (x.y + 2), (x.z + 3));\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MemberVariableVarArrayEmpty)
{
	std::string input = R"(
		var x: Array = [
		]
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <vector>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tstd::vector x = {};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include <vector>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tstd::vector<int> x = {\n\t\t\t1,3,4,\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include <vector>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tstd::vector<std::vector<int>> x = {\t\t\t{\n\t\t\t\t1,3,4,\n\t\t\t},\n};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include <vector>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tstd::vector<std::vector<int>> x = {\t\t\t{},{\n\t\t\t\t1,3,4,\n\t\t\t},\n};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}