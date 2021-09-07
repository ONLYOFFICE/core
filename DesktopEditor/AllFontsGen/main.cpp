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
#include <iostream>
#include <set>

#include "../fontengine/ApplicationFontsWorker.h"
#include "../common/Directory.h"

//#define _GENERATE_FONT_MAP_

#ifdef _GENERATE_FONT_MAP_
#include "../freetype_names/FontMaps/FontDictionary.h"
#endif

std::wstring CorrectDir(const std::wstring& sDir)
{
    if (sDir.empty())
        return L"";

    const wchar_t* data = sDir.c_str();

    std::wstring::size_type pos1 = (data[0] == '\"') ? 1 : 0;
    std::wstring::size_type pos2 = sDir.length();

    if (data[pos2 - 1] == '\"')
        --pos2;

    if (pos2 > 0 && ((data[pos2 - 1] == '\\') || (data[pos2 - 1] == '/')))
        --pos2;

    return sDir.substr(pos1, pos2 - pos1);
}

std::wstring CorrectValue(const std::wstring& value)
{
    if (value.empty())
        return L"";

    const wchar_t* data = value.c_str();

    std::wstring::size_type pos1 = (data[0] == '\"') ? 1 : 0;
    std::wstring::size_type pos2 = value.length();

    if (data[pos2 - 1] == '\"')
        --pos2;

    return value.substr(pos1, pos2 - pos1);
}

#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
    std::vector<std::wstring> arFontsDirs;
    bool bIsUseSystemFonts = false;
    std::wstring strAllFontsWebPath = L"";
    std::wstring strAllFontsPath = L"";
    std::wstring strThumbnailsFolder = L"";
    std::wstring strFontsSelectionBin = L"";
    std::wstring strOutputDir = L"";
    int nFontFlag = 3;

    for (int i = 0; i < argc; ++i)
    {
#ifdef WIN32
        std::wstring sParam(argv[i]);
#else
        std::string sParamA(argv[i]);
        std::wstring sParam = UTF8_TO_U(sParamA);
#endif

        if (sParam.find(L"--") == 0)
        {
            std::wstring sKey = L"";
            std::wstring sValue = L"";

            std::wstring::size_type _pos = sParam.find('=');
            if (std::wstring::npos == _pos)
            {
                sKey = sParam;
            }
            else
            {
                sKey = sParam.substr(0, _pos);
                sValue = sParam.substr(_pos + 1);
            }

            if (sKey == L"--use-system")
            {
                sValue = CorrectValue(sValue);
                if (sValue == L"1" || sValue == L"true")
                    bIsUseSystemFonts = true;
            }
            else if (sKey == L"--allfonts-web")
            {
                strAllFontsWebPath = CorrectDir(sValue);
            }
            else if (sKey == L"--allfonts")
            {
                strAllFontsPath = CorrectDir(sValue);
            }
            else if (sKey == L"--images")
            {
                strThumbnailsFolder = CorrectDir(sValue);
            }
            else if (sKey == L"--selection")
            {
                strFontsSelectionBin = CorrectDir(sValue);
            }
            else if (sKey == L"--input")
            {
                const wchar_t* src = sValue.c_str();
                const wchar_t* limit = src + sValue.length();

                const wchar_t* srcPrev = src;
                while (src < limit)
                {
                    if (*src == ';')
                    {
                        if (srcPrev != src)
                        {
                            arFontsDirs.push_back(CorrectDir(std::wstring(srcPrev, src - srcPrev)));
                        }
                        src++;
                        srcPrev = src;
                    }
                    else
                        src++;
                }

                if (src > srcPrev)
                {
                    arFontsDirs.push_back(CorrectDir(std::wstring(srcPrev, src - srcPrev)));
                }
            }
            else if (sKey == L"--output-web")
            {
                strOutputDir = CorrectDir(sValue);
            }
            else if (sKey == L"--font-format")
            {
                // first byte => isSupportCFF
                // second byte => isUnsupport DFont (mac)

                int nFlag = std::stoi(sValue);
                if (nFlag > 0)
                    nFontFlag = nFlag;
            }
        }
    }

    /*
    --input="./fontsInput" --allfonts="./fonts/AllFonts.js" --allfonts-web="./fonts/AllFonts2.js" --images="./fonts" --selection="./fonts/font_selection.bin" --output-web="./fonts" --use-system="false"
    */

    CApplicationFontsWorker oWorker;
    // это не рабочая папка, где только шрифты
    oWorker.m_bIsCleanDirectory = false;
    oWorker.m_bIsRemoveOldThumbnails = true;

    // input fonts
    oWorker.m_bIsUseSystemFonts = bIsUseSystemFonts;
    oWorker.m_arAdditionalFolders = arFontsDirs;

    // font_selection.bin
    // fonts.log
    oWorker.m_sDirectory = NSFile::GetDirectoryName(strFontsSelectionBin);

    // thumbnails
    oWorker.m_sThumbnailsDirectory = strThumbnailsFolder;
    oWorker.m_bIsNeedThumbnails = !oWorker.m_sThumbnailsDirectory.empty();
    oWorker.m_bSeparateThumbnails = true;

    // allfonts
    oWorker.m_sAllFontsJSPath = strAllFontsPath;
    oWorker.m_sWebAllFontsJSPath = strAllFontsWebPath;

    // webfonts
    oWorker.m_sWebFontsDirectory = strOutputDir;

    // opentype
    oWorker.m_bIsUseOpenType = (0x01 == (0x01 & nFontFlag));

    NSFonts::IApplicationFonts* pApplicationFonts = oWorker.Check();
    if (oWorker.m_bIsNeedThumbnails)
        oWorker.CheckThumbnails();

    RELEASEINTERFACE(pApplicationFonts);


#ifdef _GENERATE_FONT_MAP_

    NSCommon::DumpFontsDictionary(L"./fonts_dictionary.txt");

#endif

    return 0;
}

