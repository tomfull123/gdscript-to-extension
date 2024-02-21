#pragma once

#include "SyntaxNode.h"

class VariableDefinitionSyntaxNode : public SyntaxNode
{
public:
	VariableDefinitionSyntaxNode(
		Token* name,
		Type* dataType
	) :
		name_(name),
		dataType_(dataType)
	{
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
};