#pragma once

#include "AbstractSyntaxTree.h"
#include "SyntaxNode.h"
#include "ClassDefinitionSyntaxNode.h"
#include "CPPModule.h"

class Generator
{
public:

	CPPModule* generateCode(AbstractSyntaxTree* ast)
	{
		CppData* data = new CppData();

		hoist(ast, data);

		resolve(ast, data);

		auto cppModule = generate(ast, data);

		delete data;

		return cppModule;
	}

private:
	void hoist(const AbstractSyntaxTree* ast, CppData* data)
	{
		for (auto* classDef : ast->classes) classDef->hoist(data);
	}

	void resolve(const AbstractSyntaxTree* ast, CppData* data)
	{
		for (auto* classDef : ast->classes) classDef->resolveDefinitions(data);
		for (auto* classDef : ast->classes) classDef->resolveTypes(data);
	}

	CPPModule* generate(const AbstractSyntaxTree* ast, CppData* data)
	{
		CPPModule* cppModule = new CPPModule();

		for (auto* classDef : ast->classes)
		{
			const auto& className = classDef->getName();
			if (!className.empty() && className[0] == '_')
			{
				cppModule->classes.emplace_back(classDef->toCpp(data, ""), className);
			}
		}

		return cppModule;
	}
};
