#pragma once

#include "SyntaxNode.h"
#include "EnumValueSyntaxNode.h"

class EnumDefinitionSyntaxNode : public SyntaxNode
{
public:
	EnumDefinitionSyntaxNode(
		Token* name,
		const std::vector<EnumValueSyntaxNode*>& values
	) :
		name_(name),
		values_(values)
	{
	}

	const std::string& getName() const
	{
		return name_->value;
	}

	void hoist(CppData* data) override
	{
		data->currentClass->enumDefinitions[name_->value] = this;
		for (auto v : values_) v->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		for (auto v : values_) v->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		for (auto v : values_) v->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string valuesString;

		for (auto v : values_)
		{
			valuesString += indents + "\t" + v->toCpp(data, indents) + ",\n";
		}

		return "enum " + name_->value + "\n"
			+ indents + "{\n"
			+ valuesString
			+ indents + "};\n";
	}

private:
	Token* name_;
	std::vector<EnumValueSyntaxNode*> values_;
};
