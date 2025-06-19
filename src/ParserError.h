#pragma once

#include <string>

struct ParserError
{
	ParserError(
		const std::string& Message,
		int LineNumber,
		int ColumnNumber,
		const std::string& Filepath
	) :
		message(Message),
		lineNumber(LineNumber),
		columnNumber(ColumnNumber),
		filepath(Filepath)
	{
	}

	std::string message;
	int lineNumber;
	int columnNumber;
	std::string filepath;
};