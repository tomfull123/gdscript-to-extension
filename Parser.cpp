#include "Parser.h"

Parser::Parser(const std::vector<GDToken*>& tokens) :
	stream_(tokens)
{
}

void Parser::buildAST(AbstractSyntaxTree* ast, const std::string& fileName)
{
	ast->classes.push_back(parseScriptBody(0, fileName));
}