#include "TranspileTest.h"

TEST_F(TranspileTest, TypeDef)
{
	std::string input = R"(
		const NewType := ClassName.NewType
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include \"ClassName.h\"\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, TypeDefFunctionReturnType)
{
	std::string input = R"(
		const NewType := ClassName.NewType

		func x() -> NewType:
			return null
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include \"ClassName.h\"\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tNewType x()\n\t\t{\n\t\t\treturn nullptr;\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"x\"), &Test::x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}