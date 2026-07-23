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
