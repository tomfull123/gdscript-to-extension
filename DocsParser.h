#pragma once

#include <vector>
#include "XMLLexer.h"
#include "ParserError.h"
#include "XMLTokenStream.h"
#include "AbstractSyntaxTree.h"
#include "SyntaxNode.h"
#include "FunctionDefinitionSyntaxNode.h"
#include "ClassDefinitionSyntaxNode.h"

class DocsParser
{
public:
	explicit DocsParser(const std::vector<XMLToken*>& tokens) :
		stream_(tokens)
	{
	}

	void buildAST(AbstractSyntaxTree* ast, const std::string& fileName)
	{
		while (!stream_.end())
		{
			const auto* token = peek();
			switch (token->type)
			{
			case XMLTokenType::DeclarationStartSeparator:
				parseXMLDeclaration();
				break;
			case XMLTokenType::OpenAngleBracketSeparator:
				const auto* nextToken = peek(1);
				if (nextToken->type == XMLTokenType::Identifier && nextToken->value == "class")
				{
					ast->classes.push_back(parseClass(fileName));
				}
			}
		}
	}

	const std::vector<ParserError>& getErrors() const { return errors_; }

	static void parse(const std::string& input, AbstractSyntaxTree* ast, const std::string& fileName)
	{
		XMLLexer lexer(input);

		std::vector<XMLToken*> tokens = lexer.readAllTokens();

		DocsParser parser(tokens);

		parser.buildAST(ast, fileName);
	}

private:
	XMLTokenStream stream_;
	std::vector<ParserError> errors_;

	bool isNextTokenType(XMLTokenType type, unsigned int offset = 0)
	{
		if (offset >= stream_.getLength()) return false;
		return stream_.peek(offset)->type == type;
	}

	SyntaxNode* addError(const std::string& error, const XMLToken* token)
	{
		if (token)
			errors_.emplace_back(error, token->lineNumber, token->columnNumber, token->filename);
		else
			errors_.emplace_back(error, stream_.getLastTokenLineNumber(), stream_.getLastTokenColumnNumber(), "");

		if (stream_.end()) return nullptr;

		return nullptr;
	}

	SyntaxNode* addUnexpectedTokenError(const XMLToken* token)
	{
		if (token == nullptr) return addError("Unexpected end of file", nullptr);
		return addError("Unexpected token: " + token->value, token);
	}

	SyntaxNode* addUnexpectedNextTokenError()
	{
		return addUnexpectedTokenError(next());
	}

	XMLToken* next()
	{
		return stream_.next();
	}

	XMLToken* peek(unsigned int offset = 0)
	{
		return stream_.peek(offset);
	}

	bool end() const
	{
		return stream_.end();
	}

	XMLToken* consume(XMLTokenType tokenType)
	{
		if (isNextTokenType(tokenType))
		{
			return next();
		}

		addUnexpectedNextTokenError();
		return nullptr;
	}

	void parseXMLDeclaration()
	{
		consume(XMLTokenType::DeclarationStartSeparator); // eat <?

		while (!isNextTokenType(XMLTokenType::DeclarationEndSeparator))
		{
			next();
		}

		next(); // eat ?>
	}

	ClassDefinitionSyntaxNode* parseClass(const std::string& fileName)
	{
		consume(XMLTokenType::OpenAngleBracketSeparator); // eat <
		consume(XMLTokenType::Identifier); // eat class

		XMLToken* name = nullptr;
		XMLToken* extends = nullptr;

		while (!isNextTokenType(XMLTokenType::CloseAngleBracketSeparator))
		{
			XMLToken* identifierToken = consume(XMLTokenType::Identifier);
			if (identifierToken)
			{
				consume(XMLTokenType::EqualsSeparator); // eat =

				if (identifierToken->value == "name")
				{
					name = consume(XMLTokenType::StringLiteral);
				}
				else
				{
					next(); // skip value
				}
			}
		}

		next(); // eat >

		std::vector<FunctionDefinitionSyntaxNode*> memberFunctionDefinitions;
		std::vector<VariableDefinitionSyntaxNode*> memberVariableDefinitions;
		std::vector<EnumDefinitionSyntaxNode*> enumDefinitions;
		std::vector<FunctionDefinitionSyntaxNode*> staticFunctionDefinitions;
		std::vector<VariableDefinitionSyntaxNode*> staticVariableDefinitions;
		std::vector<ClassDefinitionSyntaxNode*> innerClasses;

		while (!isNextTokenType(XMLTokenType::ClosingTagStartSeparator) && peek(1)->value != "class")
		{
			next();
		}

		return new ClassDefinitionSyntaxNode(
			new Token(),
			new Token(),
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
};
