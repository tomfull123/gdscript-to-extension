#include "TranspileTest.h"

TEST_F(TranspileTest, MeshInstance3DMeshMethod)
{
	std::string input = R"(
		extends MeshInstance3D

		func _ready() -> void:
			var surfaceCount := mesh.get_surface_count()
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/mesh_instance3d.hpp>\n\nnamespace godot\n{\n\tclass Test : public MeshInstance3D\n\t{\n\t\tGDCLASS(Test, MeshInstance3D)\n\tpublic:\n\t\tvoid _ready()\n\t\t{\n\t\t\tauto surfaceCount = get_mesh()->get_surface_count();\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"_ready\"), &Test::_ready);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(TranspileTest, MeshInstance3DGetChildMethod)
{
	std::string input = R"(
		extends MeshInstance3D

		func _ready() -> void:
			if get_child_count() > 0:
				get_child(0).queue_free()
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/mesh_instance3d.hpp>\n\nnamespace godot\n{\n\tclass Test : public MeshInstance3D\n\t{\n\t\tGDCLASS(Test, MeshInstance3D)\n\tpublic:\n\t\tvoid _ready()\n\t\t{\n\t\t\tif ((get_child_count() > 0))\n\t\t\t{\n\t\t\t\tget_child(0)->queue_free();\n\t\t\t}\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"_ready\"), &Test::_ready);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}