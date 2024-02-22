#pragma once

#include "TokenStream.h"
#include <unordered_set>
#include <unordered_map>

const std::unordered_map<std::string, std::string> GDTYPES_TO_CPPTYPES = {
	{"int", "int"},
	{"float", "float"},
	{"String", "std::string"},
	{ "StringName", "std::string" },
};

const std::unordered_map<std::string, std::string> CPPTYPES_TO_INCLUDE_PATH = {
	{"std::string", "<string>"},
};

struct Type
{
	explicit Type(const std::string& Name) :
		name(Name)
	{}

	std::string name;
};

struct CppData
{
	std::unordered_set<std::string> types;

	std::string toCppType(Type* type)
	{
		if (!type) return "auto";

		auto it = GDTYPES_TO_CPPTYPES.find(type->name);

		if (it != GDTYPES_TO_CPPTYPES.end())
		{
			types.emplace(it->second);
			return it->second;
		}

		types.emplace(type->name);

		return type->name;
	}
};

class SyntaxNode
{
public:

	virtual bool needsSemiColon()
	{
		return true;
	}

	virtual std::string toCpp(CppData* data, const std::string& indents) = 0;
};

class ValueSyntaxNode : public SyntaxNode
{
public:
};