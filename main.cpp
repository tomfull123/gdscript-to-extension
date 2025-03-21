#include "AbstractSyntaxTree.h"
#include "FileIO.h"
#include "GDLexer.h"
#include "GDParser.h"
#include <iostream>
#include <filesystem>
#include "FileNameTransformer.h"
#include "Generator.h"
#include "CPPClassFileWriter.h"
#include "DocsParser.h"

static void printErrors(const std::vector<ParserError>& errors)
{
	for (const auto& e : errors)
	{
		std::cout << e.message << " at " << e.lineNumber << ":" << e.columnNumber;
		if (e.filename != "") std::cout << " in file " << e.filename;
		std::cout << std::endl;
	}
}

static std::string readFile(const std::string& filePath)
{
	FileIO file(filePath);

	if (!file.isOpen())
	{
		std::cout << "File " << filePath << " not found" << std::endl;
		return "";
	}

	std::string allLines;

	file.readAllLines(allLines);

	file.close();

	return allLines;
}

static bool buildClassAST(const std::string& filePath, AbstractSyntaxTree* ast)
{
	std::string allLines = readFile(filePath);

	auto fileNameExtensionStart = filePath.find_last_of(".");
	auto filePathLength = filePath.length();
	std::string filePathWithoutExtension = filePath.substr(0, filePathLength - (filePathLength - fileNameExtensionStart));
	auto fileNameStart = filePathWithoutExtension.find_last_of("/") + 1;
	std::string fileName = filePathWithoutExtension.substr(fileNameStart);

	std::string cppFileName = FileNameTransformer::toCppFileName(fileName);

	const Result* result = GDParser::parse(allLines, ast, cppFileName);

	const auto& errors = result->errors;

	if (!errors.empty())
	{
		printErrors(errors);
		return false;
	}

	return true;
}

static bool parseDocFile(const std::string& filePath, AbstractSyntaxTree* ast)
{
	std::string allLines = readFile(filePath);

	auto fileNameExtensionStart = filePath.find_last_of(".");
	auto filePathLength = filePath.length();
	std::string filePathWithoutExtension = filePath.substr(0, filePathLength - (filePathLength - fileNameExtensionStart));
	auto fileNameStart = filePathWithoutExtension.find_last_of("/") + 1;
	std::string fileName = filePathWithoutExtension.substr(fileNameStart);

	std::string cppFileName = FileNameTransformer::toCppFileName(fileName);

	const Result* result = DocsParser::parse(allLines, ast, cppFileName);

	const auto& errors = result->errors;

	if (!errors.empty())
	{
		printErrors(errors);
		return false;
	}

	return true;
}

static std::vector<std::filesystem::path> getFilesWithExtensionInDirectory(const std::string& extension, const std::filesystem::path& directory)
{
	std::vector<std::filesystem::path> filePaths;

	for (auto& entry : std::filesystem::directory_iterator(directory))
	{
		const auto& filePath = entry;
		if (entry.is_directory())
		{
			auto currentDirectoryFiles = getFilesWithExtensionInDirectory(extension, filePath);
			filePaths.insert(filePaths.end(), currentDirectoryFiles.begin(), currentDirectoryFiles.end());
		}
		else if (filePath.path().extension() == extension) filePaths.push_back(filePath);
	}

	return filePaths;
}

static std::vector<std::filesystem::path> getGDFilesInDirectory(const std::filesystem::path& directory)
{
	return getFilesWithExtensionInDirectory(".gd", directory);
}

static std::vector<std::filesystem::path> getGDFilePaths(const std::filesystem::path& path)
{
	if (std::filesystem::is_directory(path))
	{
		return getGDFilesInDirectory(path);
	}
	return { path.string() };
}

static void readArgValue(int argc, char* argv[], int currentIndex, const std::string& flagPrefix, std::string& output)
{
	if (strcmp(argv[currentIndex], flagPrefix.c_str()) == 0)
	{
		currentIndex++;
		if (currentIndex < argc) output = argv[currentIndex];
		else
		{
			std::cout << "Missing value for " + flagPrefix + " parameter" << std::endl;
		}
	}
}

int main(int argc, char* argv[])
{
	std::string projectPath;
	std::string outputPath;

	for (int i = 2; i < argc; i++)
	{
		readArgValue(argc, argv, i, "-p", projectPath); // Project Path
		readArgValue(argc, argv, i, "-o", outputPath); // Output Path
	}

	if (projectPath.empty())
	{
		projectPath = std::filesystem::current_path().string();
	}

	std::cout << "Loading docs..." << "\n" << std::endl;

	AbstractSyntaxTree* ast = new AbstractSyntaxTree();

	const auto& docPaths = getFilesWithExtensionInDirectory(".xml", "./godot_docs");

	for (const auto& filePath : docPaths)
	{
		parseDocFile(filePath.generic_string(), ast);
	}

	std::cout << "Looking for files in: " << projectPath << "\n" << std::endl;

	for (const auto& filePath : getGDFilePaths(projectPath))
	{
		if (!buildClassAST(filePath.generic_string(), ast)) return 1;
	}

	Generator generator;

	auto cppModule = generator.generateCode(ast);

	CPPClassFileWriter::write(cppModule, outputPath);

	return 0;
}