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

	std::string toCpp() override
	{
		std::string thenString;

		for (auto n : thenNodes_)
		{
			thenString += n->toCpp();
		}

		std::string elseString;

		if (!elseNodes_.empty())
		{
			elseString = "else\n"
				"{\n";
			for (auto n : elseNodes_)
			{
				elseString += n->toCpp();
			}
			elseString += "}\n";
		}

		return "if (" + condition_->toCpp() + ")"
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
