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

#include "../common.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include "gtest/gtest.h"

void processTestFile(const std::wstring &tempDir, const std::wstring &testFile, const std::wstring &resultFile, const std::wstring &exampleFile)
{

        boost::filesystem::path rootPath = std::wstring{L".."} + FILE_SEPARATOR_STR;
        rootPath =boost::filesystem::absolute(rootPath.wstring() + rootPath.wstring() + rootPath.wstring()+ rootPath.wstring());
        boost::filesystem::path filePath = rootPath.wstring() +L"OOXML" + FILE_SEPARATOR_STR + L"test" + FILE_SEPARATOR_STR +L"ExampleFiles"
            + FILE_SEPARATOR_STR + L"xlsb2xlsx" +   FILE_SEPARATOR_STR + testFile;
         boost::filesystem::path examplePath = rootPath.wstring() +L"OOXML" + FILE_SEPARATOR_STR + L"test" + FILE_SEPARATOR_STR +L"ExampleFiles"
            + FILE_SEPARATOR_STR + L"xlsb2xlsx" +   FILE_SEPARATOR_STR + exampleFile;
        
        std::wstring resultPath =  tempDir + FILE_SEPARATOR_STR + resultFile;

        auto paramsPath = CreateParamsFile(filePath.wstring(), resultPath, tempDir);
        ConvertFile(paramsPath);
        PrepareFiles(resultPath, examplePath.wstring(), tempDir);
}

class SimpleTests1 : public ::testing::Test 
{
public:

    static void SetUpTestCase() 
    {
        
        tempDir = GetWorkDir();
        processTestFile(tempDir, L"simple1.xlsb", L"result.xlsx", L"simple1.xlsx");
    }


    static void TearDownTestCase() 
    {
        RemoveWorkDir(tempDir);
    }

    static std::wstring tempDir;
};
class SimpleTests2 : public ::testing::Test 
{
public:

    static void SetUpTestCase() 
    {
        
        tempDir = GetWorkDir();
        processTestFile(tempDir, L"simple2.xlsb", L"result.xlsx", L"simple2.xlsx");
    }


    static void TearDownTestCase() 
    {
        RemoveWorkDir(tempDir);
    }

    static std::wstring tempDir;
};

class FmlaTest : public ::testing::Test 
{
public:

    static void SetUpTestCase() 
    {
        
        tempDir = GetWorkDir();
        processTestFile(tempDir, L"fmla.xlsb", L"result.xlsx", L"fmla.xlsx");
    }


    static void TearDownTestCase() 
    {
        RemoveWorkDir(tempDir);
    }

    static std::wstring tempDir;
};

std::wstring SimpleTests1::tempDir = L"";
std::wstring SimpleTests2::tempDir = L"";
std::wstring FmlaTest::tempDir = L"";

_UINT32 readFiles(const std::wstring &filePath, const std::wstring &examplePath, std::wstring &fileContent, std::wstring &exampleContent )
{
  
    boost::filesystem::path path1(filePath);
    boost::filesystem::path path2(examplePath);
    path1 = boost::filesystem::absolute(path1);
    path2 = boost::filesystem::absolute(path2);
    std::ifstream file1(path1.string());
    std::ifstream file2(path2.string());

    if (!file1.is_open() || !file2.is_open()) 
    {
        return 1;
    }
    fileContent = std::wstring((std::istreambuf_iterator<char>(file1)), std::istreambuf_iterator<char>());
    exampleContent = std::wstring((std::istreambuf_iterator<char>(file2)), std::istreambuf_iterator<char>());

    return 0;
}



TEST_F(SimpleTests1, ContentTypesTest)
{
    auto tempDir = SimpleTests1::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"[Content_Types].xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR + L"example_unpacked"+ FILE_SEPARATOR_STR + L"[Content_Types].xml");
    std::wstring content1;
    std::wstring content2;
    ASSERT_EQ(readFiles(path1, path2, content1, content2), 0);
    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
}

