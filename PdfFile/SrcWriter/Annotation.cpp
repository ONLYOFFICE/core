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
#include "ResourcesDictionary.h"
#include "Streams.h"

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
		"Check", "Checkmark", "Circle", "Comment", "Cross", "CrossHairs", "Help", "Insert", "Key", "NewParagraph", "Note", "Paragraph", "RightArrow", "RightPointer", "Star", "UpArrow", "UpLeftArrow"
	};

	void AddToVectorD(CDictObject* pObj, const std::string& sName, const std::vector<double>& arrV)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		pObj->Add(sName, pArray);

		for (const double& dV : arrV)
			pArray->Add(dV);
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
	void AddRD(CDictObject* pObj, const double& dRD1, const double& dRD2, const double& dRD3, const double& dRD4)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		pObj->Add("RD", pArray);

		pArray->Add(dRD1);
		pArray->Add(dRD2);
		pArray->Add(dRD3);
		pArray->Add(dRD4);
	}
	std::string GetColor(const std::vector<double>& arr, bool bCaps)
	{
		std::string sDA;
		for (double dColoc : arr)
		{
			sDA.append(std::to_string(dColoc));
			sDA.append(" ");
		}
		if (arr.size() == 3)
			sDA.append(bCaps ? "RG" : "rg");
		else if (arr.size() == 4)
			sDA.append(bCaps ? "K" : "k");
		else if (arr.size() == 1)
			sDA.append(bCaps ? "G" : "g");
		else
			sDA.append(bCaps ? "SC" : "sc");
		return sDA;
	}

	//----------------------------------------------------------------------------------------
	// CAnnotation
	//----------------------------------------------------------------------------------------
	CAnnotation::CAnnotation(CXref* pXref, EAnnotType eType)
	{
		m_pDocument = NULL;
		m_pXref = pXref;

		Add("Type", "Annot");
		Add("Subtype", c_sAnnotTypeNames[(int)eType]);

		// Для PDFA нужно, чтобы 0, 1, 4 биты были выключены, а второй включен
		Add("F", 4);
	}
	void CAnnotation::SetRect(const TRect& oRect)
	{
		m_oRect = oRect;

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
	void CAnnotation::SetBorder(BYTE nType, double dWidth, const std::vector<double>& arrDash)
	{
		CDictObject* pBorderStyleDict = new CDictObject();
		if (!pBorderStyleDict)
			return;

		Add("BS", pBorderStyleDict);

		pBorderStyleDict->Add("Type", "Border");
		pBorderStyleDict->Add("W", dWidth);

		EBorderSubtype eSubtype = EBorderSubtype(nType);
		if (border_subtype_Dashed == eSubtype)
			AddToVectorD(pBorderStyleDict, "D", arrDash);

		switch (eSubtype)
		{
		case border_subtype_Solid:      pBorderStyleDict->Add("S", "S"); break;
		case border_subtype_Dashed:     pBorderStyleDict->Add("S", "D"); break;
		case border_subtype_Beveled:    pBorderStyleDict->Add("S", "B"); break;
		case border_subtype_Inset:      pBorderStyleDict->Add("S", "I"); break;
		case border_subtype_Underlined: pBorderStyleDict->Add("S", "U"); break;
		}

		m_oBorder.bHave = true;
		m_oBorder.nType = nType;
		m_oBorder.dWidth = dWidth;
		m_oBorder.arrDash = arrDash;
	}
	void CAnnotation::SetAnnotFlag(const int& nAnnotFlag)
	{
		Add("F", nAnnotFlag);
	}
	void CAnnotation::SetPage(CPage* pPage)
	{
		Add("P", pPage);
		m_dPageWidth  = pPage->GetWidth();
		m_dPageHeight = pPage->GetHeight();
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
		Add("Contents", new CStringObject(sValue.c_str(), true));

		// TODO
		Remove("RC");
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
	void CAnnotation::SetC(const std::vector<double>& arrC)
	{
		AddToVectorD(this, "C", arrC);
	}
	void CAnnotation::SetDocument(CDocument* pDocument)
	{
		m_pDocument = pDocument;
	}
	CDocument* CAnnotation::GetDocument()
	{
		return m_pDocument;
	}
	std::string CAnnotation::GetBorderDash()
	{
		std::string sRes = "[";
		for (double dColoc : m_oBorder.arrDash)
		{
			sRes.append(" ");
			sRes.append(std::to_string(dColoc));
		}
		sRes.append(" ] 0 d\012");
		return sRes;
	}
	//----------------------------------------------------------------------------------------
	// CMarkupAnnotation
	//----------------------------------------------------------------------------------------
	CMarkupAnnotation::CMarkupAnnotation(CXref* pXref, EAnnotType eType) : CAnnotation(pXref, eType)
	{
	}
	void CMarkupAnnotation::SetRT(const BYTE& nRT)
	{
		Add("RT", nRT ? "Group" : "R");
	}
	CPopupAnnotation* CMarkupAnnotation::CreatePopup()
	{
		CPopupAnnotation* pAnnot = new CPopupAnnotation(m_pXref);
		m_pXref->Add(pAnnot);
		Add("Popup", pAnnot);

		pAnnot->SetOpen(false);
		pAnnot->SetParentID(this);
		pAnnot->SetAnnotFlag(28);

		TRect oRect = m_oRect;
		oRect.fLeft = m_dPageWidth - 100 - (oRect.fRight - oRect.fLeft);
		oRect.fBottom -= 100;
		oRect.fRight = m_dPageWidth;
		pAnnot->SetRect(oRect);

		return pAnnot;
	}
	void CMarkupAnnotation::SetCA(const double& dCA)
	{
		Add("CA", dCA);
	}
	void CMarkupAnnotation::SetT(const std::wstring& wsT)
	{
		std::string sValue = U_TO_UTF8(wsT);
		Add("T", new CStringObject(sValue.c_str(), true));
	}
	void CMarkupAnnotation::SetRC(const std::wstring& wsRC)
	{
		std::string sValue = U_TO_UTF8(wsRC);
		Add("RC", new CStringObject(sValue.c_str(), true));
	}
	void CMarkupAnnotation::SetCD(const std::wstring& wsCD)
	{
		std::string sValue = U_TO_UTF8(wsCD);
		Add("CreationDate", new CStringObject(sValue.c_str()));
	}
	void CMarkupAnnotation::SetSubj(const std::wstring& wsSubj)
	{
		std::string sValue = U_TO_UTF8(wsSubj);
		Add("Subj", new CStringObject(sValue.c_str(), true));
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
		m_nName = 10;
		Add("Name", "Comment");
	}
	void CTextAnnotation::SetOpen(bool bOpen)
	{
		Add("Open", new CBoolObject(bOpen));
	}
	void CTextAnnotation::SetName(BYTE nName)
	{
		m_nName = nName;
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
		if (!Get("C"))
			SetC({ 1.0, 0.8, 0.0 });
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
	void CTextAnnotation::SetAP()
	{
		switch (m_nName)
		{
		case 3:
		{
			CAnnotAppearanceObject* pAP = new CAnnotAppearanceObject(m_pXref, this);
			Add("AP", pAP);

			pAP->DrawTextComment();
			break;
		}
		case 10:
		default:
		{
			break;
		}
		}
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

			for (int i = 0; i < arrInk.size(); ++i)
				pArrayI->Add(i % 2 == 0 ? arrInk[i] : (m_dPageHeight - arrInk[i]));
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
		pArray->Add(m_dPageHeight - dL2);
		pArray->Add(dL3);
		pArray->Add(m_dPageHeight - dL4);
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
	}
	void CPopupAnnotation::SetOpen(bool bOpen)
	{
		Add("Open", new CBoolObject(bOpen));
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
		AddRD(this, dRD1, dRD2, dRD3, dRD4);
	}
	void CFreeTextAnnotation::SetCL(const std::vector<double>& arrCL)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("CL", pArray);

		for (int i = 0; i < arrCL.size(); ++i)
			pArray->Add(i % 2 == 0 ? arrCL[i] : (m_dPageHeight - arrCL[i]));
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
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("QuadPoints", pArray);

		for (int i = 0; i < arrQuadPoints.size(); ++i)
			pArray->Add(i % 2 == 0 ? arrQuadPoints[i] : (m_dPageHeight - arrQuadPoints[i]));
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
		AddRD(this, dRD1, dRD2, dRD3, dRD4);
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
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("Vertices", pArray);

		for (int i = 0; i < arrVertices.size(); ++i)
			pArray->Add(i % 2 == 0 ? arrVertices[i] : (m_dPageHeight - arrVertices[i]));
	}
	//----------------------------------------------------------------------------------------
	// CCaretAnnotation
	//----------------------------------------------------------------------------------------
	CCaretAnnotation::CCaretAnnotation(CXref* pXref) : CMarkupAnnotation(pXref, AnnotCaret)
	{
	}
	void CCaretAnnotation::SetRD(const double& dRD1, const double& dRD2, const double& dRD3, const double& dRD4)
	{
		AddRD(this, dRD1, dRD2, dRD3, dRD4);
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
		Add("Ff", 0);

		m_pMK = NULL;
		m_pParent = NULL;
		m_pAA = NULL;
		m_pA = NULL;
	}
	void CWidgetAnnotation::SetDA(CFontDict* pFont, const double& dFontSize, const double& dFontSizeAP, const std::vector<double>& arrTC)
	{
		CResourcesDict* pFieldsResources = m_pDocument->GetFieldsResources();
		const char* sFontName = pFieldsResources->GetFontName(pFont);

		std::string sDA = GetColor(arrTC, false);
		if (sFontName)
		{
			sDA.append(" /");
			sDA.append(sFontName);
		}

		sDA.append(" ");
		m_sDAforAP = sDA;
		sDA.append(std::to_string(dFontSize));
		m_sDAforAP.append(std::to_string(dFontSizeAP));
		sDA.append(" Tf");
		m_sDAforAP.append(" Tf\012");

		Add("DA", new CStringObject(sDA.c_str()));
	}
	CDictObject* CWidgetAnnotation::GetObjOwnValue(const std::string& sV)
	{
		if (Get(sV))
			return this;
		CDictObject* pParent = m_pParent;
		while (pParent)
		{
			if (pParent->Get(sV))
				return pParent;
			CObjectBase* pParent2 = pParent->Get("Parent");
			if (pParent2 && pParent2->GetType() == object_type_DICT)
				pParent = (CDictObject*)pParent2;
			else
				return NULL;
		}
		return NULL;
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
	void CWidgetAnnotation::SetParent(CDictObject* pParent)
	{
		if (!pParent)
			return;
		m_pParent = pParent;
		Add("Parent", pParent);
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
	void CWidgetAnnotation::SetBC(const std::vector<double>& arrBC)
	{
		CheckMK();
		AddToVectorD(m_pMK, "BC", arrBC);
		m_arrBC = arrBC;
	}
	void CWidgetAnnotation::SetBG(const std::vector<double>& arrBG)
	{
		CheckMK();
		AddToVectorD(m_pMK, "BG", arrBG);
		m_arrBG = arrBG;
	}
	void CWidgetAnnotation::AddAction(CAction* pAction)
	{
		if (!pAction)
			return;

		if (pAction->m_sType == "A")
		{
			CDictObject* pOwner = GetObjOwnValue(pAction->m_sType);
			if (!pOwner)
				pOwner = m_pParent ? m_pParent : this;

			pOwner->Add(pAction->m_sType.c_str(), pAction);
			return;
		}

		if (!m_pAA)
		{
			CDictObject* pOwner = GetObjOwnValue("AA");
			if (!pOwner)
			{
				Add("AA", new CDictObject());
				pOwner = this;
			}
			m_pAA = (CDictObject*)pOwner->Get("AA");
		}

		m_pAA->Add(pAction->m_sType.c_str(), pAction);
	}
	std::string CWidgetAnnotation::GetBGforAP()
	{
		return GetColor(m_arrBG, false) + "\012";
	}
	std::string CWidgetAnnotation::GetBCforAP()
	{
		return GetColor(m_arrBC, true) + "\012";
	}
	//----------------------------------------------------------------------------------------
	// CButtonWidget
	//----------------------------------------------------------------------------------------
	CButtonWidget::CButtonWidget(CXref* pXref) : CWidgetAnnotation(pXref, AnnotWidget)
	{
		Add("FT", "Btn");

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
	void CButtonWidget::SetV(bool bV)
	{
		CDictObject* pOwner = GetObjOwnValue("V");
		if (!pOwner)
		{
			if (m_pParent)
			{
				if (bV)
					m_pParent->Add("V", m_sAP_N_Yes.c_str());
			}
			else
				Add("V", (bV ? m_sAP_N_Yes.c_str() : "Off"));
		}
		else if (bV)
			pOwner->Add("V", m_sAP_N_Yes.c_str());

		// TODO
		// Add("AS", (bV ? m_sAP_N_Yes.c_str() : "Off"));
	}
	void CButtonWidget::SetDV(const std::wstring& wsDV)
	{
		std::string sValue = U_TO_UTF8(wsDV);
		Add("DV", sValue.c_str());
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
		Add("TP", (int)nTP);
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
		std::string sValue = U_TO_UTF8(wsAP_N_Yes);
		m_sAP_N_Yes = sValue;
	}
	//----------------------------------------------------------------------------------------
	// CTextWidget
	//----------------------------------------------------------------------------------------
	CTextWidget::CTextWidget(CXref* pXref) : CWidgetAnnotation(pXref, AnnotWidget)
	{
	}
	void CTextWidget::SetMaxLen(const int& nMaxLen)
	{
		if (nMaxLen > 0)
		{
			if (m_pParent)
				m_pParent->Add("MaxLen", nMaxLen);
			else
				Add("MaxLen", nMaxLen);
		}
	}
	void CTextWidget::SetV(const std::wstring& wsV)
	{
		m_sV = U_TO_UTF8(wsV);
		CDictObject* pOwner = GetObjOwnValue("V");
		if (!pOwner)
			pOwner = GetObjOwnValue("FT");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("V", new CStringObject(m_sV.c_str(), true));
	}
	void CTextWidget::SetRV(const std::wstring& wsRV)
	{
		std::string sValue = U_TO_UTF8(wsRV);
		Add("RV", new CStringObject(sValue.c_str()));
	}
	void CTextWidget::SetAP(const std::wstring& wsValue, unsigned short* pCodes, unsigned int unCount, CFontDict* pFont, const TRgb& oColor, const double& dAlpha, double dFontSize, double dX, double dY, CFontCidTrueType** ppFonts, double* pShifts)
	{
		m_pAppearance = new CAnnotAppearance(m_pXref, this);
		if (!m_pAppearance)
			return;
		Add("AP", m_pAppearance);

		CAnnotAppearanceObject* pNormal = m_pAppearance->GetNormal();
		CResourcesDict* pFieldsResources = m_pDocument->GetFieldsResources();

		const char* sExtGrStateName = NULL;
		if (fabs(dAlpha - 1.0) > 0.001)
		{
			CExtGrState* pExtGrState = m_pDocument->GetFillAlpha(dAlpha);
			sExtGrStateName = pFieldsResources->GetExtGrStateName(pExtGrState);
		}

		pNormal->DrawSimpleText(wsValue, pCodes, unCount, pFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop), ppFonts, pShifts);
	}
	void CTextWidget::StartAP(CFontDict* pFont, const double& dFontSize, const double& dAlpha)
	{
		m_pAppearance = new CAnnotAppearance(m_pXref, this);
		if (!m_pAppearance)
			return;

		Add("AP", m_pAppearance);

		CAnnotAppearanceObject* pNormal  = m_pAppearance->GetNormal();
		CResourcesDict* pFieldsResources = m_pDocument->GetFieldsResources();

		const char* sExtGrStateName = NULL;
		if (fabs(dAlpha - 1.0) > 0.001)
		{
			CExtGrState* pExtGrState = m_pDocument->GetFillAlpha(dAlpha);
			sExtGrStateName = pFieldsResources->GetExtGrStateName(pExtGrState);
		}

		pNormal->StartDrawText(pFont, dFontSize, 0, 0, 0, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));
	}
	void CTextWidget::AddLineToAP(const double& dX, const double& dY, unsigned short* pCodes, const unsigned int& unCodesCount, CFontCidTrueType** ppFonts, const double* pShifts)
	{
		if (!m_pAppearance)
			return;

		CAnnotAppearanceObject* pNormal = m_pAppearance->GetNormal();
		pNormal->DrawTextLine(dX, dY, pCodes, unCodesCount, ppFonts, pShifts);
	}
	void CTextWidget::EndAP()
	{
		if (!m_pAppearance)
			return;

		CAnnotAppearanceObject* pNormal = m_pAppearance->GetNormal();
		pNormal->EndDrawText();
	}
	bool CTextWidget::IsCombFlag()
	{
		int nFlags = ((CNumberObject*)Get("Ff"))->Get();
		return (nFlags & (1 << 24));
	}
	bool CTextWidget::IsMultiLine()
	{
		int nFlags = ((CNumberObject*)Get("Ff"))->Get();
		return (nFlags & (1 << 12));
	}
	int CTextWidget::GetMaxLen()
	{
		CNumberObject* oMaxLen = m_pParent ? (CNumberObject*)m_pParent->Get("MaxLen") : (CNumberObject*)Get("MaxLen");
		return oMaxLen ? oMaxLen->Get() : 0;
	}
	//----------------------------------------------------------------------------------------
	// CChoiceWidget
	//----------------------------------------------------------------------------------------
	CChoiceWidget::CChoiceWidget(CXref* pXref) : CWidgetAnnotation(pXref, AnnotWidget)
	{
		Add("FT", "Ch");
	}
	void CChoiceWidget::SetTI(const int& nTI)
	{
		Add("TI", nTI);
	}
	void CChoiceWidget::SetV(const std::wstring& wsV)
	{
		std::string sValue = U_TO_UTF8(wsV);
		CDictObject* pOwner = GetObjOwnValue("V");
		if (!pOwner)
			pOwner = GetObjOwnValue("FT");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("V", new CStringObject(sValue.c_str()));
	}
	void CChoiceWidget::SetOpt(const std::vector< std::pair<std::wstring, std::wstring> >& arrOpt)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("Opt", pArray);

		for (const std::pair<std::wstring, std::wstring>& PV : arrOpt)
		{
			if (PV.first.empty())
			{
				std::string sValue = U_TO_UTF8(PV.second);
				pArray->Add(new CStringObject(sValue.c_str()));
			}
			else
			{
				CArrayObject* pArray2 = new CArrayObject();
				pArray->Add(pArray2);

				std::string sValue = U_TO_UTF8(PV.first);
				pArray2->Add(new CStringObject(sValue.c_str()));

				sValue = U_TO_UTF8(PV.second);
				pArray2->Add(new CStringObject(sValue.c_str()));
			}
		}
	}
	//----------------------------------------------------------------------------------------
	// CSignatureWidget
	//----------------------------------------------------------------------------------------
	CSignatureWidget::CSignatureWidget(CXref* pXref) : CWidgetAnnotation(pXref, AnnotWidget)
	{
	}
	/*
	void CAnnotationAppearance::DrawTextWidget()
	{
		CTextWidget* pAnnot = (CTextWidget*)m_pAnnot;



		// TODO потребуется смещение Y-координаты в зависимости от размеров области и размеров шрифта
		// TODO требуется запись юникод значений, т.е необходимо писать по глифам
		m_pStream->WriteStr(" 2 6.548 Td\012");
		std::string sV = pAnnot->GetV();
		m_pStream->WriteEscapeText((BYTE*)sV.c_str(), sV.length());
		m_pStream->WriteStr(" Tj\012ET\012Q\012EMC\012");
	}
	*/
	//----------------------------------------------------------------------------------------
	// CAction
	//----------------------------------------------------------------------------------------
	CAction::CAction(CXref* pXref)
	{
		pXref->Add(this);
		Add("Type", "Action");
	}
	void CAction::SetType(const std::wstring& wsType)
	{
		m_sType = U_TO_UTF8(wsType);
	}
	//----------------------------------------------------------------------------------------
	CActionResetForm::CActionResetForm(CXref* pXref) : CAction(pXref)
	{
		Add("S", "ResetForm");
		Add("Flags", 1);
	}
	void CActionResetForm::SetFlags(int nFlag)
	{
		Add("Flags", nFlag);
	}
	void CActionResetForm::SetFields(const std::vector<std::wstring>& arrFileds)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("Fields", pArray);

		for (const std::wstring& A : arrFileds)
		{
			std::string sValue = U_TO_UTF8(A);
			pArray->Add(new CStringObject(sValue.c_str()));
		}
	}
	//----------------------------------------------------------------------------------------
	CActionJavaScript::CActionJavaScript(CXref* pXref) : CAction(pXref)
	{
		Add("S", "JavaScript");
	}
	void CActionJavaScript::SetJS(const std::wstring& wsJS)
	{
		std::string sValue = U_TO_UTF8(wsJS);
		Add("JS", new CStringObject(sValue.c_str()));
	}
	//----------------------------------------------------------------------------------------
	CActionGoTo::CActionGoTo(CXref* pXref) : CAction(pXref)
	{
		Add("S", "GoTo");
	}
	void CActionGoTo::SetDestination(CDestination* pDest)
	{
		Add("D", pDest);
	}
	//----------------------------------------------------------------------------------------
	CActionURI::CActionURI(CXref* pXref) : CAction(pXref)
	{
		Add("S", "URI");
	}
	void CActionURI::SetURI(const std::wstring& wsURI)
	{
		std::string sValue = U_TO_UTF8(wsURI);
		Add("URI", new CStringObject(sValue.c_str(), true));
	}
	//----------------------------------------------------------------------------------------
	CActionHide::CActionHide(CXref* pXref) : CAction(pXref)
	{
		Add("S", "Hide");
	}
	void CActionHide::SetH(BYTE nH)
	{
		Add("H", !!nH);
	}
	void CActionHide::SetT(const std::vector<std::wstring>& arrT)
	{

	}
	//----------------------------------------------------------------------------------------
	CActionNamed::CActionNamed(CXref* pXref) : CAction(pXref)
	{
		Add("S", "Named");
	}
	void CActionNamed::SetN(const std::wstring& wsN)
	{
		std::string sValue = U_TO_UTF8(wsN);
		Add("N", sValue.c_str());
	}
}
