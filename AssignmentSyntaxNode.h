#pragma once

#include "SyntaxNode.h"
#include "VariableSyntaxNode.h"

class AssignmentSyntaxNode : public ValueSyntaxNode
{
public:
	AssignmentSyntaxNode(
		VariableSyntaxNode* variable,
		ValueSyntaxNode* assignmentValue
	) :
		variable_(variable),
		assignmentValue_(assignmentValue)
	{}

private:
	VariableSyntaxNode* variable_;
	ValueSyntaxNode* assignmentValue_;
};
