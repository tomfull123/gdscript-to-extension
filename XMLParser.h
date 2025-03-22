#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include "XMLTokenStream.h"
#include "ParserError.h"

struct XMLTag
{
	std::unordered_map<std::string, XMLToken*> properties;
	std::unordered_map<std::string, std::vector<XMLTag*>> children;

	XMLToken* getProperty(const std::string& name) const
	{
		if (properties.contains(name))
			return properties.find(name)->second;
		return nullptr;
	}
};

class XMLParser
{
public:
	explicit XMLParser(const std::vector<XMLToken*>& tokens) :
		stream_(tokens)
	{
	}

	XMLTag* build()
	{
		XMLTag* root = new XMLTag();

		while (!end())
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
				if (nextToken->type == XMLTokenType::Identifier)
				{
					const auto& tagName = nextToken->value;
					root->children[tagName] = {};
					root->children[tagName].push_back(parseTag(tagName));
				}
				else
				{
					addUnexpectedNextTokenError();
				}
				break;
			}
			default:
				addUnexpectedNextTokenError();
			}
		}

		return root;
	}

	const std::vector<ParserError>& getErrors() const { return errors_; }

private:
	XMLTokenStream stream_;
	std::vector<ParserError> errors_;

	bool isNextTokenType(XMLTokenType type, unsigned int offset = 0)
	{
		if (offset >= stream_.getLength()) return false;
		return stream_.peek(offset)->type == type;
	}

	XMLToken* addError(const std::string& error, const XMLToken* token)
	{
		if (token)
			errors_.emplace_back(error, token->lineNumber, token->columnNumber, token->filename);
		else
			errors_.emplace_back(error, stream_.getLastTokenLineNumber(), stream_.getLastTokenColumnNumber(), "");

		if (stream_.end()) return nullptr;

		return nullptr;
	}

	XMLToken* addUnexpectedTokenError(const XMLToken* token)
	{
		if (token == nullptr) return addError("Unexpected end of file", nullptr);
		return addError("Unexpected token: " + token->value, token);
	}

	XMLToken* addUnexpectedNextTokenError()
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

	bool isNotEndTag(const std::string& tagName)
	{
		return (!isNextTokenType(XMLTokenType::ClosingTagStartSeparator) || peek(1)->value != tagName) && !end();
	}

	XMLTag* parseTag(const std::string& tagName)
	{
		consume(XMLTokenType::OpenAngleBracketSeparator); // eat <

		consume(XMLTokenType::Identifier); // eat tag name

		XMLTag* tag = new XMLTag();

		while (!isNextTokenType(XMLTokenType::CloseAngleBracketSeparator) && !isNextTokenType(XMLTokenType::EmptyTagEndSeparator))
		{
			XMLToken* identifierToken = consume(XMLTokenType::Identifier);
			if (identifierToken)
			{
				consume(XMLTokenType::EqualsSeparator); // eat =

				tag->properties[identifierToken->value] = consume(XMLTokenType::StringLiteral);
			}
		}

		if (isNextTokenType(XMLTokenType::EmptyTagEndSeparator))
		{
			consume(XMLTokenType::EmptyTagEndSeparator); // eat />
			return tag;
		}

		next(); // eat >

		while (isNotEndTag(tagName))
		{
			if (isNextTokenType(XMLTokenType::OpenAngleBracketSeparator))
			{
				const auto* nextToken = peek(1);
				if (nextToken->type == XMLTokenType::Identifier)
				{
					const auto& childTagName = nextToken->value;
					if (!tag->children.contains(childTagName)) tag->children[childTagName] = {};
					tag->children[childTagName].push_back(parseTag(childTagName));
				}
				else
				{
					addUnexpectedNextTokenError();
				}
			}
			else addUnexpectedNextTokenError();
		}

		consume(XMLTokenType::ClosingTagStartSeparator); // </

		consume(XMLTokenType::Identifier); // eat tag name

		consume(XMLTokenType::CloseAngleBracketSeparator); // eat >

		return tag;
	}
};
