#pragma once

#include <string>

struct ParserError
{
	ParserError(
		const std::string& Message,
		int LineNumber,
		int ColumnNumber,
		const std::string& Filename
	) :
		message(Message),
		lineNumber(LineNumber),
		columnNumber(ColumnNumber),
		filename(Filename)
	{}

	std::string message;
	int lineNumber;
	int columnNumber;
	std::string filename;
};