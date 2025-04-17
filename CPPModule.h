#pragma once

#include <vector>
#include "CPPClassFile.h"

struct CPPModule
{
	std::vector<CPPClassFile> classes;
	CPPClassFile registerTypeHeader;
	CPPClassFile registerTypeSource;
};
