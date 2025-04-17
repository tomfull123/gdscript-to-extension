#include "TranspileTest.h"

TEST_F(TranspileTest, MatchInt)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := 1
			match x:
				1:
					pass
				2:
					pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = 1;\n\t\t\tswitch (x)\n\t\t\t{\t\t\tcase1:\n\t\t\t{\n\t\t\t}\n\t\t\tbreak;\n\t\t\tcase2:\n\t\t\t{\n\t\t\t}\n\t\t\tbreak;\n\t\t\t};\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MatchIntReturn)
{
	std::string input = R"(
		func doStuff() -> int:
			var x := 1
			match x:
				1:
					return 2
				2:
					return 4
			return 0
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint doStuff()\n\t\t{\n\t\t\tint x = 1;\n\t\t\tswitch (x)\n\t\t\t{\t\t\tcase1:\n\t\t\t{\n\t\t\t\treturn 2;\n\t\t\t}\n\t\t\tcase2:\n\t\t\t{\n\t\t\t\treturn 4;\n\t\t\t}\n\t\t\t};\n\t\t\treturn 0;\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MatchIntMultiple)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := 1
			match x:
				1, 3, 5:
					pass
				2:
					pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = 1;\n\t\t\tswitch (x)\n\t\t\t{\t\t\tcase1:\n\t\t\tcase3:\n\t\t\tcase5:\n\t\t\t{\n\t\t\t}\n\t\t\tbreak;\n\t\t\tcase2:\n\t\t\t{\n\t\t\t}\n\t\t\tbreak;\n\t\t\t};\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MatchEnum)
{
	std::string input = R"(
		enum Type {
			Type1,
			Type2
		}
		func doStuff(type: Type) -> void:
			match type:
				Type.Type1:
					pass
				Type.Type2:
					pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tenum Type\n\t{\n\t\tType1,\n\t\tType2,\n\t};\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(Type type)\n\t\t{\n\t\t\tswitch (type)\n\t\t\t{\t\t\tcaseType::Type1:\n\t\t\t{\n\t\t\t}\n\t\t\tbreak;\n\t\t\tcaseType::Type2:\n\t\t\t{\n\t\t\t}\n\t\t\tbreak;\n\t\t\t};\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"type\"), &Test::doStuff);\n\t\t}\n\t};\n}\nVARIANT_ENUM_CAST(Type);\n";
	EXPECT_EQ(expected, actual);
}