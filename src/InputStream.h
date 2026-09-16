#pragma once

#include <string>

static bool isNewLine(const char& ch)
{
	return ch == '\n' || ch == '\r';
}

class InputStream
{
public:
	explicit InputStream(const std::string& input) :
		input_(input)
	{
	}

	char next()
	{
		char front = input_[currentIndex_];
		currentIndex_++;

		if (front == '\t') columnNumber_ += 4;
		else columnNumber_++;

		if (front == '\r' && !eof() && input_[currentIndex_] == '\n') return next();
		if (isNewLine(front))
		{
			columnNumber_ = 1;
			lineNumber_++;
		}
		return front;
	}

	template<class Function>
	std::string readWhile(const Function& function)
	{
		std::string str;

		while (!eof() && function(peek()))
		{
			str += next();
		}

		return str;
	}

	template<class Function>
	std::string readUntil(const Function& function, bool includeLast = false, bool skipEscapedCharacters = false)
	{
		std::string str;
		bool escapeNextChar = false;

		while (!eof())
		{
			char ch = peek();
			bool matches = function(ch);
			if (skipEscapedCharacters)
			{
				if (escapeNextChar)
				{
					escapeNextChar = false;
					matches = false;
				}

				if (ch == '\\') escapeNextChar = true;
			}
			if (matches && !includeLast) break;
			str += next();
			if (matches) break;
		}

		return str;
	}

	char peek(unsigned int offset = 0)
	{
		return input_[currentIndex_ + offset];
	}

	bool eof() const
	{
		return currentIndex_ >= input_.length();
	}

	void croak() const
	{
		throw;
	}

	int getLineNumber() const { return lineNumber_; }
	int getColumnNumber() const { return columnNumber_; }

private:
	std::string input_;
	int columnNumber_ = 1;
	int lineNumber_ = 1;
	int currentIndex_ = 0;
};
