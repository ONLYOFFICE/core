/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "../common.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include "gtest/gtest.h"

namespace xlsb2xlsxTests
{
void processTestFile(const std::wstring &tempDir, const std::wstring &testFile, const std::wstring &resultFile, const std::wstring &exampleFile)
{

        boost::filesystem::path rootPath = std::wstring{L".."} + FILE_SEPARATOR_STR;
        rootPath =boost::filesystem::absolute(rootPath.wstring() + rootPath.wstring() + rootPath.wstring()+ rootPath.wstring());
        boost::filesystem::path filePath = rootPath.wstring() +L"OOXML" + FILE_SEPARATOR_STR + L"test" + FILE_SEPARATOR_STR +L"ExampleFiles"
            + FILE_SEPARATOR_STR + L"xlsx2xlsb" +   FILE_SEPARATOR_STR + testFile;
         boost::filesystem::path examplePath = rootPath.wstring() +L"OOXML" + FILE_SEPARATOR_STR + L"test" + FILE_SEPARATOR_STR +L"ExampleFiles"
            + FILE_SEPARATOR_STR + L"xlsx2xlsb" +   FILE_SEPARATOR_STR + exampleFile;
        
        std::wstring resultPath =  tempDir + FILE_SEPARATOR_STR + resultFile;

        auto paramsPath = CreateParamsFile(filePath.wstring(), resultPath, tempDir);
        ConvertFile(paramsPath);
        PrepareFiles(resultPath, examplePath.wstring(), tempDir);
}

class XlsbSimpleTests1 : public ::testing::Test 
{
public:

    static void SetUpTestCase() 
    {
        
        tempDir = GetWorkDir();
        processTestFile(tempDir, L"simple1.xlsx", L"result.xlsb", L"simple1.xlsb");
    }


    static void TearDownTestCase() 
    {
        RemoveWorkDir(tempDir);
    }

    static std::wstring tempDir;
};
class XlsbSimpleTests2 : public ::testing::Test 
{
public:

    static void SetUpTestCase() 
    {
        
        tempDir = GetWorkDir();
        processTestFile(tempDir, L"simple2.xlsx", L"result.xlsb", L"simple2.xlsb");
    }


    static void TearDownTestCase() 
    {
        RemoveWorkDir(tempDir);
    }

    static std::wstring tempDir;
};
class XlsbFmlaTests : public ::testing::Test 
{
public:

    static void SetUpTestCase() 
    {
        
        tempDir = GetWorkDir();
        processTestFile(tempDir, L"fmla.xlsx", L"result.xlsb", L"fmla.xlsb");
    }


    static void TearDownTestCase() 
    {
        RemoveWorkDir(tempDir);
    }

