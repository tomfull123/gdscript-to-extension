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

private:
	std::vector<SyntaxNode*> nodes_;
};