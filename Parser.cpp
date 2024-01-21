#include "Parser.h"

Parser::Parser(const std::vector<Token*>& tokens) :
	stream_(tokens)
{
}

void Parser::buildAST(AbstractSyntaxTree* ast)
{
	ast->classes.push_back(parseScriptBody());
}