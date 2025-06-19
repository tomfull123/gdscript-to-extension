#pragma once

#include "SyntaxNode.h"

class BreakSyntaxNode : public SyntaxNode
{
public:

	void hoist(CppData* data) override
	{
	}

	void resolveDefinitions(CppData* data) override
	{
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return "break";
	}
};
