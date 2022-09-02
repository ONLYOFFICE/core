#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "directoryentry.h"
#include "../../DesktopEditor/common/File.h"

using namespace testing;
using namespace std;
using namespace CFCPP;


struct DirEntryTest : testing::Test
{
    string filename;
    Stream stream;

    DirEntryTest() :
        filename("../../../data/ex.ppt"),
        stream(OpenFileStream(filename))
    {
    }
};

void test_dirEntry_read(const DirectoryEntry& de)
{
    EXPECT_EQ(de.GetEntryName(), L"Root Entry");
    EXPECT_EQ(de.getNameLength(), 22);
    EXPECT_EQ(de.getStgType(), StgRoot);

    EXPECT_EQ(de.leftSibling, 0xFFFFFFFF);
    EXPECT_EQ(de.rightSibling, 0xFFFFFFFF);
    EXPECT_EQ(de.child, 1);

    GUID storageCLSID;
    storageCLSID.Data1 = 0x64818D10;
    storageCLSID.Data2 = 0x4F9B;
    storageCLSID.Data3 = 0x11CF;
    storageCLSID.Data4 = 0xE829B900AA00EA86;
//    EXPECT_EQ(de.getStorageCLSID(), storageCLSID);

    EXPECT_EQ(de.stateBits, 0);
    // todo
    //EXPECT_EQ(de.creationDate, 0);
//    EXPECT_EQ(de.modifyDate, 0xC0F1C03A18A1D801);
    EXPECT_EQ(de.startSetc, 3);
    EXPECT_EQ(de.size, 5632);
}

TEST_F(DirEntryTest, test_directoryentry_read)
{
    DirectoryEntry de(L"", StgInvalid);
    stream->seek(0x400, std::ios::beg);
    de.Read(stream);

    EXPECT_EQ(stream->tell(), 0x480);
    test_dirEntry_read(de);
}

TEST_F(DirEntryTest, test_directoryentry_write)
{
    DirectoryEntry de(L"", StgInvalid);
    stream->seek(0x400, std::ios::beg);
    de.Read(stream);

    std::string other_filename("../../../data/direntry.bin");
    stream = OpenFileStream(other_filename, true);
    de.Write(stream);
    EXPECT_EQ(stream->tell(), 0x80);
    stream->seek(0, std::ios::beg);

    DirectoryEntry other(L"", StgInvalid);
    other.Read(stream);
    test_dirEntry_read(other);
    remove(other_filename.c_str());
}
