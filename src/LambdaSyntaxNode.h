#pragma once

#include "FunctionDefinitionSyntaxNode.h"

class LambdaSyntaxNode : public ValueSyntaxNode
{
public:
	LambdaSyntaxNode(
		Token* token,
		FunctionDefinitionSyntaxNode* functionDefinition
	) :
		token_(token),
		functionDefinition_(functionDefinition)
	{
	}

	Type* getType() override
	{
		return nullptr;
	}

	std::string getName() override
	{
		return "";
	}

	void hoist(CppData* data) override
	{
		data->currentClass->lambdaFunctionDefinitions.push_back(functionDefinition_);
		functionDefinition_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		functionDefinition_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType) override
	{
		functionDefinition_->resolveTypes(data, otherType);
	}

	std::string toCpp(CppData* data, const std::string& indents, bool asValue) override
	{
		return functionDefinition_->getName();
	}

private:
	Token* token_;
	FunctionDefinitionSyntaxNode* functionDefinition_;
};