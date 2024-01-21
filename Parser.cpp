#include "Parser.h"

Parser::Parser(const std::vector<Token*>& tokens) :
	stream_(tokens)
{
}