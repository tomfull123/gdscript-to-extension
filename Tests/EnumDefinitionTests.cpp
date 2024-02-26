#include "TranspileTest.h"

TEST_F(TranspileTest, EnumDefinitionEmpty)
{
	std::string input = R"(
		enum EnumType {
		}
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tenum EnumType\n\t{\n\t};\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\nVARIANT_ENUM_CAST(EnumType);\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, EnumDefinitionWithValues)
{
	std::string input = R"(
		enum EnumType {
			FirstValue,
			SecondValue,
			ThirdValue
		}
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tenum EnumType\n\t{\n\t\tFirstValue,\n\t\tSecondValue,\n\t\tThirdValue,\n\t};\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\nVARIANT_ENUM_CAST(EnumType);\n";
	EXPECT_EQ(expected, actual);
}