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
	std::string expected = "#pragma once\n\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid abs()\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"abs\"), &Test::abs);\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include \"AABB.h\"\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tRef<AABB> abs()\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"abs\"), &Test::abs);\n\t\t}\n\t};\n}\n";
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
	std::string expected = "#pragma once\n\n#include \"AABB.h\"\n#include <godot_cpp/classes/ref.hpp>\n\nnamespace godot\n{\n\tclass Test : public RefCounted\n\t{\n\t\tGDCLASS(Test, RefCounted)\n\tpublic:\n\t\tvoid abs(Ref<AABB> with)\n\t\t{\n\t\t}\n\n\tprivate:\n\n\tprotected:\n\t\tstatic void _bind_methods()\n\t\t{\n\t\t\tClassDB::bind_method(D_METHOD(\"abs\", \"with\"), &Test::abs);\n\t\t}\n\t};\n}\n";
	EXPECT_EQ(expected, actual);
}