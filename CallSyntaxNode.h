#pragma once

#include "SyntaxNode.h"

class CallSyntaxNode : public ValueSyntaxNode
{
public:
	std::string toCpp(CppData* data) override
	{
		return "";
	}
private:
};
