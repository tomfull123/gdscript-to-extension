#pragma once

#include "SyntaxNode.h"

struct ContinueSyntaxNode : public SyntaxNode
{
	std::string toCpp() override
	{
		return "";
	}
};
