#include "TranspileTest.h"

TEST_F(TranspileTest, If)
{
	std::string input = R"(
		class_name Hello
	)";

	auto actual = transpile(input);
	std::string expected = "";
	EXPECT_EQ(expected, actual);
}