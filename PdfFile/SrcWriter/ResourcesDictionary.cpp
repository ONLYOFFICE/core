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
#include "ResourcesDictionary.h"
#include "Font.h"
#include "Utils.h"
#include "GState.h"
#include "Image.h"
#include "Font14.h"

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
		m_unXObjectsCount   = 0;
		m_pXObjects         = NULL;
		
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
	CObjectBase* CResourcesDict::Copy(CObjectBase* pOut) const
	{
		CResourcesDict* pDict = pOut && pOut->GetType() == object_type_DICT ? dynamic_cast<CResourcesDict*>(pOut) : new CResourcesDict(NULL, true, false);
		if (!pDict)
			return NULL;

		for (auto const &oIter : m_mList)
			pDict->Add(oIter.first, oIter.second->Copy());

		pDict->Fix();

		return pDict;
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

		CFontEmbedded* pEmbedded = dynamic_cast<CFontEmbedded*>(pFont);
		const char* sEmbeddedFontKey = NULL;
		if (pEmbedded)
		{
			sEmbeddedFontKey = pEmbedded->GetFontKey();
			CObjectBase* pObj = m_pFonts->Get(sEmbeddedFontKey);
			if (!pObj)
			{
				m_pFonts->Add(sEmbeddedFontKey, pEmbedded->GetObj());
				return sEmbeddedFontKey;
			}
		}

		const char *sKey = NULL;
		if (pEmbedded)
		{
			CObjectBase* pObj1 = pEmbedded->GetObj2();
			CObjectBase* pObj2 = m_pFonts->Get(pEmbedded->GetFontKey());
			if (pObj1 && pObj2 && pObj1->GetType() == object_type_UNKNOWN && pObj1->GetObjId() == pObj2->GetObjId())
				sKey = pEmbedded->GetFontKey();
		}
		if (!sKey)
			sKey = m_pFonts->GetKey(pEmbedded ? pEmbedded->GetObj2() : pFont);
		if (!sKey)
		{
			// if font is not registered in resources, then register it
			char sFontName[LIMIT_MAX_NAME_LEN + 1];
			char *pPointer = NULL;
			char *pEndPointer = sFontName + LIMIT_MAX_NAME_LEN;

			while (++m_unFontsCount < LIMIT_MAX_DICT_ELEMENT)
			{
				if (!m_pFonts->Get("F" + std::to_string(m_unFontsCount)))
					break;
			}

			pPointer = (char*)StrCpy(sFontName, "F", pEndPointer);
			ItoA(pPointer, m_unFontsCount, pEndPointer);
			m_pFonts->Add(sFontName, pEmbedded ? pEmbedded->GetObj() : pFont);
			sKey = m_pFonts->GetKey(pEmbedded ? pEmbedded->GetObj2() : pFont);
			if (sEmbeddedFontKey)
				pEmbedded->UpdateKey(sKey);
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
			// If ExtGState is not registered in Resource, register it.
			char sExtGrStateName[LIMIT_MAX_NAME_LEN + 1];
			char *pPointer;
			char *pEndPointer = sExtGrStateName + LIMIT_MAX_NAME_LEN;

			while (++m_unExtGStatesCount < LIMIT_MAX_DICT_ELEMENT)
			{
				if (!m_pExtGStates->Get("E" + std::to_string(m_unExtGStatesCount)))
					break;
			}

			pPointer = (char*)StrCpy(sExtGrStateName, "E", pEndPointer);
			ItoA(pPointer, m_unExtGStatesCount, pEndPointer);
			m_pExtGStates->Add(sExtGrStateName, pState);
			sKey = m_pExtGStates->GetKey(pState);
		}

		return sKey;
	}
	const char* CResourcesDict::GetXObjectName(CObjectBase* pObject)
	{
		if (!m_pXObjects)
		{
			m_pXObjects = new CDictObject();
			if (!m_pXObjects)
				return NULL;

			Add("XObject", m_pXObjects);
		}

		const char* sKey = m_pXObjects->GetKey(pObject);
		if (!sKey)
		{
			char sXObjName[LIMIT_MAX_NAME_LEN + 1];
			char *pPointer;
			char *pEndPointer = sXObjName + LIMIT_MAX_NAME_LEN;

			while (++m_unXObjectsCount < LIMIT_MAX_DICT_ELEMENT)
			{
				if (!m_pXObjects->Get("X" + std::to_string(m_unXObjectsCount)))
					break;
			}

			pPointer = (char*)StrCpy(sXObjName, "X", pEndPointer);
			ItoA(pPointer, m_unXObjectsCount, pEndPointer);
			m_pXObjects->Add(sXObjName, pObject);
			sKey = m_pXObjects->GetKey(pObject);
		}

		return sKey;
	}
	void CResourcesDict::AddXObjectWithName(const char* sXObjName, CXObject* pObject)
	{
		if (!m_pXObjects)
		{
			m_pXObjects = new CDictObject();
			if (!m_pXObjects)
				return;

			Add("XObject", m_pXObjects);
		}

		m_pXObjects->Add(sXObjName, pObject);
	}
	void CResourcesDict::Fix()
	{
		// Initialize current fonts
		CObjectBase* pFonts = Get("Font");
		if (pFonts && pFonts->GetType() == object_type_DICT)
		{
			m_pFonts = (CDictObject*)pFonts;
			m_unFontsCount = 0;
		}

		// Initialize current ExtGStates
		CObjectBase* pExtGStates = Get("ExtGState");
		if (pExtGStates && pExtGStates->GetType() == object_type_DICT)
		{
			m_pExtGStates = (CDictObject*)pExtGStates;
			m_unExtGStatesCount = 0;
		}

		// Initialize current XObject
		CObjectBase* pXObject = Get("XObject");
		if (pXObject && pXObject->GetType() == object_type_DICT)
		{
			m_pXObjects = (CDictObject*)pXObject;
			m_unXObjectsCount = 0;
		}
	}
}
