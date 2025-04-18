#include "TranspileTest.h"

TEST_F(TranspileTest, IndexDictionary)
{
	std::string input = R"(
		var x := {1: 2}

		func doStuff():
			var y = x[0]
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <map>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint y = x[0];\n\t\t}\n\n\t\tstd::map<int, int> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(std::map<int, int> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tstd::map<int, int> x = {\n\t\t\t{1,2},\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, DictionaryEmpty)
{
	std::string input = R"(
		var x := {}
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <map>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tstd::map get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(std::map newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tstd::map x = {};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, DictionaryWithSubdictionaryEmpty)
{
	std::string input = R"(
		var x := {1: {}}
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <map>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tstd::map<int, std::map> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(std::map<int, std::map> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tstd::map<int, std::map> x = {\n\t\t\t{1,{}},\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, DictionaryWithSubdictionaryEmptyFirst)
{
	std::string input = R"(
		var x := {1: {}, 2: {3: 4}}
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <map>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tstd::map<int, std::map<int, int>> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(std::map<int, std::map<int, int>> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tstd::map<int, std::map<int, int>> x = {\n\t\t\t{1,{}},\n\t\t\t{2,{\n\t{3,4},\n}},\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, DictionaryWithSubdictionaryEmptyFirstAndLast)
{
	std::string input = R"(
		var x := {1: {}, 2: {3: 4}, 9: {}}
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <map>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tstd::map<int, std::map<int, int>> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(std::map<int, std::map<int, int>> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tstd::map<int, std::map<int, int>> x = {\n\t\t\t{1,{}},\n\t\t\t{2,{\n\t{3,4},\n}},\n\t\t\t{9,{}},\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, IndexDictionaryObjectValue)
{
	std::string input = R"(
		class Stuff:
			func getStuff() -> int:
				return 123

		var x := {1: Stuff.new()}

		func doStuff():
			var y = x[0].getStuff()
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Stuff : public RefCounted\n\t{\n\t\tGDCLASS(Stuff, RefCounted)\n\tpublic:\n\t\tint getStuff()\n\t\t{\n\t\t\treturn 123;\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"getStuff\"), &Stuff::getStuff);\n\t\t}\n\t};\n\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint y = x[0]->getStuff();\n\t\t}\n\n\t\tstd::map<int, Ref<Stuff>> get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(std::map<int, Ref<Stuff>> newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tstd::map<int, Ref<Stuff>> x = {\n\t\t\t{1,memnew(Stuff())},\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}