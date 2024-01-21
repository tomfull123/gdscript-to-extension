#pragma once

#include <fstream>
#include <string>

class FileIO
{
public:
	explicit FileIO(const std::string& filePath);
	~FileIO();

	int getSize();

	std::string readAllLines();
	void readAllLines(std::string& buffer);

private:
	std::ifstream file_;

	void setCursorToBeginning();
};
