#pragma once


#include "global.h"
#include "../../DesktopEditor/common/File.h"

TEST(TestDataSet, allSourceFilesExist)
{
    EXPECT_FILE_EXIST(sourcePath + L"ex.ppt");
    EXPECT_FILE_EXIST(sourcePath + L"big.ppt");
}
