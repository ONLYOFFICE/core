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

    CompoundFileTest() : cf(filename, CFSUpdateMode::Update)
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

TEST_F(CompoundFileTest, test_compoundfile_addEmptyStorage)
{
    wstring storageName = L"storage1";
    const auto countEntris = cf.GetDirectories().size();
    cf.RootStorage()->AddStorage(storageName);

    wstring other_filename = L"../../../data/ex3.ppt";
    NSFile::CFileBinary::Remove(other_filename);
    cf.Save(other_filename);
    cf.Close();

    CompoundFile cf2(other_filename);
    EXPECT_EQ(cf2.GetDirectories().size(), countEntris + 1);
    EXPECT_TRUE(cf.RootStorage()->GetStorage(storageName).get() != nullptr);
}


TEST_F(CompoundFileTest, test_compoundfile_addEmptyStream)
{
    wstring storageName = L"storage1";
    wstring streamName = L"stream1";
    const auto countEntris = cf.GetDirectories().size();
    cf.RootStorage()->AddStorage(storageName)->AddStream(streamName);

    wstring other_filename = L"../../../data/ex4.ppt";
    NSFile::CFileBinary::Remove(other_filename);
    cf.Save(other_filename);
    cf.Close();

    CompoundFile cf2(other_filename);
    EXPECT_EQ(cf2.GetDirectories().size(), countEntris + 2);
    EXPECT_TRUE(cf.RootStorage()->GetStorage(storageName).get() != nullptr);
    EXPECT_TRUE(cf.RootStorage()->GetStream(streamName).get() != nullptr);
}

TEST_F(CompoundFileTest, test_compoundfile_add2Stream)
{
    wstring storageName = L"storage1";
    wstring streamName = L"stream";

    const auto countEntris = cf.GetDirectories().size();
    cf.RootStorage()->AddStorage(storageName)->AddStream(streamName);
    cf.RootStorage()->AddStream(streamName);

    wstring other_filename = L"../../../data/ex5.ppt";
    NSFile::CFileBinary::Remove(other_filename);
    cf.Save(other_filename);
    cf.Close();

    CompoundFile cf2(other_filename);
    EXPECT_EQ(cf2.GetDirectories().size(), countEntris + 3);
}
