#include "TranspileTest.h"

TEST_F(TranspileTest, ReturnTernaryTrueIntLiteral)
{
	std::string input = R"(
		func doStuff() -> int:
			return 1 if true else 2
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint doStuff()\n\t\t{\n\t\t\treturn true ? 1 : 2;\n\t\t}\n\n\tprivate:\n\n\t\tTest() = default;\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, ReturnTernaryTrueIntVariable)
{
	std::string input = R"(
		func doStuff(x: int, y: int) -> int:
			return x if true else y
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint doStuff(int x, int y)\n\t\t{\n\t\t\treturn true ? x : y;\n\t\t}\n\n\tprivate:\n\n\t\tTest() = default;\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"x\", \"y\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}