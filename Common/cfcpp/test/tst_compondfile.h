#pragma once

#include "global.h"
#include "compoundfile.h"
#include <iostream>
#include <iomanip>
#include <time.h>

#include <chrono>

using namespace testing;
using namespace std;
using namespace CFCPP;


struct CompoundFileTest : testing::Test
{
    wstring filename = sourcePath + L"ex.ppt";
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


TEST_F(CompoundFileTest, read)
{
    EXPECT_TRUE(cf.HasSourceStream());
}

TEST_F(CompoundFileTest, copy)
{
    wstring other_filename = InitOutPath(L"ex2.ppt");
    cf.Save(other_filename);

    EXPECT_EQ(FileLenght(filename), FileLenght(other_filename));
    EXPECT_EQ(FileSimpleHash(filename), FileSimpleHash(other_filename));
}

void TestLargeCompoundFileWrite(CompoundFile& cf, std::wstring largeFileName)
{
    wstring largeFilePath = InitOutPath(largeFileName);
    auto largeStream = cf.RootStorage()->AddStream(L"stream");

    EXPECT_NO_THROW(largeStream->Write(_70MBVector, 0));

    EXPECT_NO_THROW(cf.Save(largeFilePath));
    cf.Close();

    EXPECT_GT(FileLenght(largeFilePath), _70MBLen);
}

TEST(test_compoundfile, largeStream_v3)
{
    CompoundFile cf_v3(CFCPP::Ver_3, CFCPP::Default);
    TestLargeCompoundFileWrite(cf_v3, L"largeStream_v3.cfb");
}

TEST(test_compoundfile, largeStream_v4)
{
    CompoundFile cf_v4(CFCPP::Ver_4, CFCPP::Default);
    TestLargeCompoundFileWrite(cf_v4, L"largeStream_v3.cfb");
}

TEST_F(CompoundFileTest, SID)
{
    auto PowerPointData = cf.GetDataBySID(1);
    EXPECT_NE(PowerPointData.size(), 0);
}

TEST_F(CompoundFileTest, addEmptyStorage)
{
    wstring storageName = L"storage";
    auto storage1 = cf.RootStorage()->AddStorage(storageName);
    EXPECT_TRUE(storage1->getDataTime().getUINT64() > 116444736000000000ULL);

    wstring other_filename = InitOutPath(L"ex3.ppt");
    cf.Save(other_filename);
    cf.Close();

    CompoundFile cf2(other_filename);
    EXPECT_NO_THROW(cf2.RootStorage()->GetStorage(storageName));
}

TEST_F(CompoundFileTest, addEmptyStream)
{
    wstring streamName = L"stream";
    cf.RootStorage()->AddStream(streamName);

    wstring other_filename = InitOutPath(L"ex4.ppt");
    cf.Save(other_filename);

    cf.Close();

    CompoundFile cf2(other_filename);
    EXPECT_TRUE(cf2.RootStorage()->GetStream(streamName) != nullptr);
}

TEST_F(CompoundFileTest, add2Stream)
{
    wstring storageName = L"storage";
    wstring streamName = L"stream";

    cf.RootStorage()->AddStorage(storageName)->AddStream(streamName);
    cf.RootStorage()->AddStream(streamName);


    wstring other_filename = InitOutPath(L"ex5.ppt");
    cf.Save(other_filename);
    cf.Close();

    CompoundFile cf2(other_filename);
    EXPECT_TRUE(cf2.RootStorage()->GetStream(streamName) != nullptr);

    auto storage = cf2.RootStorage()->GetStorage(storageName);
    EXPECT_TRUE(storage != nullptr);
    EXPECT_TRUE(storage->GetStream(streamName) != nullptr);
}

TEST_F(CompoundFileTest, deleteStream)
{
    const std::wstring streamName = L"PowerPoint Document";
    EXPECT_TRUE(cf.RootStorage()->GetStream(streamName));
    cf.RootStorage()->Delete(streamName);
    EXPECT_THROW(cf.RootStorage()->GetStream(streamName), CFItemNotFound);
}

TEST_F(CompoundFileTest, deleteStreamWithSave)
{
    const std::wstring streamName = L"PowerPoint Document";
    cf.RootStorage()->Delete(L"PowerPoint Document");

    wstring other_filename = InitOutPath(L"ex6.ppt");
    cf.Save(other_filename);
    cf.Close();

    CompoundFile cf2(other_filename);
    EXPECT_THROW(cf2.RootStorage()->GetStream(streamName), CFItemNotFound);
    EXPECT_TRUE(cf2.RootStorage()->GetStream(L"Current User"));
}

TEST_F(CompoundFileTest, deleteStorage)
{
    auto storage1 = cf.RootStorage()->AddStorage(L"sto1");
    auto storage2 = storage1->AddStorage(L"sto2");
    auto stream1 = storage2->AddStream(L"str1");

    EXPECT_TRUE(cf.RootStorage()->GetStorage(L"sto1"));
    cf.RootStorage()->Delete(L"sto1");
    EXPECT_THROW(cf.RootStorage()->GetStorage(L"sto1"), CFItemNotFound);
}

TEST_F(CompoundFileTest, writeStream)
{
    std::vector<BYTE> read(_8ByteData.size());
    auto stream1 = cf.RootStorage()->AddStream(L"str1");
    stream1->Write(_8ByteData,0);

    EXPECT_TRUE(cf.RootStorage()->GetStream(L"str1"));
    EXPECT_EQ(stream1->size(), _8ByteData.size());
    EXPECT_NO_THROW(stream1->Read(read, 0, _8ByteData.size()));
    EXPECT_EQ(_8ByteData, read);
}

TEST_F(CompoundFileTest, writeStreams)
{
    for (const auto& name : arrForeignFileNames)
    {
        std::vector<BYTE> read(_8ByteData.size());
        auto stream1 = cf.RootStorage()->AddStream(name);
        EXPECT_TRUE(stream1);

        EXPECT_NO_THROW(stream1->Write(_8ByteData, 0));
        EXPECT_NO_THROW(cf.RootStorage()->GetStream(name));

        EXPECT_EQ(stream1->size(), _8ByteData.size());
        EXPECT_NO_THROW(stream1->Read(read, 0, _8ByteData.size()));
        EXPECT_EQ(_8ByteData, read);
    }
}

TEST(test_compoundfile, foreign_languages)
{
    for (const auto& name : arrForeignFileNames)
    {
        try {
            wstring path = InitOutPath(name + L".cfb");

            CompoundFile cf;
            auto stream1 = cf.RootStorage()->AddStream(L"stream");
            stream1->Write(_8ByteData, 0);

            cf.Save(path);
            cf.Close();
        } catch (...) {
            FAIL();
        }

    }
    SUCCEED();
}
