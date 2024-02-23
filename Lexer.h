#pragma once

#include <string>
#include <vector>
#include "InputStream.h"

enum class TokenType
{
	Identifier,
	Error,
	EndOfBlock,

	FuncKeyword,
	VarKeyword,
	ConstKeyword,
	ClassNameKeyword,
	ExtendsKeyword,
	ReturnKeyword,
	ForKeyword,
	IfKeyword,
	ElseKeyword,
	TrueKeyword,
	FalseKeyword,
	ClassKeyword,
	NullKeyword,
	WhileKeyword,
	EnumKeyword,
	StaticKeyword,
	BreakKeyword,
	ContinueKeyword,
	PassKeyword,

	FloatLiteral,
	IntLiteral,

	ColonSeparator,
	ArrowSeparator,
	OpenBracketSeparator,
	CloseBracketSeparator,
	OpenCurlyBracketSeparator,
	CloseCurlyBracketSeparator,
	OpenSquareBracket,
	CloseSquareBracket,
	CommaSeparator,
	LessThanSeparator,
	LessThanEqualSeparator,
	GreaterThanSeparator,
	GreaterThanEqualSeparator,
	DotSeparator,
	NotOperator,
	AssignmentOperator,
	EqualityOperator,
	NotEqualityOperator,
	Operator,
	AndOperator,
	OrOperator,
	SingleLineComment,
	MultiLineComment,
};

struct Token
{
	std::string value;
	TokenType type;
	int lineNumber = -1;
	int columnNumber = -1;
	std::string filename = "";
};

class Lexer
{
public:
	explicit Lexer(const std::string& input) :
		inputStream_(input)
	{}

	bool end() const
	{
		return inputStream_.eof();
	}

	Token* readNext()
	{
		readWhile(isWhitespace);

		if (isNewLine(inputStream_.peek())) readIndents();

		if (pendingIndent_ > 0) return createIndent();

		if (end()) return nullptr;

		char ch = inputStream_.peek();
		//std::cout << "parsing character: " << ch << ", Line: " << inputStream_.getLineNumber() << std::endl;

		if (isIdentifierStart(ch)) return readIdentifierOrKeyword();

		//if (isStringLiteralStart(ch)) return readStringLiteral();

		if (isNumberLiteralStart(ch)) return readNumberLiteral();

		//if (isCharLiteralStart(ch)) return readCharLiteral();

		if (isSeparator(ch)) return readSeparator();

		return readError();
	}

	std::vector<Token*> readAllTokens()
	{
		std::vector<Token*> tokens;

		while (!end())
		{
			Token* t = readNext();
			if (t == nullptr) continue;
			tokens.push_back(t);
		}

		return tokens;
	}

private:
	InputStream inputStream_;
	int currentIndent_ = 0;
	int pendingIndent_ = 0;

	const std::vector<char> separators = {
		'(',
		')',
		'[',
		']',
		':',
		',',
		'?',
		'!',
		'=',
		'.',
		'<',
		'>',
		'+',
		'-',
		'/',
		'*',
		'&',
		'|',
	};

	static bool isIdentifierStart(const char& ch)
	{
		return isalpha(ch) || ch == '_';
	}

	static bool isIdentifier(const char& ch)
	{
		return isalpha(ch) || ch == '_' || isdigit(ch);
	}

	Token* readIdentifierOrKeyword()
	{
		Token* token = readToken(isIdentifier, TokenType::Identifier);

		token->type = getKeywordTokenType(token->value);

		return token;
	}

	TokenType getKeywordTokenType(const std::string& value) const
	{
		if (value == "func") return TokenType::FuncKeyword;
		if (value == "var") return TokenType::VarKeyword;
		if (value == "const") return TokenType::ConstKeyword;
		if (value == "class_name") return TokenType::ClassNameKeyword;
		if (value == "extends") return TokenType::ExtendsKeyword;
		if (value == "return") return TokenType::ReturnKeyword;
		if (value == "for") return TokenType::ForKeyword;
		if (value == "if") return TokenType::IfKeyword;
		if (value == "else") return TokenType::ElseKeyword;
		if (value == "true") return TokenType::TrueKeyword;
		if (value == "false") return TokenType::FalseKeyword;
		if (value == "class") return TokenType::ClassKeyword;
		if (value == "null") return TokenType::NullKeyword;
		//if (value == "get") return TokenType::GetKeyword;
		if (value == "while") return TokenType::WhileKeyword;
		if (value == "enum") return TokenType::EnumKeyword;
		if (value == "static") return TokenType::StaticKeyword;
		if (value == "break") return TokenType::BreakKeyword;
		if (value == "continue") return TokenType::ContinueKeyword;
		if (value == "pass") return TokenType::PassKeyword;

		return TokenType::Identifier;
	}

	static bool isStringLiteralStart(const char& ch)
	{
		return isStringLiteral(ch);
	}

	static bool isStringLiteral(const char& ch)
	{
		return ch == '"';
	}

	static bool isNumberLiteralStart(const char& ch)
	{
		return isdigit(ch);
	}

	static bool isNumber(const char& ch)
	{
		return isdigit(ch);
	}

	static bool isDot(const char& ch)
	{
		return ch == '.';
	}

	Token* readNumberLiteral()
	{
		Token* token = new Token();
		bool hasDecimal = false;

		token->lineNumber = inputStream_.getLineNumber();
		token->columnNumber = inputStream_.getColumnNumber();

		std::string str;

		while (!inputStream_.eof())
		{
			char ch = inputStream_.peek();
			bool hasDot = isDot(ch);
			// break if not number and not a dot, unless we already have a dot
			if (!(isNumber(ch) || (hasDot && !hasDecimal))) break;
			if (hasDot)
			{
				char nextCh = inputStream_.peek(1);
				// break if current char is a dot, but the next char isn't a number
				if (!isNumber(nextCh)) break;
				hasDecimal = true;
			}
			str += inputStream_.next();
		}

		token->value = str;
		if (hasDecimal)
			token->type = TokenType::FloatLiteral;
		else
			token->type = TokenType::IntLiteral;

		return token;
	}

