#include "TranspileTest.h"

TEST_F(TranspileTest, FunctionCall)
{
	std::string input = R"(
		func doStuff() -> void:
			pass

		func otherStuff() -> void:
			doStuff()
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t}\n\n\t\tvoid otherStuff()\n\t\t{\n\t\t\tdoStuff();\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t\tClassDB::bind_method(D_METHOD(\"otherStuff\"), &Test::otherStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionCallWithArg)
{
	std::string input = R"(
		func doStuff(x: int) -> void:
			pass

		func otherStuff() -> void:
			doStuff(10)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(int x)\n\t\t{\n\t\t}\n\n\t\tvoid otherStuff()\n\t\t{\n\t\t\tdoStuff(10);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"x\"), &Test::doStuff);\n\t\t\tClassDB::bind_method(D_METHOD(\"otherStuff\"), &Test::otherStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionCallWithArgNull)
{
	std::string input = R"(
		func doStuff(x: int) -> void:
			pass

		func otherStuff() -> void:
			doStuff(null)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(int x)\n\t\t{\n\t\t}\n\n\t\tvoid otherStuff()\n\t\t{\n\t\t\tdoStuff(nullptr);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"x\"), &Test::doStuff);\n\t\t\tClassDB::bind_method(D_METHOD(\"otherStuff\"), &Test::otherStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionCallWithArgsNewLines)
{
	std::string input = R"(
		func doStuff(x: int, y: int, z: int) -> void:
			pass

		func otherStuff() -> void:
			doStuff(
				1,
				2,
				3
			)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(int x, int y, int z)\n\t\t{\n\t\t}\n\n\t\tvoid otherStuff()\n\t\t{\n\t\t\tdoStuff(1, 2, 3);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"x\", \"y\", \"z\"), &Test::doStuff);\n\t\t\tClassDB::bind_method(D_METHOD(\"otherStuff\"), &Test::otherStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionCallWithArgsNewLinesTrailingComma)
{
	std::string input = R"(
		func doStuff(x: int, y: int, z: int) -> void:
			pass

		func otherStuff() -> void:
			doStuff(
				1,
				2,
				3,
			)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(int x, int y, int z)\n\t\t{\n\t\t}\n\n\t\tvoid otherStuff()\n\t\t{\n\t\t\tdoStuff(1, 2, 3);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"x\", \"y\", \"z\"), &Test::doStuff);\n\t\t\tClassDB::bind_method(D_METHOD(\"otherStuff\"), &Test::otherStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionCallGeneratedCPPInit)
{
	std::string input = R"(
		func _ready() -> void:
			TerrainGenerator.new().init()
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid _ready()\n\t\t{\n\t\t\tmemnew(TerrainGenerator())->init();\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"_ready\"), &Test::_ready);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionCallThreaded)
{
	std::string input = R"(
		func _ready() -> void:
			var thread := Thread.new()
			thread.start(_callMe)
		func _callMe() -> void:
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/classes/thread.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid _ready()\n\t\t{\n\t\t\tRef<Thread> thread = memnew(Thread());\n\t\t\tthread->start(_callMe);\n\t\t}\n\n\tprivate:\n\n\t\tvoid _callMe()\n\t\t{\n\t\t}\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"_ready\"), &Test::_ready);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionCallThreadedBind)
{
	std::string input = R"(
		func _ready() -> void:
			var thread := Thread.new()
			thread.start(_callMe.bind("batman"))
		func _callMe(name: String) -> void:
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/classes/thread.hpp>\n#include <godot_cpp/variant/string.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid _ready()\n\t\t{\n\t\t\tRef<Thread> thread = memnew(Thread());\n\t\t\tthread->start(_callMe.bind(\"batman\"));\n\t\t}\n\n\tprivate:\n\n\t\tvoid _callMe(String name)\n\t\t{\n\t\t}\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"_ready\"), &Test::_ready);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionCallCallDeferred)
{
	std::string input = R"(
		func _ready() -> void:
			_callMe.call_deferred("batman")
		func _callMe(name: String) -> void:
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/string.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid _ready()\n\t\t{\n\t\t\t_callMe.call_deferred(\"batman\");\n\t\t}\n\n\tprivate:\n\n\t\tvoid _callMe(String name)\n\t\t{\n\t\t}\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"_ready\"), &Test::_ready);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}