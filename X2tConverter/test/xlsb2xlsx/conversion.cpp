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

#include "common.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <fstream>

TEST(simpleConversion, conversionTest)
{
    ASSERT_TRUE(true);
}

TEST(simpleConversion, ContentTypesTest)
{
    auto tempDir = Xlsb2XlsxConversion1::tempDirName;
    boost::filesystem::path path1(Xlsb2XlsxConversion1::tempDirName + FILE_SEPARATOR_STR + L"result_unpacked"+ FILE_SEPARATOR_STR + L"[Content_Types].xml");
    boost::filesystem::path path2(Xlsb2XlsxConversion1::tempDirName + FILE_SEPARATOR_STR +L"xlsx_unpacked"+ FILE_SEPARATOR_STR + L"[Content_Types].xml");
    path1 = boost::filesystem::absolute(path1);
    path2 = boost::filesystem::absolute(path2);
    std::ifstream file1(path1.string());
    std::ifstream file2(path2.string());

    if (!file1.is_open() || !file2.is_open()) 
    {
        ASSERT_TRUE(false);
    }

    std::string content1((std::istreambuf_iterator<char>(file1)), std::istreambuf_iterator<char>());
    std::string content2((std::istreambuf_iterator<char>(file2)), std::istreambuf_iterator<char>());

    ASSERT_TRUE(boost::algorithm::equals(content1, content2));
   
}
