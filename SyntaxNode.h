#pragma once

#include "TokenStream.h"

struct Type
{
	explicit Type(const std::string& Name) :
		name(Name)
	{}

	std::string name;
};

struct SyntaxNode
{
	virtual std::string toCpp() = 0;
};

struct ValueSyntaxNode : public SyntaxNode
{
};