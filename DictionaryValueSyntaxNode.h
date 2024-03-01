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
		for (const auto& v : values_)
		{
			v.first->hoist(data);
			v.second->hoist(data);
		}
	}

	void resolveDefinitions(CppData* data) override
	{
		for (const auto& v : values_)
		{
			v.first->resolveDefinitions(data);
			v.second->resolveDefinitions(data);
		}
	}

	void resolveTypes(CppData* data) override
	{
		for (const auto& v : values_)
		{
			v.first->resolveTypes(data);
			v.second->resolveTypes(data);
		}
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string valuesString;

		for (auto v : values_)
		{
			auto key = v.first;
			auto value = v.second;
			valuesString += indents + "\t{" + key->toCpp(data, indents) + "," + value->toCpp(data, "") + "},\n";
		}

		return "{\n" + valuesString + indents + "}";
	}

private:
	std::map<ValueSyntaxNode*, ValueSyntaxNode*> values_;
};
