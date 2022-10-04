#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "compoundfile.h"
#include "../../DesktopEditor/common/File.h"
#include <iostream>
#include <iomanip>
#include <time.h>

#include <chrono>

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

static vector<wstring> names =
{
    L"file",        // Englang
    L"fil",         // Danish
    L"文件",         // Chinese
    L"Datei",       // German
    L"ファイル",     // Japanese
    L"soubor",      // Czech
    L"dosya",       // Turkish
    L"Файл",        // Russian
    L"dossier",     // Franch
    L"expediente"   // Spanish
};


TEST(test_compoundfile, largeStream_v3)
{
    constexpr LONG64 streamLen = 1024*1024*30;
    std::vector<BYTE> data(streamLen, 0xac);

    wstring path1 = L"../../../data/largeStream.cfb";
    NSFile::CFileBinary::Remove(path1);

    CompoundFile cf1(CFCPP::Ver_3, CFCPP::Default);

    auto stream1 = cf1.RootStorage()->AddStream(L"stream1");

    EXPECT_NO_THROW(stream1->Write(data, 0));


    EXPECT_NO_THROW(cf1.Save(path1));
    cf1.Close();

    EXPECT_GT(FileLenght(path1), streamLen);
}

TEST(test_compoundfile, largeStream_v3_v4)
{
    constexpr LONG64 streamLen = 1024*256;
    std::vector<BYTE> data = {0x28, 0xFF, 0x28, 0x1D, 0x4C, 0xFA, 0x00, 0x79};

    wstring path1 = L"../../../data/large_v3.cfb";
    wstring path2 = L"../../../data/large_v4.cfb";
    NSFile::CFileBinary::Remove(path1);
    NSFile::CFileBinary::Remove(path2);

    CompoundFile cf1(CFCPP::Ver_3, CFCPP::Default);
    CompoundFile cf2(CFCPP::Ver_4, CFCPP::Default);

    auto stream1 = cf1.RootStorage()->AddStream(L"stream1");
    auto stream2 = cf2.RootStorage()->AddStream(L"stream2");

    for (LONG64 i = 0; i < streamLen; i += data.size())
    {
        EXPECT_NO_THROW(stream1->Write(reinterpret_cast<char*>(&i), i, sizeof(i)));
        EXPECT_NO_THROW(stream2->Write(reinterpret_cast<char*>(&i), i, sizeof(i)));
    }

    EXPECT_NO_THROW(cf1.Save(path1));
    EXPECT_NO_THROW(cf2.Save(path2));
    cf1.Close();
    cf2.Close();

    EXPECT_GT(FileLenght(path1), streamLen);
    EXPECT_GT(FileLenght(path2), streamLen);
}


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

TEST(test_compoundfile, largeFileCopy)
{
    wstring filename = L"../../../data/2.ppt";
    CompoundFile cf(filename, CFSUpdateMode::ReadOnly);

    wstring other_filename = L"../../../data/3.ppt";
    NSFile::CFileBinary::Remove(other_filename);
    EXPECT_NO_THROW(cf.Save(other_filename));

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
    auto storage1 = cf.RootStorage()->AddStorage(storageName);
    EXPECT_TRUE(storage1->getDataTime().getUINT64() > 116444736000000000ULL);

    wstring other_filename = L"../../../data/ex3.ppt";
    NSFile::CFileBinary::Remove(other_filename);
    cf.Save(other_filename);
    cf.Close();

    CompoundFile cf2(other_filename);
    //    EXPECT_EQ(cf2.GetDirectories().size(), countEntris + 1); // it has some empty nodes
    EXPECT_TRUE(cf2.RootStorage()->GetStorage(storageName) != nullptr);
    EXPECT_NO_THROW(cf2.RootStorage()->GetStorage(storageName));
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
    EXPECT_EQ(stream1->size(), data.size());
    EXPECT_NO_THROW(stream1->Read(read, 0, data.size()));
    EXPECT_EQ(data, read);

    printDirs();
}

TEST_F(CompoundFileTest, test_compoundfile_writeStreams)
{
    std::vector<BYTE> data = {0x28, 0xFF, 0x28, 0x1D, 0x4C, 0xFA, 0x00, 0x79, 0x40, 0x01};
    for (const auto& name : names)
    {
        std::vector<BYTE> read(data.size());
        auto stream1 = cf.RootStorage()->AddStream(name);
        EXPECT_TRUE(stream1);

        EXPECT_NO_THROW(stream1->Write(data, 0));
        EXPECT_NO_THROW(cf.RootStorage()->GetStream(name));

        EXPECT_EQ(stream1->size(), data.size());
        EXPECT_NO_THROW(stream1->Read(read, 0, data.size()));
        EXPECT_EQ(data, read);
    }

    printDirs();
}

TEST(test_compoundfile, foreign_languages)
{
    std::vector<BYTE> data = {0x28, 0xFF, 0x28, 0x1D, 0x4C, 0xFA, 0x00, 0x79, 0x40, 0x01};
    for (const auto& name : names)
    {
        try {
            wstring path = L"../../../data/" + name + L".cfb";
            NSFile::CFileBinary::Remove(path);

            CompoundFile cf;
            auto stream1 = cf.RootStorage()->AddStream(L"stream1");
            stream1->Write(data, 0);

            cf.Save(path);
            cf.Close();
        } catch (...) {
            FAIL();
        }

    }
    SUCCEED();
}
