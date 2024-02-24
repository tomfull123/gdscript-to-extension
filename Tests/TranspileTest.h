#pragma once

#include <gtest/gtest.h>
#include <Lexer.h>
#include <Parser.h>

struct TranspileTest : testing::Test
{
	std::string transpile(const std::string& input) const
	{
		Result* result = Parser::parse(input);

		auto errors = result->errors;

		if (!errors.empty()) return errors[0].message;

		for (auto c : result->ast->classes)
		{
			CppData* data = new CppData("Test");
			return c->toCpp(data, "");
		}
	}
};