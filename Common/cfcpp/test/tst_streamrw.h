/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

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
