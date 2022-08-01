#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "streamrw.h"
#include "stream_wrapper.h"

using namespace testing;
using namespace std;
using namespace CFCPP;

struct StreamRWTest : testing::Test
{
    wstring filename = L"../../../data/types/types.bin";
    Stream stream;
    shared_ptr<StreamRW> rw;
    const char symbol = 'a';
    const int integer = 13;

    StreamRWTest() : stream(getStream(filename, true))
    {
        rw.reset(new StreamRW(stream));
    }

    ~StreamRWTest()
    {
    }
};


TEST_F(StreamRWTest, test_stream_open)
{
    EXPECT_TRUE(stream != nullptr);
    NSFile::CFileBinary::Remove(filename);
}

TEST_F(StreamRWTest, test_stream_write)
{
    rw->Seek(0);
    rw->Write(symbol);
    rw->Write(integer);
    flush(stream);
    EXPECT_EQ(size(stream), 5);
}


TEST_F(StreamRWTest, test_stream_read)
{
    EXPECT_EQ(rw->Seek(0), 0);
    EXPECT_EQ(rw->Read<char>(), symbol);
    EXPECT_EQ(rw->Read<int>(), integer);
    NSFile::CFileBinary::Remove(filename);
}

TEST_F(StreamRWTest, test_stream_rw_array)
{
    int sarr[3] = {99, 0, -3};
    int darr[3] = {1,  2,  4};
    rw->WriteArray(reinterpret_cast<BYTE*>(sarr), sizeof (sarr));
    flush(stream);
    EXPECT_EQ(rw->Seek(0), 0);
    EXPECT_EQ(stream->SizeFile(), 12);
    rw->ReadArray(reinterpret_cast<BYTE*>(darr), sizeof (darr));
    EXPECT_EQ(sarr[2], darr[2]);

    NSFile::CFileBinary::Remove(filename);
}
