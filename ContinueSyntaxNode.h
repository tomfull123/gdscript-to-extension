#pragma once

#include "SyntaxNode.h"

class ContinueSyntaxNode : public SyntaxNode
{
public:
	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return indents + "continue;\n";
	}
};
