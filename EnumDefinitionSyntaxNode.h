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
	{}

	const std::string& getName() const
	{
		return name_->value;
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		data->enumDefinitions[name_->value] = this;

		std::string valuesString;

		for (auto v : values_)
		{
			valuesString += v->toCpp(data, indents) + ",\n";
		}

		return "enum " + name_->value + "\n"
			"{\n"
			+ valuesString +
			"};\n";
	}

private:
	Token* name_;
	std::vector<EnumValueSyntaxNode*> values_;
};
