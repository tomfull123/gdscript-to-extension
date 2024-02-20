#pragma once

#include <vector>
#include "SyntaxNode.h"

struct FunctionBodySyntaxNode
{
	explicit FunctionBodySyntaxNode(
		const std::vector<SyntaxNode*>& nodes
	) :
		nodes_(nodes)
	{}

	std::string toCpp()
	{
		std::string nodesString;

		for (auto node : nodes_)
		{
			nodesString += node->toCpp();
		}

		return ""
			"{\n"
			+ nodesString +
			"}\n";
	}

	std::vector<SyntaxNode*> nodes_;
};