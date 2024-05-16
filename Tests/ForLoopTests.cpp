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
	std::string expected = "#pragma once\n\n#include <vector>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tstd::vector<int> x = {\n\t\t\t\t0,2,4,\n\t\t\t};\n\t\t\tfor (auto y : x)\n\t\t\t{\n\t\t\t\tprint();\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = 10;\n\t\t\tfor (int y = 0; y < x; y++)\n\t\t\t{\n\t\t\t\tprint();\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tfor (int y = 0; y < 10; y++)\n\t\t\t{\n\t\t\t\tprint();\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = 10;\n\t\t\tfor (int y = 0; y < (x + 1); y++)\n\t\t\t{\n\t\t\t\tprint();\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = 10;\n\t\t\tfor (int y = 0; y < (x + 1); y++)\n\t\t\t{\n\t\t\t\tprint();\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}