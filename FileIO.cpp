#include "FileIO.h"

FileIO::FileIO(const std::string& filePath) :
	file_(filePath, std::ios::binary)
{
}

FileIO::~FileIO()
{
	file_.close();
}

int FileIO::getSize()
{
	return file_.tellg();
}

std::string FileIO::readAllLines()
{
	std::string allLines;

	readAllLines(allLines);

	return allLines;
}

void FileIO::readAllLines(std::string& allLines)
{
	int size = getSize();

	allLines.reserve(size);

	allLines = std::string(
		(std::istreambuf_iterator<char>(file_)),
		std::istreambuf_iterator<char>());
}

void FileIO::setCursorToBeginning()
{
	file_.seekg(0, file_.beg);
}