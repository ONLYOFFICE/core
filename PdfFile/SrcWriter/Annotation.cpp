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
#include "Annotation.h"
#include "Pages.h"

#ifndef BS_DEF_WIDTH
#define BS_DEF_WIDTH 1
#endif

namespace PdfWriter
{
	const static char* c_sAnnotTypeNames[] =
	{
		"Text",
		"Link",
		"Sound",
		"FreeText",
		"Stamp",
		"Square",
		"Circle",
		"StrikeOut",
		"Highlight",
		"Underline",
		"Ink",
		"FileAttachment",
		"Popup"
	};
	const static char* c_sAnnotIconNames[] =
	{
		"Comment",
		"Key",
		"Note",
		"Help",
		"NewParagraph",
		"Paragraph",
		"Insert"
	};

	//----------------------------------------------------------------------------------------
	// CAnnotation
	//----------------------------------------------------------------------------------------
	CAnnotation::CAnnotation(CXref* pXref, EAnnotType eType, const TRect& oRect)
	{
		pXref->Add(this);

		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("Rect", pArray);

		if (oRect.fTop < oRect.fBottom)
		{
			pArray->Add(oRect.fLeft);
			pArray->Add(oRect.fTop);
			pArray->Add(oRect.fRight);
			pArray->Add(oRect.fBottom);
		}
		else
		{
			pArray->Add(oRect.fLeft);
			pArray->Add(oRect.fBottom);
			pArray->Add(oRect.fRight);
			pArray->Add(oRect.fTop);
		}

		Add("Type", "Annot");
		Add("Subtype", c_sAnnotTypeNames[(int)eType]);

		// Для PDFA нужно, чтобы 0, 1, 4 биты были выключены, а второй включен
		Add("F", 4);
	}
	void CAnnotation::SetBorderStyle(EBorderSubtype eSubtype, float fWidth, unsigned short nDashOn, unsigned short nDashOff, unsigned short nDashPhase)
	{
		CDictObject* pBorderStyleDict = new CDictObject();
		if (!pBorderStyleDict)
			return;

		Add("BS", pBorderStyleDict);

		if (::fabs(BS_DEF_WIDTH - fWidth) > 0.01)
			pBorderStyleDict->Add("W", fWidth);

		if (fWidth < 0.01)
			return;

		if (border_subtype_Dashed == eSubtype)
		{
			CArrayObject* pDash = new CArrayObject();
			if (!pDash)
				return;

			pBorderStyleDict->Add("D", pDash);
			pBorderStyleDict->Add("Type", "Border");
			pDash->Add(nDashOn);
			pDash->Add(nDashOff);

			if (0 != nDashPhase)
				pDash->Add(nDashOff);
		}

		switch (eSubtype)
		{
		case border_subtype_Solid:      pBorderStyleDict->Add("S", "S"); break;
		case border_subtype_Dashed:     pBorderStyleDict->Add("S", "D"); break;
		case border_subtype_Beveled:    pBorderStyleDict->Add("S", "B"); break;
		case border_subtype_Inset:      pBorderStyleDict->Add("S", "I"); break;
		case border_subtype_Underlined: pBorderStyleDict->Add("S", "U"); break;
		}
	}
	//----------------------------------------------------------------------------------------
	// CLinkAnnotation
	//----------------------------------------------------------------------------------------
	CLinkAnnotation::CLinkAnnotation(CXref* pXref, const TRect& oRect, CDestination* pDestination) : CAnnotation(pXref, AnnotLink, oRect)
	{
		Add("Dest", (CObjectBase*)pDestination);
	}
	void CLinkAnnotation::SetBorderStyle(float fWidth, unsigned short nDashOn, unsigned short nDashOff)
	{
        fWidth = std::max(fWidth, 0.f);

		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("Border", pArray);

		pArray->Add(0);
		pArray->Add(0);
		pArray->Add(fWidth);

		if (nDashOn && nDashOff)
		{
			CArrayObject* pDash = new CArrayObject();
			if (!pDash)
				return;

			pArray->Add(pDash);

			pDash->Add(nDashOn);
			pDash->Add(nDashOff);
		}
	}
	void CLinkAnnotation::SetHighlightMode(EAnnotHighlightMode eMode)
	{
		switch (eMode)
		{
		case AnnotNoHighlight:    Add("H", "N"); break;
		case AnnotInvertBorder:   Add("H", "O"); break;
		case AnnotDownAppearance: Add("H", "P"); break;
		default:                  Remove("H"); break;
		}
	}
	//----------------------------------------------------------------------------------------
	// CTextAnnotation
	//----------------------------------------------------------------------------------------
	CTextAnnotation::CTextAnnotation(CXref* pXref, const TRect& oRect, const char* sText) : CAnnotation(pXref, AnnotTextNotes, oRect)
	{
		Add("Contents", new CStringObject(sText));
	}
	void SetIcon(EAnnotIcon eIcon);
	void SetOpened(bool bOpened);
	void CTextAnnotation::SetIcon(EAnnotIcon eIcon)
	{
        eIcon = (EAnnotIcon)std::min(std::max(AnnotIconMin, eIcon), AnnotIconMax);
		Add("Name", c_sAnnotIconNames[(int)eIcon]);
	}
	void CTextAnnotation::SetOpened(bool bOpened)
	{
		Add("Open", new CBoolObject(bOpened));
	}
	//----------------------------------------------------------------------------------------
	// CUriLinkAnnotation
	//----------------------------------------------------------------------------------------
	CUriLinkAnnotation::CUriLinkAnnotation(CXref* pXref, const TRect& oRect, const char* sUri) : CAnnotation(pXref, AnnotLink, oRect)
	{
		CDictObject* pAction = new CDictObject();
		if (!pAction)
			return;

		Add("A", pAction);
		pAction->Add("Type", "Action");
		pAction->Add("S", "URI");
		pAction->Add("URI", new CStringObject(sUri));
	}
}
