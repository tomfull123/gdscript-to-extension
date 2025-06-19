#pragma once

#include <string>
#include <filesystem>

class FileNameTransformer
{
public:

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

	static std::string getOutputFilePath(const std::string& cppFileName, const std::string& overridePath, bool header = true)
	{
		std::string outputCppPath = std::filesystem::absolute(overridePath).generic_string();
		if (!std::filesystem::path(overridePath).has_extension())
		{
			outputCppPath += "/" + cppFileName;
			if (header) outputCppPath += ".h";
			else outputCppPath += ".cpp";
		}
		std::filesystem::create_directories(std::filesystem::path(outputCppPath).remove_filename());
		return outputCppPath;
	}
};