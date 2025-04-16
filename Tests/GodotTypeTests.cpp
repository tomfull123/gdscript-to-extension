#include "TranspileTest.h"

TEST_F(TranspileTest, GodotAbsi)
{
	std::string input = R"(
		func doStuff():
			var x := absi(-1)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/core/math.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tauto x = Math::abs(-1);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotAbsf)
{
	std::string input = R"(
		func doStuff():
			var x := absf(-1.0)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/core/math.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tauto x = Math::abs(-1.0f);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotAbsVector3)
{
	std::string input = R"(
		func doStuff():
			var x := abs(Vector3(-1, 2, -3))
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tauto x = (Vector3(-1, 2, -3)).abs();\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotMini)
{
	std::string input = R"(
		func doStuff():
			var x := mini(-1)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <algorithm>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = std::min(-1);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotMinf)
{
	std::string input = R"(
		func doStuff():
			var x := minf(-1.0)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tfloat x = std::fminf(-1.0f);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotColor)
{
	std::string input = R"(
		func doStuff() -> Color:
			return Color.BLACK
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/color.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tColor doStuff()\n\t\t{\n\t\t\treturn Color::named(\"BLACK\");\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotMeshPrimitiveTriangles)
{
	std::string input = R"(
		func doStuff() -> void:
			var surftool := SurfaceTool.new()
			surftool.begin(Mesh.PRIMITIVE_TRIANGLES)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/mesh.hpp>\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/classes/surface_tool.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tRef<SurfaceTool> surftool = memnew(SurfaceTool());\n\t\t\tsurftool->begin(Mesh::PRIMITIVE_TRIANGLES);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotRenderingServerSetWireframes)
{
	std::string input = R"(
		func doStuff() -> void:
			RenderingServer.set_debug_generate_wireframes(true)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/classes/rendering_server.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tRenderingServer::get_singleton()->set_debug_generate_wireframes(true);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotRoundi)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := roundi(10.12)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <cmath>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tint x = std::round(10.12f);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotRoundf)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := roundf(10.12)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <cmath>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tfloat x = std::roundf(10.12f);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotArray)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := [1, 2]
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tTypedArray<int> x = {1,2,\n};\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotArrayAppend)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := [1, 2]
			x.append(5)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tTypedArray<int> x = {1,2,\n};\n\t\t\tx.push_back(5);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotArrayIsEmpty)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := [1, 2]
			if x.is_empty():
				pass
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tTypedArray<int> x = {1,2,\n};\n\t\t\tif (x.is_empty())\n\t\t\t{\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotDictionary)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := {1: 2, 2: 4}
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <map>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tstd::map<int, int> x = {\n\t\t\t\t{1,2},\n\t\t\t\t{2,4},\n\t\t\t};\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotDictionaryHas)
{
	std::string input = R"(
		func doStuff() -> void:
			var x := {1: 2, 2: 4}
			x.has(5)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <map>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tstd::map<int, int> x = {\n\t\t\t\t{1,2},\n\t\t\t\t{2,4},\n\t\t\t};\n\t\t\tx.contains(5);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotPrint)
{
	std::string input = R"(
		func doStuff() -> void:
			print("test")
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/utility_functions.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tUtilityFunctions::print(\"test\");\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotLoad)
{
	std::string input = R"(
		func doStuff() -> void:
			var resource : = load("res://x/y/z.tscn")
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/packed_scene.hpp>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tRef<PackedScene> resource = ResourceLoader::get_singleton()->load(\"res://x/y/z.tscn\");\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotNode3DPositionSetter)
{
	std::string input = R"(
		func doStuff(node: Node3D) -> void:
			node.position = Vector3(1, 2, 3)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/node3d.hpp>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(Node3D* node)\n\t\t{\n\t\t\tnode->set_position(Vector3(1, 2, 3));\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"node\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotRayCast3DTargetPositionSetter)
{
	std::string input = R"(
		func doStuff(ray: RayCast3D) -> void:
			ray.target_position = Vector3(1, 2, 3)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ray_cast3d.hpp>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(RayCast3D* ray)\n\t\t{\n\t\t\tray->set_target_position(Vector3(1, 2, 3));\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"ray\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotRayCast3DGlobalPositionSetter)
{
	std::string input = R"(
		func doStuff(ray: RayCast3D) -> void:
			ray.global_position = Vector3(1, 2, 3)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ray_cast3d.hpp>\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff(RayCast3D* ray)\n\t\t{\n\t\t\tray->set_global_position(Vector3(1, 2, 3));\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\", \"ray\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotFindKeyNode3DCast)
{
	std::string input = R"(
		func doStuff():
			var x := {}
			var y := x.find_key("abc") as String
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/string.hpp>\n#include <map>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tstd::map x = {};\n\t\t\tString y = (String)x.find_key(\"abc\");\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, GodotViewportSetDebugDraw)
{
	std::string input = R"(
		func doStuff():
			get_viewport().debug_draw = Viewport.DEBUG_DRAW_WIREFRAME
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/classes/viewport.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tget_viewport()->set_debug_draw(Viewport::DEBUG_DRAW_WIREFRAME);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}