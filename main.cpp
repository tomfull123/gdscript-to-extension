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

static std::string toCppFileName(const std::string& fileName)
{
	size_t currentPos = 0;

	std::string cppFileName = fileName;

	cppFileName[0] = (char)std::toupper(cppFileName[0]);

	while (true)
	{
		size_t underscoreIndex = cppFileName.find('_', currentPos);

		if (underscoreIndex == -1) break;

		cppFileName.erase(underscoreIndex, 1);

		cppFileName[underscoreIndex] = (char)std::toupper(cppFileName[underscoreIndex]);

		currentPos = underscoreIndex;
	}

	return cppFileName;
}

static int transpileFile(const std::string& filePath)
{
	FileIO file(filePath);

	if (!file.isOpen())
	{
		std::cout << "File not found" << std::endl;
		return 1;
	}

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

		std::string cppFileName = toCppFileName(fileName);
		std::string cppFilePathWithoutExtension = filePathWithoutExtension.substr(0, fileNameStart) + cppFileName;

		for (auto c : result->ast->classes)
		{
			auto data = CppData(cppFileName);
			c->hoist(&data);
			c->resolveDefinitions(&data);
			c->resolveTypes(&data);
			std::string classCode = c->toCpp(&data, "");
			std::ofstream headerFile(cppFilePathWithoutExtension + ".h");
			headerFile << classCode;
			headerFile.close();
		}
	}

	return 0;
}

int main(int argc, char* argv[])
{
	std::string filePath = "C:/Dev/Godot/Sandbox/gameplay/actions/action.gd";
	//std::string filePath = "C:/Dev/Godot/Sandbox/gameplay/attributes/attribute.gd";

	return transpileFile(filePath);
}