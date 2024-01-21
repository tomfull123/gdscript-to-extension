#pragma once

#include "TokenStream.h"

class Parser
{
public:
	explicit Parser(const std::vector<Token*>& tokens);

	void buildAST(AbstractSyntaxTree* ast);

private:
	TokenStream stream_;
};
