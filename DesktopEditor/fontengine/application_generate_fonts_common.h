/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#ifndef APPLICATION_GENERATE_FONTS_COMMON_H
#define APPLICATION_GENERATE_FONTS_COMMON_H

#include "../common/File.h"
#include "../common/Directory.h"
#include "../common/StringBuilder.h"

namespace NSCommon
{
    static void string_replace(std::wstring& text, const std::wstring& replaceFrom, const std::wstring& replaceTo)
    {
        size_t posn = 0;
        while (std::wstring::npos != (posn = text.find(replaceFrom, posn)))
        {
            text.replace(posn, replaceFrom.length(), replaceTo);
            posn += replaceTo.length();
        }
    }
    static void string_replaceA(std::string& text, const std::string& replaceFrom, const std::string& replaceTo)
    {
        size_t posn = 0;
        while (std::string::npos != (posn = text.find(replaceFrom, posn)))
        {
            text.replace(posn, replaceFrom.length(), replaceTo);
            posn += replaceTo.length();
        }
    }

    static void url_correct(std::wstring& url)
    {
        string_replace(url, L"/./", L"/");

        size_t posn = 0;
        while (std::wstring::npos != (posn = url.find(L"/../")))
        {
            std::wstring::size_type pos2 = url.rfind(L"/", posn - 1);

            if (std::wstring::npos != pos2)
            {
                url.erase(pos2, posn - pos2 + 3);
            }
        }
        
        // MAC
        if (0 == url.find(L"file:/") && 0 != url.find(L"file://"))
        {
            url.replace(0, 6, L"file:///");
        }
    }

    static void makeUpper(std::string& url)
    {
        int nLen = (int)url.length();
        char* pStr = (char*)url.c_str();

        for (int i = 0; i < nLen; ++i)
        {
            if (pStr[i] >= 'a' && pStr[i] <= 'z')
                pStr[i] = pStr[i] + 'A' - 'a';
        }
    }
    static void makeUpperW(std::wstring& url)
    {
        int nLen = (int)url.length();
        wchar_t* pStr = (wchar_t*)url.c_str();

        for (int i = 0; i < nLen; ++i)
        {
            if (pStr[i] >= 'a' && pStr[i] <= 'z')
                pStr[i] = pStr[i] + 'A' - 'a';
        }
    }
    static void makeLower(std::string& url)
    {
        int nLen = (int)url.length();
        char* pStr = (char*)url.c_str();

        for (int i = 0; i < nLen; ++i)
        {
            if (pStr[i] >= 'A' && pStr[i] <= 'Z')
                pStr[i] = pStr[i] + 'a' - 'A';
        }
    }
    static void makeLowerW(std::wstring& url)
    {
        int nLen = (int)url.length();
        wchar_t* pStr = (wchar_t*)url.c_str();

        for (int i = 0; i < nLen; ++i)
        {
            if (pStr[i] >= 'A' && pStr[i] <= 'Z')
                pStr[i] = pStr[i] + 'a' - 'A';
        }
    }
    
    static void WriteLog(const std::string& sLogFile, const std::wstring& sData)
    {
        FILE* f = fopen(sLogFile.c_str(), "a+");
        std::string sDataA = U_TO_UTF8(sData);
        fprintf(f, sDataA.c_str());
        fprintf(f, "\n");
        fclose(f);
    }
}

namespace NSCommon
{
    static std::wstring GetFileExtention(const std::wstring& sPath)
    {
        std::wstring::size_type nPos = sPath.rfind('.');
        if (nPos != std::wstring::npos)
            return sPath.substr(nPos + 1);
        return sPath;
    }
    static std::wstring GetFileName(const std::wstring& sPath)
    {
        std::wstring::size_type nPos1 = sPath.rfind('\\');
        std::wstring::size_type nPos2 = sPath.rfind('/');
        std::wstring::size_type nPos = std::wstring::npos;

        if (nPos1 != std::wstring::npos)
        {
            nPos = nPos1;
            if (nPos2 != std::wstring::npos && nPos2 > nPos)
                nPos = nPos2;
        }
        else
            nPos = nPos2;

        if (nPos == std::wstring::npos)
            return sPath;
        return sPath.substr(nPos + 1);
    }
    static std::wstring GetDirectoryName(const std::wstring& sPath)
    {
        std::wstring::size_type nPos1 = sPath.rfind('\\');
        std::wstring::size_type nPos2 = sPath.rfind('/');
        std::wstring::size_type nPos = std::wstring::npos;

        if (nPos1 != std::wstring::npos)
        {
            nPos = nPos1;
            if (nPos2 != std::wstring::npos && nPos2 > nPos)
                nPos = nPos2;
        }
        else
            nPos = nPos2;

        if (nPos == std::wstring::npos)
            return sPath;
        return sPath.substr(0, nPos);
    }
}

#endif // APPLICATION_GENERATE_FONTS_COMMON_H
