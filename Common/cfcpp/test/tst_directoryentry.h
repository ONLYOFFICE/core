#pragma once

#include "global.h"
#include "directoryentry.h"


struct DirEntryTest : testing::Test
{
    wstring filename;
    Stream stream;

    DirEntryTest() :
        filename(sourcePath + L"ex.ppt"),
        stream(OpenFileStream(filename))
    {}

    DirectoryEntry LoadDirectoryEntryFromPPTFile()
    {
        DirectoryEntry de(L"", StgInvalid);
        const int fileShift = 0x400;
        stream->seek(fileShift, ios::beg);
        de.Read(stream);

        return de;
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

    _GUID_ storageCLSID;
    storageCLSID.Data1 = 0x64818D10;
    storageCLSID.Data2 = 0x4F9B;
    storageCLSID.Data3 = 0x11CF;
    storageCLSID.Data4 = 0xE829B900AA00EA86;

    EXPECT_EQ(de.stateBits, 0);
    EXPECT_EQ(de.startSetc, 3);
    EXPECT_EQ(de.size, 5632);
}

TEST_F(DirEntryTest, read)
{
    auto de = LoadDirectoryEntryFromPPTFile();
    EXPECT_EQ(stream->tell(), 0x480);
    test_dirEntry_read(de);
}

void SaveDirectoryEntryToFile(const DirectoryEntry& de, wstring filename)
{
    wstring other_filename = InitOutPath(filename);
    auto stream = OpenFileStream(other_filename, true);
    de.Write(stream);
    EXPECT_EQ(stream->tell(), 0x80);
}

DirectoryEntry LoadDirectoryEntryFromFile(wstring filePath)
{
    DirectoryEntry de(L"", StgInvalid);
    auto stream = OpenFileStream(outPath + filePath, false);
    de.Read(stream);

    return de;
}

TEST_F(DirEntryTest, write)
{
    auto de = LoadDirectoryEntryFromPPTFile();

    wstring saveFilename = L"direntry.bin";
    SaveDirectoryEntryToFile(de, saveFilename);
    auto readDirectoryEntry = LoadDirectoryEntryFromFile(saveFilename);

    test_dirEntry_read(readDirectoryEntry);
}
