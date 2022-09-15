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
    for (const auto& dir : *dirs)
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
//    EXPECT_EQ(cf2.GetDirectories().size(), countEntris + 1); // it has some empty nodes
    EXPECT_TRUE(cf2.RootStorage()->GetStorage(storageName) != nullptr);
    EXPECT_EQ(cf2.RootStorage()->GetStorage(storageName)->Name(), storageName);
}


TEST_F(CompoundFileTest, test_compoundfile_addEmptyStream)
{
    wstring streamName = L"stream1";
    const auto countEntris = cf.GetDirectories().size();
    cf.RootStorage()->AddStream(streamName);

    wstring other_filename = L"../../../data/ex4.ppt";
    NSFile::CFileBinary::Remove(other_filename);
    cf.Save(other_filename);

    auto dirs = cf.GetDirectories();
    for (const auto& dir : *dirs)
    {
        if (dir == nullptr)
            continue;

        wcout << left << setw(3) << dir->getSid()
              << left << setw(6) << (dir->getColor() ? L"Black" : L"Red")
              << left << setw(3) << dir->getLeftSibling()
              << left << setw(3) << dir->getRightSibling()
              << left << setw(3) << dir->getChild()
              << left << dir->GetEntryName()
              << endl;
    }

    cf.Close();

    CompoundFile cf2(other_filename);
    EXPECT_TRUE(cf2.RootStorage()->GetStream(streamName) != nullptr);
}

TEST_F(CompoundFileTest, test_compoundfile_add2Stream)
{
    wstring storageName = L"storage1";
    wstring streamName = L"stream";

    cf.RootStorage()->AddStorage(storageName)->AddStream(streamName);
    cf.RootStorage()->AddStream(streamName);

    auto dirs = cf.GetDirectories();
    for (const auto& dir : *dirs)
    {
        if (dir == nullptr)
            continue;

        wcout << left << setw(3) << dir->getSid()
              << left << setw(6) << (dir->getColor() ? L"Black" : L"Red")
              << left << setw(3) << dir->getLeftSibling()
              << left << setw(3) << dir->getRightSibling()
              << left << setw(3) << dir->getChild()
              << left << dir->GetEntryName()
              << endl;
    }

    wstring other_filename = L"../../../data/ex5.ppt";
    NSFile::CFileBinary::Remove(other_filename);
    cf.Save(other_filename);
    cf.Close();

    CompoundFile cf2(other_filename);
    EXPECT_TRUE(cf2.RootStorage()->GetStream(streamName) != nullptr);

    auto storage = cf2.RootStorage()->GetStorage(storageName);
    EXPECT_TRUE(storage != nullptr);
    EXPECT_TRUE(storage->GetStream(streamName) != nullptr);
}
