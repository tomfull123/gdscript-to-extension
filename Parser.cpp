#include "Parser.h"

GDParser::GDParser(const std::vector<GDToken*>& tokens) :
	stream_(tokens)
{
}

void GDParser::buildAST(AbstractSyntaxTree* ast, const std::string& fileName)
{
	ast->classes.push_back(parseScriptBody(0, fileName));
}