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
};