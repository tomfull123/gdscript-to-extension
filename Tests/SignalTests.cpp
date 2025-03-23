#include "TranspileTest.h"

TEST_F(TranspileTest, Signal)
{
	std::string input = R"(
		signal value_changed
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tSignal get_value_changed()\n\t\t{\n\t\t\treturn value_changed;\n\t\t}\n\n\t\tvoid set_value_changed(Signal newvalue_changed)\n\t\t{\n\t\t\tvalue_changed = newvalue_changed;\n\t\t}\n\n\t\tSignal value_changed;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_value_changed\"), &Test::get_value_changed);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_value_changed\", \"newvalue_changed\"), &Test::set_value_changed);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, SignalBrackets)
{
	std::string input = R"(
		signal value_changed()
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tSignal get_value_changed()\n\t\t{\n\t\t\treturn value_changed;\n\t\t}\n\n\t\tvoid set_value_changed(Signal newvalue_changed)\n\t\t{\n\t\t\tvalue_changed = newvalue_changed;\n\t\t}\n\n\t\tSignal value_changed;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_value_changed\"), &Test::get_value_changed);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_value_changed\", \"newvalue_changed\"), &Test::set_value_changed);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, SignalIntArg)
{
	std::string input = R"(
		signal value_changed(newValue: int)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tSignal get_value_changed()\n\t\t{\n\t\t\treturn value_changed;\n\t\t}\n\n\t\tvoid set_value_changed(Signal newvalue_changed)\n\t\t{\n\t\t\tvalue_changed = newvalue_changed;\n\t\t}\n\n\t\tSignal value_changed;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_value_changed\"), &Test::get_value_changed);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_value_changed\", \"newvalue_changed\"), &Test::set_value_changed);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}