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

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		for (const auto& k : keys_) k->resolveTypes(data);
		for (const auto& v : values_) v->resolveTypes(data);

		std::vector<Type*> subtypes;

		if (!keys_.empty() && !values_.empty())
		{
			auto keyType = getKeyType();
			auto valueType = getValueType();
			if (keyType)
			{
				subtypes.push_back(keyType);
				if (valueType) subtypes.push_back(valueType);
			}
		}

		type_ = new Type("Dictionary", subtypes);
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

	Type* getKeyType()
	{
		return getElementType(keys_);
	}

	Type* getValueType()
	{
		return getElementType(values_);
	}

	Type* getElementType(const std::vector<ValueSyntaxNode*>& elements)
	{
		Type* elementType = nullptr;

		for (auto e : elements)
		{
			auto type = e->getType();

			if (type)
			{
				if (type->name == "Dictionary")
				{
					if (!type->subtypes.empty()) return type;
					elementType = type;
				}
				else return type;
			}
		}

		return elementType;
	}
};
