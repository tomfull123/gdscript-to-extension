#pragma once

#include <gtest/gtest.h>
#include <DocsParser.h>

struct DocsTest : testing::Test
{
	std::string transpile(const std::string& input) const
	{
		AbstractSyntaxTree* ast = new AbstractSyntaxTree();

		const Result* result = DocsParser::parse(input, ast, "Test", "");

		const auto& errors = result->errors;

		if (!errors.empty()) return errors[0].message;

		for (auto c : result->ast->classes)
		{
			CppData data;
			c->hoist(&data);
			c->resolveDefinitions(&data);
			c->resolveTypes(&data);
			return c->toCpp(&data, "");
		}

		return "";
	}
};