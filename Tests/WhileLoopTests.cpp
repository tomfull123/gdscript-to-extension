#include "TranspileTest.h"

TEST_F(TranspileTest, WhileLoop)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := 0
			while x < 10:
				x = x + 1
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = 0;\n\t\t\twhile ((x < 10))\n\t\t\t{\n\t\t\t\tx = (x + 1);\n\t\t\t}\n\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}