TEST_F(SimpleTests1, WorkbookTest)
{
    auto tempDir = SimpleTests1::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"workbook.xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"workbook.xml");
    std::wstring content1;
    std::wstring content2;
    ASSERT_EQ(readFiles(path1, path2, content1, content2), 0);
    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
}

TEST_F(SimpleTests1, StylesTest)
{
    auto tempDir = SimpleTests1::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"styles.xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"styles.xml");
    std::wstring content1;
    std::wstring content2;
    ASSERT_EQ(readFiles(path1, path2, content1, content2), 0);
    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
}

TEST_F(SimpleTests1, SharedStringsTest)
{
    auto tempDir = SimpleTests1::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"sharedStrings.xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"sharedStrings.xml");
    std::wstring content1;
    std::wstring content2;
    ASSERT_EQ(readFiles(path1, path2, content1, content2), 0);
    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
}

TEST_F(SimpleTests1, WorksheetsTest)
{
    auto tempDir = SimpleTests1::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet1.xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet1.xml");
    std::wstring content1;
    std::wstring content2;
    ASSERT_EQ(readFiles(path1, path2, content1, content2), 0);
    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
}

TEST_F(SimpleTests2, ContentTypesTest)
{
    auto tempDir = SimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"[Content_Types].xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR + L"example_unpacked"+ FILE_SEPARATOR_STR + L"[Content_Types].xml");
    std::wstring content1;
    std::wstring content2;
    ASSERT_EQ(readFiles(path1, path2, content1, content2), 0);
    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
}

TEST_F(SimpleTests2, WorkbookTest)
{
    auto tempDir = SimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"workbook.xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"workbook.xml");
    std::wstring content1;
    std::wstring content2;
    ASSERT_EQ(readFiles(path1, path2, content1, content2), 0);
    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
}

TEST_F(SimpleTests2, StylesTest)
{
    auto tempDir = SimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"styles.xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"styles.xml");
    std::wstring content1;
    std::wstring content2;
    ASSERT_EQ(readFiles(path1, path2, content1, content2), 0);
    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
}

TEST_F(SimpleTests2, SharedStringsTest)
{
    auto tempDir = SimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"sharedStrings.xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"sharedStrings.xml");
    std::wstring content1;
    std::wstring content2;
    ASSERT_EQ(readFiles(path1, path2, content1, content2), 0);
    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
}

TEST_F(SimpleTests2, WorksheetsTest)
{
    auto tempDir = SimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet1.xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet1.xml");
    std::wstring content1;
    std::wstring content2;
    ASSERT_EQ(readFiles(path1, path2, content1, content2), 0);
    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
}

TEST_F(FmlaTest, ContentTypesTest)
{
    auto tempDir = FmlaTest::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"[Content_Types].xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR + L"example_unpacked"+ FILE_SEPARATOR_STR + L"[Content_Types].xml");
    std::wstring content1;
    std::wstring content2;
    ASSERT_EQ(readFiles(path1, path2, content1, content2), 0);
    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
}

TEST_F(FmlaTest, WorkbookTest)
{
    auto tempDir = FmlaTest::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"workbook.xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"workbook.xml");
    std::wstring content1;
    std::wstring content2;
    ASSERT_EQ(readFiles(path1, path2, content1, content2), 0);
    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
}

TEST_F(FmlaTest, StylesTest)
{
    auto tempDir = FmlaTest::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"styles.xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"styles.xml");
    std::wstring content1;
    std::wstring content2;
    ASSERT_EQ(readFiles(path1, path2, content1, content2), 0);
    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
}

TEST_F(FmlaTest, SharedStringsTest)
{
    auto tempDir = FmlaTest::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"sharedStrings.xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"sharedStrings.xml");
    std::wstring content1;
    std::wstring content2;
    ASSERT_EQ(readFiles(path1, path2, content1, content2), 0);
    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
}

TEST_F(FmlaTest, WorksheetsTest)
{
    auto tempDir = FmlaTest::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet1.xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet1.xml");
    std::wstring content1;
    std::wstring content2;
    ASSERT_EQ(readFiles(path1, path2, content1, content2), 0);
    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
}