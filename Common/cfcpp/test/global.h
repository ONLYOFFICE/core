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
