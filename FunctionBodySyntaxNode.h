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

	void resolveTypes(CppData* data) override
	{
		for (auto n : nodes_) n->resolveTypes(data);
	}

	void hoist(CppData* data) override
	{
		for (auto n : nodes_) n->hoist(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string nodesString;

		for (auto node : nodes_)
		{
			nodesString += indents + "\t" + node->toCpp(data, indents + "\t");
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