#pragma once

class ClassDefinitionSyntaxNode
{
public:
	ClassDefinitionSyntaxNode(Token* name) :
		name_(name)
	{}

private:
	Token* name_;
};