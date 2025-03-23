#pragma once

#include <vector>
#include "XMLParser.h"
#include "ParserError.h"
#include "XMLTokenStream.h"
#include "AbstractSyntaxTree.h"
#include "SyntaxNode.h"
#include "FunctionDefinitionSyntaxNode.h"
#include "GDParser.h"

class DocsParser
{
public:
	void buildAST(const XMLTag* rootTag, AbstractSyntaxTree* ast, const std::string& fileName)
	{
		const auto& classTags = rootTag->children.find("class")->second;

		for (const auto classTag : classTags)
		{
			ast->classes.push_back(parseClass(classTag, fileName));
		}
	}

	static Result* parse(const std::string& input, AbstractSyntaxTree* ast, const std::string& fileName)
	{
		XMLLexer lexer(input);

		std::vector<XMLToken*> tokens = lexer.readAllTokens();

		XMLParser xmlParser(tokens);

		auto rootTag = xmlParser.build();

		DocsParser parser;

		parser.buildAST(rootTag, ast, fileName);

		const auto& errors = xmlParser.getErrors();

		return new Result(ast, errors);
	}

private:

	ClassDefinitionSyntaxNode* parseClass(const XMLTag* classTag, const std::string& fileName)
	{
		XMLToken* name = nullptr;
		XMLToken* extends = nullptr;

		name = classTag->getProperty("name");
		extends = classTag->getProperty("inherits");

		std::vector<FunctionDefinitionSyntaxNode*> memberFunctionDefinitions;
		std::vector<VariableDefinitionSyntaxNode*> memberVariableDefinitions;
		std::vector<EnumDefinitionSyntaxNode*> enumDefinitions;
		std::vector<FunctionDefinitionSyntaxNode*> staticFunctionDefinitions;
		std::vector<VariableDefinitionSyntaxNode*> staticVariableDefinitions;
		std::vector<ClassDefinitionSyntaxNode*> innerClasses;

		for (const auto& [tagName, tag] : classTag->children)
		{
			if (tag.empty()) continue;
			if (tagName == "methods")
			{
				memberFunctionDefinitions = parseMethods(tag[0]);
			}
			else if (tagName == "members")
			{
				memberVariableDefinitions = parseMembers(tag[0]);
			}
		}

		return new ClassDefinitionSyntaxNode(
			name,
			extends,
			memberFunctionDefinitions,
			memberVariableDefinitions,
			enumDefinitions,
			staticFunctionDefinitions,
			staticVariableDefinitions,
			innerClasses,
			false,
			fileName
		);
	}

	std::vector<FunctionDefinitionSyntaxNode*> parseMethods(const XMLTag* methodsTag)
	{
		std::vector<FunctionDefinitionSyntaxNode*> methods;

		for (const auto& [tagName, tags] : methodsTag->children)
		{
			for (const auto* tag : tags)
			{
				if (tagName == "method")
				{
					auto method = parseMethod(tag);
					if (method) methods.push_back(method);
				}
			}
		}

		return methods;
	}

	FunctionDefinitionSyntaxNode* parseMethod(const XMLTag* methodTag)
	{
		Type* returnType = nullptr;
		std::vector<VariableDefinitionSyntaxNode*> args;

		for (const auto& [tagName, tags] : methodTag->children)
		{
			for (const auto* tag : tags)
			{
				if (tagName == "return")
				{
					const auto* typeToken = tag->getProperty("type");
					if (typeToken) returnType = new Type(typeToken->value);
				}
				else if (tagName == "param")
				{
					args.push_back(new VariableDefinitionSyntaxNode(
						tag->getProperty("name"),
						new Type(tag->getProperty("type")->value),
						nullptr,
						false,
						false,
						false,
						nullptr,
						nullptr
					));
				}
			}
		}

		FunctionPrototypeSyntaxNode* prototype = new FunctionPrototypeSyntaxNode(
			methodTag->getProperty("name"),
			args,
			returnType,
			false
		);

		return new FunctionDefinitionSyntaxNode(
			prototype,
			new BodySyntaxNode({})
		);
	}

	std::vector<VariableDefinitionSyntaxNode*> parseMembers(const XMLTag* membersTag)
	{
		std::vector<VariableDefinitionSyntaxNode*> members;

		for (const auto& [tagName, tags] : membersTag->children)
		{
			for (const auto* tag : tags)
			{
				if (tagName == "member")
				{
					auto member = parseMember(tag);
					if (member) members.push_back(member);
				}
			}
		}

		return members;
	}

	VariableDefinitionSyntaxNode* parseMember(const XMLTag* memberTag)
	{
		return new VariableDefinitionSyntaxNode(
			memberTag->getProperty("name"),
			new Type(memberTag->getProperty("type")->value),
			nullptr,
			false,
			true,
			false,
			memberTag->getProperty("getter"),
			memberTag->getProperty("setter")
		);
	}
};
