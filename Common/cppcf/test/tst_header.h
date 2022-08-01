#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "header.h"
#include "stream_wrapper.h"

using namespace testing;
using namespace std;
using namespace CFCPP;


struct HeaderTest : testing::Test
{
    wstring filename = L"../../../data/ex.ppt";
    Stream stream;
    Header hd;
    bool isOpen = true;

    HeaderTest() : stream(getStream(filename))
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

TEST_F(HeaderTest, test_header_read)
{
    EXPECT_TRUE(isOpen);
    hd.Read(stream);
    test_header_state(hd);
}

TEST_F(HeaderTest, test_header_write)
{
    hd.Read(stream);

    std::wstring other_filename(L"../../../data/types/header.bin");
    stream = getStream(other_filename, true);

    hd.Write(stream);
    flush(stream);
    stream->CloseFile();

    Header other;
    seek(stream, 0);
    other.Read(stream);
    test_header_state(other);
    NSFile::CFileBinary::Remove(other_filename);
}
