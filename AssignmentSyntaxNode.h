#pragma once

#include "SyntaxNode.h"
#include "VariableSyntaxNode.h"

class AssignmentSyntaxNode : public ValueSyntaxNode
{
public:
	AssignmentSyntaxNode(
		ValueSyntaxNode* variable,
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
	ValueSyntaxNode* variable_;
	ValueSyntaxNode* assignmentValue_;
};
