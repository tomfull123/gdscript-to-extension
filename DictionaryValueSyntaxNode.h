#pragma once

#include "SyntaxNode.h"
#include "LiteralValueSyntaxNode.h"
#include <unordered_map>

class DictionaryValueSyntaxNode : public ValueSyntaxNode
{
public:
	explicit DictionaryValueSyntaxNode(
		const std::unordered_map<ValueSyntaxNode*, ValueSyntaxNode*>& values
	) :
		values_(values)
	{
	}

	Type* getType() override
	{
		return type_;
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

		if (!values_.empty())
		{
			std::vector<Type*> subtypes;
			auto value = values_.begin();
			subtypes.push_back(value->first->getType());
			subtypes.push_back(value->second->getType());
			type_ = new Type("Dictionary", subtypes);
		}
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string valuesString;

		for (const auto& v : values_)
		{
			auto key = v.first;
			auto value = v.second;
			valuesString += indents + "\t{" + key->toCpp(data, indents) + "," + value->toCpp(data, "") + "},\n";
		}

		return "{\n" + valuesString + indents + "}";
	}

private:
	std::unordered_map<ValueSyntaxNode*, ValueSyntaxNode*> values_;
	Type* type_;
};
