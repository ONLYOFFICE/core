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
#pragma once

#include "../../Common/DocxFormat/Source/SystemUtility/File.h"
#include "BinReaderWriterDefines.h"
#include "FontCutter.h"

namespace NSFontCutter
{
	class CFontDstManager
	{
	public:		
		std::map<std::wstring, std::wstring> m_mapPicks;
		NSFonts::IApplicationFonts* m_pApplicationFonts;
		NSFonts::IFontManager* m_pFontManager;

		std::wstring m_strFontsDir;

        bool					m_bIsEmbeddedFonts;

        CEmbeddedFontsManager	m_oEmbeddedFonts;

	public:
		CFontDstManager() : m_mapPicks()
		{
			m_pApplicationFonts = NSFonts::NSApplication::Create();
			m_strFontsDir = _T("");

			m_pFontManager = NULL;
            m_bIsEmbeddedFonts = false;
		}
		~CFontDstManager()
		{
			RELEASEOBJECT(m_pFontManager);
			RELEASEOBJECT(m_pApplicationFonts);
		}

		void Init(const std::wstring& strDir)
		{
			if(strDir.empty())
				m_pApplicationFonts->Initialize();
			else
				m_pApplicationFonts->InitializeFromFolder(strDir);
			m_pFontManager = m_pApplicationFonts->GenerateFontManager();
		}

		std::wstring GetTypefacePickByName(const std::wstring& strTypeface)
		{
			std::wstring sFind = strTypeface;

			int nFindTh = (int)sFind.find(_T("+mj"));
			if (0 == nFindTh)
				return sFind;
			nFindTh = (int)sFind.find(_T("+mn"));
			if (0 == nFindTh)
				return sFind;

			if (_T("") == sFind)
			{
				sFind = _T("Arial");
			}
			
			std::map<std::wstring, std::wstring>::iterator pPair = m_mapPicks.find(sFind);
			if (pPair != m_mapPicks.end())
				return pPair->second;

			//подбор перенесен в js
			m_mapPicks.insert(std::pair<std::wstring,std::wstring>(sFind, sFind));
			return sFind;

			std::wstring sInputSave = sFind;

			NSFonts::CFontSelectFormat oFontSelectFormat;
			oFontSelectFormat.wsName = new std::wstring(sFind);

			//oFontSelectFormat.
			NSFonts::CFontInfo* pFontInfo = m_pFontManager->GetFontInfoByParams(oFontSelectFormat);
			std::wstring sRes = _T("Arial");
			if(NULL != pFontInfo)
			{
				sRes = std::wstring(pFontInfo->m_wsFontName.c_str());


                if (m_bIsEmbeddedFonts)
					m_oEmbeddedFonts.CheckFont(sRes, m_pFontManager);

                m_mapPicks.insert(std::pair<std::wstring,std::wstring>(sInputSave, sRes));
				
			}
			return sRes;
		}

        template<typename TTextFont>
		std::wstring GetTypefacePick(TTextFont& textFont)
		{
            return GetTypefacePickByName(textFont.typeface);
		}

	};
}

class COfficeFontPicker 	
{
private:
	NSFontCutter::CFontDstManager m_oPicker;

public:
	
	HRESULT Init(const std::wstring& bsFontsDirectory)
	{
		m_oPicker.Init(bsFontsDirectory);
		return S_OK;
	}
	
	NSFonts::IFontManager* get_FontManager()
	{
		return m_oPicker.m_pFontManager;
	}

	HRESULT SetEmbeddedFontsDirectory(const std::wstring& bsFontsDirectory)
	{
        m_oPicker.m_oEmbeddedFonts.m_strEmbeddedFontsFolder = (std::wstring)bsFontsDirectory;
		return S_OK;
	}
	HRESULT SetEmbeddedFontsParam(LONG lParam)
	{
		switch (lParam)
		{
		case 0:
            m_oPicker.m_bIsEmbeddedFonts = false;
			break;
		case 1:
            m_oPicker.m_bIsEmbeddedFonts = true;
			break;
		default:
			break;
		}
		return S_OK;
	}

	HRESULT CheckString(const std::wstring& bsText)
	{

        m_oPicker.m_oEmbeddedFonts.CheckString((std::wstring)bsText);

		return S_OK;
	}
	HRESULT CheckFont(const std::wstring& bsFontName)
	{

        m_oPicker.m_oEmbeddedFonts.CheckFont((std::wstring)bsFontName, m_oPicker.m_pFontManager);

		return S_OK;
	}

	HRESULT PickFont(LONG lParamType, const std::wstring& bsParams, std::wstring* pDstName)
	{
		if (NULL == pDstName)
			return S_FALSE;

		if (0 == lParamType)
		{
			std::wstring strResult = m_oPicker.GetTypefacePickByName((std::wstring)bsParams);
            *pDstName = strResult;//strResult.AllocSysString();
			return S_OK;
		}
		// not impl
		return S_OK;
	}

	HRESULT GetBinaryData(LONG lType, BYTE** ppyArray, size_t& szCount);

	NSFontCutter::CFontDstManager* GetNativePicker()
	{
		return &m_oPicker;
	}

    NSFontCutter::CEmbeddedFontsManager* GetNativeCutter()
	{
		return &m_oPicker.m_oEmbeddedFonts;
	}

};
