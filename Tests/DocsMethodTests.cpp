#include "DocsTest.h"

TEST_F(DocsTest, DocsMethodName)
{
	std::string input = R"(
		<class name="Test">
			<methods>
				<method name="abs">
				</method>
			</methods>
		</class>
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n\nnamespace godot\n{\n\tclass Test : public \n\t{\n\t\tGDCLASS(Test, )\n\tpublic:\n\t\tvoid abs()\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"abs\"), &Test::abs);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(DocsTest, DocsMethodReturnType)
{
	std::string input = R"(
		<class name="Test">
			<methods>
				<method name="abs">
					<return type="AABB" />
				</method>
			</methods>
		</class>
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include \"AABB.h\"\n\nnamespace godot\n{\n\tclass Test : public \n\t{\n\t\tGDCLASS(Test, )\n\tpublic:\n\t\tAABB abs()\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"abs\"), &Test::abs);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}

TEST_F(DocsTest, DocsMethodParam)
{
	std::string input = R"(
		<class name="Test">
			<methods>
				<method name="abs">
					<param name="with" type="AABB" />
				</method>
			</methods>
		</class>
	)";

	auto actual = transpile(input);
	std::string expected = "#pragma once\n\n#include \"AABB.h\"\n\nnamespace godot\n{\n\tclass Test : public \n\t{\n\t\tGDCLASS(Test, )\n\tpublic:\n\t\tvoid abs(AABB with)\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"abs\", \"with\"), &Test::abs);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}