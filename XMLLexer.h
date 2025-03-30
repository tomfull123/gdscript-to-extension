#pragma once

#include <vector>
#include "Lexer.h"

enum class XMLTokenType
{
	Identifier,
	Error,

	StringLiteral,

	OpenAngleBracketSeparator, // <
	CloseAngleBracketSeparator, // >
	ClosingTagStartSeparator, // </
	EmptyTagEndSeparator, // />
	EqualsSeparator, // =
	DeclarationStartSeparator, // <?
	DeclarationEndSeparator, // ?>
};

struct XMLToken : public Token
{
	XMLTokenType type;
};

class XMLLexer : public Lexer
{
public:
	explicit XMLLexer(const std::string& input, const std::string& filename) :
		Lexer(input, filename)
	{
	}

	std::vector<XMLToken*> readAllTokens()
	{
		std::vector<XMLToken*> tokens;

		while (!end())
		{
			XMLToken* t = readNext();
			if (t == nullptr) continue;
			tokens.push_back(t);
		}

		return tokens;
	}

private:
	XMLToken* readNext()
	{
		inputStream_.readWhile(isWhitespace);

		if (end()) return nullptr;

		char ch = inputStream_.peek();

		if (isIdentifierStart(ch)) return readIdentifier();

		if (isStringLiteralStart(ch)) return readStringLiteral();

		if (isSeparator(ch)) return readSeparator();

		return readError();
	}

	const std::vector<char> separators = {
		'<',
		'>',
		'=',
		'?',
		'/',
	};

	static bool isWhitespace(const char& ch)
	{
		return ch == ' ' || ch == '\n' || ch == '\t';
	}

	static bool isIdentifierStart(const char& ch)
	{
		return isalpha(ch);
	}

	static bool isIdentifier(const char& ch)
	{
		return isalpha(ch) || ch == ':' || ch == '_';
	}

	static bool isStringLiteralStart(const char& ch)
	{
		return ch == '"';
	}

	static bool isStringLiteralEnd(const char& ch)
	{
		return ch == '"';
	}

	static bool isTagStart(const char& ch)
	{
		return ch == '<';
	}

	bool isSeparator(const char& ch) const
	{
		for (const auto& s : separators)
		{
			if (s == ch) return true;
		}

		return false;
	}

	XMLToken* readSeparator()
	{
		XMLToken* token = new XMLToken();

		token->lineNumber = inputStream_.getLineNumber();
		token->columnNumber = inputStream_.getColumnNumber();
		token->filepath = filepath_;

		token->value = inputStream_.next();
		const auto& value = token->value;

		if (value == "<")
		{
			if (inputStream_.peek() == '?') token->value += inputStream_.next();
			else if (inputStream_.peek() == '/') token->value += inputStream_.next();
		}
		else if (value == "?")
		{
			if (inputStream_.peek() == '>') token->value += inputStream_.next();
		}
		else if (value == "/")
		{
			if (inputStream_.peek() == '>') token->value += inputStream_.next();
		}

		token->type = getSeparatorTokenType(token->value);

		if (token->type == XMLTokenType::CloseAngleBracketSeparator)
		{
			inputStream_.readUntil(isTagStart);
		}

		return token;
	}

	XMLTokenType getSeparatorTokenType(const std::string& value) const
	{
		if (value == "=") return XMLTokenType::EqualsSeparator;
		if (value == "<") return XMLTokenType::OpenAngleBracketSeparator;
		if (value == ">") return XMLTokenType::CloseAngleBracketSeparator;
		if (value == "<?") return XMLTokenType::DeclarationStartSeparator;
		if (value == "?>") return XMLTokenType::DeclarationEndSeparator;
		if (value == "</") return XMLTokenType::ClosingTagStartSeparator;
		if (value == "/>") return XMLTokenType::EmptyTagEndSeparator;

		return XMLTokenType::Error;
	}

	XMLToken* readIdentifier()
	{
		XMLToken* token = readToken(isIdentifier, XMLTokenType::Identifier);

		token->type = XMLTokenType::Identifier;

		return token;
	}

	XMLToken* readStringLiteral()
	{
		XMLToken* token = new XMLToken();

		token->lineNumber = inputStream_.getLineNumber();
		token->columnNumber = inputStream_.getColumnNumber();
		token->filepath = filepath_;

		inputStream_.next(); // eat "

		token->value = inputStream_.readUntil(isStringLiteralEnd, false);
		token->type = XMLTokenType::StringLiteral;

		inputStream_.next(); // eat "

		return token;
	}

	XMLToken* readError()
	{
		XMLToken* token = new XMLToken();

		token->lineNumber = inputStream_.getLineNumber();
		token->columnNumber = inputStream_.getColumnNumber();

		token->value = inputStream_.readUntil(isWhitespace, false);
		token->type = XMLTokenType::Error;
		token->filepath = filepath_;

		return token;
	}

	template<class Function>
	XMLToken* readToken(const Function& function, XMLTokenType type)
	{
		XMLToken* token = new XMLToken();

		token->lineNumber = inputStream_.getLineNumber();
		token->columnNumber = inputStream_.getColumnNumber();

		token->value = inputStream_.readWhile(function);
		token->type = type;
		token->filepath = filepath_;

		return token;
	}
};