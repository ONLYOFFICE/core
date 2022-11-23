#pragma once

#include "global.h"
#include "streamrw.h"
#include <array>
#include <cstring>


struct StreamRWTest : testing::Test
{
    wstring filename;
    Stream stream;
    shared_ptr<StreamRW> rw;
    const char symbol = 'a';
    const int integer = 13;

    StreamRWTest() :
        filename(L"types.bin"),
        stream(OpenFileStream(InitOutPath(filename), true)),
        rw(new StreamRW(stream))
    {
    }

    ~StreamRWTest()
    {
    }
};


TEST_F(StreamRWTest, open)
{
    EXPECT_TRUE(IsOpen(stream));
}

TEST_F(StreamRWTest, write)
{
    rw->Write(symbol);
    rw->Write(integer);
    stream->flush();
    EXPECT_EQ((int)Length(stream), 5);
}

TEST_F(StreamRWTest, read)
{
    rw->Write(symbol);
    rw->Write(integer);

    EXPECT_EQ(rw->Seek(0), 0);
    EXPECT_EQ(rw->Read<char>(), symbol);
    EXPECT_EQ(rw->Read<_INT32> (), integer);
}

TEST_F(StreamRWTest, rw_array)
{
    int sarr[3] = {99, 0, -3};
    int darr[3] = {-1,-1,-1};
    rw->WriteArray(reinterpret_cast<char*>(sarr), sizeof (sarr));
    rw->Seek(0);
    rw->ReadArray(reinterpret_cast<char*>(darr), sizeof (darr));
    ASSERT_FALSE( memcmp(sarr, darr, sizeof(darr)) );
}

TEST_F(StreamRWTest, seek)
{
    stream->write(array<char, 16>().data(), 16);
    stream->seek(2);
    int dataVal(1234567890);
    stream->write(reinterpret_cast<char*>(&dataVal), 4);
    EXPECT_EQ(Length(stream), 16);
}
