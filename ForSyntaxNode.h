#pragma once

#include "SyntaxNode.h"
#include "BodySyntaxNode.h"

class ForSyntaxNode : public SyntaxNode
{
public:
	ForSyntaxNode(
		VariableDefinitionSyntaxNode* variableDefinition,
		ValueSyntaxNode* array,
		BodySyntaxNode* body
	) :
		variableDefinition_(variableDefinition),
		array_(array),
		body_(body)
	{
	}

	bool needsSemiColon() override
	{
		return false;
	}

	void hoist(CppData* data) override
	{
		variableDefinition_->hoist(data);
		array_->hoist(data);
		body_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		variableDefinition_->resolveDefinitions(data);
		array_->resolveDefinitions(data);
		body_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data) override
	{
		variableDefinition_->resolveTypes(data);
		array_->resolveTypes(data);
		body_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string code = "for (";
		auto arrayType = array_->getType();

		std::string varName = variableDefinition_->getName();
		Type* varType = variableDefinition_->getType();

		if (arrayType && arrayType->name == "int")
			code += "int " + varName + " = 0; " + varName + " < " + array_->toCpp(data, "") + "; " + varName + "++";
		else
			code += "auto " + varName + " : " + array_->toCpp(data, "");

		code += ")\n";

		return code + body_->toCpp(data, indents);
	}

private:
	VariableDefinitionSyntaxNode* variableDefinition_;
	ValueSyntaxNode* array_;
	BodySyntaxNode* body_;
};
