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

#include "FontCutter.h"

namespace NSFontCutter
{
	CEmbeddedFontsManager::CEmbeddedFontInfo::CEmbeddedFontInfo()
	{
		Name = _T("");

		PathRegular = _T("");
		FaceRegular = -1;

		PathBold = _T("");
		FaceBold = -1;

		PathItalic = _T("");
		FaceItalic = -1;

		PathBoldItalic = _T("");
		FaceBoldItalic = -1;
	}
	CEmbeddedFontsManager::CEmbeddedFontInfo::CEmbeddedFontInfo(const CEmbeddedFontsManager::CEmbeddedFontInfo& oSrc)
	{
		*this = oSrc;
	}
	CEmbeddedFontsManager::CEmbeddedFontInfo& CEmbeddedFontsManager::CEmbeddedFontInfo::operator=(const CEmbeddedFontsManager::CEmbeddedFontInfo& oSrc)
	{
		Name = oSrc.Name;

		PathRegular = oSrc.PathRegular;
		FaceRegular = oSrc.FaceRegular;

		PathBold = oSrc.PathBold;
		FaceBold = oSrc.FaceBold;

		PathItalic = oSrc.PathItalic;
		FaceItalic = oSrc.FaceItalic;

		PathBoldItalic = oSrc.PathBoldItalic;
		FaceBoldItalic = oSrc.FaceBoldItalic;

		return *this;
	}

	CEmbeddedFontsManager::CEmbeddedFontsManager()
	{
		m_strEmbeddedFontsFolder = _T("");
	}
	void CEmbeddedFontsManager::CheckFont(const std::wstring& strName, NSFonts::IFontManager* pManager)
	{
		std::map<std::wstring, CEmbeddedFontInfo>::const_iterator pPair = m_mapFontsEmbeddded.find(strName);
		if (pPair != m_mapFontsEmbeddded.end())
			return;

		CEmbeddedFontInfo oInfo;
		oInfo.Name = strName;

		std::vector<NSFonts::CFontInfo*> aFontInfos = pManager->GetAllStylesByFontName(strName);
		for(std::vector<NSFonts::CFontInfo*>::iterator i = aFontInfos.begin(); i != aFontInfos.end(); i++)
		{
			NSFonts::CFontInfo* pFontInfo = *i;
			if(0 != pFontInfo->m_bBold && 0 != pFontInfo->m_bItalic)
			{
				oInfo.PathBoldItalic = std::wstring(pFontInfo->m_wsFontPath.c_str());
				oInfo.FaceBoldItalic = pFontInfo->m_lIndex;
			}
			else if(0 != pFontInfo->m_bBold)
			{
				oInfo.PathBold = std::wstring(pFontInfo->m_wsFontPath.c_str());
				oInfo.FaceBold = pFontInfo->m_lIndex;
			}
			else if(0 != pFontInfo->m_bItalic)
			{
				oInfo.PathItalic = std::wstring(pFontInfo->m_wsFontPath.c_str());
				oInfo.FaceItalic = pFontInfo->m_lIndex;
			}
			else
			{
				oInfo.PathRegular = std::wstring(pFontInfo->m_wsFontPath.c_str());
				oInfo.FaceRegular = pFontInfo->m_lIndex;
			}
		}

		m_mapFontsEmbeddded [strName] = oInfo;
	}
	void CEmbeddedFontsManager::CheckString(const nullable_string& val)
	{
		if (val.is_init())
			CheckString(*val);
	}
	void CEmbeddedFontsManager::CheckString(const std::wstring& val)
	{
		size_t len = val.length();

		std::wstring str_lower = XmlUtils::GetLower(val);
		std::wstring str_upper = XmlUtils::GetUpper(val);

		for (size_t i = 0; i < len; ++i)
		{
			m_CharMap [str_lower[i]] = true;
			m_CharMap [str_upper[i]] = true;
		}
	}
	bool CEmbeddedFontsManager::GenerateSafearray(USHORT **ppArray, size_t& nCount)
	{
		// цифры нужны всем. остальное - каждый
		CheckString(_T("0123456789"));
		m_CharMap [(WCHAR)0x00B0] = true;
		m_CharMap [(WCHAR)0x00B7] = true;
		m_CharMap [(WCHAR)0x00B6] = true;
		m_CharMap [(WCHAR)0x00A4] = true;
		m_CharMap [(WCHAR)0x00A0] = true;
		m_CharMap [(WCHAR)0x0022] = true;
		m_CharMap [(WCHAR)0x0032] = true;
		m_CharMap [(WCHAR)0x0038] = true;
		m_CharMap [(WCHAR)0x0097] = true;

		nCount = (ULONG)m_CharMap.size();

		USHORT *pArray = new USHORT [nCount];

		if (NULL == pArray)
			return false;

		USHORT *pBuffer = pArray;

		for (std::map<WCHAR, bool>::const_iterator pPair = m_CharMap.begin(); pPair != m_CharMap.end(); ++pPair)
		{
			*pBuffer = pPair->first;
			++pBuffer;
		}
		*ppArray = pArray;
		return true;
	}

#if !defined(DONT_WRITE_EMBEDDED_FONTS)
	void CEmbeddedFontsManager::WriteFont(std::wstring& strName, LONG& lFaceIndex, std::wstring& strFontPath, CFile* pFile, USHORT* pArrayUnicodes, size_t pArrayUnicodesLength, Fonts::IFontConverter* pFontConverter)
	{
		LONG lFontConverterFlag = 16; // truetype only

		BSTR bsFontIn = strFontPath.AllocSysString();

		std::wstring _strName = strName + _T("_Embedded");
		//BSTR bsName = _strName.AllocSysString();

		SAFEARRAYBOUND rgsab_ArrayUnicodes;
		rgsab_ArrayUnicodes.lLbound	= 0;
		rgsab_ArrayUnicodes.cElements	= pArrayUnicodesLength;

		SAFEARRAY* saArrayUnicodes = SafeArrayCreate(VT_UI1, 1, &rgsab_ArrayUnicodes);
		memcpy(saArrayUnicodes->pvData, pArrayUnicodes, pArrayUnicodesLength);

		SAFEARRAY* pArrayData = NULL;

		pFontConverter->ToOTF2(bsFontIn, saArrayUnicodes, _strName.AllocSysString(), lFontConverterFlag, lFaceIndex, &pArrayData); // TrueType only

		//SysFreeString(bsFontIn);
		//SysFreeString(bsName);

		BYTE* pbBinBuffer = (BYTE*)pArrayData->pvData;
		int nBinBufferLen = pArrayData->rgsabound[0].cElements;
		int nBase64BufferLen = Base64::Base64EncodeGetRequiredLength(nBinBufferLen, Base64::B64_BASE64_FLAG_NOCRLF);
		BYTE* pbBase64Buffer = new BYTE[nBase64BufferLen];
		if (true == Base64::Base64Encode(pbBinBuffer, nBinBufferLen, (LPSTR)pbBase64Buffer, &nBase64BufferLen, Base64::B64_BASE64_FLAG_NOCRLF))
		{
			std::string s = "\"";
			pFile->WriteFile(s.GetBuffer(), s.length());

			pFile->WriteFile(pbBase64Buffer, nBase64BufferLen);

			pFile->WriteFile(s.GetBuffer(), s.length());
		}

		RELEASEARRAYOBJECTS(pbBase64Buffer);

		RELEASEARRAY(pArrayData);
	}
#endif // #if !defined(DONT_WRITE_EMBEDDED_FONTS)
}

