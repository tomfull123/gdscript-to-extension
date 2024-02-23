#pragma once

#include <gtest/gtest.h>
#include <Lexer.h>
#include <Parser.h>

struct TranspileTest : testing::Test
{
	std::string transpile(const std::string& input) const
	{
		AbstractSyntaxTree* ast = new AbstractSyntaxTree();

		Lexer lexer(input);

		std::vector<Token*> tokens = lexer.readAllTokens();

		Parser parser(tokens);

		parser.buildAST(ast);

		const auto& errors = parser.getErrors();

		if (!errors.empty()) return errors[0].message;

		for (auto c : ast->classes)
		{
			CppData* data = new CppData("Test");
			return c->toCpp(data, "");
		}
	}
};