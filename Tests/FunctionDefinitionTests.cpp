#include "TranspileTest.h"

TEST_F(TranspileTest, PublicFunctionDefinition)
{
	std::string input = R"(
		func doStuff() -> void:
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, PublicFunctionDefinitionWithArgs)
{
	std::string input = R"(
		func doStuff(x: String, y: int) -> void:
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/string.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(String x, int y)\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"x\", \"y\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, PublicFunctionDefinitionWith5Args)
{
	std::string input = R"(
		func doStuff(x: String, y: int, z: int, a: String, b: bool) -> void:
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/string.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(\n\t\t\tString x,\n\t\t\tint y,\n\t\t\tint z,\n\t\t\tString a,\n\t\t\tbool b\n\t\t)\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"x\", \"y\", \"z\", \"a\", \"b\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, PublicFunctionDefinitionInferredReturnType)
{
	std::string input = R"(
		func doStuff():
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, PrivateFunctionDefinition)
{
	std::string input = R"(
		func _doStuff() -> void:
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\tprivate:\n\n\t\tvoid _doStuff()\n\t\t{\n\t\t}\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, PrivateFunctionDefinitionWithArgs)
{
	std::string input = R"(
		func _doStuff(x: String, y: int) -> void:
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/string.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\tprivate:\n\n\t\tvoid _doStuff(String x, int y)\n\t\t{\n\t\t}\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, PrivateFunctionDefinitionArgsTypeInferred)
{
	std::string input = R"(
		func _doStuff(x) -> void:
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\tprivate:\n\n\t\tvoid _doStuff(Variant x)\n\t\t{\n\t\t}\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, PublicFunctionDefinitionStatic)
{
	std::string input = R"(
		static func doStuff() -> void:
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tstatic void doStuff()\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_static_method(\"Test\", D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionDefinitionReturnInt)
{
	std::string input = R"(
		func doStuff() -> int:
			return 1
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tint doStuff()\n\t\t{\n\t\t\treturn 1;\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionDefinitionReturnvoid)
{
	std::string input = R"(
		func doStuff() -> void:
			return
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\treturn;\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionDefinitionArgs)
{
	std::string input = R"(
		func doStuff(x: int, y: int, z: int) -> void:
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(int x, int y, int z)\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"x\", \"y\", \"z\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionDefinitionArgsNewLines)
{
	std::string input = R"(
		func doStuff(
			x: int,
			y: int,
			z: int
		) -> void:
			pass
		)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(int x, int y, int z)\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"x\", \"y\", \"z\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionDefinitionArgsNewLinesTrailingComma)
{
	std::string input = R"(
		func doStuff(
			x: int,
			y: int,
			z: int,
		) -> void:
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(int x, int y, int z)\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"x\", \"y\", \"z\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionDefinitionInit)
{
	std::string input = R"(
		var x: int
		func _init(x: int) -> void:
			self.x = x
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tRef<Test> init(int x)\n\t\t{\n\t\t\tthis->x = x;\n\t\t\treturn this;\n\t\t}\n\n\t\tint get_x()\n\t\t{\n\t\t\treturn x;\n\t\t}\n\n\t\tvoid set_x(int newx)\n\t\t{\n\t\t\tx = newx;\n\t\t}\n\n\t\tint x;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"init\", \"x\"), &Test::init);\n\t\t\tClassDB::bind_method(D_METHOD(\"get_x\"), &Test::get_x);\n\t\t\tClassDB::bind_method(D_METHOD(\"set_x\", \"newx\"), &Test::set_x);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, FunctionDefinitionInferVariableDefinitioniFromFunctionParameter)
{
	std::string input = R"(
			const CENTER_OFFSET := 0.5
			func doStuff(
				size: float,
				posOffset: Vector3,
				resolution: int,
			) -> void:
				for z in resolution+1:
					for y in resolution+1:
						for x in resolution+1:
							var percent := Vector3(x, y, z) / resolution

							var pointOnMesh := Vector3((percent.x-CENTER_OFFSET), (percent.y-CENTER_OFFSET), (percent.z-CENTER_OFFSET))

							var vertex := pointOnMesh * size

							var xPos := posOffset.x+vertex.x
							var yPos := posOffset.y+vertex.y
							var zPos := posOffset.z+vertex.z
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/vector3.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(float size, Vector3 posOffset, int resolution)\n\t\t{\n\t\t\tfor (int z = 0; z < (resolution + 1); z++)\n\t\t\t{\n\t\t\t\tfor (int y = 0; y < (resolution + 1); y++)\n\t\t\t\t{\n\t\t\t\t\tfor (int x = 0; x < (resolution + 1); x++)\n\t\t\t\t\t{\n\t\t\t\t\t\tVector3 percent = (Vector3(x, y, z) / resolution);\n\t\t\t\t\t\tVector3 pointOnMesh = Vector3((percent.x - CENTER_OFFSET), (percent.y - CENTER_OFFSET), (percent.z - CENTER_OFFSET));\n\t\t\t\t\t\tVector3 vertex = (pointOnMesh * size);\n\t\t\t\t\t\tauto xPos = (posOffset.x + vertex.x);\n\t\t\t\t\t\tauto yPos = (posOffset.y + vertex.y);\n\t\t\t\t\t\tauto zPos = (posOffset.z + vertex.z);\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\n\t\tconst float CENTER_OFFSET = 0.5f;\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"size\", \"posOffset\", \"resolution\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, PublicFunctionDefinitionWithClassTypeArg)
{
	std::string input = R"(
		class _MyClass:

		func doStuff(x: MyClass) -> void:
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass MyClass : public RefCounted\n\t{\n\t\tGDCLASS(MyClass, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(Ref<MyClass> x)\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"x\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, PublicFunctionDefinitionWithUnderscoredClassTypeArg)
{
	std::string input = R"(
		class _MyClass:

		func doStuff(x: _MyClass) -> void:
			pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass MyClass : public RefCounted\n\t{\n\t\tGDCLASS(MyClass, RefCounted)\n\tpublic:\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t}\n\t};\n\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(Ref<MyClass> x)\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"x\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}