#pragma once

#include "SyntaxNode.h"

class ContinueSyntaxNode : public SyntaxNode
{
public:
	std::string toCpp(CppData* data) override
	{
		return "";
	}
};
