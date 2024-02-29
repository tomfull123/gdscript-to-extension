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

	Type* getType() override
	{
		return variable_->getType();
	}

	std::string getName() override
	{
		return "";
	}

	void hoist(CppData* data) override
	{
		variable_->hoist(data);
		assignmentValue_->hoist(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		if (variable_->hasParent())
			return variable_->toCpp(data, indents) + "(" + assignmentValue_->toCpp(data, "") + ")";
		return variable_->toCpp(data, indents) + " = " + assignmentValue_->toCpp(data, "");
	}

private:
	ValueSyntaxNode* variable_;
	ValueSyntaxNode* assignmentValue_;
};
