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

	void resolveType() override
	{
		if (initialValue_) initialValue_->resolveType();
		if (!dataType_) dataType_ = initialValue_->getType();
	}

	std::string toCpp(CppData* data) override
	{
		std::string code = data->toCppType(dataType_) + " " + name_->value;

		code += ";\n";

		return code;
	}

private:
	Token* name_;
	Type* dataType_;
	ValueSyntaxNode* initialValue_;
};