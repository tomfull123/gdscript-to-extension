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

	if (!hasErrors)
	{
		int fileNameExtensionStart = filePath.find_last_of(".");
		int filePathLength = filePath.length();
		std::string filePathWithoutExtension = filePath.substr(0, filePathLength - (filePathLength - fileNameExtensionStart));
		int fileNameStart = filePathWithoutExtension.find_last_of("/") + 1;
		std::string fileName = filePathWithoutExtension.substr(fileNameStart);

		for (auto c : ast->classes)
		{
			CppData* data = new CppData(fileName);
			std::string classCode = c->toCpp(data, "");
			std::ofstream headerFile(filePathWithoutExtension + ".h");
			headerFile << classCode;
			headerFile.close();
		}
	}

	return 0;
}