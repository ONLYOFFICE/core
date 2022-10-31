#pragma once

#include "global.h"
#include "../../DesktopEditor/common/File.h"


TEST(TestDataSet, allSourceFilesExistAndCorrect)
{
    EXPECT_FILE_FNV_HASH(sourcePath + L"ex.ppt", 4659199142276128161);
}
