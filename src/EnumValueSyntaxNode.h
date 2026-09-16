#pragma once

#include "SyntaxNode.h"

class EnumValueSyntaxNode : public SyntaxNode
{
public:
	EnumValueSyntaxNode(
		Token* name,
		ValueSyntaxNode* initialValue
	) :
		name_(name),
		initialValue_(initialValue)
	{
	}

	void hoist(CppData* data) override
	{
	}

	void resolveDefinitions(CppData* data) override
	{
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string code = name_->value;

		if (initialValue_) code += " = " + initialValue_->toCpp(data, "");

		return code;
	}

private:
	Token* name_;
	ValueSyntaxNode* initialValue_;
};
