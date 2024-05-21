#pragma once

#include "SyntaxNode.h"
#include "LiteralValueSyntaxNode.h"
#include <unordered_map>

class DictionaryValueSyntaxNode : public ValueSyntaxNode
{
public:
	DictionaryValueSyntaxNode(
		const std::vector<ValueSyntaxNode*>& keys,
		const std::vector<ValueSyntaxNode*>& values
	) :
		keys_(keys),
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
		for (const auto& k : keys_) k->hoist(data);
		for (const auto& v : values_) v->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		for (const auto& k : keys_) k->resolveDefinitions(data);
		for (const auto& v : values_) v->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data) override
	{
		for (const auto& k : keys_) k->resolveTypes(data);
		for (const auto& v : values_) v->resolveTypes(data);

		if (!keys_.empty() && !values_.empty())
		{
			std::vector<Type*> subtypes;
			auto key = keys_[0];
			auto value = values_[0];
			subtypes.push_back(key->getType());
			subtypes.push_back(value->getType());
			type_ = new Type("Dictionary", subtypes);
		}
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string valuesString;

		for (int i = 0; i < keys_.size(); i++)
		{
			auto key = keys_[i];
			auto value = values_[i];
			valuesString += indents + "\t{" + key->toCpp(data, indents) + "," + value->toCpp(data, "") + "},\n";
		}

		return "{\n" + valuesString + indents + "}";
	}

private:
	std::vector<ValueSyntaxNode*> keys_;
	std::vector<ValueSyntaxNode*> values_;
	Type* type_;
};
