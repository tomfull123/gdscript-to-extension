#pragma once

#include "SyntaxNode.h"

class BreakSyntaxNode : public SyntaxNode
{
public:
	std::string toCpp(CppData* data) override
	{
		return "break;\n";
	}
};
