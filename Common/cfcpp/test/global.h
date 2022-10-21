#pragma once

#include <vector>
#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"

using namespace testing;
using namespace std;


constexpr int _70MBLen = 1024 * 1024 * 70;
const std::vector<unsigned char> _8ByteData = {0x28, 0xFF, 0x28, 0x1D, 0x4C, 0xFA, 0x00, 0x79};
const std::vector<BYTE> _70MBVector(_70MBLen, 0x90);

const std::wstring testDataPath = L"../../../data/";
const std::wstring sourcePath = testDataPath + L"src/";
const std::wstring outPath = testDataPath + L"out/";



void EXPECT_FILE_EXIST(const std::wstring& filePath)
{
    EXPECT_TRUE(NSFile::CFileBinary::Exists(filePath));
}

std::wstring InitOutPath(const std::wstring& filename)
{
    NSDirectory::CreateDirectory(outPath);
    wstring fullpath = outPath + filename;
    NSFile::CFileBinary::Remove(fullpath);

    return fullpath;
}


std::vector<std::wstring> arrForeignFileNames=
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
