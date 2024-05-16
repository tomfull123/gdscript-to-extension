#pragma once

#include <string>

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
};