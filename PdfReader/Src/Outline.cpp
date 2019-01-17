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
#include "MemoryUtils.h"
#include "StringExt.h"
#include "List.h"
#include "Link.h"
#include "PDFDocEncoding.h"
#include "Outline.h"
#include "Dict.h"

namespace PdfReader
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Outline
	//-------------------------------------------------------------------------------------------------------------------------------

	Outline::Outline(Object *pOutlineObject, XRef *pXref)
	{
		m_pItems = NULL;

		if (!pOutlineObject->IsDict())
		{
			return;
		}

		Object oFirst, oLast;
		m_pItems = OutlineItem::ReadItemList(pOutlineObject->DictLookupAndCopy("First", &oFirst), pOutlineObject->DictLookupAndCopy("Last", &oLast), pXref);
		oFirst.Free();
		oLast.Free();
	}

	Outline::~Outline()
	{
		if (m_pItems)
		{
			DeleteCList(m_pItems, OutlineItem);
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	// OutlineItem
	//-------------------------------------------------------------------------------------------------------------------------------

	OutlineItem::OutlineItem(Dict *pDict, XRef *pXref)
	{
		m_pXref   = pXref;
		m_pTitle  = NULL;
		m_pAction = NULL;
		m_pKids   = NULL;

		Object oDictItem;
		if (pDict->Search("Title", &oDictItem)->IsString())
		{
			StringExt *seTitle = oDictItem.GetString();
			if ((seTitle->GetAt(0) & 0xff) == 0xfe && (seTitle->GetAt(1) & 0xff) == 0xff)
			{
				m_nTitleLen = (seTitle->GetLength() - 2) / 2;
				m_pTitle = (Unicode *)MemUtilsMallocArray(m_nTitleLen, sizeof(Unicode));
				for (int nIndex = 0; nIndex < m_nTitleLen; ++nIndex)
				{
					m_pTitle[nIndex] = ((seTitle->GetAt(2 + 2 * nIndex) & 0xff) << 8) | (seTitle->GetAt(3 + 2 * nIndex) & 0xff);
				}
			}
			else
			{
				m_nTitleLen = seTitle->GetLength();
				m_pTitle = (Unicode *)MemUtilsMallocArray(m_nTitleLen, sizeof(Unicode));

				for (int nIndex = 0; nIndex < m_nTitleLen; ++nIndex)
				{
					m_pTitle[nIndex] = c_arrPDFDocEncoding[seTitle->GetAt(nIndex) & 0xff];
				}
			}
		}
		else
		{
			m_nTitleLen = 0;
		}
		oDictItem.Free();

		if (!pDict->Search("Dest", &oDictItem)->IsNull())
		{
			m_pAction = LinkAction::ParseDestination(&oDictItem);
		}
		else
		{
			oDictItem.Free();
			if (!pDict->Search("A", &oDictItem)->IsNull())
			{
				m_pAction = LinkAction::ParseAction(&oDictItem);
			}
		}
		oDictItem.Free();

		pDict->SearchAndCopy("First", &m_oFirstRef);
		pDict->SearchAndCopy("Last", &m_oLastRef);
		pDict->SearchAndCopy("Next", &m_oNextRef);

		m_bOpen = false;
		if (pDict->Search("Count", &oDictItem)->IsInt())
		{
			if (oDictItem.GetInt() > 0)
			{
				m_bOpen = true;
			}
		}
		oDictItem.Free();
	}

	OutlineItem::~OutlineItem()
	{
		Close();

		if (m_pTitle)
		{
			MemUtilsFree(m_pTitle);
		}
		if (m_pAction)
		{
			delete m_pAction;
		}
		m_oFirstRef.Free();
		m_oLastRef.Free();
		m_oNextRef.Free();
	}

	CList *OutlineItem::ReadItemList(Object *pFirstItemRef, Object *pLastItemRef, XRef *pXref)
	{
		CList *pItems = new CList();
		Object *pCurItem = pFirstItemRef;

		while (pCurItem->IsRef())
		{
			Object oTemp;
			if (!pCurItem->Fetch(pXref, &oTemp)->IsDict())
			{
				oTemp.Free();
				break;
			}
			OutlineItem *pItem = new OutlineItem(oTemp.GetDict(), pXref);
			oTemp.Free();

			pItems->Append(pItem);
			if (pCurItem->GetRef().nNum == pLastItemRef->GetRef().nNum && pCurItem->GetRef().nGen == pLastItemRef->GetRef().nGen)
			{
				break;
			}
			pCurItem = &pItem->m_oNextRef;
		}
		return pItems;
	}

	void OutlineItem::Open()
	{
		if (!m_pKids)
		{
			m_pKids = ReadItemList(&m_oFirstRef, &m_oLastRef, m_pXref);
		}
	}

	void OutlineItem::Close()
	{
		if (m_pKids)
		{
			DeleteCList(m_pKids, OutlineItem);
			m_pKids = NULL;
		}
	}
}