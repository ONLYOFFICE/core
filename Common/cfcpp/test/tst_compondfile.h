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

    void printDirs()
    {
        for (const auto& dir : *cf.GetDirectories())
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
        wcout << endl;
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

    printDirs();

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

TEST_F(CompoundFileTest, test_compoundfile_deleteStream)
{
    const std::wstring streamName = L"PowerPoint Document";
    EXPECT_TRUE(cf.RootStorage()->GetStream(streamName));
    cf.RootStorage()->Delete(streamName);
    EXPECT_THROW(cf.RootStorage()->GetStream(streamName), CFItemNotFound);
}

TEST_F(CompoundFileTest, test_compoundfile_deleteStreamWithSave)
{
    const std::wstring streamName = L"PowerPoint Document";
    cf.RootStorage()->Delete(L"PowerPoint Document");

    const wstring other_filename = L"../../../data/ex6.ppt";
    NSFile::CFileBinary::Remove(other_filename);
    cf.Save(other_filename);
    cf.Close();

    CompoundFile cf2(other_filename);
    EXPECT_THROW(cf2.RootStorage()->GetStream(streamName), CFItemNotFound);
    EXPECT_TRUE(cf2.RootStorage()->GetStream(L"Current User"));
}

TEST_F(CompoundFileTest, test_compoundfile_deleteStorage)
{
    auto storage1 = cf.RootStorage()->AddStorage(L"sto1");
    auto storage2 = storage1->AddStorage(L"sto2");
    auto stream1 = storage2->AddStream(L"str1");

    printDirs();
    EXPECT_TRUE(cf.RootStorage()->GetStorage(L"sto1"));
    cf.RootStorage()->Delete(L"sto1");
    EXPECT_THROW(cf.RootStorage()->GetStorage(L"sto1"), CFItemNotFound);
    printDirs();
}

TEST_F(CompoundFileTest, test_compoundfile_writeStream)
{
    std::vector<BYTE> data = {0x28, 0xFF, 0x28, 0x1D, 0x4C, 0xFA, 0x00, 0x79, 0x40, 0x01};
    std::vector<BYTE> read(data.size());
    auto stream1 = cf.RootStorage()->AddStream(L"str1");
    stream1->Write(data,0);

    EXPECT_TRUE(cf.RootStorage()->GetStream(L"str1"));
    stream1->Read(read, 0, data.size());
    EXPECT_EQ(data.size(), read.size());

    printDirs();
}
