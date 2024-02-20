#pragma once

#include "SyntaxNode.h"

struct CallSyntaxNode : public ValueSyntaxNode
{
	std::string toCpp() override
	{
		return "";
	}
};