    static std::wstring tempDir;
};

std::wstring XlsbSimpleTests1::tempDir = L"";
std::wstring XlsbSimpleTests2::tempDir = L"";
std::wstring XlsbFmlaTests::tempDir = L"";

_UINT32 readBinaryFiles(const std::wstring &filePath, const std::wstring &examplePath, std::vector<char> &fileContent, std::vector<char> &exampleContent)
{
    boost::filesystem::path path1(filePath);
    boost::filesystem::path path2(examplePath);
    path1 = boost::filesystem::absolute(path1);
    path2 = boost::filesystem::absolute(path2);
    std::ifstream file1(path1.string(), std::ios::binary);
    std::ifstream file2(path2.string(), std::ios::binary);

    if (!file1.is_open() || !file2.is_open())
    {
        return 1;
    }

    fileContent = std::vector<char>((std::istreambuf_iterator<char>(file1)), std::istreambuf_iterator<char>());
    exampleContent = std::vector<char>((std::istreambuf_iterator<char>(file2)), std::istreambuf_iterator<char>());

    return 0;
}


TEST_F(XlsbSimpleTests1, ContentTypesTest)
{
    auto tempDir = XlsbSimpleTests1::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"[Content_Types].xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR + L"example_unpacked"+ FILE_SEPARATOR_STR + L"[Content_Types].xml");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests1, WorkbookTest)
{
    auto tempDir = XlsbSimpleTests1::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"workbook.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"workbook.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests1, StylesTest)
{
   auto tempDir = XlsbSimpleTests1::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"styles.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"styles.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests1, SharedStringsTest)
{
   auto tempDir = XlsbSimpleTests1::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"sharedStrings.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"sharedStrings.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests1, WorksheetsTest)
{
   auto tempDir = XlsbSimpleTests1::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet1.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet1.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests2, ContentTypesTest)
{
    auto tempDir = XlsbSimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"[Content_Types].xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR + L"example_unpacked"+ FILE_SEPARATOR_STR + L"[Content_Types].xml");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests2, WorkbookTest)
{
    auto tempDir = XlsbSimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"workbook.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"workbook.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests2, StylesTest)
{
    auto tempDir = XlsbSimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"styles.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"styles.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests2, SharedStringsTest)
{
    auto tempDir = XlsbSimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"sharedStrings.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"sharedStrings.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests2, Worksheet1Test)
{
    auto tempDir = XlsbSimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet1.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet1.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests2, Worksheet2Test)
{
    auto tempDir = XlsbSimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet2.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet2.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests2, CommentsTest)
{
   auto tempDir = XlsbSimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"comments2.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"comments2.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests2, TablesTest)
{
    auto tempDir = XlsbSimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"tables" + FILE_SEPARATOR_STR + L"table2.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"tables" + FILE_SEPARATOR_STR + L"table2.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests2, SlicerTest)
{
    auto tempDir = XlsbSimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"slicers" + FILE_SEPARATOR_STR + L"slicer2.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"slicers" + FILE_SEPARATOR_STR + L"slicer2.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests2, SlicerCacheTest)
{
    auto tempDir = XlsbSimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"slicerCaches" + FILE_SEPARATOR_STR + L"slicerCache2.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"slicerCaches" + FILE_SEPARATOR_STR + L"slicerCache2.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests2, PivotTableTest)
{
    auto tempDir = XlsbSimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"pivotTables" + FILE_SEPARATOR_STR + L"pivotTable2.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"pivotTables" + FILE_SEPARATOR_STR + L"pivotTable2.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests2, PivotCacheDefTest)
{
    auto tempDir = XlsbSimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"pivotCache" + FILE_SEPARATOR_STR + L"pivotCacheDefinition2.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"pivotCache" + FILE_SEPARATOR_STR + L"pivotCacheDefinition2.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests2, PivotCacheRecordsTest)
{
    auto tempDir = XlsbSimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"pivotCache" + FILE_SEPARATOR_STR + L"pivotCacheRecords2.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"pivotCache" + FILE_SEPARATOR_STR + L"pivotCacheRecords2.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbSimpleTests2, ChartSheetTest)
{
    auto tempDir = XlsbSimpleTests2::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"chartsheets" + FILE_SEPARATOR_STR + L"sheet1.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"chartsheets" + FILE_SEPARATOR_STR + L"sheet1.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}


TEST_F(XlsbFmlaTests, ContentTypesTest)
{
    auto tempDir = XlsbFmlaTests::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"[Content_Types].xml");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR + L"example_unpacked"+ FILE_SEPARATOR_STR + L"[Content_Types].xml");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbFmlaTests, WorkbookTest)
{
    auto tempDir = XlsbFmlaTests::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"workbook.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"workbook.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbFmlaTests, StylesTest)
{
   auto tempDir = XlsbFmlaTests::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"styles.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"styles.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbFmlaTests, SharedStringsTest)
{
    auto tempDir = XlsbFmlaTests::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"sharedStrings.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"sharedStrings.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbFmlaTests, WorksheetsTest1)
{
   auto tempDir = XlsbFmlaTests::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet1.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet1.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbFmlaTests, WorksheetsTest2)
{
   auto tempDir = XlsbFmlaTests::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet2.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet2.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}

TEST_F(XlsbFmlaTests, WorksheetsTest3)
{
   auto tempDir = XlsbFmlaTests::tempDir;
    std::wstring path1(tempDir + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet3.bin");
    std::wstring path2(tempDir + FILE_SEPARATOR_STR +L"example_unpacked"+ FILE_SEPARATOR_STR + L"xl" + 
        FILE_SEPARATOR_STR + L"worksheets" + FILE_SEPARATOR_STR + L"sheet3.bin");
    std::vector<char> fileContent;
    std::vector<char> exampleContent;
    ASSERT_EQ(readBinaryFiles(path1, path2, fileContent, exampleContent), 0);
    ASSERT_EQ(fileContent, exampleContent);
}


}