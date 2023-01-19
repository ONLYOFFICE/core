#pragma once

#include "global.h"
#include "header.h"
#include "streamrw.h"


struct HeaderTest : testing::Test
{
    wstring filename;
    Stream stream;
    Header hd;

    HeaderTest() :
        filename(sourcePath + L"ex.ppt"),
        stream(OpenFileStream(filename, false))
    {
    }
};


void test_header_state(const Header& hd)
{
    ASSERT_THAT(hd.headerSignature, ElementsAre(0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1));
    EXPECT_EQ(hd.minorVersion, 0x003E);
    EXPECT_EQ(hd.majorVersion, 0x0003);
    EXPECT_EQ(hd.byteOrder, 0xFFFE);
    EXPECT_EQ(hd.sectorShift, 0x0009);
    EXPECT_EQ(hd.miniSectorShift, 0x0006);
    EXPECT_EQ(hd.directorySectorsNumber, 0);
    EXPECT_EQ(hd.fatSectorsNumber, 1);
    EXPECT_EQ(hd.firstDirectorySectorID, 1);
    EXPECT_EQ(hd.minSizeStandardStream, 0x1000);
    EXPECT_EQ(hd.firstMiniFATSectorID, 2);
    EXPECT_EQ(hd.miniFATSectorsNumber, 1);
    EXPECT_EQ(hd.firstDIFATSectorID, -2);
    EXPECT_EQ(hd.difatSectorsNumber, 0);

    int difat[109];
    memset(reinterpret_cast<char*>(difat), 0xFF, sizeof(difat));
    difat[0] = 0;
    ASSERT_FALSE(memcmp(hd.difat, difat, sizeof(difat)));
}

TEST_F(HeaderTest, open)
{
    EXPECT_TRUE(IsOpen(stream));
}

TEST_F(HeaderTest, read)
{
    hd.Read(stream);
    test_header_state(hd);
}

void SaveHeaderToFile(const Header& header, wstring filename)
{
    wstring savePath = InitOutPath(filename);
    auto stream = OpenFileStream(savePath, true);
    header.Write(stream);
}

Header LoadHeaderFromFile(wstring filename)
{
    auto stream = OpenFileStream(outPath + filename, false);
    Header hd;
    hd.Read(stream);

    return hd;
}

TEST_F(HeaderTest, write)
{
    hd.Read(stream);

    wstring saveFilename = L"header.bin";
    SaveHeaderToFile(hd, saveFilename);
    Header readHeader = LoadHeaderFromFile(saveFilename);

    test_header_state(readHeader);
}
