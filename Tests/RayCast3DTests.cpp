#include "TranspileTest.h"

TEST_F(TranspileTest, RayCast3DGetCollisionPointMethod)
{
	std::string input = R"(
		func _ready() -> void:
			var raycast := $RayCast3D as RayCast3D
			var collisionPoint := raycast.get_collision_point()
			var y := collisionPoint.y
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ray_cast3d.hpp>\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/vector3.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid _ready()\n\t\t{\n\t\t\tRef<RayCast3D> raycast = (Ref<RayCast3D>)find_child(\"RayCast3D\");\n\t\t\tVector3 collisionPoint = raycast->get_collision_point();\n\t\t\tauto y = collisionPoint.y;\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"_ready\"), &Test::_ready);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}