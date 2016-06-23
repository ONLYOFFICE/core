/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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


//#include "../../DesktopEditor/fontengine/FontManager.h"
#include "../../DesktopEditor/fontengine/ApplicationFonts.h"

namespace NSFontCutter
{
	class CFontDstManager
	{
	public:		
		std::map<CString, CString> m_mapPicks;
		CApplicationFonts m_oApplicationFonts;
		CFontManager* m_pFontManager;

		CString m_strFontsDir;

        bool					m_bIsEmbeddedFonts;

        CEmbeddedFontsManager	m_oEmbeddedFonts;

	public:
		CFontDstManager() : m_mapPicks()
		{
			m_strFontsDir = _T("");

			m_pFontManager = NULL;
            m_bIsEmbeddedFonts = false;
		}
		~CFontDstManager()
		{
			RELEASEOBJECT(m_pFontManager);
		}

		void Init(const CString& strDir)
		{
			if(strDir.IsEmpty())
				m_oApplicationFonts.Initialize();
			else
				m_oApplicationFonts.InitializeFromFolder(string2std_string(strDir));
			CFontList* pFontList = m_oApplicationFonts.GetList();
			if(NULL != pFontList)
			{
				std::wstring sDefaultFont(_T("Arial"));
				pFontList->SetDefaultFont(sDefaultFont);
			}
			m_pFontManager = m_oApplicationFonts.GenerateFontManager();
		}

		CString GetTypefacePickByName(const CString& strTypeface)
		{
			CString sFind = strTypeface;

			int nFindTh = sFind.Find(_T("+mj"));
			if (0 == nFindTh)
				return sFind;
			nFindTh = sFind.Find(_T("+mn"));
			if (0 == nFindTh)
				return sFind;

			if (_T("") == sFind)
			{
				sFind = _T("Arial");
			}
			
			std::map<CString, CString>::iterator pPair = m_mapPicks.find(sFind);
			if (pPair != m_mapPicks.end())
				return pPair->second;

			//подбор перенесен в js
			m_mapPicks.insert(std::pair<CString,CString>(sFind, sFind));
			return sFind;

			CString sInputSave = sFind;

			CFontSelectFormat oFontSelectFormat;
			oFontSelectFormat.wsName = new std::wstring;
			*oFontSelectFormat.wsName = std::wstring(sFind.GetString());
			//oFontSelectFormat.
			CFontInfo* pFontInfo = m_pFontManager->GetFontInfoByParams(oFontSelectFormat);
			CString sRes = _T("Arial");
			if(NULL != pFontInfo)
			{
				sRes = CString(pFontInfo->m_wsFontName.c_str());


                if (m_bIsEmbeddedFonts)
					m_oEmbeddedFonts.CheckFont(sRes, m_pFontManager);

                m_mapPicks.insert(std::pair<CString,CString>(sInputSave, sRes));
				
			}
			return sRes;
		}

        template<typename TTextFont>
		CString GetTypefacePick(TTextFont& textFont)
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
	
	HRESULT Init(const CString& bsFontsDirectory)
	{
		m_oPicker.Init(bsFontsDirectory);
		return S_OK;
	}
	
	CFontManager* get_FontManager()
	{
		return m_oPicker.m_pFontManager;
	}

	HRESULT SetEmbeddedFontsDirectory(const CString& bsFontsDirectory)
	{
        m_oPicker.m_oEmbeddedFonts.m_strEmbeddedFontsFolder = (CString)bsFontsDirectory;
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

	HRESULT CheckString(const CString& bsText)
	{

        m_oPicker.m_oEmbeddedFonts.CheckString((CString)bsText);

		return S_OK;
	}
	HRESULT CheckFont(const CString& bsFontName)
	{

        m_oPicker.m_oEmbeddedFonts.CheckFont((CString)bsFontName, m_oPicker.m_pFontManager);

		return S_OK;
	}

	HRESULT PickFont(LONG lParamType, const CString& bsParams, CString* pDstName)
	{
		if (NULL == pDstName)
			return S_FALSE;

		if (0 == lParamType)
		{
			CString strResult = m_oPicker.GetTypefacePickByName((CString)bsParams);
            *pDstName = strResult;//strResult.AllocSysString();
			return S_OK;
		}
		// not impl
		return S_OK;
	}

	//HRESULT GetBinaryData(LONG lType, SAFEARRAY** ppBinaryArray);
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
