#pragma once

#include <vector>
#include "XMLParser.h"
#include "ParserError.h"
#include "XMLTokenStream.h"
#include "AbstractSyntaxTree.h"
#include "SyntaxNode.h"
#include "FunctionDefinitionSyntaxNode.h"
#include "ConstantValueMapping.h"
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
		XMLLexer lexer(input, fileName);

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
		std::unordered_map<std::string, std::vector<XMLToken*>> enumValues;
		std::vector<FunctionDefinitionSyntaxNode*> staticFunctionDefinitions;
		std::vector<VariableDefinitionSyntaxNode*> staticVariableDefinitions;
		std::vector<ClassDefinitionSyntaxNode*> innerClasses;
		std::vector<ConstantValueMapping*> constantValueMappings;

		for (const auto& [tagName, tags] : classTag->children)
		{
			if (tags.empty()) continue;
			if (tagName == "methods")
			{
				memberFunctionDefinitions = parseMethods(tags[0]);
			}
			else if (tagName == "members")
			{
				memberVariableDefinitions = parseMembers(tags[0]);
			}
			else if (tagName == "constants")
			{
				const auto constantsTag = tags[0];

				for (const auto& [constantsChildTagName, constantsChildTags] : constantsTag->children)
				{
					if (constantsChildTagName == "constant")
					{
						for (const auto constantTag : constantsChildTags)
						{
							auto enumToken = constantTag->getProperty("enum");

							if (enumToken != nullptr)
							{
								if (!enumValues.contains(enumToken->value))
									enumValues[enumToken->value] = {};
								enumValues[enumToken->value].push_back(constantTag->getProperty("name"));
							}
							else
							{
								constantValueMappings.push_back(new ConstantValueMapping(constantTag->getProperty("name"), constantTag->getProperty("value")));
							}
						}
					}
				}
			}
		}

		std::vector<EnumDefinitionSyntaxNode*> enumDefinitions;

		for (const auto& [enumName, values] : enumValues)
		{
			std::vector<EnumValueSyntaxNode*> enumValueNodes;
			for (const auto& value : values)
			{
				enumValueNodes.push_back(new EnumValueSyntaxNode(value));
			}
			enumDefinitions.push_back(new EnumDefinitionSyntaxNode(new Token(enumName), enumValueNodes));
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
			constantValueMappings,
			false,
			fileName,
			true
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
