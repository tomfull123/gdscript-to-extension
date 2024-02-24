#include "AbstractSyntaxTree.h"
#include "FileIO.h"
#include "Lexer.h"
#include "Parser.h"
#include <iostream>

static void printErrors(const std::vector<ParserError>& errors)
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
	std::string filePath = "C:/Dev/Godot/Sandbox/gameplay/actions/action.gd";
	//std::string filePath = "C:/Dev/Godot/Sandbox/gameplay/attributes/attribute.gd";

	FileIO file(filePath);

	std::string allLines;

	file.readAllLines(allLines);

	const Result* result = Parser::parse(allLines);

	const auto& errors = result->errors;

	if (!errors.empty())
	{
		printErrors(errors);
	}
	else
	{
		auto fileNameExtensionStart = filePath.find_last_of(".");
		auto filePathLength = filePath.length();
		std::string filePathWithoutExtension = filePath.substr(0, filePathLength - (filePathLength - fileNameExtensionStart));
		auto fileNameStart = filePathWithoutExtension.find_last_of("/") + 1;
		std::string fileName = filePathWithoutExtension.substr(fileNameStart);

		for (auto c : result->ast->classes)
		{
			auto data = CppData(fileName);
			std::string classCode = c->toCpp(&data, "");
			std::ofstream headerFile(filePathWithoutExtension + ".h");
			headerFile << classCode;
			headerFile.close();
		}
	}

	return 0;
}