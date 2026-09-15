#include "TranspileTest.h"

TEST_F(TranspileTest, LambdaFunctionInline)
{
	std::string input = R"(
		func doStuff() -> void:
			var nodes: Array[Node] = []
			nodes.filter(func (node: Node) -> bool: return true)
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include \"Node.h\"\n#include <godot_cpp/classes/ref.hpp>\n#include <godot_cpp/variant/typed_array.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid doStuff()\n\t\t{\n\t\t\tTypedArray<Node> nodes = {};\n\t\t\tnodes.filter(lambda-1);\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"doStuff\"), &Test::doStuff);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}