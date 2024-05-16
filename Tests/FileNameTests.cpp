#pragma once

#include <gtest/gtest.h>
#include "FileNameTransformer.h"

struct FileNameTest : testing::Test
{
};

TEST_F(FileNameTest, SnakeCaseToPascalCase)
{
	EXPECT_EQ("TestFile", FileNameTransformer::toCppFileName("test_file"));
}