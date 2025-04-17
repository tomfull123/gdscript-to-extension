#pragma once

#include "AbstractSyntaxTree.h"
#include "SyntaxNode.h"
#include "ClassDefinitionSyntaxNode.h"
#include "CPPModule.h"
#include "RegisterTypesGenerator.h"

class Generator
{
public:

	CPPModule* generateCode(const AbstractSyntaxTree* ast)
	{
		CppData data;

		hoist(ast, &data);

		resolve(ast, &data);

		return generate(ast, &data);
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
			const auto& classNameToken = classDef->getNameToken();
			if (classNameToken && classNameToken->value[0] == '_')
			{
				cppModule->classes.emplace_back(classDef->toCpp(data, ""), classDef->getName());
			}
		}

		cppModule->registerTypeHeader.className = RegisterTypesGenerator::filename;
		cppModule->registerTypeHeader.code = RegisterTypesGenerator::generateHeader();
		cppModule->registerTypeSource.className = RegisterTypesGenerator::filename;
		cppModule->registerTypeSource.code = RegisterTypesGenerator::generateSource(*cppModule);

		return cppModule;
	}
};
