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
	virtual std::string toCpp() = 0;
};

class ValueSyntaxNode : public SyntaxNode
{
public:
};