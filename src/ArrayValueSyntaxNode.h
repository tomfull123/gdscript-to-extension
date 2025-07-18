#pragma once

#include "SyntaxNode.h"

class ArrayValueSyntaxNode : public ValueSyntaxNode
{
public:
	explicit ArrayValueSyntaxNode(
		const std::vector<ValueSyntaxNode*>& expressions
	) :
		expressions_(expressions)
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
		for (auto e : expressions_) e->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		for (auto e : expressions_) e->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
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
		std::string code = "{";

		code += getExpressionsString(data, indents);

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
				if (type->getName() == "Array")
				{
					if (!type->subtypes.empty()) return type;
					elementType = type;
				}
				else return type;
			}
		}

		return elementType;
	}

	std::string getExpressionsString(CppData* data, const std::string& indents) const
	{
		std::string expressionsString;
		bool putOnNewLine = expressions_.size() > 2;

		if (putOnNewLine) expressionsString += "\n";

		size_t lastIndex = expressions_.size() - 1;

		for (int i = 0; i < expressions_.size(); i++)
		{
			auto e = expressions_[i];
			auto expressionString = e->toCpp(data, indents + "\t");

			if (i == 0)
			{
				if (putOnNewLine) expressionsString += indents + "\t";
				else expressionsString += " ";
			}
			expressionString += ",";
			if (i == lastIndex)
			{
				expressionString += "\n";
				if (!putOnNewLine) expressionString += indents;
			}
			expressionsString += expressionString;
		}

		if (putOnNewLine) expressionsString += indents;

		return expressionsString;
	}
};