	bool isSeparator(const char& ch) const
	{
		for (const auto& s : separators)
		{
			if (s == ch) return true;
		}

		return false;
	}

	Token* readSeparator()
	{
		Token* token = new Token();

		token->lineNumber = inputStream_.getLineNumber();
		token->columnNumber = inputStream_.getColumnNumber();

		token->value = inputStream_.next();
		std::string value = token->value;

		if (value == "<" || value == ">")
		{
			if (inputStream_.peek() == '=') token->value += inputStream_.next();
		}
		else if (value == "=")
		{
			if (inputStream_.peek() == '=') token->value += inputStream_.next();
			else if (inputStream_.peek() == '>') token->value += inputStream_.next();
		}
		else if (value == "!") // !=
		{
			if (inputStream_.peek() == '=') token->value += inputStream_.next();
		}
		else if (value == "&") // &&
		{
			if (inputStream_.peek() == '&') token->value += inputStream_.next();
		}
		else if (value == "|") // ||
		{
			if (inputStream_.peek() == '|') token->value += inputStream_.next();
		}
		else if (value == "+") // +=
		{
			char peek = inputStream_.peek();
			if (peek == '=') token->value += inputStream_.next();
		}
		else if (value == "-") // -+, ->
		{
			char peek = inputStream_.peek();
			if (peek == '=' || peek == '>') token->value += inputStream_.next();
		}

		token->type = getSeparatorTokenType(token->value);

		if (token->type == TokenType::SingleLineComment)
		{
			readUntil(isNewLine);
			return nullptr;
		}
		/*if (token->type == TokenType::MultiLineComment)
		{
			readUntil('*', '/');
			return nullptr;
		}*/

		return token;
	}

	TokenType getSeparatorTokenType(const std::string& value) const
	{
		if (value == ":") return TokenType::ColonSeparator;
		if (value == "->") return TokenType::ArrowSeparator;
		if (value == "(") return TokenType::OpenBracketSeparator;
		if (value == ")") return TokenType::CloseBracketSeparator;
		if (value == "{") return TokenType::OpenCurlyBracketSeparator;
		if (value == "}") return TokenType::CloseCurlyBracketSeparator;
		if (value == "[") return TokenType::OpenSquareBracket;
		if (value == "]") return TokenType::CloseSquareBracket;
		if (value == ",") return TokenType::CommaSeparator;
		if (value == "<") return TokenType::LessThanSeparator;
		if (value == "<=") return TokenType::LessThanEqualSeparator;
		if (value == ">") return TokenType::GreaterThanSeparator;
		if (value == ">=") return TokenType::GreaterThanEqualSeparator;
		if (value == ".") return TokenType::DotSeparator;
		if (value == "!") return TokenType::NotOperator;
		if (value == "=") return TokenType::AssignmentOperator;
		if (value == "==") return TokenType::EqualityOperator;
		if (value == "!=") return TokenType::NotEqualityOperator;
		if (value == "+") return TokenType::Operator;
		if (value == "-") return TokenType::Operator;
		if (value == "/") return TokenType::Operator;
		if (value == "*") return TokenType::Operator;
		if (value == "+=") return TokenType::Operator;
		if (value == "++") return TokenType::Operator;
		if (value == "-=") return TokenType::Operator;
		if (value == "--") return TokenType::Operator;
		if (value == "&&") return TokenType::AndOperator;
		if (value == "||") return TokenType::OrOperator;
		if (value == "#") return TokenType::SingleLineComment;
		if (value == "\"\"\"") return TokenType::MultiLineComment;

		return TokenType::Error;
	}

	Token* readError()
	{
		Token* token = new Token();

		token->lineNumber = inputStream_.getLineNumber();
		token->columnNumber = inputStream_.getColumnNumber();

		token->value = readUntil(isWhitespace, false);
		token->type = TokenType::Error;

		return token;
	}

	template<class Function>
	Token* readToken(const Function& function, TokenType type)
	{
		Token* token = new Token();

		token->lineNumber = inputStream_.getLineNumber();
		token->columnNumber = inputStream_.getColumnNumber();

		token->value = readWhile(function);
		token->type = type;

		return token;
	}

	Token* createIndent()
	{
		pendingIndent_--;

		Token* token = new Token();

		token->type = TokenType::EndOfBlock;

		return token;
	}

	void readIndents()
	{
		int indent = 0;

		while (isNewLine(inputStream_.peek()) || inputStream_.peek() == '\t')
		{
			auto value = inputStream_.next();

			if (value == '\t') indent++;
			else indent = 0;
		}

		if (indent < currentIndent_)
		{
			pendingIndent_ = currentIndent_ - indent;
		}

		currentIndent_ = indent;
	}

	template<class Function>
	std::string readWhile(const Function& function)
	{
		std::string str;

		while (!inputStream_.eof() && function(inputStream_.peek()))
		{
			str += inputStream_.next();
		}

		return str;
	}

	template<class Function>
	std::string readUntil(const Function& function, bool includeLast = false)
	{
		std::string str;

		while (!inputStream_.eof())
		{
			bool matches = function(inputStream_.peek());
			if (matches && !includeLast) break;
			str += inputStream_.next();
			if (matches) break;
		}

		return str;
	}

	static bool isWhitespace(const char& ch)
	{
		return ch == ' ';
	}
};
