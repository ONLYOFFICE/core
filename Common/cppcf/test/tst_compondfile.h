#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "compoundfile.h"

using namespace testing;
using namespace std;
using namespace CFCPP;


struct CompoundFileTest : testing::Test
{
    wstring filename = L"../../../data/ex.ppt";
    CompoundFile cf;

    CompoundFileTest() : cf(filename)
    {

    }
};

TEST_F(CompoundFileTest, test_compoundfile_read)
{
    EXPECT_TRUE(cf.HasSourceStream());
}
