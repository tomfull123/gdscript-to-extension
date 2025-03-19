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
			std::string outputCppPath = FileNameTransformer::getOutputFilePath(classFile.className, outputPath);
			std::ofstream headerFile(outputCppPath);
			headerFile << classFile.code;
			headerFile.close();
			std::cout << "Transpiled " << outputCppPath << std::endl;
		}
	}
};
