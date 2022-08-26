#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "compoundfile.h"
#include "../../DesktopEditor/common/File.h"
#include <iostream>
#include <iomanip>

using namespace testing;
using namespace std;
using namespace CFCPP;


struct CompoundFileTest : testing::Test
{
    wstring filename = L"../../../data/ex.ppt";
    CompoundFile cf;

    CompoundFileTest() : cf(filename)
    {

    }
};

TEST_F(CompoundFileTest, test_compoundfile_read)
{
    EXPECT_TRUE(cf.HasSourceStream());
}


TEST_F(CompoundFileTest, test_compoundfile_write)
{
    wstring other_filename = L"../../../data/ex2.ppt";
    NSFile::CFileBinary::Remove(other_filename);
    cf.Save(other_filename);
    EXPECT_EQ(FileLenght(filename), FileLenght(other_filename));
    EXPECT_EQ(FileSimpleHash(filename), FileSimpleHash(other_filename));
}

TEST_F(CompoundFileTest, test_compoundfile_SID)
{
    auto dirs = cf.GetDirectories();
    for (const auto& dir : dirs)
    {
        wcout << left << setw(2) << dir->getSid() << L" "
              << left << setw(30) << dir->GetEntryName() << L" "
              << left << setw(3) << dir->getNameLength() << L" "
              << left << setw(4) << dir->getSize() << L" "
              << left << setw(4) << dir->getStgType() << L" "
              << left << setw(4) << dir->getChild()
              << "\n";
    }
    auto PowerPointData = cf.GetDataBySID(1);
    EXPECT_NE(PowerPointData.size(), 0);
}
