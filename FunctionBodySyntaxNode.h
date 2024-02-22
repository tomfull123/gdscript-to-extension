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

	std::string toCpp(CppData* data, const std::string& indents)
	{
		std::string nodesString;

		for (auto node : nodes_)
		{
			nodesString += node->toCpp(data, indents + "\t");
			if (node->needsSemiColon()) nodesString += ";";
			nodesString += "\n";
		}

		return ""
			+ indents + "{\n"
			+ nodesString
			+ indents + "}\n";
	}

private:
	std::vector<SyntaxNode*> nodes_;
};