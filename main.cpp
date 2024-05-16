#include "AbstractSyntaxTree.h"
#include "FileIO.h"
#include "Lexer.h"
#include "Parser.h"
#include <iostream>
#include <filesystem>
#include "FileNameTransformer.h"

static void printErrors(const std::vector<ParserError>& errors)
{
	for (const auto& e : errors)
	{
		std::cout << e.message << " at " << e.lineNumber << ":" << e.columnNumber;
		if (e.filename != "") std::cout << " in file " << e.filename;
		std::cout << std::endl;
	}
}

static bool transpileFile(const std::string& filePath)
{
	FileIO file(filePath);

	if (!file.isOpen())
	{
		std::cout << "File not found" << std::endl;
		return false;
	}

	std::string allLines;

	file.readAllLines(allLines);

	file.close();

	const Result* result = Parser::parse(allLines);

	const auto& errors = result->errors;

	if (!errors.empty())
	{
		printErrors(errors);
		return false;
	}
	else
	{
		auto fileNameExtensionStart = filePath.find_last_of(".");
		auto filePathLength = filePath.length();
		std::string filePathWithoutExtension = filePath.substr(0, filePathLength - (filePathLength - fileNameExtensionStart));
		auto fileNameStart = filePathWithoutExtension.find_last_of("/") + 1;
		std::string fileName = filePathWithoutExtension.substr(fileNameStart);

		std::string cppFileName = FileNameTransformer::toCppFileName(fileName);
		std::string cppFilePathWithoutExtension = filePathWithoutExtension.substr(0, fileNameStart) + cppFileName;

		for (auto c : result->ast->classes)
		{
			auto data = CppData(cppFileName);
			c->hoist(&data);
			c->resolveDefinitions(&data);
			c->resolveTypes(&data);
			std::string classCode = c->toCpp(&data, "");
			std::string outputPath = cppFilePathWithoutExtension + ".h";
			std::ofstream headerFile(outputPath);
			headerFile << classCode;
			headerFile.close();
			std::cout << "Transpiled " << outputPath << std::endl;
		}
	}

	return true;
}

static std::vector<std::filesystem::path> getGDFilesInDirectory(const std::filesystem::path& directory)
{
	std::vector<std::filesystem::path> filePaths;

	for (auto& entry : std::filesystem::directory_iterator(directory))
	{
		const auto& filePath = entry;
		if (entry.is_directory())
		{
			auto currentDirectoryFiles = getGDFilesInDirectory(filePath);
			filePaths.insert(filePaths.end(), currentDirectoryFiles.begin(), currentDirectoryFiles.end());
		}
		else if (filePath.path().extension() == ".gd") filePaths.push_back(filePath);
	}

	return filePaths;
}

static std::vector<std::filesystem::path> getFilePaths(const std::filesystem::path& path)
{
	if (std::filesystem::is_directory(path))
	{
		return getGDFilesInDirectory(path);
	}
	return { path.string() };
}

int main(int argc, char* argv[])
{
	std::string entryPointFileName;
	std::string projectDirectory;

	for (int i = 2; i < argc; i++)
	{
		if (strcmp(argv[i], "-p") == 0)
		{
			i++;
			if (i < argc) projectDirectory = argv[i];
			else
			{
				std::cout << "Missing value for -p parameter" << std::endl;
			}
		}
	}

	if (projectDirectory.empty())
	{
		std::cout << "No directory provided" << std::endl;
		return 1;
	}

	auto filePaths = getFilePaths(projectDirectory);

	for (const auto& filePath : filePaths)
	{
		if (!transpileFile(filePath.generic_string())) return 1;
	}

	return 0;
}