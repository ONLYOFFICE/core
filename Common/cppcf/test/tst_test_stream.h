#ifndef TST_TEST_STREAM_H
#define TST_TEST_STREAM_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "streamrw.h"

using namespace testing;
using namespace std;
using namespace CFCPP;

struct StreamRWTest : testing::Test
{
    Stream stream;
    shared_ptr<StreamRW> rw;
    string filename = "../data/types/types.bin";
    const char symbol = 'a';
    const int integer = 13;

    StreamRWTest()
    {
        stream.reset(new std::fstream(filename, ios::app | ios::in | ios::out | ios::binary));
        rw.reset(new StreamRW(stream));
    }

    ~StreamRWTest()
    {
    }
};


TEST_F(StreamRWTest, test_stream_open)
{
    EXPECT_EQ(static_pointer_cast<fstream>(stream)->is_open(), 1);
}

TEST_F(StreamRWTest, test_stream_write)
{
    rw->Seek(0);
    rw->Write(symbol);
    rw->Write(integer);
    stream->flush();
    EXPECT_EQ((int)Length(stream), 5);
}


TEST_F(StreamRWTest, test_stream_read)
{
    EXPECT_EQ(rw->Seek(0), 0);
    EXPECT_EQ(rw->Read<char>(), symbol);
    EXPECT_EQ(rw->Read<int>(), integer);
    remove(filename.c_str());
}

TEST_F(StreamRWTest, test_stream_rw_array)
{
    int sarr[3] = {99, 0, -3};
    int darr[3] = {-1,-1,-1};
    rw->WriteArray(reinterpret_cast<char*>(sarr), sizeof (sarr));
    rw->Seek(0);
    rw->ReadArray(reinterpret_cast<char*>(darr), sizeof (darr));
    EXPECT_EQ(sarr[2], darr[2]);

    remove(filename.c_str());
}

#endif // TST_TEST_STREAM_H
