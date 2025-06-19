#pragma once

#include "SyntaxNode.h"

class CastSyntaxNode : public ValueSyntaxNode
{
public:
	CastSyntaxNode(ValueSyntaxNode* value, Type* type) :
		value_(value),
		type_(type)
	{
	}

	Type* getType() override
	{
		return type_;
	}

	std::string getName() override
	{
		return value_->getName();
	}

	void hoist(CppData* data) override
	{
		value_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		value_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		value_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return "(" + data->toCppType(type_) + ")" + value_->toCpp(data, "");
	}

private:
	ValueSyntaxNode* value_;
	Type* type_;
};