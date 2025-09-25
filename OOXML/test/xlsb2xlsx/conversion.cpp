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