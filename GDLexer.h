#pragma once

#include <vector>
#include <cwctype>
#include "Lexer.h"

enum class GDTokenType
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

struct GDToken : public Token
{
	GDTokenType type;
	GDToken() = default;
	explicit GDToken(const std::string& newValue) :
		Token(newValue) {
	}
};

class GDLexer : public Lexer
{
public:
	explicit GDLexer(const std::string& input, const std::string& filename) :
		Lexer(input, filename)
	{
	}

	std::vector<GDToken*> readAllTokens()
	{
		std::vector<GDToken*> tokens;

		while (!end())
		{
			GDToken* t = readNext();
			if (t == nullptr) continue;
			tokens.push_back(t);
		}

		return tokens;
	}

private:
	int currentIndent_ = 0;

	GDToken* readNext()
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
		return std::iswalpha(ch) || ch == '_';
	}

	static bool isIdentifier(const char& ch)
	{
		return std::iswalpha(ch) || ch == '_' || isdigit(ch);
	}

	GDToken* readIdentifierOrKeyword()
	{
		GDToken* token = readToken(isIdentifier, GDTokenType::Identifier);

		token->type = getKeywordTokenType(token->value);
		token->indentDepth = currentIndent_;

		return token;
	}

	GDTokenType getKeywordTokenType(const std::string& value) const
	{
		if (value == "func") return GDTokenType::FuncKeyword;
		if (value == "var") return GDTokenType::VarKeyword;
		if (value == "const") return GDTokenType::ConstKeyword;
		if (value == "class_name") return GDTokenType::ClassNameKeyword;
		if (value == "extends") return GDTokenType::ExtendsKeyword;
		if (value == "return") return GDTokenType::ReturnKeyword;
		if (value == "for") return GDTokenType::ForKeyword;
		if (value == "if") return GDTokenType::IfKeyword;
		if (value == "else") return GDTokenType::ElseKeyword;
		if (value == "true") return GDTokenType::TrueKeyword;
		if (value == "false") return GDTokenType::FalseKeyword;
		if (value == "class") return GDTokenType::ClassKeyword;
		if (value == "null") return GDTokenType::NullKeyword;
		//if (value == "get") return TokenType::GetKeyword;
		if (value == "while") return GDTokenType::WhileKeyword;
		if (value == "enum") return GDTokenType::EnumKeyword;
		if (value == "static") return GDTokenType::StaticKeyword;
		if (value == "break") return GDTokenType::BreakKeyword;
		if (value == "continue") return GDTokenType::ContinueKeyword;
		if (value == "pass") return GDTokenType::PassKeyword;
		if (value == "signal") return GDTokenType::SignalKeyword;
		if (value == "preload") return GDTokenType::PreloadKeyword;
		if (value == "in") return GDTokenType::InKeyword;
		if (value == "is") return GDTokenType::IsKeyword;
		if (value == "or") return GDTokenType::OrOperator;
		if (value == "not") return GDTokenType::NotOperator;
		if (value == "range") return GDTokenType::RangeKeyword;
		if (value == "as") return GDTokenType::AsKeyword;

		return GDTokenType::Identifier;
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

	GDToken* readNumberLiteral()
	{
		GDToken* token = new GDToken();
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
			token->type = GDTokenType::FloatLiteral;
		else
			token->type = GDTokenType::IntLiteral;

		token->indentDepth = currentIndent_;
		token->filename = filename_;

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

	GDToken* readSeparator()
	{
		GDToken* token = new GDToken();

		token->lineNumber = inputStream_.getLineNumber();
		token->columnNumber = inputStream_.getColumnNumber();

		token->indentDepth = currentIndent_;
		token->filename = filename_;

		token->value = inputStream_.next();
		const auto& value = token->value;

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

		if (token->type == GDTokenType::SingleLineComment)
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

	GDToken* readNodePath()
	{
		inputStream_.next(); // eat $

		return readToken(isIdentifier, GDTokenType::NodePath);
	}

	static bool isAnnotation(const char& ch)
	{
		return isalpha(ch) || ch == '_';
	}

	GDToken* readAnnotation()
	{
		inputStream_.next(); // eat @

		GDToken* token = readToken(isAnnotation, GDTokenType::Annotation);

		return token;
	}

	GDTokenType getSeparatorTokenType(const std::string& value) const
	{
		if (value == ":") return GDTokenType::ColonSeparator;
		if (value == "->") return GDTokenType::ArrowSeparator;
		if (value == "(") return GDTokenType::OpenBracketSeparator;
		if (value == ")") return GDTokenType::CloseBracketSeparator;
		if (value == "{") return GDTokenType::OpenCurlyBracketSeparator;
		if (value == "}") return GDTokenType::CloseCurlyBracketSeparator;
		if (value == "[") return GDTokenType::OpenSquareBracket;
		if (value == "]") return GDTokenType::CloseSquareBracket;
		if (value == ",") return GDTokenType::CommaSeparator;
		if (value == "<") return GDTokenType::LessThanSeparator;
		if (value == "<=") return GDTokenType::LessThanEqualSeparator;
		if (value == ">") return GDTokenType::GreaterThanSeparator;
		if (value == ">=") return GDTokenType::GreaterThanEqualSeparator;
		if (value == ".") return GDTokenType::DotSeparator;
		if (value == "!") return GDTokenType::NotOperator;
		if (value == "=") return GDTokenType::AssignmentOperator;
		if (value == "==") return GDTokenType::EqualityOperator;
		if (value == "!=") return GDTokenType::NotEqualityOperator;
		if (value == "+") return GDTokenType::Operator;
		if (value == "-") return GDTokenType::Operator;
		if (value == "/") return GDTokenType::Operator;
		if (value == "*") return GDTokenType::Operator;
		if (value == "+=") return GDTokenType::Operator;
		if (value == "++") return GDTokenType::Operator;
		if (value == "-=") return GDTokenType::Operator;
		if (value == "--") return GDTokenType::Operator;
		if (value == "&&") return GDTokenType::AndOperator;
		if (value == "||") return GDTokenType::OrOperator;
		if (value == "|") return GDTokenType::LogicalOrOperator;
		if (value == "|=") return GDTokenType::LogicalOrAssignOperator;
		if (value == "#") return GDTokenType::SingleLineComment;
		if (value == "\"\"\"") return GDTokenType::MultiLineComment;

		return GDTokenType::Error;
	}

	GDToken* readError()
	{
		GDToken* token = new GDToken();

		token->lineNumber = inputStream_.getLineNumber();
		token->columnNumber = inputStream_.getColumnNumber();

		token->value = inputStream_.readUntil(isWhitespace, false);
		token->type = GDTokenType::Error;

		token->indentDepth = currentIndent_;
		token->filename = filename_;

		return token;
	}

	template<class Function>
	GDToken* readToken(const Function& function, GDTokenType type)
	{
		GDToken* token = new GDToken();

		token->lineNumber = inputStream_.getLineNumber();
		token->columnNumber = inputStream_.getColumnNumber();

		token->value = inputStream_.readWhile(function);
		token->type = type;

		token->indentDepth = currentIndent_;
		token->filename = filename_;

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

	GDToken* readStringLiteral()
	{
		GDToken* token = new GDToken();

		token->lineNumber = inputStream_.getLineNumber();
		token->columnNumber = inputStream_.getColumnNumber();
		token->filename = filename_;

		inputStream_.next(); // eat "

		token->value = inputStream_.readUntil(isStringLiteral, false);
		unescape(token->value);
		token->type = GDTokenType::StringLiteral;

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
