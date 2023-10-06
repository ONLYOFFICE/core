/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "Utils.h"

#include "../../DesktopEditor/common/File.h"

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
		"Popup",
		"Line",
		"Squiggly",
		"Polygon",
		"PolyLine",
		"Caret",
		"Widget"
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
	CAnnotation::CAnnotation(CXref* pXref, EAnnotType eType)
	{
		m_pXref = pXref;
		pXref->Add(this);

		m_nID = 0;

		Add("Type", "Annot");
		Add("Subtype", c_sAnnotTypeNames[(int)eType]);

		// Для PDFA нужно, чтобы 0, 1, 4 биты были выключены, а второй включен
		Add("F", 4);

		//Add("M", new CStringObject(DateNow().c_str()));
	}
	void CAnnotation::SetRect(const TRect& oRect)
	{
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
	}
	void CAnnotation::SetBorder(BYTE nType, double dWidth, double dDashesAlternating, double dGaps)
	{
		CDictObject* pBorderStyleDict = new CDictObject();
		if (!pBorderStyleDict)
			return;

		Add("BS", pBorderStyleDict);

		pBorderStyleDict->Add("Type", "Border");
		pBorderStyleDict->Add("W", dWidth);

		if (dWidth < 0.01)
			return;

		EBorderSubtype eSubtype = EBorderSubtype(nType);
		if (border_subtype_Dashed == eSubtype)
		{
			CArrayObject* pDash = new CArrayObject();
			if (pDash)
			{
				pBorderStyleDict->Add("D", pDash);
				pDash->Add(dDashesAlternating);
				pDash->Add(dGaps);
			}
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
	void CAnnotation::SetID(const int& nID)
	{
		m_nID = nID;
	}
	void CAnnotation::SetAnnotFlag(const int& nAnnotFlag)
	{
		Add("F", nAnnotFlag);
	}
	void CAnnotation::SetPage(CPage* pPage)
	{
		Add("P", pPage);
	}
	void CAnnotation::SetBE(BYTE nType, const double& dBE)
	{
		CDictObject* pBEDict = new CDictObject();
		if (!pBEDict)
			return;

		Add("BE", pBEDict);

		std::string sValue;
		switch (nType)
		{
		case 0:
		{ sValue = "S"; break; }
		case 1:
		{ sValue = "C"; break; }
		}

		pBEDict->Add("S", sValue.c_str());
		pBEDict->Add("I", dBE);
	}
	void CAnnotation::SetContents(const std::wstring& wsText)
	{
		std::string sValue = U_TO_UTF8(wsText);
		Add("Contents", new CStringObject(sValue.c_str()));
	}
	void CAnnotation::SetNM(const std::wstring& wsNM)
	{
		std::string sValue = U_TO_UTF8(wsNM);
		Add("NM", new CStringObject(sValue.c_str()));
	}
	void CAnnotation::SetLM(const std::wstring& wsLM)
	{
		std::string sValue = U_TO_UTF8(wsLM);
		Add("M", new CStringObject(sValue.c_str()));
	}
	void AddToVectorD(CDictObject* pObj, const std::string& sName, const std::vector<double>& arrV)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		pObj->Add(sName, pArray);

		for (const double& dV : arrV)
			pArray->Add(dV);
	}
	void CAnnotation::SetC(const std::vector<double>& arrC)
	{
		AddToVectorD(this, "C", arrC);
	}
	//----------------------------------------------------------------------------------------
	// CMarkupAnnotation
	//----------------------------------------------------------------------------------------
	CMarkupAnnotation::CMarkupAnnotation(CXref* pXref, EAnnotType eType) : CAnnotation(pXref, eType)
	{
		m_nIRTID   = 0;

		// Add("CreationDate", new CStringObject(DateNow().c_str()));
	}
	void CMarkupAnnotation::SetRT(const BYTE& nRT)
	{
		Add("RT", nRT ? "Group" : "R");
	}
	void CMarkupAnnotation::SetPopupID(const int& nPopupID)
	{
		//m_nPopupID = nPopupID;
		CPopupAnnotation* pAnnot = new CPopupAnnotation(m_pXref);
		pAnnot->SetOpen(false);
		pAnnot->SetParentID(this);
		Add("Popup", pAnnot);
	}
	void CMarkupAnnotation::SetIRTID(const int& nIRTID)
	{
		m_nIRTID = nIRTID;
	}
	void CMarkupAnnotation::SetCA(const double& dCA)
	{
		Add("CA", dCA);
	}
	void CMarkupAnnotation::SetT(const std::wstring& wsT)
	{
		std::string sValue = U_TO_UTF8(wsT);
		Add("T", new CStringObject(sValue.c_str()));
	}
	void CMarkupAnnotation::SetRC(const std::wstring& wsRC)
	{
		std::string sValue = U_TO_UTF8(wsRC);
		Add("RC", new CStringObject(sValue.c_str()));
	}
	void CMarkupAnnotation::SetCD(const std::wstring& wsCD)
	{
		std::string sValue = U_TO_UTF8(wsCD);
		Add("CreationDate", new CStringObject(sValue.c_str()));
	}
	void CMarkupAnnotation::SetSubj(const std::wstring& wsSubj)
	{
		std::string sValue = U_TO_UTF8(wsSubj);
		Add("Subj", new CStringObject(sValue.c_str()));
	}
	void CMarkupAnnotation::SetIRTID(CAnnotation* pAnnot)
	{
		Add("IRT", pAnnot);
	}
	//----------------------------------------------------------------------------------------
	// CLinkAnnotation
	//----------------------------------------------------------------------------------------
	CLinkAnnotation::CLinkAnnotation(CXref* pXref, CDestination* pDestination) : CAnnotation(pXref, AnnotLink)
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
	CTextAnnotation::CTextAnnotation(CXref* pXref) : CMarkupAnnotation(pXref, AnnotText)
	{

	}
	void CTextAnnotation::SetOpen(bool bOpen)
	{
		Add("Open", new CBoolObject(bOpen));
	}
	void CTextAnnotation::SetName(BYTE nName)
	{
		Add("Name", c_sAnnotIconNames[nName]);
	}
	void CTextAnnotation::SetState(BYTE nState)
	{
		std::string sValue;
		switch (nState)
		{
		case 0:
		{ sValue = "Marked"; break; }
		case 1:
		{ sValue = "Unmarked"; break; }
		case 2:
		{ sValue = "Accepted"; break; }
		case 3:
		{ sValue = "Rejected"; break; }
		case 4:
		{ sValue = "Cancelled"; break; }
		case 5:
		{ sValue = "Completed"; break; }
		case 6:
		{ sValue = "None"; break; }
		}

		Add("State", new CStringObject(sValue.c_str()));
	}
	void CTextAnnotation::SetStateModel(BYTE nStateModel)
	{
		std::string sValue;
		switch (nStateModel)
		{
		case 0:
		{ sValue = "Marked"; break; }
		case 1:
		{ sValue = "Review"; break; }
		}

		Add("StateModel", new CStringObject(sValue.c_str()));
	}
	//----------------------------------------------------------------------------------------
	// CUriLinkAnnotation
	//----------------------------------------------------------------------------------------
	CUriLinkAnnotation::CUriLinkAnnotation(CXref* pXref, const char* sUri) : CAnnotation(pXref, AnnotLink)
	{
		CDictObject* pAction = new CDictObject();
		if (!pAction)
			return;

		Add("A", pAction);
		pAction->Add("Type", "Action");
		pAction->Add("S", "URI");
		pAction->Add("URI", new CStringObject(sUri));
	}
	//----------------------------------------------------------------------------------------
	// CInkAnnotation
	//----------------------------------------------------------------------------------------
	CInkAnnotation::CInkAnnotation(CXref* pXref) : CMarkupAnnotation(pXref, AnnotInk)
	{

	}
	void CInkAnnotation::SetInkList(const std::vector< std::vector<double> >& arrInkList)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("InkList", pArray);

		for (const std::vector<double>& arrInk : arrInkList)
		{
			CArrayObject* pArrayI = new CArrayObject();
			pArray->Add(pArrayI);

			for (const double& dInk : arrInk)
				pArrayI->Add(dInk);
		}
	}
	//----------------------------------------------------------------------------------------
	// CLineAnnotation
	//----------------------------------------------------------------------------------------
	CLineAnnotation::CLineAnnotation(CXref* pXref) : CMarkupAnnotation(pXref, AnnotLine)
	{

	}
	void CLineAnnotation::SetCap(bool bCap)
	{
		Add("Open", new CBoolObject(bCap));
	}
	void CLineAnnotation::SetIT(const BYTE& nIT)
	{
		std::string sValue;
		switch (nIT)
		{
		case 0:
		{ sValue = "LineDimension"; break; }
		case 1:
		{ sValue = "LineArrow"; break; }
		}

		Add("IT", sValue.c_str());
	}
	void CLineAnnotation::SetCP(const BYTE& nCP)
	{
		std::string sValue;
		switch (nCP)
		{
		case 0:
		{ sValue = "Inline"; break; }
		case 1:
		{ sValue = "Top"; break; }
		}

		Add("CP", sValue.c_str());
	}
	void CLineAnnotation::SetLL(const double& dLL)
	{
		Add("LL", dLL);
	}
	void CLineAnnotation::SetLLE(const double& dLLE)
	{
		Add("LLE", dLLE);
	}
	void CLineAnnotation::SetLLO(const double& dLLO)
	{
		Add("LLO", dLLO);
	}
	std::string AddLE(const BYTE& nLE)
	{
		std::string sValue;
		switch (nLE)
		{
		case 0:
		{ sValue = "Square"; break; }
		case 1:
		{ sValue = "Circle"; break; }
		case 2:
		{ sValue = "Diamond"; break; }
		case 3:
		{ sValue = "OpenArrow"; break; }
		case 4:
		{ sValue = "ClosedArrow"; break; }
		case 5:
		{ sValue = "None"; break; }
		case 6:
		{ sValue = "Butt"; break; }
		case 7:
		{ sValue = "ROpenArrow"; break; }
		case 8:
		{ sValue = "RClosedArrow"; break; }
		case 9:
		{ sValue = "Slash"; break; }
		}

		return sValue;
	}
	void CLineAnnotation::SetLE(const BYTE& nLE1, const BYTE& nLE2)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("LE", pArray);

		pArray->Add(AddLE(nLE1).c_str());
		pArray->Add(AddLE(nLE2).c_str());
	}
	void CLineAnnotation::SetL(const double& dL1, const double& dL2, const double& dL3, const double& dL4)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("L", pArray);

		pArray->Add(dL1);
		pArray->Add(dL2);
		pArray->Add(dL3);
		pArray->Add(dL4);
	}
	void CLineAnnotation::SetCO(const double& dCO1, const double& dCO2)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("CO", pArray);

		pArray->Add(dCO1);
		pArray->Add(dCO2);
	}
	void CLineAnnotation::SetIC(const std::vector<double>& arrIC)
	{
		AddToVectorD(this, "IC", arrIC);
	}
	//----------------------------------------------------------------------------------------
	// CPopupAnnotation
	//----------------------------------------------------------------------------------------
	CPopupAnnotation::CPopupAnnotation(CXref* pXref) : CAnnotation(pXref, AnnotPopup)
	{
		m_nParentID = 0;
	}
	void CPopupAnnotation::SetOpen(bool bOpen)
	{
		Add("Open", new CBoolObject(bOpen));
	}
	void CPopupAnnotation::SetParentID(const int& nParentID)
	{
		m_nParentID = nParentID;
	}
	void CPopupAnnotation::SetParentID(CAnnotation* pAnnot)
	{
		Add("Parent", pAnnot);
	}
	//----------------------------------------------------------------------------------------
	// CFreeTextAnnotation
	//----------------------------------------------------------------------------------------
	CFreeTextAnnotation::CFreeTextAnnotation(CXref* pXref) : CMarkupAnnotation(pXref, AnnotFreeText)
	{

	}
	void CFreeTextAnnotation::SetQ(const BYTE& nQ)
	{
		Add("Q", (int)nQ);
	}
	void CFreeTextAnnotation::SetIT(const BYTE& nIT)
	{
		std::string sValue;
		switch (nIT)
		{
		case 0:
		{ sValue = "FreeText"; break; }
		case 1:
		{ sValue = "FreeTextCallout"; break; }
		case 2:
		{ sValue = "FreeTextTypeWriter"; break; }
		}

		Add("IT", sValue.c_str());
	}
	void CFreeTextAnnotation::SetLE(const BYTE& nLE)
	{
		Add("LE", AddLE(nLE).c_str());
	}
	void CFreeTextAnnotation::SetDS(const std::wstring& wsDS)
	{
		std::string sValue = U_TO_UTF8(wsDS);
		Add("DS", new CStringObject(sValue.c_str()));
	}
	void CFreeTextAnnotation::SetRD(const double& dRD1, const double& dRD2, const double& dRD3, const double& dRD4)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("RD", pArray);

		pArray->Add(dRD1);
		pArray->Add(dRD2);
		pArray->Add(dRD3);
		pArray->Add(dRD4);
	}
	void CFreeTextAnnotation::SetCL(const std::vector<double>& arrCL)
	{
		AddToVectorD(this, "CL", arrCL);
	}
	//----------------------------------------------------------------------------------------
	// CTextMarkupAnnotation
	//----------------------------------------------------------------------------------------
	CTextMarkupAnnotation::CTextMarkupAnnotation(CXref* pXref) : CMarkupAnnotation(pXref, AnnotHighLight)
	{
		m_nSubtype = AnnotHighLight;
	}
	void CTextMarkupAnnotation::SetSubtype(const BYTE& nSubtype)
	{
		switch (nSubtype)
		{
		case 8:
		{ m_nSubtype = AnnotHighLight; break; }
		case 9:
		{ m_nSubtype = AnnotUnderline; break; }
		case 10:
		{ m_nSubtype = AnnotSquiggly; break; }
		case 11:
		{ m_nSubtype = AnnotStrikeOut; break; }
		}

		Add("Subtype", c_sAnnotTypeNames[(int)m_nSubtype]);
	}
	void CTextMarkupAnnotation::SetQuadPoints(const std::vector<double>& arrQuadPoints)
	{
		AddToVectorD(this, "QuadPoints", arrQuadPoints);
	}
	//----------------------------------------------------------------------------------------
	// CSquareCircleAnnotation
	//----------------------------------------------------------------------------------------
	CSquareCircleAnnotation::CSquareCircleAnnotation(CXref* pXref) : CMarkupAnnotation(pXref, AnnotSquare)
	{
		m_nSubtype = AnnotSquare;
	}
	void CSquareCircleAnnotation::SetSubtype(const BYTE& nSubtype)
	{
		switch (nSubtype)
		{
		case 4:
		{ m_nSubtype = AnnotSquare; break; }
		case 5:
		{ m_nSubtype = AnnotCircle; break; }
		}

		Add("Subtype", c_sAnnotTypeNames[(int)m_nSubtype]);
	}
	void CSquareCircleAnnotation::SetRD(const double& dRD1, const double& dRD2, const double& dRD3, const double& dRD4)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("RD", pArray);

		pArray->Add(dRD1);
		pArray->Add(dRD2);
		pArray->Add(dRD3);
		pArray->Add(dRD4);
	}
	void CSquareCircleAnnotation::SetIC(const std::vector<double>& arrIC)
	{
		AddToVectorD(this, "IC", arrIC);
	}
	//----------------------------------------------------------------------------------------
	// CPolygonLineAnnotation
	//----------------------------------------------------------------------------------------
	CPolygonLineAnnotation::CPolygonLineAnnotation(CXref* pXref) : CMarkupAnnotation(pXref, AnnotPolygon)
	{
		m_nSubtype = AnnotPolygon;
	}
	void CPolygonLineAnnotation::SetIT(const BYTE& nIT)
	{
		std::string sValue;
		switch (nIT)
		{
		case 0:
		{ sValue = "PolygonCloud"; break; }
		case 1:
		{ sValue = "PolyLineDimension"; break; }
		case 2:
		{ sValue = "PolygonDimension"; break; }
		}

		Add("IT", sValue.c_str());
	}
	void CPolygonLineAnnotation::SetSubtype(const BYTE& nSubtype)
	{
		switch (nSubtype)
		{
		case 6:
		{ m_nSubtype = AnnotPolygon; break; }
		case 7:
		{ m_nSubtype = AnnotPolyLine; break; }
		}

		Add("Subtype", c_sAnnotTypeNames[(int)m_nSubtype]);
	}
	void CPolygonLineAnnotation::SetLE(const BYTE& nLE1, const BYTE& nLE2)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("LE", pArray);

		pArray->Add(AddLE(nLE1).c_str());
		pArray->Add(AddLE(nLE2).c_str());
	}
	void CPolygonLineAnnotation::SetIC(const std::vector<double>& arrIC)
	{
		AddToVectorD(this, "IC", arrIC);
	}
	void CPolygonLineAnnotation::SetVertices(const std::vector<double>& arrVertices)
	{
		AddToVectorD(this, "Vertices", arrVertices);
	}
	//----------------------------------------------------------------------------------------
	// CCaretAnnotation
	//----------------------------------------------------------------------------------------
	CCaretAnnotation::CCaretAnnotation(CXref* pXref) : CMarkupAnnotation(pXref, AnnotCaret)
	{

	}
	void CCaretAnnotation::SetRD(const double& dRD1, const double& dRD2, const double& dRD3, const double& dRD4)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("RD", pArray);

		pArray->Add(dRD1);
		pArray->Add(dRD2);
		pArray->Add(dRD3);
		pArray->Add(dRD4);
	}
	void CCaretAnnotation::SetSy(const BYTE& nSy)
	{
		std::string sValue;
		switch (nSy)
		{
		case 0:
		{ sValue = "P"; break; }
		case 1:
		{ sValue = "None"; break; }
		}

		Add("IT", sValue.c_str());
	}
	//----------------------------------------------------------------------------------------
	// CWidgetAnnotation
	//----------------------------------------------------------------------------------------
	CWidgetAnnotation::CWidgetAnnotation(CXref* pXref, EAnnotType eType) : CAnnotation(pXref, eType)
	{
		m_pMK = NULL;
	}
	void CWidgetAnnotation::CheckMK()
	{
		if (!m_pMK)
		{
			m_pMK = new CDictObject();
			Add("MK", m_pMK);
		}
	}
	void CWidgetAnnotation::SetQ(const BYTE& nQ)
	{
		Add("Q", (int)nQ);
	}
	void CWidgetAnnotation::SetH(const BYTE& nH)
	{
		std::string sValue;
		switch (nH)
		{
		case 0:
		{ sValue = "N"; break; }
		case 1:
		{ sValue = "I"; break; }
		case 2:
		{ sValue = "P"; break; }
		case 3:
		{ sValue = "O"; break; }
		}

		Add("H", sValue.c_str());
	}
	void CWidgetAnnotation::SetR(const int& nR)
	{
		CheckMK();

		m_pMK->Add("R", nR);
	}
	void CWidgetAnnotation::SetFlag(const int& nFlag)
	{
		Add("Ff", nFlag);
	}
	void CWidgetAnnotation::SetParentID(const int& nParentID)
	{

	}
	void CWidgetAnnotation::SetTU(const std::wstring& wsTU)
	{
		std::string sValue = U_TO_UTF8(wsTU);
		Add("TU", new CStringObject(sValue.c_str()));
	}
	void CWidgetAnnotation::SetDS(const std::wstring& wsDS)
	{

	}
	void CWidgetAnnotation::SetDV(const std::wstring& wsDV)
	{

	}
	void CWidgetAnnotation::SetT(const std::wstring& wsT)
	{
		std::string sValue = U_TO_UTF8(wsT);
		Add("T", new CStringObject(sValue.c_str()));
	}
	void CWidgetAnnotation::SetTC(const std::vector<double>& arrTC)
	{
		// Text color из DA
	}
	void CWidgetAnnotation::SetBC(const std::vector<double>& arrBC)
	{
		CheckMK();
		AddToVectorD(m_pMK, "BC", arrBC);
	}
	void CWidgetAnnotation::SetBG(const std::vector<double>& arrBG)
	{
		CheckMK();
		AddToVectorD(m_pMK, "BG", arrBG);
	}
	//----------------------------------------------------------------------------------------
	// CButtonWidget
	//----------------------------------------------------------------------------------------
	CButtonWidget::CButtonWidget(CXref* pXref) : CWidgetAnnotation(pXref, AnnotWidget)
	{
		m_pIF = NULL;
	}
	void CButtonWidget::CheckIF()
	{
		CheckMK();

		if (!m_pIF)
		{
			m_pIF = new CDictObject();
			m_pMK->Add("IF", m_pIF);
		}
	}
	void CButtonWidget::SetS(const BYTE& nS)
	{
		CheckIF();

		std::string sValue;
		switch (nS)
		{
		case 0:
		{ sValue = "P"; break; }
		case 1:
		{ sValue = "A"; break; }
		}

		m_pIF->Add("S", sValue.c_str());
	}
	void CButtonWidget::SetTP(const BYTE& nTP)
	{
		Add("TP", nTP);
	}
	void CButtonWidget::SetSW(const BYTE& nSW)
	{
		CheckIF();

		std::string sValue;
		switch (nSW)
		{
		case 0:
		{ sValue = "A"; break; }
		case 1:
		{ sValue = "N"; break; }
		case 2:
		{ sValue = "B"; break; }
		case 3:
		{ sValue = "S"; break; }
		}

		m_pIF->Add("SW", sValue.c_str());
	}
	void CButtonWidget::SetStyle(const BYTE& nStyle)
	{
		CheckMK();

		std::string sValue;
		switch (nStyle)
		{
		case 0:
		{ sValue = "4"; break; }
		case 1:
		{ sValue = "8"; break; }
		case 2:
		{ sValue = "u"; break; }
		case 3:
		{ sValue = "l"; break; }
		case 4:
		{ sValue = "H"; break; }
		case 5:
		{ sValue = "n"; break; }
		}

		m_pMK->Add("CA", new CStringObject(sValue.c_str()));
	}
	void CButtonWidget::SetIFFlag(const int& nIFFlag)
	{
		CheckIF();

		m_pIF->Add("FB", (nIFFlag & (1 << 4)) ? true : false);
	}
	void CButtonWidget::SetA(const double& dA1, const double& dA2)
	{
		CheckIF();

		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		m_pIF->Add("A", pArray);

		pArray->Add(dA1);
		pArray->Add(dA2);
	}
	void CButtonWidget::SetCA(const std::wstring& wsCA)
	{
		CheckMK();

		std::string sValue = U_TO_UTF8(wsCA);
		m_pMK->Add("CA", new CStringObject(sValue.c_str()));
	}
	void CButtonWidget::SetRC(const std::wstring& wsRC)
	{
		CheckMK();

		std::string sValue = U_TO_UTF8(wsRC);
		m_pMK->Add("RC", new CStringObject(sValue.c_str()));
	}
	void CButtonWidget::SetAC(const std::wstring& wsAC)
	{
		CheckMK();

		std::string sValue = U_TO_UTF8(wsAC);
		m_pMK->Add("AC", new CStringObject(sValue.c_str()));
	}
	void CButtonWidget::SetAP_N_Yes(const std::wstring& wsAP_N_Yes)
	{

	}
}
