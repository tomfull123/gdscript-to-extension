#pragma once

#include "SyntaxNode.h"

class CallSyntaxNode : public ValueSyntaxNode
{
public:
	std::string toCpp() override
	{
		return "";
	}
private:
};
