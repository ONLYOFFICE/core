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

#include <vector>
#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../../DesktopEditor/common/File.h"
#include "../Stream/fstream_utils.h"
#include "../Stream/stream_utils.h"
#include "../../DesktopEditor/common/Directory.h"


using namespace testing;
using namespace std;
using namespace CFCPP;


constexpr int _70MBLen = 1024 * 1024 * 70;
const vector<BYTE> _8ByteData = {0x28, 0xFF, 0x28, 0x1D, 0x4C, 0xFA, 0x00, 0x79};
const vector<BYTE> _70MBVector(_70MBLen, 0x90);

const wstring testDataPath = L"../../../data/";
const wstring sourcePath = testDataPath + L"src/";
const wstring outPath = testDataPath + L"out/";


void EXPECT_FILE_EXIST(const wstring& filePath)
{
    EXPECT_TRUE(NSFile::CFileBinary::Exists(filePath));
}

void EXPECT_FILE_FNV_HASH(const wstring& filePath, LONG hash)
{
    EXPECT_EQ(CFCPP::FileFNVHash(filePath), hash);
}

wstring InitOutPath(const wstring& filename)
{
    NSDirectory::CreateDirectory(outPath);
    wstring fullpath = outPath + filename;
    NSFile::CFileBinary::Remove(fullpath);

    return fullpath;
}


vector<wstring> arrForeignFileNames=
{
    L"file",        // Englang
    L"fil",         // Danish
    L"文件",         // Chinese
    L"Datei",       // German
    L"ファイル",     // Japanese
    L"soubor",      // Czech
    L"dosya",       // Turkish
    L"Файл",        // Russian
    L"dossier",     // Franch
    L"expediente"   // Spanish
};
