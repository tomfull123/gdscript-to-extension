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

	Type* getType() override
	{
		return variable_->getType();
	}

	void resolveType() override
	{
		variable_->resolveType();
		if (assignmentValue_) assignmentValue_->resolveType();
	}

	std::string toCpp(CppData* data) override
	{
		return "";
	}

private:
	VariableSyntaxNode* variable_;
	ValueSyntaxNode* assignmentValue_;
};
