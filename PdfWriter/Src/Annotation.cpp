#include "Annotation.h"
#include "Pages.h"

#define BS_DEF_WIDTH 1

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
	}
	void CAnnotation::SetBorderStyle(EBorderSubtype eSubtype, float fWidth, unsigned short nDashOn, unsigned short nDashOff, unsigned short nDashPhase)
	{
		CDictObject* pBorderStyleDict = new CDictObject();
		if (!pBorderStyleDict)
			return;

		Add("BS", pBorderStyleDict);

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

		if (BS_DEF_WIDTH != fWidth)
			pBorderStyleDict->Add("W", fWidth);
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
		fWidth = max(fWidth, 0);

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
		eIcon = (EAnnotIcon)min(max(AnnotIconMin, eIcon), AnnotIconMax);
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