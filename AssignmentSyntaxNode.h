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

	std::string toCpp(CppData* data) override
	{
		return "";
	}

private:
	VariableSyntaxNode* variable_;
	ValueSyntaxNode* assignmentValue_;
};
