#pragma once

#include "SyntaxNode.h"
#include "LiteralValueSyntaxNode.h"
#include <map>

class DictionaryValueSyntaxNode : public ValueSyntaxNode
{
public:
	explicit DictionaryValueSyntaxNode(
		const std::map<ValueSyntaxNode*, ValueSyntaxNode*>& values
	) :
		values_(values)
	{}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string valuesString;

		for (auto v : values_)
		{
			auto key = v.first;
			auto value = v.second;
			valuesString += "{" + key->toCpp(data, indents) + "," + value->toCpp(data, "") + "},\n";
		}

		return "{" + valuesString + "}";
	}

private:
	std::map<ValueSyntaxNode*, ValueSyntaxNode*> values_;
};
