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

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		if (variable_->hasParent())
			return indents + variable_->toCpp(data, indents) + "(" + assignmentValue_->toCpp(data, "") + ")";
		return indents + variable_->toCpp(data, indents) + " = " + assignmentValue_->toCpp(data, "");
	}

private:
	VariableSyntaxNode* variable_;
	ValueSyntaxNode* assignmentValue_;
};
