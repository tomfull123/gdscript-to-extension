#pragma once

#include "SyntaxNode.h"

class WhileSyntaxNode : public SyntaxNode
{
public:
	WhileSyntaxNode(
		ValueSyntaxNode* condition,
		const std::vector<SyntaxNode*>& nodes
	) :
		condition_(condition),
		nodes_(nodes)
	{
	}

	bool needsSemiColon() override
	{
		return false;
	}

	void hoist(CppData* data) override
	{
		for (auto n : nodes_) n->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		for (auto n : nodes_) n->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data) override
	{
		for (auto n : nodes_) n->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string bodyString;

		for (auto n : nodes_) bodyString += indents + "\t" + n->toCpp(data, indents) + ";\n";

		return "while (" + condition_->toCpp(data, "") + ")\n"
			+ indents + "{\n"
			+ bodyString
			+ indents + "}\n";
	}

private:
	ValueSyntaxNode* condition_;
	std::vector<SyntaxNode*> nodes_;
};
