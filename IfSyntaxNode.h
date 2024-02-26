#pragma once

#include "SyntaxNode.h"
#include <vector>

class IfSyntaxNode : public SyntaxNode
{
public:
	IfSyntaxNode(
		SyntaxNode* condition,
		const std::vector<SyntaxNode*>& thenNodes,
		const std::vector<SyntaxNode*>& elseNodes
	) :
		condition_(condition),
		thenNodes_(thenNodes),
		elseNodes_(elseNodes)
	{}

	bool needsSemiColon() override
	{
		return false;
	}

	void hoist(CppData* data) override
	{
		condition_->hoist(data);
		for (auto n : thenNodes_) n->hoist(data);
		for (auto n : elseNodes_) n->hoist(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string thenString;

		for (auto n : thenNodes_)
		{
			thenString += indents + "\t" + n->toCpp(data, indents + "\t");
			if (n->needsSemiColon()) thenString += ";";
			thenString += "\n";
		}

		std::string elseString;

		if (!elseNodes_.empty())
		{
			elseString = "\n" + indents + "else\n"
				+ indents + "{\n";
			for (auto n : elseNodes_)
			{
				elseString += indents + "\t" + n->toCpp(data, indents + "\t");
				if (n->needsSemiColon()) elseString += ";";
				elseString += "\n";
			}
			elseString += indents + "}";
		}

		return "if (" + condition_->toCpp(data, "") + ")\n"
			+ indents + "{\n"
			+ thenString
			+ indents + "}"
			+ elseString;
	}

private:
	SyntaxNode* condition_;
	std::vector<SyntaxNode*> thenNodes_;
	std::vector<SyntaxNode*> elseNodes_;
};
