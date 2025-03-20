#pragma once

#include <string>
#include <vector>
#include "InputStream.h"

enum class TokenType
{
	Identifier,
	Error,

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
	SignalKeyword,
	PreloadKeyword,
	InKeyword,
	IsKeyword,
	RangeKeyword,
	AsKeyword,

	FloatLiteral,
	IntLiteral,
	StringLiteral,

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
	LogicalOrOperator,
	LogicalOrAssignOperator,
	SingleLineComment,
	MultiLineComment,
	Annotation,

	NodePath,
};

struct Token
{
	std::string value;
	TokenType type;
	int lineNumber = -1;
	int columnNumber = -1;
	std::string filename = "";
	int indentDepth = -1;
	Token() = default;
	explicit Token(const std::string& newValue) :
		value(newValue) {
	}
};

class GDLexer
{
public:
	explicit GDLexer(const std::string& input) :
		inputStream_(input)
	{
	}

	bool end() const
	{
		return inputStream_.eof();
	}

	Token* readNext()
	{
		inputStream_.readWhile(isWhitespace);

		if (isNewLine(inputStream_.peek())) readIndents();

		inputStream_.readWhile(isWhitespace);

		if (end()) return nullptr;

		char ch = inputStream_.peek();
		//std::cout << "parsing character: " << ch << ", Line: " << inputStream_.getLineNumber() << std::endl;

		if (isIdentifierStart(ch)) return readIdentifierOrKeyword();

		if (isStringLiteralStart(ch)) return readStringLiteral();

		if (isNumberLiteralStart(ch)) return readNumberLiteral();

		//if (isCharLiteralStart(ch)) return readCharLiteral();

		if (isNodePathStart(ch)) return readNodePath();

		if (isSeparator(ch)) return readSeparator();

		if (isAnnotationStart(ch)) return readAnnotation();

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

	const std::vector<char> separators = {
		'(',
		')',
		'[',
		']',
		'{',
		'}',
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
		'#',
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
		token->indentDepth = currentIndent_;

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
		if (value == "signal") return TokenType::SignalKeyword;
		if (value == "preload") return TokenType::PreloadKeyword;
		if (value == "in") return TokenType::InKeyword;
		if (value == "is") return TokenType::IsKeyword;
		if (value == "or") return TokenType::OrOperator;
		if (value == "not") return TokenType::NotOperator;
		if (value == "range") return TokenType::RangeKeyword;
		if (value == "as") return TokenType::AsKeyword;

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

		token->indentDepth = currentIndent_;

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

		token->indentDepth = currentIndent_;

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
		else if (value == "|") // ||, |=
		{
			if (inputStream_.peek() == '|') token->value += inputStream_.next();
			else if (inputStream_.peek() == '=') token->value += inputStream_.next();
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
			inputStream_.readUntil(isNewLine);
			return nullptr;
		}
		/*if (token->type == TokenType::MultiLineComment)
		{
			readUntil('*', '/');
			return nullptr;
		}*/

		return token;
	}

	static bool isNodePathStart(const char& ch)
	{
		return ch == '$';
	}

	Token* readNodePath()
	{
		inputStream_.next(); // eat $

		return readToken(isIdentifier, TokenType::NodePath);
	}

	static bool isAnnotation(const char& ch)
	{
		return isalpha(ch) || ch == '_';
	}

	Token* readAnnotation()
	{
		inputStream_.next(); // eat @

		Token* token = readToken(isAnnotation, TokenType::Annotation);

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
		if (value == "|") return TokenType::LogicalOrOperator;
		if (value == "|=") return TokenType::LogicalOrAssignOperator;
		if (value == "#") return TokenType::SingleLineComment;
		if (value == "\"\"\"") return TokenType::MultiLineComment;

		return TokenType::Error;
	}

	Token* readError()
	{
		Token* token = new Token();

		token->lineNumber = inputStream_.getLineNumber();
		token->columnNumber = inputStream_.getColumnNumber();

		token->value = inputStream_.readUntil(isWhitespace, false);
		token->type = TokenType::Error;

		token->indentDepth = currentIndent_;

		return token;
	}

	template<class Function>
	Token* readToken(const Function& function, TokenType type)
	{
		Token* token = new Token();

		token->lineNumber = inputStream_.getLineNumber();
		token->columnNumber = inputStream_.getColumnNumber();

		token->value = inputStream_.readWhile(function);
		token->type = type;

		token->indentDepth = currentIndent_;

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

		currentIndent_ = indent;
	}

	Token* readStringLiteral()
	{
		Token* token = new Token();

		token->lineNumber = inputStream_.getLineNumber();
		token->columnNumber = inputStream_.getColumnNumber();

		inputStream_.next(); // eat "

		token->value = inputStream_.readUntil(isStringLiteral, false);
		unescape(token->value);
		token->type = TokenType::StringLiteral;

		inputStream_.next(); // eat "

		return token;
	}

	static bool isWhitespace(const char& ch)
	{
		return ch == ' ';
	}

	static bool isAnnotationStart(const char& ch)
	{
		return ch == '@';
	}

	void unescape(std::string& str) const
	{
		replaceAll(str, "\\n", "\n");
		replaceAll(str, "\\t", "\t");
	}

	void replaceAll(std::string& str, const std::string& from, const std::string& to) const
	{
		if (from.empty())
			return;
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}
};
