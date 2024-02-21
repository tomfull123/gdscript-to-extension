#pragma once

#include "SyntaxNode.h"

class BreakSyntaxNode : public SyntaxNode
{
public:
	void resolveType() override
	{
	}

	std::string toCpp(CppData* data) override
	{
		return "break;\n";
	}
};
