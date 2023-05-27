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
#include <fstream>
#include <array>

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
std::string CorrectValue(const std::string& value)
{
    if (value.empty())
        return "";

    const char* data = value.c_str();

    std::string::size_type pos1 = (data[0] == '\"') ? 1 : 0;
    std::string::size_type pos2 = value.length();

    if (data[pos2 - 1] == '\"')
        --pos2;

    return value.substr(pos1, pos2 - pos1);
}

bool SplitStringAsVector(const std::string& sData, const std::string& sDelimiter, std::vector<std::string>& arrOutput)
{
    arrOutput.clear();

    if ( sData.length() )
    {
        std::string sTmp = sData;
        NSStringUtils::string_replaceA(sTmp, ", ", ",");

        size_t pos_start = 0, pos_end, delim_len = sDelimiter.length();
        std::string token = "";

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
    std::string m_sName;
    std::string m_sGuid;
    std::string m_sGuestOS;

    CVm()
    {
        m_sName = "";
        m_sGuid = "";
        m_sGuestOS = "";
    }

    CVm(const std::string& sName, const std::string& sGuid, const std::string& sGuestOS)
    {
        m_sName = sName;
        m_sGuid = sGuid;
        m_sGuestOS = sGuestOS;
    }
};

class CVirtualBox
{
private:
    std::string m_sVbmPath;

    std::string m_sVmUser;
    std::string m_sVmPassword;
    std::string m_sDownloadsDir;
    std::string m_sDesktopUrl;

    std::vector<CVm*> m_arrVms;

public:
    CVirtualBox()
    {
        m_sVmUser = "";
        m_sVmPassword = "";

        // test url, need parse somewhere
        m_sDesktopUrl = "https://s3.eu-west-1.amazonaws.com/repo-doc-onlyoffice-com/desktop/linux/debian/onlyoffice-desktopeditors_7.4.0-125~cef107_amd64.deb";

#ifdef WIN32
        m_sVbmPath = "\"c:\\Program Files\\Oracle\\VirtualBox\\VBoxManage.exe\" ";
#endif

#ifdef LINUX
        m_sVBoxManagePath = "/usr/lib/virtualbox/VBoxManage ";
#endif
    }

    bool InitVms()
    {
        m_arrVms.clear();
        std::string sOutput = ExecuteCommand("list vms");

        std::vector<std::string> arrLines;
        if ( SplitStringAsVector(sOutput, "\n", arrLines) )
        {
            for (size_t i = 0; i < arrLines.size(); i++)
            {
                std::string sLine = arrLines[i];

                std::string::size_type pos1 = sLine.find("{");
                std::string::size_type pos2 = sLine.find("}", pos1);

                if (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1)
                {
                    std::string sGuid = sLine.substr(pos1, pos2 - pos1 + 1);
                    std::string sName = sLine.substr(0, pos1 - 1);
                    sName = CorrectValue(sName);
                    std::string sOs = GetVmOS(sGuid);

                    m_arrVms.push_back(new CVm(sName, sGuid, sOs));
                }
            }
        }

        return m_arrVms.size() > 0;
    }

    bool StartVm(const std::string& sGuid)
    {
        bool bResult = false;

        if ( sGuid.length() )
        {
            std::string sCommand = "startvm " + sGuid;
            std::string sOutput = ExecuteCommand(sCommand);

            bResult = sOutput.find("has been successfully started") != std::string::npos;
        }

        return bResult;
    }

    bool StopVm(const std::string& sGuid, bool bSaveState = false)
    {
        bool bResult = false;

        if ( sGuid.length() )
        {
            std::string sCommand = "controlvm " + sGuid + (bSaveState ? " savestate" : " poweroff");
            std::string sOutput = ExecuteCommand(sCommand);

            bResult = sOutput.find("100%") != std::string::npos;
        }

        return bResult;
    }

    bool GetScreenshot(const std::string& sGuid, const std::string& sFilePath)
    {
        bool bResult = false;

        if ( sGuid.length() )
        {
            if ( NSFile::CFileBinary::Exists(UTF8_TO_U(sFilePath)) )
                NSFile::CFileBinary::Remove(UTF8_TO_U(sFilePath));

            std::string sCcommand = "controlvm " + sGuid + " screenshotpng " + sFilePath;
            std::string sOutput = ExecuteCommand(sCcommand);

            bResult = NSFile::CFileBinary::Exists(UTF8_TO_U(sFilePath));
        }

        return bResult;
    }

    bool DownloadDesktop(const std::string& sGuid)
    {
        bool bResult = false;

        if ( sGuid.length() && m_sDesktopUrl.length() )
        {
            if ( PrepareWorkingDir(sGuid) )
            {
                std::string sCommand = "--nologo guestcontrol " + sGuid +
                                      " run --exe /usr/bin/wget" +
                                      " --username " + m_sVmUser +
                                      " --password " + m_sVmPassword +
                                      " --wait-stdout -- wget/arg0 " + m_sDesktopUrl +
                                      " -P " + m_sDownloadsDir;

                std::string sOutput = ExecuteCommand(sCommand);

                bResult = sOutput.find("") != std::string::npos;
            }
        }

        return bResult;
    }

private:
    bool PrepareWorkingDir(const std::string& sGuid)
    {
        bool bResult = false;

        if ( sGuid.length() && m_sVmUser.length() )
        {
            m_sDownloadsDir = "/home/" + m_sVmUser + "/vboxtester";

            std::string sCommand = "guestcontrol " + sGuid +
                                   " --username " + m_sVmUser +
                                   " --password " + m_sVmPassword +
                                   " rmdir " + m_sDownloadsDir;

            std::string sOutput = ExecuteCommand(sCommand);

            sCommand = "guestcontrol " + sGuid +
                       " --username " + m_sVmUser +
                       " --password " + m_sVmPassword +
                       " mkdir " + m_sDownloadsDir;

            sOutput = ExecuteCommand(sCommand);

            bResult = true;
        }

        return bResult;
    }

    std::string GetVmOS(const std::string& sGuid)
    {
        std::string sOs = "";

        if ( sGuid.length() )
        {
            std::string command = "showvminfo " + sGuid;
            std::string sOutput = ExecuteCommand(command);

            std::vector<std::string> arrLines;
            if ( SplitStringAsVector(sOutput, "\n", arrLines) )
            {
                std::string sPrefix = "Guest OS:";

                for (size_t i = 0; i < arrLines.size(); i++)
                {
                    std::string sLine = arrLines[i];

                    std::string::size_type pos = sLine.find(sPrefix);
                    if ( pos != std::string::npos )
                    {
                        sOs = sLine;
                        pos = sOs.find(sPrefix + " ");
                        while ( pos != std::string::npos )
                        {
                            NSStringUtils::string_replaceA(sOs, sPrefix + " ", sPrefix);
                            pos = sOs.find(sPrefix + " ");
                        }
                        NSStringUtils::string_replaceA(sOs, sPrefix, "");
                        break;
                    }
                }
            }
        }

        return sOs;
    }

    std::string ExecuteCommand(const std::string& sArgs)
    {
        std::string sResult = "";

        std::array<char, 128> aBuffer;
        std::string sCommand = m_sVbmPath + sArgs;

 #ifdef WIN32
        FILE* pipe = _popen(sCommand.c_str(), "r");
 #endif
 #ifdef LINUX
        FILE* pipe = popen(sCommand.c_str(), "r");
 #endif
        if (!pipe)
            return sResult;

        while ( fgets(aBuffer.data(), 128, pipe) != NULL )
        {
            sResult += aBuffer.data();
        }

        return sResult;
    }
};

// Main
int main(int argc, char** argv)
{
    // Test
    CVirtualBox oTester;

    oTester.DownloadDesktop("{b9c4a4fe-afcc-47d5-b674-0fcbc11383e3}");

    /*oTester.GetVms();
    oTester.StartVm("{b9c4a4fe-afcc-47d5-b674-0fcbc11383e3}");
    oTester.GetScreenshot("{b9c4a4fe-afcc-47d5-b674-0fcbc11383e3}", "c:\\Tmp\\123.png");
    oTester.StopVm("{b9c4a4fe-afcc-47d5-b674-0fcbc11383e3}");*/

    // Parse arguments
    for (int i = 0; i < argc; ++i)
    {
        std::string sParam(argv[i]);

        if (sParam.find("--") == 0)
        {
            std::string sKey = "";
            std::string sValue = "";

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
                        sValue = std::string(argv[i]);
                    }

                    // Checks if value or next key exist
                    if ( !sValue.length() || (sValue.find("--") == 0) )
                    {
                        std::cout << "\nError. Check input parameters\n";
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
                std::cout << "\nError. Unknown parameter " << sKey << "\n" << "Print usage information --help\n";
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

