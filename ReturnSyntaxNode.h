#pragma once

#include "SyntaxNode.h"

class ReturnSyntaxNode : public SyntaxNode
{
public:
	explicit ReturnSyntaxNode(ValueSyntaxNode* value) :
		value_(value)
	{}

	void hoist(CppData* data) override
	{
		if (value_) value_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		if (value_) value_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		if (value_) value_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string code = "return";

		if (value_) code += " " + value_->toCpp(data, "");

		return code;
	}

private:
	ValueSyntaxNode* value_;
};