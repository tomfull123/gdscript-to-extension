#include "TranspileTest.h"

TEST_F(TranspileTest, ClassName)
{
	std::string input = R"(
		class_name Hello
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\nnamespace godot\n{\n\tclass Hello\n\t{\n\tpublic:\n\tprivate:\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\t\n\t\t}\n\t}\n}\n";
	EXPECT_EQ(expected, actual);
}