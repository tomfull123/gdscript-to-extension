#pragma once

#include "SyntaxNode.h"

struct BreakSyntaxNode : public SyntaxNode
{
	std::string toCpp() override
	{
		return "break;\n";
	}
};
