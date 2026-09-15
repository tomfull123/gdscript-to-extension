#include "TranspileTest.h"

TEST_F(TranspileTest, LambdaFunctionInline)
{
	std::string input = R"(
		func doStuff() -> void:
			var nodes: Array[Node] = []
			nodes.filter(func (node: Node) -> bool: return true)
	)";

	auto actual = transpile(input);
	std::string expected = "";
	EXPECT_EQ(expected, actual);
}