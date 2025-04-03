#pragma once

#include <string>
#include <vector>

struct Type
{
	Type(
		const std::string& name,
		const std::vector<Type*>& subtypes = {}
	) :
		name(name),
		subtypes(subtypes)
	{
	}

	Type* subtype(int index)
	{
		if (index >= subtypes.size()) return nullptr;
		return subtypes[index];
	}

	std::string name;
	std::vector<Type*> subtypes;

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

		return name + "<" + subtypeString + ">";
	}
};