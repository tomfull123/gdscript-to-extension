#include "TranspileTest.h"

TEST_F(TranspileTest, ViewportGetCamera3DMethod)
{
	std::string input = R"(
		func _ready() -> void:
			var camera := get_viewport().get_camera_3d()
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/camera3d.hpp>\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/classes/viewport.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid _ready()\n\t\t{\n\t\t\tCamera3D camera = get_viewport()->get_camera_3d();\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"_ready\"), &Test::_ready);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}