﻿/*
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
#include "FontCidTT.h"
#include "Streams.h"
#include "Image.h"

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
	std::string GetColor(const std::vector<double>& arr, bool bCaps, double dDiff = 0)
	{
		if (arr.empty())
			return bCaps ? "1 G" : "1 g";

		std::string sDA;
		for (double dColoc : arr)
		{
			sDA.append(std::to_string(dColoc + dDiff));
			sDA.append(" ");
		}
		if (arr.size() == 3)
			sDA.append(bCaps ? "RG" : "rg");
		else if (arr.size() == 4)
			sDA.append(bCaps ? "K" : "k");
		else if (arr.size() == 1)
			sDA.append(bCaps ? "G" : "g");
		// else
		// 	sDA.append(bCaps ? "SC" : "sc");
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

		m_oBorder.bHave = false;
		m_oBorder.nType = 0;
		m_oBorder.dWidth = 0;
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
	void CAnnotation::SetEmptyBorder()
	{
		m_oBorder.nType = 0;
		m_oBorder.dWidth = 1;
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
		pBEDict->Add("I", dBE);

		std::string sValue;
		switch (nType)
		{
		case 0:
		{ sValue = "S"; break; }
		case 1:
		{ sValue = "C"; break; }
		default: { return; }
		}

		pBEDict->Add("S", sValue.c_str());
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
	void CMarkupAnnotation::SetRT(BYTE nRT)
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
	void CMarkupAnnotation::RemoveAP()
	{
		Remove("AP");
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
		default: { return; }
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
		default: { return; }
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
	void CLineAnnotation::SetIT(BYTE nIT)
	{
		std::string sValue;
		switch (nIT)
		{
		case 0:
		{ sValue = "LineDimension"; break; }
		case 1:
		{ sValue = "LineArrow"; break; }
		default: { return; }
		}

		Add("IT", sValue.c_str());
	}
	void CLineAnnotation::SetCP(BYTE nCP)
	{
		std::string sValue;
		switch (nCP)
		{
		case 0:
		{ sValue = "Inline"; break; }
		case 1:
		{ sValue = "Top"; break; }
		default: { return; }
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
	void CLineAnnotation::SetLE(BYTE nLE1, BYTE nLE2)
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
	void CFreeTextAnnotation::SetQ(BYTE nQ)
	{
		Add("Q", (int)nQ);
	}
	void CFreeTextAnnotation::SetIT(BYTE nIT)
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
		default: { return; }
		}

		Add("IT", sValue.c_str());
	}
	void CFreeTextAnnotation::SetLE(BYTE nLE)
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
	void CTextMarkupAnnotation::SetSubtype(BYTE nSubtype)
	{
		switch (nSubtype)
		{
		case 8:
		{ m_nSubtype = AnnotHighLight; break; }
		case 9:
		{ m_nSubtype = AnnotUnderline; break; }
		case 10:
		{ m_nSubtype = AnnotSquiggly; break; }
		default:
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
	void CSquareCircleAnnotation::SetSubtype(BYTE nSubtype)
	{
		switch (nSubtype)
		{
		case 4:
		{ m_nSubtype = AnnotSquare; break; }
		default:
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
	void CPolygonLineAnnotation::SetIT(BYTE nIT)
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
		default: { return; }
		}

		Add("IT", sValue.c_str());
	}
	void CPolygonLineAnnotation::SetSubtype(BYTE nSubtype)
	{
		switch (nSubtype)
		{
		case 6:
		{ m_nSubtype = AnnotPolygon; break; }
		default:
		case 7:
		{ m_nSubtype = AnnotPolyLine; break; }
		}

		Add("Subtype", c_sAnnotTypeNames[(int)m_nSubtype]);
	}
	void CPolygonLineAnnotation::SetLE(BYTE nLE1, BYTE nLE2)
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
	void CCaretAnnotation::SetSy(BYTE nSy)
	{
		std::string sValue;
		switch (nSy)
		{
		case 0:
		{ sValue = "P"; break; }
		case 1:
		{ sValue = "None"; break; }
		default: { return; }
		}

		Add("IT", sValue.c_str());
	}
	//----------------------------------------------------------------------------------------
	// CWidgetAnnotation
	//----------------------------------------------------------------------------------------
	CWidgetAnnotation::CWidgetAnnotation(CXref* pXref, EAnnotType eType) : CAnnotation(pXref, eType)
	{
		m_pMK         = NULL;
		m_pParent     = NULL;
		m_pAA         = NULL;
		m_pA          = NULL;
		m_pAppearance = NULL;
		m_pFont       = NULL;
		m_dFontSizeAP = 0;
		m_nQ          = 0;
		m_dFontSize   = 10.0;
		m_bBold       = false;
		m_bItalic     = false;
		m_nSubtype    = WidgetUnknown;
	}
	void CWidgetAnnotation::SetSubtype(BYTE nSubtype)
	{
		m_nSubtype = (EWidgetType)nSubtype;
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
		sDA.append(std::to_string(dFontSize));
		sDA.append(" Tf");

		CDictObject* pOwner = GetObjOwnValue("DA");
		if (pOwner)
			pOwner->Remove("DA");
		Add("DA", new CStringObject(sDA.c_str()));

		m_arrTC = arrTC;
		m_dFontSizeAP = dFontSizeAP;
	}
	void CWidgetAnnotation::SetFont(CFontCidTrueType* pFont, double dFontSize, bool bBold, bool bItalic)
	{
		m_pFont = pFont;
		m_dFontSize = dFontSize;
		m_bBold     = bBold;
		m_bItalic   = bItalic;
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
	CObjectBase* CWidgetAnnotation::GetObjValue(const std::string& sV)
	{
		CObjectBase* pRes = Get(sV);
		if (pRes)
			return pRes;
		CDictObject* pParent = m_pParent;
		while (pParent)
		{
			pRes = pParent->Get(sV);
			if (pRes)
				return pRes;
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
	void CWidgetAnnotation::SetQ(BYTE nQ)
	{
		CDictObject* pOwner = GetObjOwnValue("Q");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("Q", (int)nQ);
		m_nQ = nQ;
	}
	void CWidgetAnnotation::SetH(BYTE nH)
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
		default: { return; }
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
		CDictObject* pOwner = GetObjOwnValue("Ff");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("Ff", nFlag);
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
		CDictObject* pOwner = GetObjOwnValue("TU");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("TU", new CStringObject(sValue.c_str()));
	}
	void CWidgetAnnotation::SetDS(const std::wstring& wsDS)
	{
		std::string sValue = U_TO_UTF8(wsDS);
		CDictObject* pOwner = GetObjOwnValue("DS");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("DS", new CStringObject(sValue.c_str(), true));
	}
	void CWidgetAnnotation::SetDV(const std::wstring& wsDV)
	{
		std::string sValue = U_TO_UTF8(wsDV);
		CDictObject* pOwner = GetObjOwnValue("DV");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("DV", new CStringObject(sValue.c_str(), true));
	}
	void CWidgetAnnotation::SetT(const std::wstring& wsT)
	{
		std::string sValue = U_TO_UTF8(wsT);
		CDictObject* pOwner = GetObjOwnValue("T");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("T", new CStringObject(sValue.c_str(), true));
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
				pOwner = this;

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
	std::string CWidgetAnnotation::GetDAforAP(CFontDict* pFont)
	{
		CResourcesDict* pFieldsResources = m_pDocument->GetFieldsResources();
		const char* sFontName = pFieldsResources->GetFontName(pFont);

		std::string sDA = GetColor(m_arrTC, false);
		if (sFontName)
		{
			sDA.append(" /");
			sDA.append(sFontName);
		}

		sDA.append(" ");
		sDA.append(std::to_string(m_dFontSizeAP));
		sDA.append(" Tf\012");

		return sDA;
	}
	std::string CWidgetAnnotation::GetBGforAP(double dDiff)
	{
		return GetColor(m_arrBG, false, dDiff) + "\012";
	}
	std::string CWidgetAnnotation::GetBCforAP()
	{
		return GetColor(m_arrBC, true) + "\012";
	}
	void CWidgetAnnotation::SetAP(const std::wstring& wsValue, unsigned short* pCodes, unsigned int unCount, double dX, double dY, CFontCidTrueType** ppFonts, double* pShifts)
	{
		m_pAppearance = new CAnnotAppearance(m_pXref, this);
		if (!m_pAppearance)
			return;
		Add("AP", m_pAppearance);
		CAnnotAppearanceObject* pNormal = m_pAppearance->GetNormal();
		pNormal->DrawSimpleText(wsValue, pCodes, unCount, m_pFont, m_dFontSize, dX, dY, 0, 0, 0, NULL, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop), ppFonts, pShifts);
	}
	void CWidgetAnnotation::StartAP()
	{
		m_pAppearance = new CAnnotAppearance(m_pXref, this);
		if (!m_pAppearance)
			return;
		Add("AP", m_pAppearance);
		CAnnotAppearanceObject* pNormal  = m_pAppearance->GetNormal();
		pNormal->StartDrawText(m_pFont, m_dFontSize, 0, 0, 0, NULL, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));
	}
	void CWidgetAnnotation::AddLineToAP(const double& dX, const double& dY, unsigned short* pCodes, const unsigned int& unCodesCount, CFontCidTrueType** ppFonts, const double* pShifts)
	{
		if (!m_pAppearance)
			return;
		CAnnotAppearanceObject* pNormal = m_pAppearance->GetNormal();
		pNormal->DrawTextLine(dX, dY, pCodes, unCodesCount, ppFonts, pShifts);
	}
	void CWidgetAnnotation::EndAP()
	{
		if (!m_pAppearance)
			return;
		CAnnotAppearanceObject* pNormal = m_pAppearance->GetNormal();
		pNormal->EndDrawText();
	}
	//----------------------------------------------------------------------------------------
	// CPushButtonWidget
	//----------------------------------------------------------------------------------------
	CPushButtonWidget::CPushButtonWidget(CXref* pXref) : CWidgetAnnotation(pXref, AnnotWidget)
	{
		m_pIF = NULL;
		m_nI  = -1;
		m_nRI = -1;
		m_nIX = -1;
		m_nTP = 0;
		m_nScaleType = 0;
		m_bRespectBorders = false;
		m_bConstantProportions = true;
		m_dShiftX = 0.5;
		m_dShiftY = 0.5;
	}
	void CPushButtonWidget::CheckIF()
	{
		CheckMK();

		if (!m_pIF)
		{
			m_pIF = new CDictObject();
			m_pMK->Add("IF", m_pIF);
		}
	}
	void CPushButtonWidget::SetV(const std::wstring& wsV)
	{
		std::string sV = U_TO_UTF8(wsV);
		CDictObject* pOwner = GetObjOwnValue("V");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("V", new CStringObject(sV.c_str(), true));
	}
	void CPushButtonWidget::SetS(BYTE nS)
	{
		m_bConstantProportions = !nS;

		CheckIF();

		std::string sValue;
		switch (nS)
		{
		case 0:
		{ sValue = "P"; break; }
		case 1:
		{ sValue = "A"; break; }
		default: { return; }
		}

		m_pIF->Add("S", sValue.c_str());
	}
	void CPushButtonWidget::SetTP(BYTE nTP)
	{
		CheckMK();

		m_pMK->Add("TP", (int)nTP);
		m_nTP = nTP;
	}
	void CPushButtonWidget::SetSW(BYTE nSW)
	{
		m_nScaleType = nSW;

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
		default: { return; }
		}

		m_pIF->Add("SW", sValue.c_str());
	}
	void CPushButtonWidget::SetIFFlag(const int& nIFFlag)
	{
		CheckIF();

		m_bRespectBorders = (nIFFlag & (1 << 4)) ? false : true;
		m_pIF->Add("FB", !m_bRespectBorders);
	}
	void CPushButtonWidget::SetFlag(const int& nFlag)
	{
		int nFlags = nFlag;
		nFlags |= (1 << 16);
		CWidgetAnnotation::SetFlag(nFlags);
	}
	void CPushButtonWidget::SetI(const int& nI)
	{
		m_nI = nI;
	}
	void CPushButtonWidget::SetRI(const int& nRI)
	{
		m_nRI = nRI;
	}
	void CPushButtonWidget::SetIX(const int& nIX)
	{
		m_nIX = nIX;
	}
	void CPushButtonWidget::SetA(const double& dA1, const double& dA2)
	{
		m_dShiftX = dA1;
		m_dShiftY = dA2;

		CheckIF();

		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		m_pIF->Add("A", pArray);

		pArray->Add(dA1);
		pArray->Add(dA2);
	}
	void CPushButtonWidget::SetCA(const std::wstring& wsCA)
	{
		CheckMK();

		std::string sValue = U_TO_UTF8(wsCA);
		m_pMK->Add("CA", new CStringObject(sValue.c_str(), true));
		m_wsCA = wsCA;
	}
	void CPushButtonWidget::SetRC(const std::wstring& wsRC)
	{
		CheckMK();

		std::string sValue = U_TO_UTF8(wsRC);
		m_pMK->Add("RC", new CStringObject(sValue.c_str(), true));
		m_wsRC = wsRC;
	}
	void CPushButtonWidget::SetAC(const std::wstring& wsAC)
	{
		CheckMK();

		std::string sValue = U_TO_UTF8(wsAC);
		m_pMK->Add("AC", new CStringObject(sValue.c_str(), true));
		m_wsAC = wsAC;
	}
	void CPushButtonWidget::SetAP(CXObject* pForm, BYTE nAP, unsigned short* pCodes, unsigned int unCount, double dX, double dY, double dLineW, double dLineH, CFontCidTrueType** ppFonts)
	{
		m_pAppearance = new CAnnotAppearance(m_pXref, this);
		if (!m_pAppearance)
			return;

		CAnnotAppearanceObject* pAppearance = NULL;
		if (nAP == 0)
			pAppearance = m_pAppearance->GetNormal();
		else if (nAP == 1)
			pAppearance = m_pAppearance->GetRollover();
		else if (nAP == 2)
			pAppearance = m_pAppearance->GetDown();
		if (!pAppearance)
			return;
		Add("AP", m_pAppearance);

		double dHeight = fabs(m_oRect.fTop - m_oRect.fBottom);
		double dWidth  = fabs(m_oRect.fRight - m_oRect.fLeft);

		pAppearance->StartDraw(dWidth, dHeight);

		if (pForm)
		{
			double dH = dHeight;
			double dW = dWidth;

			double dOriginW = pForm->GetWidth();
			double dOriginH = pForm->GetHeight();

			bool bNeedScale = (0 == m_nScaleType
				|| (2 == m_nScaleType && (dOriginH > dH || dOriginW > dW))
				|| (3 == m_nScaleType && dOriginH < dH && dOriginW < dW));

			double dBorderSize = m_oBorder.dWidth;
			if (m_oBorder.nType == 1 || m_oBorder.nType == 3)
				dBorderSize *= 2;

			double dDstW = dOriginW;
			double dDstH = dOriginH;
			double dDstX = 0;
			double dDstY = 0;

			if (m_nTP == 2)
			{
				dH -= (dLineH + dBorderSize);
				dDstY += (dLineH + dBorderSize);
			}
			if (m_nTP == 3)
				dH -= (dLineH + dBorderSize);
			if (m_nTP == 4)
				dW -= (dLineW + dBorderSize);
			if (m_nTP == 5)
			{
				dW -= (dLineW + dBorderSize);
				dDstX += (dLineW + dBorderSize);
			}

			if (m_bRespectBorders)
			{
				dDstX += 2 * dBorderSize;
				dDstY += 2 * dBorderSize;
				dH -= 4 * dBorderSize;
				dW -= 4 * dBorderSize;
			}

			if (bNeedScale)
			{
				if (!m_bConstantProportions)
				{
					dDstH = dH;
					dDstW = dW;
				}
				else
				{
					double dScaleKoef = fmin(dW / dOriginW, dH / dOriginH);
					dDstW = dScaleKoef * dOriginW;
					dDstH = dScaleKoef * dOriginH;
				}
			}

			dDstX += (dW - dDstW) * m_dShiftX;
			dDstY += (dH - dDstH) * m_dShiftY;

			pAppearance->DrawPictureInline(pForm->GetName().c_str(), dDstX, dDstY, dDstW / dOriginW, dDstH / dOriginH, m_bRespectBorders);

			CheckMK();
			std::string sAP = nAP == 0 ? "I" : (nAP == 1 ? "RI" : "IX");
			m_pMK->Add(sAP, pForm);
		}

		if (pCodes)
		{
			pAppearance->StartText(m_pFont, m_dFontSize);
			pAppearance->DrawTextLine(dX, dY, pCodes, unCount, ppFonts, NULL);
			pAppearance->EndText();
		}

		pAppearance->EndDraw();
	}
	//----------------------------------------------------------------------------------------
	// CCheckBoxWidget
	//----------------------------------------------------------------------------------------
	CCheckBoxWidget::CCheckBoxWidget(CXref* pXref) : CWidgetAnnotation(pXref, AnnotWidget)
	{
		m_nSubtype = WidgetRadiobutton;
	}
	void CCheckBoxWidget::SetV(const std::wstring& wsV)
	{
		std::string sV = U_TO_UTF8(wsV);
		CDictObject* pOwner = GetObjOwnValue("V");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("V", new CStringObject(sV.c_str(), true));
	}
	std::wstring CCheckBoxWidget::SetStyle(BYTE nStyle)
	{
		CheckMK();

		std::string sValue;
		switch (nStyle)
		{
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
		default:
		case 0:
		{ sValue = "4"; break; }
		}

		m_pMK->Add("CA", new CStringObject(sValue.c_str()));

		return UTF8_TO_U(sValue);
	}
	void CCheckBoxWidget::SetAP_N_Yes(const std::wstring& wsAP_N_Yes)
	{
		std::string sValue = U_TO_UTF8(wsAP_N_Yes);
		m_sAP_N_Yes = sValue;
	}
	void CCheckBoxWidget::SwitchAP(const std::string& sV)
	{
		Add("AS", sV == m_sAP_N_Yes ? sV.c_str() : "Off");
	}
	void CCheckBoxWidget::SetFlag(const int& nFlag)
	{
		int nFlags = nFlag;
		if (m_nSubtype == WidgetRadiobutton)
			nFlags |= (1 << 15);
		CWidgetAnnotation::SetFlag(nFlags);
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
			CDictObject* pOwner = GetObjOwnValue("MaxLen");
			if (!pOwner)
				pOwner = this;
			pOwner->Add("MaxLen", nMaxLen);
		}
	}
	void CTextWidget::SetV(const std::wstring& wsV)
	{
		m_sV = U_TO_UTF8(wsV);
		CDictObject* pOwner = GetObjOwnValue("V");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("V", new CStringObject(m_sV.c_str(), true));
	}
	void CTextWidget::SetRV(const std::wstring& wsRV)
	{
		std::string sValue = U_TO_UTF8(wsRV);
		CDictObject* pOwner = GetObjOwnValue("RV");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("RV", new CStringObject(sValue.c_str(), true));
	}
	bool CTextWidget::IsCombFlag()
	{
		int nFlags = ((CNumberObject*)GetObjValue("Ff"))->Get();
		return (nFlags & (1 << 24));
	}
	bool CTextWidget::IsMultiLine()
	{
		int nFlags = ((CNumberObject*)GetObjValue("Ff"))->Get();
		return (nFlags & (1 << 12));
	}
	unsigned int CTextWidget::GetMaxLen()
	{
		CNumberObject* oMaxLen = (CNumberObject*)GetObjValue("MaxLen");
		return oMaxLen ? oMaxLen->Get() : 0;
	}
	//----------------------------------------------------------------------------------------
	// CChoiceWidget
	//----------------------------------------------------------------------------------------
	CChoiceWidget::CChoiceWidget(CXref* pXref) : CWidgetAnnotation(pXref, AnnotWidget)
	{
		m_dHeight = 0;
		m_nTI = -1;
	}
	void CChoiceWidget::SetFlag(const int& nFlag)
	{
		int nFlags = nFlag;
		if (m_nSubtype == WidgetCombobox)
			nFlags |= (1 << 17);

		CDictObject* pOwner = GetObjOwnValue("Ff");
		if (!pOwner)
			pOwner = this;
		else
		{
			int nFlags2 = ((CNumberObject*)(pOwner->Get("Ff")))->Get();
			if (nFlags2 & (1 << 19))
				nFlags |= (1 << 19);
		}
		pOwner->Add("Ff", nFlags);
	}
	void CChoiceWidget::SetTI(const int& nTI)
	{
		m_nTI = nTI;
		CDictObject* pOwner = GetObjOwnValue("TI");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("TI", nTI);
	}
	void CChoiceWidget::SetV(const std::wstring& wsV)
	{
		std::string sValue = U_TO_UTF8(wsV);
		CDictObject* pOwner = GetObjOwnValue("V");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("V", new CStringObject(sValue.c_str(), true));
	}
	void CChoiceWidget::SetI(const std::vector<int>& arrI)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		CDictObject* pOwner = GetObjOwnValue("I");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("I", pArray);

		for (int i = 0; i < arrI.size(); ++i)
			pArray->Add(arrI[i]);
	}
	void CChoiceWidget::SetV(const std::vector<std::wstring>& arrV)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		CDictObject* pOwner = GetObjOwnValue("V");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("V", pArray);

		for (int i = 0; i < arrV.size(); ++i)
			pArray->Add(new PdfWriter::CStringObject(U_TO_UTF8(arrV[i]).c_str(), true));
	}
	void CChoiceWidget::SetOpt(const std::vector< std::pair<std::wstring, std::wstring> >& arrOpt)
	{
		m_arrOpt = arrOpt;
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		CDictObject* pOwner = GetObjOwnValue("Opt");
		if (!pOwner)
			pOwner = this;
		pOwner->Add("Opt", pArray);

		for (const std::pair<std::wstring, std::wstring>& PV : arrOpt)
		{
			if (PV.first.empty())
			{
				std::string sValue = U_TO_UTF8(PV.second);
				pArray->Add(new CStringObject(sValue.c_str(), true));
			}
			else
			{
				CArrayObject* pArray2 = new CArrayObject();
				pArray->Add(pArray2);

				std::string sValue = U_TO_UTF8(PV.first);
				pArray2->Add(new CStringObject(sValue.c_str(), true));

				sValue = U_TO_UTF8(PV.second);
				pArray2->Add(new CStringObject(sValue.c_str(), true));
			}
		}
	}
	std::wstring CChoiceWidget::GetValue(const std::wstring& wsExportV)
	{
		for (int i = 0; i < m_arrOpt.size(); ++i)
		{
			if (( m_arrOpt[i].first.empty() && m_arrOpt[i].second == wsExportV) ||
				(!m_arrOpt[i].first.empty() && m_arrOpt[i].first  == wsExportV))
				return m_arrOpt[i].second;
		}
		return L"";
	}
	std::wstring CChoiceWidget::SetListBoxIndex(const std::vector<std::wstring>& arrV)
	{
		std::wstring sRes;
		int i = 0;
		if (m_nTI < 0)
		{
			// Ищем верхний элемент отрисовки
			for (; i < m_arrOpt.size(); ++i)
			{
				if (( m_arrOpt[i].first.empty() && m_arrOpt[i].second == arrV.front()) ||
					(!m_arrOpt[i].first.empty() && m_arrOpt[i].first  == arrV.front()))
				{
					m_nTI = i;
					break;
				}
			}
		}
		else
			i = m_nTI;

		if (m_nTI < 0)
			return L"";

		for (; i < m_arrOpt.size(); ++i)
			sRes += (m_arrOpt[i].second + L"\n");
		i = m_nTI;

		for (const std::wstring& sV : arrV)
		{
			for (int j = i; j < m_arrOpt.size(); ++j)
			{
				if (( m_arrOpt[j].first.empty() && m_arrOpt[j].second == sV) ||
					(!m_arrOpt[j].first.empty() && m_arrOpt[j].first  == sV))
				{
					m_arrIndex.push_back(j - m_nTI);
					i = j + 1;
					break;
				}
			}
		}

		if (!sRes.empty())
		{
			sRes.pop_back();
			return sRes;
		}
		return L"";
	}
	//----------------------------------------------------------------------------------------
	// CSignatureWidget
	//----------------------------------------------------------------------------------------
	CSignatureWidget::CSignatureWidget(CXref* pXref) : CWidgetAnnotation(pXref, AnnotWidget)
	{
	}
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
	void CActionGoTo::SetDestination(CArrayObject* pDest)
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
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("T", pArray);

		for (const std::wstring& A : arrT)
		{
			std::string sValue = U_TO_UTF8(A);
			pArray->Add(new CStringObject(sValue.c_str()));
		}
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
