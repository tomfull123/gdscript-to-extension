#pragma once

#include "SyntaxNode.h"
#include "BodySyntaxNode.h"
#include "RangeSyntaxNode.h"
#include <typeinfo>

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

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		array_->resolveTypes(data);
		auto arrayType = array_->getType();
		Type* variableType = nullptr;
		if (arrayType)
		{
			if (arrayType->name == "int") variableType = arrayType;
			else if (!arrayType->subtypes.empty())
			{
				variableType = arrayType->subtypes[0];
			}
		}
		if (!variableType && typeid(*array_) == typeid(RangeSyntaxNode)) variableType = new Type("int");
		variableDefinition_->resolveTypes(data, variableType);
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
		else if (typeid(*array_) == typeid(RangeSyntaxNode))
		{
			auto range = (RangeSyntaxNode*)array_;

			code += "int " + varName + " = " + range->getStartValue()->toCpp(data, "") + "; "
				+ varName + " < " + range->getEndValue()->toCpp(data, "") + "; ";

			auto incrementValue = range->getIncrementValue();

			if (incrementValue) code += incrementValue->toCpp(data, "");
			else code += varName + "++";
		}
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
