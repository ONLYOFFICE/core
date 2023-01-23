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
#include <stdlib.h>
#include "SystemUtils.h"
#include "Directory.h"

#ifdef _WIN32
#include "ShlObj.h"
#ifdef CreateDirectory
#undef CreateDirectory
#endif
#ifdef CreateFile
#undef CreateFile
#endif
#endif

namespace NSSystemUtils
{
    std::string GetEnvVariableA(const std::wstring& strName)
    {
        std::wstring sTmp = GetEnvVariable(strName);
        return U_TO_UTF8(sTmp);
    }

	std::wstring GetEnvVariable(const std::wstring& strName)
	{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
		wchar_t* pVal = _wgetenv(strName.c_str());
		if (NULL != pVal)
		{
			std::wstring sRes(pVal);
			return sRes;
		}
		else
		{
			return L"";
		}
#else
		std::string strNameA = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strName);
		char* pValA = getenv(strNameA.c_str());
		if (NULL != pValA)
		{
			return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pValA, strlen(pValA));
		}
		else
		{
			return L"";
		}
#endif
	}

    std::wstring GetAppDataDir()
    {
        std::wstring sBranding = GetBuildBranding();
        std::wstring sAppDataPath;

#ifdef _WIN32
        wchar_t sAppDataLocal[65535];

		if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, sAppDataLocal)))
        {
            sAppDataPath = std::wstring(sAppDataLocal);
        }
#else
        std::wstring sHomeDir = NSSystemUtils::GetEnvVariable(L"HOME");

        if (!sHomeDir.empty())
        {
            if (NSDirectory::Exists(sHomeDir + L"/.local/share"))
                sHomeDir = sHomeDir + L"/.local/share";
            else if (NSDirectory::Exists(sHomeDir + L"/.local"))
                sHomeDir = sHomeDir + L"/.local";
        }

        sAppDataPath = sHomeDir;
#endif

        if (!NSDirectory::Exists(sAppDataPath))
            return L"";

        sAppDataPath += (L"/" + sBranding);

        if (!NSDirectory::Exists(sAppDataPath))
            NSDirectory::CreateDirectory(sAppDataPath);

        return sAppDataPath;
    }

    std::wstring GetBuildBranding()
    {
        std::string sBrandingA = "ONLYOFFICE";
    #ifdef BUILD_BRANDING_NAME
        sBrandingA = BUILD_BRANDING_NAME;
    #endif

        return UTF8_TO_U(sBrandingA);
    }
}
