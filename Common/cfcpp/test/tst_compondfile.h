#pragma once

#include "global.h"
#include "../compoundfile.h"
#include <iostream>
#include <iomanip>


struct CompoundFileTest : testing::Test
{
    wstring srcFilePath = sourcePath + L"ex.ppt";
    CompoundFile cf;

    CompoundFileTest() : cf(srcFilePath, CFSUpdateMode::Update)
    {

    }
};


wstring SaveToFile(CompoundFile& cf, wstring filename)
{
    wstring filePath = InitOutPath(filename);
    EXPECT_NO_THROW(cf.Save(filePath));

    return filePath;
}

TEST_F(CompoundFileTest, read)
{
    EXPECT_TRUE(cf.HasSourceStream());
}

TEST_F(CompoundFileTest, copy)
{
    wstring savedFilePath = SaveToFile(cf, L"ex2.ppt");

    EXPECT_EQ(FileLenght(srcFilePath), FileLenght(savedFilePath));
    EXPECT_EQ(FileFNVHash(srcFilePath), FileFNVHash(savedFilePath));
}

void TestLargeCompoundFileWrite(CompoundFile& cf, wstring largeFileName)
{
    auto largeStream = cf.RootStorage()->AddStream(L"stream");

    EXPECT_NO_THROW(largeStream->Write(_70MBVector, 0));

    wstring savedFilePath = SaveToFile(cf, largeFileName);
    EXPECT_GT(FileLenght(savedFilePath), _70MBLen);
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

    wstring savedFilePath = SaveToFile(cf, L"ex3.ppt");

    CompoundFile cf2(savedFilePath);
    EXPECT_NO_THROW(cf2.RootStorage()->GetStorage(storageName));
}

TEST_F(CompoundFileTest, timestamp)
{
    wstring storageName = L"storage";
    auto storage = cf.RootStorage()->AddStorage(storageName);
    EXPECT_TRUE(storage->getDataTime().getUINT64() > 116444736000000000ULL);
}

TEST_F(CompoundFileTest, addEmptyStream)
{
    wstring streamName = L"stream";
    cf.RootStorage()->AddStream(streamName);

    wstring savedFilePath = SaveToFile(cf, L"ex4.ppt");

    CompoundFile cf2(savedFilePath);
    EXPECT_NO_THROW(cf2.RootStorage()->GetStream(streamName));
}

TEST_F(CompoundFileTest, add2StreamWithSameName)
{
    wstring storageName = L"storage";
    wstring streamsName = L"stream";

    cf.RootStorage()->AddStorage(storageName)->AddStream(streamsName);
    cf.RootStorage()->AddStream(streamsName);


    wstring savedFilePath = SaveToFile(cf, L"ex5.ppt");

    CompoundFile cf2(savedFilePath);
    EXPECT_NO_THROW(cf2.RootStorage()->GetStream(streamsName));
    EXPECT_NO_THROW(cf2.RootStorage()->GetStorage(storageName)->GetStream(streamsName));
}

TEST_F(CompoundFileTest, deleteStream)
{
    const wstring streamName = L"PowerPoint Document";
    EXPECT_TRUE(cf.RootStorage()->GetStream(streamName));
    cf.RootStorage()->Delete(streamName);
    EXPECT_THROW(cf.RootStorage()->GetStream(streamName), CFItemNotFound);
}

TEST_F(CompoundFileTest, deleteStreamWithSave)
{
    const wstring streamName = L"PowerPoint Document";
    cf.RootStorage()->Delete(streamName);

    wstring savedFilePath = SaveToFile(cf, L"ex6.ppt");

    CompoundFile cf2(savedFilePath);
    EXPECT_THROW(cf2.RootStorage()->GetStream(streamName), CFItemNotFound);
}

TEST_F(CompoundFileTest, deleteStorage)
{
    auto storage1 = cf.RootStorage()->AddStorage(L"sto1");
    auto storage2 = storage1->AddStorage(L"sto2");
    auto stream1 = storage2->AddStream(L"str1");

    EXPECT_NO_THROW(cf.RootStorage()->GetStorage(L"sto1"));
    cf.RootStorage()->Delete(L"sto1");
    EXPECT_THROW(cf.RootStorage()->GetStorage(L"sto1"), CFItemNotFound);
}

void WriteStreamWithDefaultData(CompoundFile &cf, wstring streamName)
{
    auto stream = cf.RootStorage()->AddStream(streamName);

    EXPECT_NO_THROW(stream->Write(_8ByteData, 0));
    EXPECT_NO_THROW(cf.RootStorage()->GetStream(streamName));
}

void CheckStreamWithDefaultData(CompoundFile &cf, wstring streamName)
{
    vector<BYTE> vecForRead(_8ByteData.size());

    shared_ptr<CFStream> stream;
    EXPECT_TRUE(cf.RootStorage()->TryGetStream(streamName, stream));

    EXPECT_NO_THROW(stream->Read(vecForRead, 0, _8ByteData.size()));
    EXPECT_EQ(_8ByteData, vecForRead);
}

TEST_F(CompoundFileTest, writeStreams)
{
    for (const auto& name : arrForeignFileNames)
    {
        WriteStreamWithDefaultData(cf, name);
        CheckStreamWithDefaultData(cf, name);
    }
}

void TryToSaveEmptyCompoundFile(wstring filename)
{
    wstring outPath = InitOutPath(filename);
    try {
        CompoundFile cf;
        cf.Save(outPath);
    } catch (...) {
        FAIL();
    }
}

TEST(test_compoundfile, foreign_languages)
{
    for (const auto& name : arrForeignFileNames)
    {
        wstring filename = name + L".cfb";
        TryToSaveEmptyCompoundFile(filename);
    }
    SUCCEED();
}
