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
#include "ResourcesDictionary.h"
#include "Font.h"
#include "Utils.h"
#include "GState.h"

namespace PdfWriter
{
	//----------------------------------------------------------------------------------------
	// CResourcesDict
	//----------------------------------------------------------------------------------------
	CResourcesDict::CResourcesDict(CXref* pXref, bool bInline, bool bProcSet)
	{
		m_unFontsCount      = 0;
		m_pFonts            = NULL;
		m_unExtGStatesCount = 0;
		m_pExtGStates       = NULL;
		
		if (!bInline)
			pXref->Add(this);

		if (bProcSet)
		{
			CArrayObject* pProcset = new CArrayObject();
			if (!pProcset)
				return;

			Add("ProcSet", pProcset);
			pProcset->Add(new CNameObject("PDF"));
			pProcset->Add(new CNameObject("Text"));
			pProcset->Add(new CNameObject("ImageB"));
			pProcset->Add(new CNameObject("ImageC"));
			pProcset->Add(new CNameObject("ImageI"));
		}
	}
	const char* CResourcesDict::GetFontName(CFontDict* pFont)
	{
		if (!m_pFonts)
		{
			m_pFonts = new CDictObject();
			if (!m_pFonts)
				return NULL;

			Add("Font", m_pFonts);
		}

		const char *sKey = m_pFonts->GetKey(pFont);
		if (!sKey)
		{
			// если фонт не зарегистрирован в ресурсах, тогда регистрируем его
			char sFontName[LIMIT_MAX_NAME_LEN + 1];
			char *pPointer = NULL;
			char *pEndPointer = sFontName + LIMIT_MAX_NAME_LEN;

			pPointer = (char*)StrCpy(sFontName, "F", pEndPointer);
			ItoA(pPointer, m_unFontsCount + 1, pEndPointer);
			m_unFontsCount++;
			m_pFonts->Add(sFontName, pFont);
			sKey = m_pFonts->GetKey(pFont);
		}

		return sKey;
	}
	const char* CResourcesDict::GetExtGrStateName(CExtGrState* pState)
	{
		if (!m_pExtGStates)
		{
			m_pExtGStates = new CDictObject();
			if (!m_pExtGStates)
				return NULL;

			Add("ExtGState", m_pExtGStates);
		}

		const char* sKey = m_pExtGStates->GetKey(pState);
		if (!sKey)
		{
			// Если ExtGState не зарегистрирован в Resource, регистрируем
			char sExtGrStateName[LIMIT_MAX_NAME_LEN + 1];
			char *pPointer;
			char *pEndPointer = sExtGrStateName + LIMIT_MAX_NAME_LEN;

			pPointer = (char*)StrCpy(sExtGrStateName, "E", pEndPointer);
			ItoA(pPointer, m_unExtGStatesCount + 1, pEndPointer);
			m_unExtGStatesCount++;
			m_pExtGStates->Add(sExtGrStateName, pState);
			sKey = m_pExtGStates->GetKey(pState);
		}

		return sKey;
	}
}
