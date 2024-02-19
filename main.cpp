#include "AbstractSyntaxTree.h"
#include "FileIO.h"
#include "Lexer.h"
#include "Parser.h"
#include <iostream>

void printErrors(const std::vector<ParserError>& errors)
{
	for (const auto& e : errors)
	{
		std::cout << e.message << " at " << e.lineNumber << ":" << e.columnNumber;
		if (e.filename != "") std::cout << " in file " << e.filename;
		std::cout << std::endl;
	}
}

int main(int argc, char* argv[])
{
	AbstractSyntaxTree* ast = new AbstractSyntaxTree();
	bool hasErrors = false;

	std::string filePath = "C:/Dev/Godot/Sandbox/gameplay/actions/action.gd";
	//std::string filePath = "C:/Dev/Godot/Sandbox/gameplay/attributes/attribute.gd";

	FileIO file(filePath);

	std::string allLines;

	file.readAllLines(allLines);

	Lexer lexer(allLines);

	std::vector<Token*> tokens = lexer.readAllTokens();

	Parser parser(tokens);

	parser.buildAST(ast);

	const auto& errors = parser.getErrors();

	if (!errors.empty())
	{
		printErrors(errors);
		hasErrors = true;
	}

	return 0;
}