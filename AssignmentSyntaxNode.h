#pragma once

#include "SyntaxNode.h"
#include "VariableSyntaxNode.h"

struct AssignmentSyntaxNode : public ValueSyntaxNode
{
	AssignmentSyntaxNode(
		VariableSyntaxNode* variable,
		ValueSyntaxNode* assignmentValue
	) :
		variable_(variable),
		assignmentValue_(assignmentValue)
	{}

	std::string toCpp() override
	{
		return "";
	}

	VariableSyntaxNode* variable_;
	ValueSyntaxNode* assignmentValue_;
};
