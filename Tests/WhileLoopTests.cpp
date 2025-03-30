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
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = 0;\n\t\t\twhile ((x < 10))\n\t\t\t{\n\t\t\t\tx = (x + 1);\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\t\tTest() = default;\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, WhileLoopArray)
{
	std::string input = R"(
		func doStuff() -> void:
			var indicies: Array = [1, 2, 3, 5]
			var i := 0
			while indicies.size() > i:
				pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tTypedArray<int> indicies = {\n\t\t\t\t1,2,3,5,\n\t\t\t};\n\t\t\tint i = 0;\n\t\t\twhile ((indicies.size() > i))\n\t\t\t{\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\t\tTest() = default;\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, WhileBreak)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := 0
			while x < 10:
				x = x + 1
				break
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = 0;\n\t\t\twhile ((x < 10))\n\t\t\t{\n\t\t\t\tx = (x + 1);\n\t\t\t\tbreak;\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\t\tTest() = default;\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, WhileContinue)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := 0
			while x < 10:
				x = x + 1
				continue
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = 0;\n\t\t\twhile ((x < 10))\n\t\t\t{\n\t\t\t\tx = (x + 1);\n\t\t\t\tcontinue;\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\t\tTest() = default;\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}