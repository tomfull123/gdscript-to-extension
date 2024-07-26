#include "TranspileTest.h"

TEST_F(TranspileTest, ForInLoop)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := [0, 2, 4]
			for y in x:
				print("")
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/utility_functions.hpp>\n#include <vector>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tstd::vector<int> x = {\n\t\t\t\t0,2,4,\n\t\t\t};\n\t\t\tfor (auto& y : x)\n\t\t\t{\n\t\t\t\tUtilityFunctions::print(\"\");\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ForInLoopVariableType)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := [0, 2, 4]
			for y: int in x:
				print("")
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/utility_functions.hpp>\n#include <vector>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tstd::vector<int> x = {\n\t\t\t\t0,2,4,\n\t\t\t};\n\t\t\tfor (auto& y : x)\n\t\t\t{\n\t\t\t\tUtilityFunctions::print(\"\");\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ForInLoopVariableDefinition)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := [0, 2, 4]
			for y in x:
				var z := y - 1
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <vector>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tstd::vector<int> x = {\n\t\t\t\t0,2,4,\n\t\t\t};\n\t\t\tfor (auto& y : x)\n\t\t\t{\n\t\t\t\tint z = (y - 1);\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ForInLoopIntVariable)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := 10
			for y in x:
				print("")
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/utility_functions.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = 10;\n\t\t\tfor (int y = 0; y < x; y++)\n\t\t\t{\n\t\t\t\tUtilityFunctions::print(\"\");\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ForInLoopIntLiteral)
{
	std::string input = R"(
		func doStuff() -> void:
			for y in 10:
				print("")
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/utility_functions.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tfor (int y = 0; y < 10; y++)\n\t\t\t{\n\t\t\t\tUtilityFunctions::print(\"\");\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ForInLoopIntPlusLiteral)
{
	std::string input = R"(
		func doStuff() -> void:
			var x: int = 10
			for y in (x + 1):
				print("")
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/utility_functions.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = 10;\n\t\t\tfor (int y = 0; y < (x + 1); y++)\n\t\t\t{\n\t\t\t\tUtilityFunctions::print(\"\");\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ForInLoopIntInferredPlusLiteral)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := 10
			for y in (x + 1):
				print("")
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/utility_functions.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = 10;\n\t\t\tfor (int y = 0; y < (x + 1); y++)\n\t\t\t{\n\t\t\t\tUtilityFunctions::print(\"\");\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ForInLoopRangeLiteral)
{
	std::string input = R"(
		func doStuff() -> void:
			for y in range(1, 10):
				pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tfor (int y = 1; y < 10; y++)\n\t\t\t{\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ForInLoopRangeVar)
{
	std::string input = R"(
		func doStuff() -> void:
			var min := 0
			var max := 11
			for y in range(min, max):
				pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint min = 0;\n\t\t\tint max = 11;\n\t\t\tfor (int y = min; y < max; y++)\n\t\t\t{\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ForInLoopRangeVarIncrementInt)
{
	std::string input = R"(
		func doStuff() -> void:
			var min := 0
			var max := 11
			for y in range(min, max, 2):
				pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint min = 0;\n\t\t\tint max = 11;\n\t\t\tfor (int y = min; y < max; y += 2)\n\t\t\t{\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ForInLoopRangeVarDecrementInt)
{
	std::string input = R"(
		func doStuff() -> void:
			var min := 0
			var max := 11
			for y in range(min, max, -1):
				pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint min = 0;\n\t\t\tint max = 11;\n\t\t\tfor (int y = min; y < max; y += -1)\n\t\t\t{\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}