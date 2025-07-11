#pragma once

#include <vector>
#include "SyntaxNode.h"

class BodySyntaxNode : public SyntaxNode
{
public:
	explicit BodySyntaxNode(
		const std::vector<SyntaxNode*>& nodes
	) :
		nodes_(nodes)
	{
	}

	bool returnsValue() override
	{
		if (nodes_.empty()) return false;
		return nodes_[nodes_.size() - 1]->returnsValue();
	}

	void hoist(CppData* data) override
	{
		for (auto n : nodes_) n->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		for (auto n : nodes_) n->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		for (auto n : nodes_) n->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string nodesString = getNodesString(data, indents);

		return ""
			+ indents + "{\n"
			+ nodesString
			+ indents + "}\n";
	}

private:
	std::vector<SyntaxNode*> nodes_;

	std::string getNodesString(CppData* data, const std::string& indents) const
	{
		std::string nodesString;

		for (auto node : nodes_)
		{
			nodesString += indents + "\t" + node->toCpp(data, indents + "\t");
			if (node->needsSemiColon()) nodesString += ";\n";
		}
		return nodesString;
	}
};