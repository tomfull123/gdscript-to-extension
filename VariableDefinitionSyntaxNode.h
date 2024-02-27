#pragma once

#include "SyntaxNode.h"

class VariableDefinitionSyntaxNode : public SyntaxNode
{
public:
	VariableDefinitionSyntaxNode(
		Token* name,
		Type* dataType,
		ValueSyntaxNode* initialValue,
		bool isConstant
	) :
		name_(name),
		dataType_(dataType),
		initialValue_(initialValue),
		isConstant_(isConstant)
	{
	}

	std::string getName() const
	{
		return name_->value;
	}

	void hoist(CppData* data) override
	{
		data->variableDefinitions[name_->value] = this;
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string code;

		if (isConstant_) code += "const ";

		if (!initialValue_ && !dataType_) code += "Variant";
		else code += data->toCppType(dataType_);
		code += " " + name_->value;

		if (initialValue_) code += " = " + initialValue_->toCpp(data, indents);

		return code;
	}

private:
	Token* name_;
	Type* dataType_;
	ValueSyntaxNode* initialValue_;
	bool isConstant_;
};