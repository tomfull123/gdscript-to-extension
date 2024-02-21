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

	void resolveType() override
	{
		condition_->resolveType();
		for (auto n : thenNodes_) n->resolveType();
		for (auto n : elseNodes_) n->resolveType();
	}

	std::string toCpp(CppData* data) override
	{
		std::string thenString;

		for (auto n : thenNodes_)
		{
			thenString += n->toCpp(data);
		}

		std::string elseString;

		if (!elseNodes_.empty())
		{
			elseString = "else\n"
				"{\n";
			for (auto n : elseNodes_)
			{
				elseString += n->toCpp(data);
			}
			elseString += "}\n";
		}

		return "if (" + condition_->toCpp(data) + ")"
			"{\n"
			+ thenString +
			"}\n"
			+ elseString;
	}

private:
	SyntaxNode* condition_;
	std::vector<SyntaxNode*> thenNodes_;
	std::vector<SyntaxNode*> elseNodes_;
};
