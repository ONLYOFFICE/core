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

#include <iostream>
#include <map>
#include <iomanip>
#include <array>
#include <locale.h>

#include "help.h"
#include "../common/File.h"
#include "../common/Directory.h"
#include "../../DesktopEditor/common/StringBuilder.h"
#include "../../DesktopEditor/common/SystemUtils.h"
#include "../../DesktopEditor/graphics/BaseThread.h"
#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../Common/Network/FileTransporter/include/FileTransporter.h"

#ifdef CreateDirectory
#undef CreateDirectory
#endif

#ifdef GetTempPath
#undef GetTempPath
#endif

#ifdef LINUX
#include <unistd.h>
#include <stdio.h>
#endif

// Misc
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

bool SplitStringAsVector(const std::wstring& sData, const std::wstring& sDelimiter, std::vector<std::wstring>& arrOutput)
{
    arrOutput.clear();

    if ( sData.length() )
    {
        std::wstring sTmp = sData;
        NSStringUtils::string_replace(sTmp, L", ", L",");

        size_t pos_start = 0, pos_end, delim_len = sDelimiter.length();
        std::wstring token = L"";

        while ((pos_end = sTmp.find(sDelimiter, pos_start)) != std::string::npos)
        {
            token = sTmp.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            if (token.length())
                arrOutput.push_back(token);
        }

        token = sTmp.substr(pos_start);
        if ( token.length() )
            arrOutput.push_back(token);
    }

    return arrOutput.size() > 0;
}

//
class CVm
{
public:
    std::wstring m_sName;
    std::wstring m_sGuid;

    CVm()
    {
        m_sName = L"";
        m_sGuid = L"";
    }

    CVm(const std::wstring& sName, const std::wstring& sGuid)
    {
        m_sName = sName;
        m_sGuid = sGuid;
    }
};

class CVirtualBox
{
private:
    std::wstring m_sDir;
    std::vector<CVm*> m_arrVms;

public:
    CVirtualBox()
    {
        m_sDir = L"";
    }

    bool GetVms()
    {
        bool bResult = false;

        std::wstring sOutput = ExecuteCommand(L"list vms");

        return bResult;
    }

private:
    std::wstring ExecuteCommand(const std::wstring& sArgs)
    {
        std::wstring sResult = L"";

        std::array<wchar_t, 128> aBuffer;
        std::wstring command = L"VBoxManage " + sArgs;

        // пусть будет чисто юниксовой темой

        /*FILE* pipe = _wpopen(command.c_str(), "r");
        if (!pipe)
            return false;

        while ( fgetws(aBuffer.data(), 128, pipe) != NULL ) {
            sResult += aBuffer.data();
        }*/

        return sResult;
    }
};

// Main
#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
    setlocale(LC_ALL, "");

    // Test
    CVirtualBox oTester;

    // Parse arguments
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

            // Parse key - value
            std::wstring::size_type pos = sParam.find('=');
            if ( pos == std::wstring::npos )
            {
                sKey = sParam;

                if ( IsNeedSetValue(sKey))
                {
                    if (i < argc - 1)
                    {
                        i++;
#ifdef WIN32
                        sValue = std::wstring(argv[i]);
#else
                        std::string sValueA(argv[i]);
                        sValue = UTF8_TO_U(sValueA);
#endif
                    }

                    // Checks if value or next key exist
                    if ( !sValue.length() || (sValue.find(L"--") == 0) )
                    {
                        std::wcout << L"\nError. Check input parameters\n";
                        return 1;
                    }
                }
            }
            else
            {
                sKey = sParam.substr(0, pos);
                sValue = sParam.substr( pos + 1 );
            }

            // Check key
            if ( !IsCommandExists(sKey) )
            {
                std::wcout << L"\nError. Unknown parameter " << sKey << L"\n" << "Print usage information --help\n";
                return 1;
            }

            // Usability
            if (sKey == sCmdHelp || sKey == sCmdHelpFull)
            {
            }

            // Settings
            else if (sKey == sCmdDir)
            {
                sValue = CorrectValue(sValue);

            }

        }
    }    

    return 0;
}

