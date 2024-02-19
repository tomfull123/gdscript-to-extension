#pragma once

#include "SyntaxNode.h"
#include <vector>

class IfSyntaxNode : public SyntaxNode
{
public:
	IfSyntaxNode(
		SyntaxNode* condition,
		std::vector<SyntaxNode*> thenNodes,
		std::vector<SyntaxNode*> elseNodes
	) :
		condition_(condition),
		thenNodes_(thenNodes),
		elseNodes_(elseNodes)
	{}

private:
	SyntaxNode* condition_;
	std::vector<SyntaxNode*> thenNodes_;
	std::vector<SyntaxNode*> elseNodes_;
};
