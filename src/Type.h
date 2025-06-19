#pragma once

#include <string>
#include <vector>
#include "Lexer.h"

class Type
{
public:
	Type(
		const std::string& name,
		const std::vector<Type*>& subtypes = {}
	) :
		Type(
			new Token(name),
			subtypes
		)
	{
	}

	Type(
		const Token* nameToken,
		const std::vector<Type*>& subtypes = {},
		const Type* namespaceType = nullptr
	) :
		nameToken(nameToken),
		subtypes(subtypes),
		namespaceType(namespaceType)
	{
	}

	const std::string& getName() const
	{
		return nameToken->value;
	}

	Type* subtype(int index)
	{
		if (index >= subtypes.size()) return nullptr;
		return subtypes[index];
	}

	std::string toString()
	{
		std::string subtypeString;

		int lastIndex = subtypes.size() - 1;

		for (int i = 0; i < subtypes.size(); i++)
		{
			auto subtype = subtypes[i];
			subtypeString += subtype->toString();
			if (i != lastIndex) subtypeString += ", ";
		}

		return getName() + "<" + subtypeString + ">";
	}

	const std::vector<Type*> subtypes;

private:
	const Token* nameToken;
	const Type* namespaceType;
};