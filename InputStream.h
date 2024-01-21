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
