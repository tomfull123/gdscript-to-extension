#pragma once

#include "SyntaxNode.h"

class BreakSyntaxNode : public SyntaxNode
{
public:
	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return indents + "break;\n";
	}
};
