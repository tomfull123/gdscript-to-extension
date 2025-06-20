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
		if (e.filepath != "") std::cout << " in file " << e.filepath;
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

static void buildClassAST(const std::string& filepath, AbstractSyntaxTree* ast)
{
	std::string allLines = readFile(filepath);

	auto filenameExtensionStart = filepath.find_last_of(".");
	auto filepathLength = filepath.length();
	std::string filePathWithoutExtension = filepath.substr(0, filepathLength - (filepathLength - filenameExtensionStart));
	auto filenameStart = filePathWithoutExtension.find_last_of("/") + 1;
	std::string filename = filePathWithoutExtension.substr(filenameStart);

	std::string cppFilename = FileNameTransformer::toCppFileName(filename);

	const Result* result = GDParser::parse(allLines, ast, cppFilename, filepath);

	const auto& errors = result->errors;

	if (!errors.empty())
	{
		printErrors(errors);

		std::cout << "Parser errors, but continuing...\n" << std::endl;
	}
}

static bool parseDocFile(const std::string& filepath, AbstractSyntaxTree* ast)
{
	std::string allLines = readFile(filepath);

	auto filenameExtensionStart = filepath.find_last_of(".");
	auto filepathLength = filepath.length();
	std::string filepathWithoutExtension = filepath.substr(0, filepathLength - (filepathLength - filenameExtensionStart));
	auto filenameStart = filepathWithoutExtension.find_last_of("/") + 1;
	std::string filename = filepathWithoutExtension.substr(filenameStart);

	std::string cppFilename = FileNameTransformer::toCppFileName(filename);

	const Result* result = DocsParser::parse(allLines, ast, cppFilename, filepath);

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
	std::string outputPath = "./cpp/src";

	for (int i = 2; i < argc; i++)
	{
		readArgValue(argc, argv, i, "-p", projectPath); // Project Path
		readArgValue(argc, argv, i, "-o", outputPath); // Output Path
	}

	if (projectPath.empty())
	{
		projectPath = std::filesystem::current_path().string();
	}

	std::cout << "Loading docs..." << std::endl;

	auto ast = AbstractSyntaxTree();

	std::string executableDir = ".";

	if (true) executableDir = std::filesystem::weakly_canonical(std::filesystem::path(argv[0])).parent_path().string();

	const auto& docPaths = getFilesWithExtensionInDirectory(".xml", executableDir + "/godot_docs");

	std::cout << docPaths.size() << " doc files found\n" << std::endl;

	for (const auto& filePath : docPaths)
	{
		parseDocFile(filePath.generic_string(), &ast);
	}

	std::cout << "Looking for files in: " << projectPath << "\n" << std::endl;

	for (const auto& filePath : getGDFilePaths(projectPath))
	{
		buildClassAST(filePath.generic_string(), &ast);
	}

	Generator generator;

	auto cppModule = generator.generateCode(&ast);

	CPPClassFileWriter::write(cppModule, outputPath);

	return 0;
}