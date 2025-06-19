#pragma once

#include <fstream>
#include <string>

class FileIO
{
public:
	explicit FileIO(const std::string& filePath);
	~FileIO();

	bool isOpen() const
	{
		return file_.is_open();
	}

	int getSize();

	std::string readAllLines();
	void readAllLines(std::string& buffer);

	void close();

private:
	std::ifstream file_;

	void setCursorToBeginning();
};
