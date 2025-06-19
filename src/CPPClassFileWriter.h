#pragma once

#include "CPPModule.h"
#include "FileNameTransformer.h"
#include <fstream>
#include <iostream>

class CPPClassFileWriter
{
public:
	static void write(const CPPModule* cppModule, const std::string& outputPath)
	{
		for (const auto& classFile : cppModule->classes)
		{
			writeClassToFile(classFile, outputPath);
		}

		writeClassToFile(cppModule->registerTypeHeader, outputPath);
		writeClassToFile(cppModule->registerTypeSource, outputPath, false);
	}

private:

	static void writeClassToFile(const CPPClassFile& classFile, const std::string& outputPath, bool header = true)
	{
		std::string outputCppPath = FileNameTransformer::getOutputFilePath(classFile.className, outputPath, header);
		std::ofstream file(outputCppPath);
		file << classFile.code;
		file.close();
		std::cout << "Wrote file: " << outputCppPath << std::endl;
	}
};
