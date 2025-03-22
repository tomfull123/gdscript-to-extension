#pragma once

#include <gtest/gtest.h>
#include <GDParser.h>
#include <DocsParser.h>
#include <filesystem>
#include "FileIO.h"
#include "FileNameTransformer.h"

static AbstractSyntaxTree* ast = new AbstractSyntaxTree();

struct TranspileTest : testing::Test
{
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

	static void parseDocFile(const std::string& filePath, AbstractSyntaxTree* ast)
	{
		std::string allLines = readFile(filePath);

		auto fileNameExtensionStart = filePath.find_last_of(".");
		auto filePathLength = filePath.length();
		std::string filePathWithoutExtension = filePath.substr(0, filePathLength - (filePathLength - fileNameExtensionStart));
		auto fileNameStart = filePathWithoutExtension.find_last_of("/") + 1;
		std::string fileName = filePathWithoutExtension.substr(fileNameStart);

		std::string cppFileName = FileNameTransformer::toCppFileName(fileName);

		DocsParser::parse(allLines, ast, cppFileName);
	}

	static void SetUpTestCase()
	{
		const auto& docPaths = getFilesWithExtensionInDirectory(".xml", "./../godot_docs");

		for (const auto& filePath : docPaths)
		{
			parseDocFile(filePath.generic_string(), ast);
		}
	}

	std::string transpile(const std::string& input) const
	{
		const Result* result = GDParser::parse(input, ast, "Test");

		const auto& errors = result->errors;

		if (!errors.empty()) return errors[0].message;

		const auto& classes = result->ast->classes;

		if (classes.empty()) return "";

		auto c = result->ast->classes.back();

		CppData data;
		c->hoist(&data);
		c->resolveDefinitions(&data);
		c->resolveTypes(&data);

		const auto& code = c->toCpp(&data, "");

		ast->classes.pop_back();

		return code;
	}
};