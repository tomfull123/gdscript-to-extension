#pragma once

#include "SyntaxNode.h"

class VariableDefinitionSyntaxNode : public SyntaxNode
{
public:
	VariableDefinitionSyntaxNode(
		Token* name,
		Type* dataType,
		ValueSyntaxNode* initialValue
	) :
		name_(name),
		dataType_(dataType),
		initialValue_(initialValue)
	{
	}

	std::string toCpp(CppData* data) override
	{
		std::string code = data->toCppType(dataType_) + " " + name_->value;

		if (initialValue_) code += " = " + initialValue_->toCpp(data);

		return code;
	}

private:
	Token* name_;
	Type* dataType_;
	ValueSyntaxNode* initialValue_;
};