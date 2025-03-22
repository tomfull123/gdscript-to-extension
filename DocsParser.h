#pragma once

#include <vector>
#include "XMLLexer.h"
#include "ParserError.h"
#include "XMLTokenStream.h"
#include "AbstractSyntaxTree.h"
#include "SyntaxNode.h"
#include "FunctionDefinitionSyntaxNode.h"
#include "GDParser.h"

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
			{
				const auto* nextToken = peek(1);
				if (nextToken->type == XMLTokenType::Identifier && nextToken->value == "class")
				{
					ast->classes.push_back(parseClass(fileName));
				}
				else
				{
					next();
				}
				break;
			}
			default:
				next();
			}
		}
	}

	const std::vector<ParserError>& getErrors() const { return errors_; }

	static Result* parse(const std::string& input, AbstractSyntaxTree* ast, const std::string& fileName)
	{
		XMLLexer lexer(input);

		std::vector<XMLToken*> tokens = lexer.readAllTokens();

		DocsParser parser(tokens);

		parser.buildAST(ast, fileName);

		const auto& errors = parser.getErrors();

		return new Result(ast, errors);
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

	void skipTag(const std::string& tagName)
	{
		consume(XMLTokenType::OpenAngleBracketSeparator); // eat <

		consume(XMLTokenType::Identifier); // eat tag name

		while (!isNextTokenType(XMLTokenType::CloseAngleBracketSeparator) && !isNextTokenType(XMLTokenType::EmptyTagEndSeparator))
		{
			XMLToken* identifierToken = consume(XMLTokenType::Identifier);
			if (identifierToken)
			{
				consume(XMLTokenType::EqualsSeparator); // eat =

				consume(XMLTokenType::StringLiteral);
			}
		}

		if (next()->type == XMLTokenType::EmptyTagEndSeparator) return;

		while (isNotEndTag(tagName))
		{
			next();
		}

		consumeEndTag();
	}

	void consumeEndTag()
	{
		consume(XMLTokenType::ClosingTagStartSeparator); // </

		consume(XMLTokenType::Identifier); // eat tag name

		consume(XMLTokenType::CloseAngleBracketSeparator); // eat >
	}

	bool isNotEndTag(const std::string& tagName)
	{
		return (!isNextTokenType(XMLTokenType::ClosingTagStartSeparator) || peek(1)->value != tagName) && !end();
	}

	Token* peekTag()
	{
		const auto* t1 = peek();
		if (t1->type == XMLTokenType::OpenAngleBracketSeparator)
		{
			auto* t2 = peek(1);

			if (t2->type == XMLTokenType::Identifier)
			{
				return t2;
			}
		}
		return nullptr;
	}

	std::unordered_map<std::string, XMLToken*> parseTagProperties()
	{
		consume(XMLTokenType::OpenAngleBracketSeparator); // eat <

		consume(XMLTokenType::Identifier); // eat tag name

		std::unordered_map<std::string, XMLToken*> properties;

		while (!isNextTokenType(XMLTokenType::CloseAngleBracketSeparator) && !isNextTokenType(XMLTokenType::EmptyTagEndSeparator))
		{
			XMLToken* identifierToken = consume(XMLTokenType::Identifier);
			if (identifierToken)
			{
				consume(XMLTokenType::EqualsSeparator); // eat =

				properties[identifierToken->value] = consume(XMLTokenType::StringLiteral);
			}
		}

		next(); // eat > or />

		return properties;
	}

	ClassDefinitionSyntaxNode* parseClass(const std::string& fileName)
	{
		XMLToken* name = nullptr;
		XMLToken* extends = nullptr;

		auto properties = parseTagProperties();

		name = properties["name"];
		extends = properties["inherits"];

		std::vector<FunctionDefinitionSyntaxNode*> memberFunctionDefinitions;
		std::vector<VariableDefinitionSyntaxNode*> memberVariableDefinitions;
		std::vector<EnumDefinitionSyntaxNode*> enumDefinitions;
		std::vector<FunctionDefinitionSyntaxNode*> staticFunctionDefinitions;
		std::vector<VariableDefinitionSyntaxNode*> staticVariableDefinitions;
		std::vector<ClassDefinitionSyntaxNode*> innerClasses;

		while (isNotEndTag("class"))
		{
			const auto* tag = peekTag();
			if (tag)
			{
				const auto& tagName = tag->value;

				// if (tagName == "constructors")
				if (tagName == "methods")
				{
					memberFunctionDefinitions = parseMethods();
				}
				else
				{
					skipTag(tagName);
				}
			}
			else
			{
				addUnexpectedNextTokenError();
			}
		}

		consumeEndTag();

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

	std::vector<FunctionDefinitionSyntaxNode*> parseMethods()
	{
		consume(XMLTokenType::OpenAngleBracketSeparator); // eat <

		consume(XMLTokenType::Identifier); // eat methods

		consume(XMLTokenType::CloseAngleBracketSeparator); // eat >

		std::vector<FunctionDefinitionSyntaxNode*> methods;

		while (isNotEndTag("methods"))
		{
			const auto* tag = peekTag();

			if (tag)
			{
				const auto& tagName = tag->value;

				if (tagName == "method")
				{
					auto method = parseMethod();
					if (method) methods.push_back(method);
				}
				else
				{
					skipTag(tagName);
				}
			}
			else
			{
				addUnexpectedNextTokenError();
			}
		}

		consumeEndTag();

		return methods;
	}

	FunctionDefinitionSyntaxNode* parseMethod()
	{
		auto properties = parseTagProperties();

		Type* returnType = nullptr;
		std::vector<VariableDefinitionSyntaxNode*> args;

		while (isNotEndTag("method"))
		{
			const auto* tag = peekTag();

			if (tag)
			{
				const auto& tagName = tag->value;

				if (tagName == "return")
				{
					auto returnTag = parseTagProperties();
					returnType = new Type(returnTag["type"]->value);
				}
				else if (tagName == "param")
				{
					auto paramTag = parseTagProperties();
					args.push_back(new VariableDefinitionSyntaxNode(
						paramTag["name"],
						new Type(paramTag["type"]->value),
						nullptr,
						false,
						true,
						false
					));
				}
				else
				{
					skipTag(tagName);
				}
			}
			else
			{
				addUnexpectedNextTokenError();
			}
		}

		consumeEndTag();

		FunctionPrototypeSyntaxNode* prototype = new FunctionPrototypeSyntaxNode(properties["name"], args, returnType, false);

		return new FunctionDefinitionSyntaxNode(
			prototype,
			new BodySyntaxNode({})
		);
	}
};
