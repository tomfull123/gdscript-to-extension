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
		char front = input_.front();
		input_.erase(0, 1);
		columnNumber_++;
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
	std::string readUntil(const Function& function, bool includeLast = false)
	{
		std::string str;

		while (!eof())
		{
			bool matches = function(peek());
			if (matches && !includeLast) break;
			str += next();
			if (matches) break;
		}

		return str;
	}

	char peek(unsigned int offset = 0)
	{
		return input_[offset];
	}

	bool eof() const
	{
		return input_.length() <= 0;
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
};
