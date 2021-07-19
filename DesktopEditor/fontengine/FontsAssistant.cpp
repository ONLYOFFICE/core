/*
* (c) Copyright Ascensio System SIA 2010-2020
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

#include "FontsAssistant.h"
#include "ApplicationFontsWorker.h"

namespace ASC
{
    CFontsAssistant::CFontsAssistant(const std::vector<std::wstring>& arFontsPaths, const std::wstring& sDataFontsPath)
    {
        m_arFontsPaths = arFontsPaths;
        m_sDataFontsPath = sDataFontsPath;
    }

    void CFontsAssistant::AddExcludeFont(const std::wstring& sFontName) {
        m_arExcludeFonts.push_back(sFontName);
    }

    std::wstring CFontsAssistant::GetSystemFontPath()
    {
    #if defined(_IOS) || defined(_MAC)
        return std::wstring(L"/System/Library/Fonts");
    #elif defined(__ANDROID__)
        return std::wstring(L"/system/fonts");
    #endif
        return std::wstring(L"");
    }

    NSFonts::IApplicationFonts* CFontsAssistant::Load()
    {
        CApplicationFontsWorker oFontsWorker;
    
        oFontsWorker.m_bIsUseOpenType = true;
        oFontsWorker.m_bIsUseSystemFonts = true;
        oFontsWorker.m_bIsNeedThumbnails = false;
        oFontsWorker.m_sDirectory = m_sDataFontsPath;
    
        for (size_t i = 0; i < m_arFontsPaths.size(); ++i)
        {
            oFontsWorker.m_arAdditionalFolders.push_back(m_arFontsPaths[i]);
        }
    
        if (!GetSystemFontPath().empty())
        {
            oFontsWorker.m_arAdditionalFolders.push_back(GetSystemFontPath());
        }
    
        NSFonts::IApplicationFonts* pAppFonts = oFontsWorker.Check();
           
        m_arAvailableFonts = oFontsWorker.GetFontNamesWithExcludes(pAppFonts, m_arExcludeFonts);
        m_sScriptData = oFontsWorker.GetAllFonts();
        
        return pAppFonts;
    }

    void CFontsAssistant::Check()
    {
        CApplicationFontsWorker worker;
        
        worker.m_bIsUseOpenType = true;
        worker.m_bIsUseSystemFonts = true;
        worker.m_bIsNeedThumbnails = false;
        worker.m_sDirectory = m_sDataFontsPath;
        
        for (size_t i = 0; i < m_arFontsPaths.size(); ++i)
        {
            worker.m_arAdditionalFolders.push_back(m_arFontsPaths[i]);
        }
        
        if (!GetSystemFontPath().empty())
        {
            worker.m_arAdditionalFolders.push_back(GetSystemFontPath());
        }
        
        NSFonts::IApplicationFonts* appFonts = worker.Check();
        
        RELEASEINTERFACE(appFonts);
    }

    std::vector<std::wstring> CFontsAssistant::GetAvailableFonts()
    {
        return m_arAvailableFonts;
    }

    std::string CFontsAssistant::GetScriptData()
    {
        return m_sScriptData;
    }
}
