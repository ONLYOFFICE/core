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
    string filename = "../../data/types/types.bin";
    const char symbol = 'a';
    const int integer = 13;

    StreamRWTest()
    {
        stream.reset(new std::fstream(filename, ios::app | ios::in | ios::out));
        rw.reset(new StreamRW(stream));
    }

    ~StreamRWTest()
    {
        remove(filename.c_str());
    }
};


TEST_F(StreamRWTest, test_stream_open)
{
    EXPECT_EQ(static_pointer_cast<fstream>(stream)->is_open(), 1);
}

TEST_F(StreamRWTest, test_stream_write)
{
    rw->Write(symbol);
    rw->Write(integer);
    stream->flush();
    EXPECT_EQ((int)Length(stream), 5);
}


TEST_F(StreamRWTest, test_stream_read)
{
    EXPECT_EQ(rw->Seek(-5), 0);
    EXPECT_EQ(rw->Read<char>(), symbol);
    EXPECT_EQ(rw->Read<int>(), integer);
}


#endif // TST_TEST_STREAM_H
