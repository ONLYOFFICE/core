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

#include "./../common_deploy.h"
#include "../docbuilder.h"
#include "../../common/File.h"

#ifdef LINUX
#include "../../../DesktopEditor/common/File.h"
#endif

#include <iostream>

#include <string>
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)

#ifdef WIN32
void parse_args(NSDoctRenderer::CDocBuilder* builder, int argc, wchar_t *argv[])
#else
void parse_args(NSDoctRenderer::CDocBuilder* builder, int argc, char *argv[])
#endif
{
    for (int i = 0; i < argc; ++i)
    {
#ifdef WIN32
        std::wstring sW(argv[i]);
        std::string sParam = U_TO_UTF8(sW);
#else
        std::string sParam(argv[i]);
#endif

        if (sParam.find("--") == 0)
        {
            std::string::size_type _pos = sParam.find('=');
            if (std::string::npos == _pos)
                builder->SetProperty(sParam.c_str(), L"");
            else
            {
                std::string sName = sParam.substr(0, _pos);
                std::string sValue = sParam.substr(_pos + 1);

                std::wstring sValueW = UTF8_TO_U(sValue);
                builder->SetProperty(sName.c_str(), sValueW.c_str());
            }
        }
        else
            continue;
    }
}

#ifdef WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    if (argc <= 0)
        return 0;

    bool bIsHelp = false;
    bool bIsFonts = false;
    for (int i = 0; i < argc; ++i)
    {
#ifdef WIN32
        std::wstring sW(argv[i]);
        std::string sParam(sW.begin(), sW.end());
#else
        std::string sParam(argv[i]);
#endif
        if (sParam == "-v" || sParam == "-version")
        {
            std::cout << "v" VALUE(INTVER) << std::endl;
            NSDoctRenderer::CDocBuilder oBuilder;
            oBuilder.ExecuteCommand(L"checkL");

            char* sSdkVer = oBuilder.GetVersion();
            if (NULL != sSdkVer)
            {
                std::string sSdkVerStd(sSdkVer);
                std::cout << "sdk version: " << sSdkVerStd << std::endl;
                delete [] sSdkVer;
            }

            return 0;
        }
        if (sParam == "-h" || sParam == "-help")
        {
            bIsHelp = true;
        }
        else if (sParam == "-f" || sParam == "-fonts")
        {
            bIsFonts = true;
        }
    }

    if (bIsFonts)
    {
        NSDoctRenderer::CDocBuilder oBuilder;
        parse_args(&oBuilder, argc, argv);

        oBuilder.ExecuteCommand(L"checkL");
        return 0;
    }

    if (argc < 2 || bIsHelp)
    {
        std::cout << "USAGE: documentbuilder \"path_to_script_file\"" << std::endl;
        NSDoctRenderer::CDocBuilder oBuilder;
        oBuilder.ExecuteCommand(L"checkL");
        return 0;
    }

#ifdef WIN32
    std::wstring sBuildFile(argv[argc - 1]);
#else
    std::string sBuildFileA(argv[argc - 1]);
    std::wstring sBuildFile = UTF8_TO_U(sBuildFileA);
#endif

    if (true)
    {
        NSDoctRenderer::CDocBuilder oBuilder;

        // если отключена эта опция - то она отключится на parse_args
        oBuilder.SetProperty("--check-fonts", L"");

        //oBuilder.SetProperty("--use-doctrenderer-scheme", L"");
        //oBuilder.SetProperty("--work-directory", L"builder");

        parse_args(&oBuilder, argc - 1, argv);

        oBuilder.Run(sBuildFile.c_str());
    }

    NSDoctRenderer::CDocBuilder::Dispose();
    return 0;
}
