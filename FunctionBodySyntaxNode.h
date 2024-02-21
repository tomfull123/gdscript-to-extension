#pragma once

#include <vector>
#include "SyntaxNode.h"

class FunctionBodySyntaxNode : public SyntaxNode
{
public:
	explicit FunctionBodySyntaxNode(
		const std::vector<SyntaxNode*>& nodes
	) :
		nodes_(nodes)
	{}

	std::string toCpp(CppData* data)
	{
		std::string nodesString;

		for (auto node : nodes_)
		{
			nodesString += node->toCpp(data);
		}

		return ""
			"{\n"
			+ nodesString +
			"}\n";
	}

private:
	std::vector<SyntaxNode*> nodes_;
};