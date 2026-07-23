/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "BinWriters.h"

namespace NSFontCutter
{
	CFontDstManager::CFontDstManager() : m_mapPicks()
	{
		m_pApplicationFonts = NSFonts::NSApplication::Create();
		m_strFontsDir = _T("");

		m_pFontManager = NULL;
		m_bIsEmbeddedFonts = false;
	}
	CFontDstManager::~CFontDstManager()
	{
		RELEASEOBJECT(m_pFontManager);
		RELEASEOBJECT(m_pApplicationFonts);
	}
	void CFontDstManager::Init(const std::wstring& strDir)
	{
		if(strDir.empty())
			m_pApplicationFonts->Initialize();
		else
			m_pApplicationFonts->InitializeFromFolder(strDir);
		m_pFontManager = m_pApplicationFonts->GenerateFontManager();
	}
	std::wstring CFontDstManager::GetTypefacePickByName(const std::wstring& strTypeface)
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

		//font matching moved to js
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
}

HRESULT COfficeFontPicker::Init(const std::wstring& bsFontsDirectory)
{
	m_oPicker.Init(bsFontsDirectory);
	return S_OK;
}

NSFonts::IFontManager* COfficeFontPicker::get_FontManager()
{
	return m_oPicker.m_pFontManager;
}

HRESULT COfficeFontPicker::SetEmbeddedFontsDirectory(const std::wstring& bsFontsDirectory)
{
	m_oPicker.m_oEmbeddedFonts.m_strEmbeddedFontsFolder = (std::wstring)bsFontsDirectory;
	return S_OK;
}
HRESULT COfficeFontPicker::SetEmbeddedFontsParam(LONG lParam)
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

HRESULT COfficeFontPicker::CheckString(const std::wstring& bsText)
{

	m_oPicker.m_oEmbeddedFonts.CheckString((std::wstring)bsText);

	return S_OK;
}
HRESULT COfficeFontPicker::CheckFont(const std::wstring& bsFontName)
{

	m_oPicker.m_oEmbeddedFonts.CheckFont((std::wstring)bsFontName, m_oPicker.m_pFontManager);

	return S_OK;
}

HRESULT COfficeFontPicker::PickFont(LONG lParamType, const std::wstring& bsParams, std::wstring* pDstName)
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

NSFontCutter::CFontDstManager* COfficeFontPicker::GetNativePicker()
{
	return &m_oPicker;
}

NSFontCutter::CEmbeddedFontsManager* COfficeFontPicker::GetNativeCutter()
{
	return &m_oPicker.m_oEmbeddedFonts;
}

HRESULT COfficeFontPicker::GetBinaryData(LONG lType, BYTE** ppyArray, size_t& szCount)
{
	if (NULL == ppyArray)
		return S_FALSE;

	*ppyArray = NULL;
	if (FONT_PICKER_BINARYDATA_SAVEEMBEDDED == lType)
	{
		NSBinPptxRW::CBinaryFileWriter oWriter;
		m_oPicker.m_oEmbeddedFonts.CheckString(_T(".)abcdefghijklmnopqrstuvwxyz"));
		m_oPicker.m_oEmbeddedFonts.CheckFont(_T("Wingdings 3"), m_oPicker.m_pFontManager);
		m_oPicker.m_oEmbeddedFonts.CheckFont(_T("Arial"), m_oPicker.m_pFontManager);

		oWriter.StartRecord(NSBinPptxRW::NSMainTables::FontsEmbedded);
		m_oPicker.m_oEmbeddedFonts.WriteEmbeddedFonts(&oWriter);
		oWriter.EndRecord();

		oWriter.GetSafearray(ppyArray, szCount);
	}
	else if (FONT_PICKER_BINARYDATA_SAVEMAP == lType)
	{
	}
	return S_OK;
}
