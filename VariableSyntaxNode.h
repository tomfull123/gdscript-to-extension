#pragma once

#include "SyntaxNode.h"

class VariableSyntaxNode : public ValueSyntaxNode
{
public:
	VariableSyntaxNode(Token* name, ValueSyntaxNode* parentInstance) :
		name_(name),
		parentInstance_(parentInstance)
	{}

	Type* getType() override
	{
		return nullptr;
	}

	void resolveType() override
	{
		if (parentInstance_) parentInstance_->resolveType();
	}

	std::string toCpp(CppData* data) override
	{
		return "";
	}

private:
	Token* name_;
	ValueSyntaxNode* parentInstance_;
};