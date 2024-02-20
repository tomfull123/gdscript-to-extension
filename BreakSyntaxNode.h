#pragma once

#include "SyntaxNode.h"

class BreakSyntaxNode : public SyntaxNode
{
public:
	std::string toCpp() override
	{
		return "break;\n";
	}
};
