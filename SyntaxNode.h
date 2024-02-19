#pragma once

#include "TokenStream.h"

struct Type
{
	explicit Type(const std::string& Name) :
		name(Name)
	{}

	std::string name;
};

class SyntaxNode
{
public:
};

class ValueSyntaxNode : public SyntaxNode
{
public:
};