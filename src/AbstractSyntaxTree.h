#pragma once

#include <vector>

class ClassDefinitionSyntaxNode;

struct AbstractSyntaxTree
{
	std::vector<ClassDefinitionSyntaxNode*> classes;
};