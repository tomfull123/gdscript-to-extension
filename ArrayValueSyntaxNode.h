#pragma once

#include "SyntaxNode.h"

class ArrayValueSyntaxNode : public ValueSyntaxNode
{
public:
	explicit ArrayValueSyntaxNode(
		const std::vector<ValueSyntaxNode*>& expressions
	) :
		expressions_(expressions)
	{}

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
		for (auto e : expressions_) e->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		for (auto e : expressions_) e->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data) override
	{
		for (auto e : expressions_) e->resolveTypes(data);
		std::vector<Type*> subtypes;
		if (!expressions_.empty())
		{
			auto elementType = getElementType();
			if (elementType) subtypes.push_back(elementType);
		}
		type_ = new Type("Array", subtypes);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string expressionsString;

		int lastIndex = expressions_.size() - 1;

		for (int i = 0; i < expressions_.size(); i++)
		{
			auto e = expressions_[i];
			auto expressionString = e->toCpp(data, indents + "\t");

			if (i == 0) expressionsString += indents + "\t";
			expressionsString += expressionString + ",";
			if (i == lastIndex) expressionsString += "\n";
		}

		std::string code = "{";

		bool putOnNewLine = expressions_.size() > 2;

		if (putOnNewLine) code += "\n";

		code += expressionsString;

		if (putOnNewLine) code += indents;

		return code + "}";
	}

private:
	std::vector<ValueSyntaxNode*> expressions_;
	Type* type_ = nullptr;

	Type* getElementType()
	{
		Type* elementType = nullptr;

		for (auto e : expressions_)
		{
			auto type = e->getType();
			if (type)
			{
				if (type->name == "Array")
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