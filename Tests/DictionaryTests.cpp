#include "TranspileTest.h"

TEST_F(TranspileTest, IndexDictionary)
{
	std::string input = R"(
		var x := {1: 2}

		func doStuff():
			var y = x[0]
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <map>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tauto y = x[0];\n\t\t}\n\n\t\tstd::map<int, int> x = {\n\t\t\t{1,2},\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, IndexDictionaryObjectValue)
{
	std::string input = R"(
		var x := {1: Stuff.new()}

		func doStuff():
			var y = x[0].getStuff()
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <map>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tauto y = x[0]->getStuff();\n\t\t}\n\n\t\tstd::map<int, Stuff> x = {\n\t\t\t{1,Stuff()},\n\t\t};\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}