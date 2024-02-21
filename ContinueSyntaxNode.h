#pragma once

#include "SyntaxNode.h"

class ContinueSyntaxNode : public SyntaxNode
{
public:
	void resolveType() override
	{
	}

	std::string toCpp(CppData* data) override
	{
		return "";
	}
};
