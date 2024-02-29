#pragma once

#include "SyntaxNode.h"

class ContinueSyntaxNode : public SyntaxNode
{
public:

	void resolveTypes(CppData* data) override
	{
	}

	void hoist(CppData* data) override
	{
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return indents + "continue;\n";
	}
};
