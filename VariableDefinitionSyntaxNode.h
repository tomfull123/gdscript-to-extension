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

	std::string getName() const
	{
		return name_->value;
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		data->variableDefinitions[name_->value] = this;

		std::string code = indents;

		if (!initialValue_ && !dataType_) code += "Variant";
		else code += data->toCppType(dataType_);
		code += " " + name_->value;

		if (initialValue_) code += " = " + initialValue_->toCpp(data, "");

		return code;
	}

private:
	Token* name_;
	Type* dataType_;
	ValueSyntaxNode* initialValue_;
};