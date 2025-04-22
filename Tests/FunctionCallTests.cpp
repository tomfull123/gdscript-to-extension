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
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid _ready()\n\t\t{\n\t\t\tRef(memnew(TerrainGenerator))->init();\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/classes/thread.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid _ready()\n\t\t{\n\t\t\tRef<Thread> thread = Ref(memnew(Thread));\n\t\t\tthread->start(callable_mp(this, &Test::_callMe));\n\t\t}\n\n\tprivate:\n\n\t\tvoid _callMe()\n\t\t{\n\t\t}\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/classes/thread.hpp>\n#include <godot_cpp/variant/string.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid _ready()\n\t\t{\n\t\t\tRef<Thread> thread = Ref(memnew(Thread));\n\t\t\tthread->start(callable_mp(this, &Test::_callMe).bind(\"batman\"));\n\t\t}\n\n\tprivate:\n\n\t\tvoid _callMe(String name)\n\t\t{\n\t\t}\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/string.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid _ready()\n\t\t{\n\t\t\t_callMe.call_deferred(\"batman\");\n\t\t}\n\n\tprivate:\n\n\t\tvoid _callMe(String name)\n\t\t{\n\t\t}\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionCallInit)
{
	std::string input = R"(
		class Abc:
			func _init() -> void:
				pass
		func x() -> void:
			Abc.new()
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Abc : public RefCounted\n\t{\n\t\tGDCLASS(Abc, RefCounted)\n\tpublic:\n\t\tRef<Abc> init()\n\t\t{\n\t\t\treturn this;\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"init\"), &Abc::init);\n\t\t}\n\t};\n\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid x()\n\t\t{\n\t\t\tRef(memnew(Abc));\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"x\"), &Test::x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionCallInitWithArgs)
{
	std::string input = R"(
		class Abc:
			var x: int
			var y: int
			func _init(x: int, y: int) -> void:
				self.x = x
				self.y = y
		func x() -> void:
			Abc.new(1, 2)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Abc : public RefCounted\n\t{\n\t\tGDCLASS(Abc, RefCounted)\n\tpublic:\n\t\tRef<Abc> init(int x, int y)\n\t\t{\n\t\t\tthis->x = x;\n\t\t\tthis->y = y;\n\t\t\treturn this;\n\t\t}\n\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint get_y()\n\t\t{\n\t\t\treturn y;\n\t\t}\n\n\t\tvoid set_y(int newy)\n\t\t{\n\t\t\ty = newy;\n\t\t}\n\n\t\tint x;\n\t\tint y;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"init\", \"x\", \"y\"), &Abc::init);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Abc::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Abc::set_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_y\"), &Abc::get_y);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_y\", \"newy\"), &Abc::set_y);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"x\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_x\", \"get_x\");\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::INT, \"y\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_y\", \"get_y\");\n\t\t}\n\t};\n\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid x()\n\t\t{\n\t\t\tRef(memnew(Abc))->init(1, 2);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"x\"), &Test::x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionCallStaticGodotMethod)
{
	std::string input = R"(
		var color := Color.from_rgba8(114.0, 162.0, 104.0)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/color.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tColor get_color()\n\t\t{\n\t\t\treturn color;\n\t\t}\n\n\t\tvoid set_color(Color newcolor)\n\t\t{\n\t\t\tcolor = newcolor;\n\t\t}\n\n\t\tColor color = Color::from_rgba8(114.0f, 162.0f, 104.0f);\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"get_color\"), &Test::get_color);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_color\", \"newcolor\"), &Test::set_color);\n\t\t\tADD_PROPERTY(PropertyInfo(Variant::COLOR, \"color\", PROPERTY_HINT_NONE, \"\", PROPERTY_USAGE_NONE), \"set_color\", \"get_color\");\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}