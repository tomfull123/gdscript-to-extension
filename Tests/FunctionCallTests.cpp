#include "TranspileTest.h"

TEST_F(TranspileTest, FunctionCall)
{
	std::string input = R"(
		func doStuff() -> void:
			pass

		func otherStuff() -> void:
			doStuff()
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t}\n\n\t\tvoid otherStuff()\n\t\t{\n\t\t\tdoStuff();\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t\tClassDB::bind_method(D_METHOD(\"otherStuff\"), &Test::otherStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionCallWithArg)
{
	std::string input = R"(
		func doStuff(x: int) -> void:
			pass

		func otherStuff() -> void:
			doStuff(10)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(int x)\n\t\t{\n\t\t}\n\n\t\tvoid otherStuff()\n\t\t{\n\t\t\tdoStuff(10);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"x\"), &Test::doStuff);\n\t\t\tClassDB::bind_method(D_METHOD(\"otherStuff\"), &Test::otherStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionCallWithArgNull)
{
	std::string input = R"(
		func doStuff(x: int) -> void:
			pass

		func otherStuff() -> void:
			doStuff(null)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(int x)\n\t\t{\n\t\t}\n\n\t\tvoid otherStuff()\n\t\t{\n\t\t\tdoStuff(null);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"x\"), &Test::doStuff);\n\t\t\tClassDB::bind_method(D_METHOD(\"otherStuff\"), &Test::otherStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}