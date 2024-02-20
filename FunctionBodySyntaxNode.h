#pragma once

#include <vector>
#include "SyntaxNode.h"

class FunctionBodySyntaxNode
{
public:
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

private:
	std::vector<SyntaxNode*> nodes_;
};