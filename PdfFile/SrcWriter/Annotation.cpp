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
		case ELineEndType::Square:       sValue = "Square";       break;
		case ELineEndType::Circle:       sValue = "Circle";       break;
		case ELineEndType::Diamond:      sValue = "Diamond";      break;
		case ELineEndType::OpenArrow:    sValue = "OpenArrow";    break;
		case ELineEndType::ClosedArrow:  sValue = "ClosedArrow";  break;
		case ELineEndType::None:         sValue = "None";         break;
		case ELineEndType::Butt:         sValue = "Butt";         break;
		case ELineEndType::ROpenArrow:   sValue = "ROpenArrow";   break;
		case ELineEndType::RClosedArrow: sValue = "RClosedArrow"; break;
		case ELineEndType::Slash:        sValue = "Slash";        break;
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
		pArray->Add(dRD4);
		pArray->Add(dRD3);
		pArray->Add(dRD2);
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
		return sDA;
	}
	std::string GetColor(CArrayObject* pArr, bool bCAPS, float dDiff = 0)
	{
		std::string sDA;
		if (!pArr)
			return sDA;

		int nSize = pArr->GetCount();
		for (int i = 0; i < nSize; ++i)
		{
			CObjectBase* pColor = pArr->Get(i);
			float fColor = pColor->GetType() == object_type_REAL ? ((CRealObject*)pColor)->Get() : ((CNumberObject*)pColor)->Get();

			sDA.append(std::to_string(fColor + dDiff));
			sDA.append(" ");
		}

		if (nSize == 3)
			sDA.append(bCAPS ? "RG" : "rg");
		else if (nSize == 4)
			sDA.append(bCAPS ? "K" : "k");
		else if (nSize == 1)
			sDA.append(bCAPS ? "G" : "g");

		return sDA;
	}

	//----------------------------------------------------------------------------------------
	// CAnnotation
	//----------------------------------------------------------------------------------------
	CAnnotation::CAnnotation(CXref* pXref, EAnnotType eType)
	{
		m_pAppearance = NULL;
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
			m_oRect.fTop = oRect.fBottom;
			m_oRect.fBottom = oRect.fTop;
		}

		pArray->Add(m_oRect.fLeft);
		pArray->Add(m_oRect.fBottom);
		pArray->Add(m_oRect.fRight);
		pArray->Add(m_oRect.fTop);
	}
	void CAnnotation::SetBorder(BYTE nType, double dWidth, const std::vector<double>& arrDash)
	{
		CDictObject* pBorderStyleDict = new CDictObject();
		if (!pBorderStyleDict)
			return;

		Add("BS", pBorderStyleDict);

		pBorderStyleDict->Add("Type", "Border");
		pBorderStyleDict->Add("W", dWidth);

		EBorderType eBT = EBorderType(nType);
		if (EBorderType::Dashed == eBT)
			AddToVectorD(pBorderStyleDict, "D", arrDash);

		switch (eBT)
		{
		case EBorderType::Solid:     pBorderStyleDict->Add("S", "S"); break;
		case EBorderType::Dashed:    pBorderStyleDict->Add("S", "D"); break;
		case EBorderType::Beveled:   pBorderStyleDict->Add("S", "B"); break;
		case EBorderType::Inset:     pBorderStyleDict->Add("S", "I"); break;
		case EBorderType::Underline: pBorderStyleDict->Add("S", "U"); break;
		}

		m_oBorder.bHave = true;
		m_oBorder.nType = EBorderType(nType);
		m_oBorder.dWidth = dWidth;
		m_oBorder.arrDash = arrDash;
	}
	void CAnnotation::SetAnnotFlag(const int& nAnnotFlag)
	{
		Add("F", nAnnotFlag);
	}
	void CAnnotation::SetPage(CPage* pPage, double dW, double dH, double dX)
	{
		Add("P", pPage);
		m_dPageWidth  = dW;
		m_dPageH = dH;
		m_dPageX = dX;
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
	void CAnnotation::SetOUserID(const std::wstring& wsLM)
	{
		std::string sValue = U_TO_UTF8(wsLM);
		Add("OUserID", new CStringObject(sValue.c_str()));
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
		for (double dDash : m_oBorder.arrDash)
		{
			sRes.append(" ");
			sRes.append(std::to_string(dDash));
		}
		sRes.append(" ] 0 d\012");
		return sRes;
	}
	CAnnotAppearanceObject* CAnnotation::StartAP()
	{
		m_pAppearance = new CAnnotAppearance(m_pXref, this);
		if (!m_pAppearance)
			return NULL;
		Add("AP", m_pAppearance);
		return m_pAppearance->GetNormal();
	}
	void CAnnotation::APFromFakePage(CPage* pFakePage)
	{
		if (!m_pAppearance)
			return;
		CAnnotAppearanceObject* pNormal = m_pAppearance->GetNormal();

		pNormal->AddBBox(GetRect().fLeft, GetRect().fBottom, GetRect().fRight, GetRect().fTop);
		pNormal->AddMatrix(1, 0, 0, 1, -GetRect().fLeft, -GetRect().fBottom);
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
		if (!Get("T"))
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
		std::string sColor = GetColor(dynamic_cast<CArrayObject*>(Get("C")), false);

		CAnnotAppearance* pAP = new CAnnotAppearance(m_pXref, this);
		Add("AP", pAP);
		CAnnotAppearanceObject* pN = pAP->GetNormal();
		CAnnotAppearanceObject* pR = pAP->GetRollover();

		switch (m_nName)
		{
		case 0:
		{
			pN->AddBBox(0, 0, 19, 19);
			pN->DrawTextCheck(sColor);
			pR->AddBBox(0, 0, 19, 19);
			pR->DrawTextCheck(sColor);
			break;
		}
		case 1:
		{
			pN->AddBBox(0, 0, 20, 20);
			pN->DrawTextCheckmark();
			pR->AddBBox(0, 0, 20, 20);
			pR->DrawTextCheckmark();
			break;
		}
		case 2:
		{
			pN->AddBBox(0, 0, 20, 20);
			pN->DrawTextCircle(sColor);
			pR->AddBBox(0, 0, 20, 20);
			pR->DrawTextCircle(sColor);
			break;
		}
		case 3:
		{
			pN->AddBBox(0, 0, 24, 24);
			pN->DrawTextCommentN(sColor);
			pR->AddBBox(0, 0, 24, 24);
			pR->DrawTextCommentR(sColor);
			break;
		}
		case 4:
		{
			pN->AddBBox(0, 0, 19, 19);
			pN->DrawTextCross(sColor);
			pR->AddBBox(0, 0, 19, 19);
			pR->DrawTextCross(sColor);
			break;
		}
		case 5:
		{
			pN->AddBBox(0, 0, 20, 20);
			pN->DrawTextCrossHairs(sColor);
			pR->AddBBox(0, 0, 20, 20);
			pR->DrawTextCrossHairs(sColor);
			break;
		}
		case 6:
		{
			pN->AddBBox(0, 0, 20, 20);
			pN->DrawTextHelp(sColor);
			pR->AddBBox(0, 0, 20, 20);
			pR->DrawTextHelp(sColor);
			break;
		}
		case 7:
		{
			pN->AddBBox(0, 0, 17, 20);
			pN->DrawTextInsert(sColor);
			pR->AddBBox(0, 0, 17, 20);
			pR->DrawTextInsert(sColor);
			break;
		}
		case 8:
		{
			pN->AddBBox(0, 0, 13, 18);
			pN->DrawTextKey(sColor);
			pR->AddBBox(0, 0, 13, 18);
			pR->DrawTextKey(sColor);
			break;
		}
		case 9:
		{
			pN->AddBBox(0, 0, 13, 20);
			pN->DrawTextNewParagraph(sColor);
			pR->AddBBox(0, 0, 13, 20);
			pR->DrawTextNewParagraph(sColor);
			break;
		}
		case 11:
		{
			pN->AddBBox(0, 0, 20, 20);
			pN->DrawTextParagraph(sColor);
			pR->AddBBox(0, 0, 20, 20);
			pR->DrawTextParagraph(sColor);
			break;
		}
		case 12:
		{
			pN->AddBBox(0, 0, 20, 20);
			pN->DrawTextRightArrow(sColor);
			pR->AddBBox(0, 0, 20, 20);
			pR->DrawTextRightArrow(sColor);
			break;
		}
		case 13:
		{
			pN->AddBBox(0, 0, 20, 17);
			pN->DrawTextRightPointer(sColor);
			pR->AddBBox(0, 0, 20, 17);
			pR->DrawTextRightPointer(sColor);
			break;
		}
		case 14:
		{
			pN->AddBBox(0, 0, 20, 19);
			pN->DrawTextStar(sColor);
			pR->AddBBox(0, 0, 20, 19);
			pR->DrawTextStar(sColor);
			break;
		}
		case 15:
		{
			pN->AddBBox(0, 0, 17, 20);
			pN->DrawTextUpArrow(sColor);
			pR->AddBBox(0, 0, 17, 20);
			pR->DrawTextUpArrow(sColor);
			break;
		}
		case 16:
		{
			pN->AddBBox(0, 0, 17, 17);
			pN->DrawTextUpLeftArrow(sColor);
			pR->AddBBox(0, 0, 17, 17);
			pR->DrawTextUpLeftArrow(sColor);
			break;
		}
		case 10:
		default:
		{
			pN->AddBBox(0, 0, 18, 20);
			pN->DrawTextNote(sColor);
			pR->AddBBox(0, 0, 18, 20);
			pR->DrawTextNote(sColor);
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
				pArrayI->Add(i % 2 == 0 ? (arrInk[i] + m_dPageX) : (m_dPageH - arrInk[i]));
		}
	}
	//----------------------------------------------------------------------------------------
	// CLineAnnotation
	//----------------------------------------------------------------------------------------
	CLineAnnotation::CLineAnnotation(CXref* pXref) : CMarkupAnnotation(pXref, AnnotLine), dL{0.0, 0.0, 0.0, 0.0}
	{
		m_nLE1 = ELineEndType::None;
		m_nLE2 = ELineEndType::None;
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
		case ELineIntentType::LineDimension: sValue = "LineDimension"; break;
		case ELineIntentType::LineArrow:     sValue = "LineArrow";     break;
		default: return;
		}

		Add("IT", sValue.c_str());
	}
	void CLineAnnotation::SetCP(BYTE nCP)
	{
		if (!Get("Open"))
			return;

		std::string sValue;
		switch (nCP)
		{
		case ECaptionPositioning::Inline: sValue = "Inline"; break;
		case ECaptionPositioning::Top:    sValue = "Top";    break;
		default: return;
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

		m_nLE1 = ELineEndType(nLE1);
		m_nLE2 = ELineEndType(nLE2);
	}
	void CLineAnnotation::SetL(const double& dL1, const double& dL2, const double& dL3, const double& dL4)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("L", pArray);

		dL[0] = dL1 + m_dPageX;
		dL[1] = m_dPageH - dL2;
		dL[2] = dL3 + m_dPageX;
		dL[3] = m_dPageH - dL4;

		pArray->Add(dL[0]);
		pArray->Add(dL[1]);
		pArray->Add(dL[2]);
		pArray->Add(dL[3]);
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
	void AdjustLineEndpoint(ELineEndType nType, double x, double y, double dx, double dy, double w, double& tx, double& ty)
	{
		tx = x;
		ty = y;

		switch (nType)
		{
		case ELineEndType::ClosedArrow:
		case ELineEndType::OpenArrow:
		case ELineEndType::Diamond:
		{
			tx += w * dx;
			if ((dx > 0.001 && dy > 0) || (dx < -0.001 && dy < 0))
				ty += w * dy;
			break;
		}
		case ELineEndType::Square:
		case ELineEndType::Circle:
		{
			if ((dx > -0.02 && dy < 0.02) || (dx < 0.02 && dy > -0.02))
				tx += w * dx;
			break;
		}
		case ELineEndType::Slash:
		case ELineEndType::Butt:
		case ELineEndType::ROpenArrow:
		case ELineEndType::RClosedArrow:
		case ELineEndType::None:
		default:
			break;
		}
	}
	void SreamWriteXYMove(CStream* pStream, double x, double y)
	{
		pStream->WriteReal(x);
		pStream->WriteChar(' ');
		pStream->WriteReal(y);
		pStream->WriteStr(" m\012");
	}
	void SreamWriteXYLine(CStream* pStream, double x, double y)
	{
		pStream->WriteReal(x);
		pStream->WriteChar(' ');
		pStream->WriteReal(y);
		pStream->WriteStr(" l\012");
	}
	void SreamWriteXYCurve(CStream* pStream, double x1, double y1, double x2, double y2, double x3, double y3)
	{
		pStream->WriteReal(x1);
		pStream->WriteChar(' ');
		pStream->WriteReal(y1);
		pStream->WriteChar(' ');
		pStream->WriteReal(x2);
		pStream->WriteChar(' ');
		pStream->WriteReal(y2);
		pStream->WriteChar(' ');
		pStream->WriteReal(x3);
		pStream->WriteChar(' ');
		pStream->WriteReal(y3);
		pStream->WriteStr(" c\012");
	}
	void StreamWriteRect(CStream* pStream, double x1, double y1, double x2, double y2)
	{
		pStream->WriteReal(x1);
		pStream->WriteChar(' ');
		pStream->WriteReal(y1);
		pStream->WriteChar(' ');
		pStream->WriteReal(x2);
		pStream->WriteChar(' ');
		pStream->WriteReal(y2);
		pStream->WriteStr(" re\012");
	}
	void SreamWriteCircle(CStream* pStream, double cx, double cy, double r)
	{
		double bezierCircle = 0.55228475 * r;
		SreamWriteXYMove(pStream, cx + r, cy);
		SreamWriteXYCurve(pStream, cx + r, cy + bezierCircle, cx + bezierCircle, cy + r, cx, cy + r);
		SreamWriteXYCurve(pStream, cx - bezierCircle, cy + r, cx - r, cy + bezierCircle, cx - r, cy);
		SreamWriteXYCurve(pStream, cx - r, cy - bezierCircle, cx - bezierCircle, cy - r, cx, cy - r);
		SreamWriteXYCurve(pStream, cx + bezierCircle, cy - r, cx + r, cy - bezierCircle, cx + r, cy);
	}
	void DrawArrow(CStream* pStream, ELineEndType nType, double x, double y, double dx, double dy, double w)
	{
		double lineEndSize1 = 3, pi = 3.14159265358979323846;
		switch (nType)
		{
		case ELineEndType::Butt:
		{
			w *= lineEndSize1;
			SreamWriteXYMove(pStream, x + w * dy, y - w * dx);
			SreamWriteXYLine(pStream, x - w * dy, y + w * dx);
			pStream->WriteStr("S\012");
			break;
		}
		case ELineEndType::Circle:
		{
			SreamWriteCircle(pStream, x, y, w * lineEndSize1);
			pStream->WriteStr("h\012B\012");
			break;
		}
		case ELineEndType::Diamond:
		{
			w *= lineEndSize1;
			SreamWriteXYMove(pStream, x - w, y);
			SreamWriteXYLine(pStream, x, y + w);
			SreamWriteXYLine(pStream, x + w, y);
			SreamWriteXYLine(pStream, x, y - w);
			pStream->WriteStr("b\012");
			break;
		}
		case ELineEndType::OpenArrow:
		case ELineEndType::ClosedArrow:
		{
			w *= lineEndSize1 * lineEndSize1;
			double d32 = pi * 32.0 / 180.0;
			double d28 = pi * 28.0 / 180.0;
			if ((dx > 0.001 && dy < 0) || (dx < -0.001 && dy > 0))
			{
				SreamWriteXYMove(pStream, x + w * cos(d32) * dx + w * sin(d32) * dy, y + w * cos(d32) * dy - w * sin(d32) * dx);
				SreamWriteXYLine(pStream, x, y);
				SreamWriteXYLine(pStream, x + w * cos(d28) * dx - w * sin(d28) * dy, y + w * cos(d28) * dy + w * sin(d28) * dx);
			}
			else
			{
				double dCos = w * cos(pi / 6.0);
				double dSin = w * sin(pi / 6.0);

				SreamWriteXYMove(pStream, x + dCos * dx + dSin * dy, y + dCos * dy - dSin * dx);
				SreamWriteXYLine(pStream, x, y);
				SreamWriteXYLine(pStream, x + dCos * dx - dSin * dy, y + dCos * dy + dSin * dx);
			}
			pStream->WriteStr(nType == ELineEndType::OpenArrow ? "S\012" : "b\012");
			break;
		}
		case ELineEndType::ROpenArrow:
		case ELineEndType::RClosedArrow:
		{
			x -= cos(pi / 18.0) * dx * w;
			y -= cos(pi / 18.0) * dy * w;
			w *= lineEndSize1 * lineEndSize1;
			double dCos = w * cos(pi / 6.0);
			double dSin = w * sin(pi / 6.0);
			SreamWriteXYMove(pStream, x - dCos * dx + dSin * dy, y - dCos * dy - dSin * dx);
			SreamWriteXYLine(pStream, x, y);
			SreamWriteXYLine(pStream, x - dCos * dx - dSin * dy, y - dCos * dy + dSin * dx);
			pStream->WriteStr(nType == ELineEndType::ROpenArrow ? "S\012" : "b\012");
			break;
		}
		case ELineEndType::Slash:
		{
			w *= lineEndSize1 * lineEndSize1;
			double dCos = w * cos(pi / 6.0);
			double dSin = w * sin(pi / 6.0);
			SreamWriteXYMove(pStream, x + dCos * dy - dSin * dx, y - dCos * dx - dSin * dy);
			SreamWriteXYLine(pStream, x - dCos * dy + dSin * dx, y + dCos * dx + dSin * dy);
			pStream->WriteStr("S\012");
			break;
		}
		case ELineEndType::Square:
		{
			w *= lineEndSize1;
			pStream->WriteReal(x - w);
			pStream->WriteChar(' ');
			pStream->WriteReal(y - w);
			pStream->WriteChar(' ');
			pStream->WriteReal(w * 2);
			pStream->WriteChar(' ');
			pStream->WriteReal(w * 2);
			pStream->WriteStr(" re\012");
			pStream->WriteStr("B\012");
			break;
		}
		case ELineEndType::None:
		default:
		{
			break;
		}
		}
	}
	void DrawLineArrow(CStream* pStream, double dBorderSize, double x1, double y1, double x2, double y2, ELineEndType nLE1, ELineEndType nLE2, double dLL = 0, double dLLO = 0, double dLLE = 0)
	{
		double dDX = x2 - x1;
		double dDY = y2 - y1;
		double dLen = sqrt(dDX * dDX + dDY * dDY);
		if (dLen > 0)
		{
			dDX /= dLen;
			dDY /= dLen;
		}

		double lx1, ly1, lx2, ly2;
		double ax1, ay1, ax2, ay2;
		double bx1, by1, bx2, by2;
		if (dLL != 0)
		{
			ax1 = x1 + dLLO * dDY;
			ay1 = y1 - dLLO * dDX;
			lx1 = ax1 + dLL * dDY;
			ly1 = ay1 - dLL * dDX;
			bx1 = lx1 + dLLE * dDY;
			by1 = ly1 - dLLE * dDX;
			ax2 = x2 + dLLO * dDY;
			ay2 = y2 - dLLO * dDX;
			lx2 = ax2 + dLL * dDY;
			ly2 = ay2 - dLL * dDX;
			bx2 = lx2 + dLLE * dDY;
			by2 = ly2 - dLLE * dDX;
		}
		else
		{
			lx1 = x1;
			ly1 = y1;
			lx2 = x2;
			ly2 = y2;
			ax1 = ay1 = ax2 = ay2 = 0;
			bx1 = by1 = bx2 = by2 = 0;
		  }

		double tx1, ty1, tx2, ty2;
		AdjustLineEndpoint(nLE1, lx1, ly1,  dDX,  dDY, dBorderSize, tx1, ty1);
		AdjustLineEndpoint(nLE2, lx2, ly2, -dDX, -dDY, dBorderSize, tx2, ty2);

		if (dLL)
		{
			SreamWriteXYMove(pStream, ax1, ay1);
			SreamWriteXYLine(pStream, bx1, by1);

			SreamWriteXYMove(pStream, ax2, ay2);
			SreamWriteXYLine(pStream, bx2, by2);
		}

		SreamWriteXYMove(pStream, tx1, ty1);
		SreamWriteXYLine(pStream, tx2, ty2);
		pStream->WriteStr("S\012");

		DrawArrow(pStream, nLE1, tx1, ty1,  dDX,  dDY, dBorderSize);
		DrawArrow(pStream, nLE2, tx2, ty2, -dDX, -dDY, dBorderSize);
	}
	void CLineAnnotation::SetAP()
	{
		CAnnotAppearance* pAppearance = new CAnnotAppearance(m_pXref, this);
		Add("AP", pAppearance);
		CAnnotAppearanceObject* pNormal = pAppearance->GetNormal();
		CStream* pStream = pNormal->GetStream();

		pNormal->AddBBox(GetRect().fLeft, GetRect().fBottom, GetRect().fRight, GetRect().fTop);
		pNormal->AddMatrix(1, 0, 0, 1, -GetRect().fLeft, -GetRect().fBottom);

		if (GetBorderType() == EBorderType::Dashed)
			pStream->WriteStr(GetBorderDash().c_str());

		double dBorderSize = GetBorderWidth();
		pStream->WriteReal(dBorderSize);
		pStream->WriteStr(" w\012");

		CObjectBase* pObj = Get("IC");
		if (pObj && pObj->GetType() == object_type_ARRAY)
		{
			pStream->WriteStr(GetColor(dynamic_cast<CArrayObject*>(pObj), false).c_str());
			pStream->WriteStr("\012");
		}

		pStream->WriteStr(GetColor(dynamic_cast<CArrayObject*>(Get("C")), true).c_str());
		pStream->WriteStr("\012");

		pObj = Get("CA");
		if (pObj && pObj->GetType() == object_type_REAL)
		{
			float dAlpha = ((CRealObject*)pObj)->Get();
			if (dAlpha != 1)
			{
				CExtGrState* pExtGrState = m_pDocument->GetExtGState(dAlpha, dAlpha);
				const char* sExtGrStateName =  m_pDocument->GetFieldsResources()->GetExtGrStateName(pExtGrState);
				if (sExtGrStateName)
				{
					pStream->WriteEscapeName(sExtGrStateName);
					pStream->WriteStr(" gs\012");
				}
			}
		}

		double dLL = 0, dLLE = 0, dLLO = 0;
		pObj = Get("LL");
		if (pObj && pObj->GetType() == object_type_REAL)
			dLL = ((CRealObject*)pObj)->Get();
		pObj = Get("LLE");
		if (pObj && pObj->GetType() == object_type_REAL)
			dLLE = ((CRealObject*)pObj)->Get();
		pObj = Get("LLO");
		if (pObj && pObj->GetType() == object_type_REAL)
			dLLO = ((CRealObject*)pObj)->Get();

		DrawLineArrow(pStream, dBorderSize, dL[0], dL[1], dL[2], dL[3], m_nLE1, m_nLE2, dLL, dLLE, dLLO);
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
	void CFreeTextAnnotation::SetDA(CFontDict* pFont, const double& dFontSize, const std::vector<double>& arrC)
	{
		CResourcesDict* pFieldsResources = m_pDocument->GetFieldsResources();
		const char* sFontName = pFieldsResources->GetFontName(pFont);

		std::vector<double> _arrC = arrC;
		if (arrC.empty())
			_arrC = {0};
		std::string sDA = GetColor(_arrC, false);
		if (sFontName)
		{
			sDA.append(" /");
			sDA.append(sFontName);
		}

		sDA.append(" ");
		sDA.append(std::to_string(dFontSize));
		sDA.append(" Tf");

		Add("DA", new CStringObject(sDA.c_str()));
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
	void CFreeTextAnnotation::SetRotate(int nRotate)
	{
		Add("Rotate", nRotate);
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
			pArray->Add(i % 2 == 0 ? (arrCL[i] + m_dPageX) : (m_dPageH - arrCL[i]));
	}
	void CFreeTextAnnotation::SetIC(const std::vector<double>& arrIC)
	{
		SetC(arrIC);
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
			pArray->Add(i % 2 == 0 ? (arrQuadPoints[i] + m_dPageX) : (m_dPageH - arrQuadPoints[i]));
	}
	void CTextMarkupAnnotation::SetAP(const std::vector<double>& arrQuadPoints, const double& dCA)
	{
		CAnnotAppearance* pAP = new CAnnotAppearance(m_pXref, this);
		Add("AP", pAP);
		CAnnotAppearanceObject* pN = pAP->GetNormal();
		CStream* pStream = pN->GetStream();

		pN->AddBBox(GetRect().fLeft, GetRect().fBottom, GetRect().fRight, GetRect().fTop);
		pN->AddMatrix(1, 0, 0, 1, -GetRect().fLeft, -GetRect().fBottom);

		CExtGrState* pExtGrState = m_pDocument->GetExtGState(dCA, dCA, m_nSubtype == AnnotHighLight ? blendmode_Multiply : blendmode_Unknown);
		const char* sExtGrStateName =  m_pDocument->GetFieldsResources()->GetExtGrStateName(pExtGrState);
		if (sExtGrStateName)
		{
			pStream->WriteEscapeName(sExtGrStateName);
			pStream->WriteStr(" gs\012");
		}
		std::string sColor = GetColor(dynamic_cast<CArrayObject*>(Get("C")), m_nSubtype != AnnotHighLight);
		pStream->WriteStr(sColor.c_str());
		pStream->WriteChar('\012');

		switch (m_nSubtype)
		{
		case AnnotHighLight:
		{
			pStream->WriteStr("1 w\012");

			for (int i = 0; i < arrQuadPoints.size(); i += 8)
			{
				pStream->WriteReal(arrQuadPoints[i] + m_dPageX);
				pStream->WriteChar(' ');
				pStream->WriteReal(m_dPageH - arrQuadPoints[i + 1]);
				pStream->WriteStr(" m\012");

				pStream->WriteReal(arrQuadPoints[i + 2] + m_dPageX);
				pStream->WriteChar(' ');
				pStream->WriteReal(m_dPageH - arrQuadPoints[i + 3]);
				pStream->WriteStr(" l\012");

				pStream->WriteReal(arrQuadPoints[i + 6] + m_dPageX);
				pStream->WriteChar(' ');
				pStream->WriteReal(m_dPageH - arrQuadPoints[i + 7]);
				pStream->WriteStr(" l\012");

				pStream->WriteReal(arrQuadPoints[i + 4] + m_dPageX);
				pStream->WriteChar(' ');
				pStream->WriteReal(m_dPageH - arrQuadPoints[i + 5]);
				pStream->WriteStr(" l\012f\012");
			}
			break;
		}
		case AnnotSquiggly:
		case AnnotUnderline:
		{
			for (int i = 0; i < arrQuadPoints.size(); i += 8)
			{
				double dX = arrQuadPoints[i + 2] - arrQuadPoints[i];
				double dY = arrQuadPoints[i + 3] - arrQuadPoints[i + 1];
				double dAngle = atan2(dY, dX);
				double dHeight = sqrt(pow(arrQuadPoints[i] - arrQuadPoints[i + 4], 2) + pow(arrQuadPoints[i + 1] - arrQuadPoints[i + 5], 2));
				double dLineWidth = std::max(0.5, dHeight * 0.075);
				double dIndentX = sin(dAngle) * dLineWidth * 1.9;
				double dIndentY = cos(dAngle) * dLineWidth * 1.9;

				pStream->WriteReal(dLineWidth);
				pStream->WriteStr(" w\012");

				pStream->WriteReal(arrQuadPoints[i + 4] + m_dPageX + dIndentX);
				pStream->WriteChar(' ');
				pStream->WriteReal(m_dPageH - arrQuadPoints[i + 5] + dIndentY);
				pStream->WriteStr(" m\012");

				pStream->WriteReal(arrQuadPoints[i + 6] + m_dPageX + dIndentX);
				pStream->WriteChar(' ');
				pStream->WriteReal(m_dPageH - arrQuadPoints[i + 7] + dIndentY);
				pStream->WriteStr(" l\012S\012");
			}
			break;
		}
		case AnnotStrikeOut:
		default:
		{
			for (int i = 0; i < arrQuadPoints.size(); i += 8)
			{
				double dX1 = arrQuadPoints[i] + (arrQuadPoints[i + 4] - arrQuadPoints[i]) / 2.0;
				double dY1 = arrQuadPoints[i + 1] + (arrQuadPoints[i + 5] - arrQuadPoints[i + 1]) / 2.0;
				double dX2 = arrQuadPoints[i + 2] + (arrQuadPoints[i + 6] - arrQuadPoints[i + 2]) / 2.0;
				double dY2 = arrQuadPoints[i + 3] + (arrQuadPoints[i + 7] - arrQuadPoints[i + 3]) / 2.0;
				double dHeight = sqrt(pow(arrQuadPoints[i] - arrQuadPoints[i + 4], 2) + pow(arrQuadPoints[i + 1] - arrQuadPoints[i + 5], 2));
				double dLineWidth = std::max(0.5, dHeight * 0.075);

				pStream->WriteReal(dLineWidth);
				pStream->WriteStr(" w\012");

				pStream->WriteReal(dX1 + m_dPageX);
				pStream->WriteChar(' ');
				pStream->WriteReal(m_dPageH - dY1);
				pStream->WriteStr(" m\012");

				pStream->WriteReal(dX2 + m_dPageX);
				pStream->WriteChar(' ');
				pStream->WriteReal(m_dPageH - dY2);
				pStream->WriteStr(" l\012S\012");
			}
			break;
		}
		}
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
			pArray->Add(i % 2 == 0 ? (arrVertices[i] + m_dPageX) : (m_dPageH - arrVertices[i]));
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
	// CStampAnnotation
	//----------------------------------------------------------------------------------------
	CStampAnnotation::CStampAnnotation(CXref* pXref) : CMarkupAnnotation(pXref, AnnotStamp), m_pAPStream(NULL)
	{
	}
	void CStampAnnotation::SetRotate(double nRotate)
	{
		Add("Rotate", nRotate);

		if (!m_pAPStream)
			return;

		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		double ca = cos(nRotate / 180.0 * M_PI);
		double sa = sin(nRotate / 180.0 * M_PI);
		m_pAPStream->Add("Matrix", pArray);
		pArray->Add(ca);
		pArray->Add(sa);
		pArray->Add(-sa);
		pArray->Add(ca);
		pArray->Add(0);
		pArray->Add(0);
	}
	void CStampAnnotation::SetName(const std::wstring& wsName)
	{
		std::string sValue = U_TO_UTF8(wsName);
		Add("Name", sValue.c_str());
	}
	void CStampAnnotation::SetAPStream(CDictObject* pStream)
	{
		m_pAPStream = pStream;
	}
	//----------------------------------------------------------------------------------------
	// CWidgetAnnotation
	//----------------------------------------------------------------------------------------
	CWidgetAnnotation::CWidgetAnnotation(CXref* pXref, EAnnotType eType) : CAnnotation(pXref, eType)
	{
		m_pMK         = NULL;
		m_pParent     = NULL;
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
			CObjectBase* pMK = Get("MK");
			if (pMK && pMK->GetType() == object_type_DICT)
			{
				m_pMK = (CDictObject*)pMK;
				return;
			}

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
		if (nFlag < 0)
			return;
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
		pOwner->Add("TU", new CStringObject(sValue.c_str(), true));
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
		{
			pOwner = this;
			pOwner->Add("T", new CStringObject(sValue.c_str(), true));
		}
	}
	void CWidgetAnnotation::SetOMetadata(const std::wstring& wsOMetadata)
	{
		std::string sValue = U_TO_UTF8(wsOMetadata);
		CDictObject* pOwner = GetObjOwnValue("OMetadata");
		if (!pOwner)
		{
			pOwner = this;
			pOwner->Add("OMetadata", new CStringObject(sValue.c_str(), true));
		}
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

		std::string sAA = pAction->m_sType;
		CDictObject* pAA = NULL;
		if (m_pParent && (sAA == "K" || sAA == "F" || sAA == "V" || sAA == "C"))
			pAA = (CDictObject*)m_pParent->Get("AA");
		else if (sAA == "E" || sAA == "X" || sAA == "D" || sAA == "U" || sAA == "Fo" || sAA == "Bl" || sAA == "PO" || sAA == "PC" || sAA == "PV" || sAA == "PI")
		{
			pAA = (CDictObject*)Get("AA");
			if (!pAA)
			{
				pAA = new CDictObject();
				Add("AA", pAA);
			}
		}

		if (!pAA)
		{
			pAA = (CDictObject*)GetObjValue("AA");
			if (!pAA)
			{
				pAA = new CDictObject();
				Add("AA", pAA);
			}
		}

		if (pAA)
			pAA->Add(sAA.c_str(), pAction);
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
		if (m_pMK)
			return GetColor(dynamic_cast<CArrayObject*>(m_pMK->Get("BG")), false, dDiff);
		return "";
	}
	std::string CWidgetAnnotation::GetBCforAP()
	{
		if (m_pMK)
			return GetColor(dynamic_cast<CArrayObject*>(m_pMK->Get("BC")), true);
		return "";
	}
	bool CWidgetAnnotation::HaveBG()
	{
		if (!m_pMK)
			return false;
		CObjectBase* pObj = m_pMK->Get("BG");
		return pObj && pObj->GetType() == object_type_ARRAY;
	}
	bool CWidgetAnnotation::HaveBC()
	{
		if (!m_pMK)
			return false;
		CObjectBase* pObj = m_pMK->Get("BC");
		return pObj && pObj->GetType() == object_type_ARRAY;
	}
	void CWidgetAnnotation::APFromFakePage(CPage* pFakePage)
	{
		if (!m_pAppearance)
			return;
		CAnnotAppearanceObject* pNormal = m_pAppearance->GetNormal();
		pNormal->AddBBox(0, 0, GetWidth(), GetHeight());
	}
	void CWidgetAnnotation::SetEmptyAP()
	{
		if (!m_pAppearance)
			m_pAppearance = new CAnnotAppearance(m_pXref, this);
		Add("AP", m_pAppearance);
		CAnnotAppearanceObject* pAppearance = m_pAppearance->GetNormal();

		double dHeight = fabs(m_oRect.fTop - m_oRect.fBottom);
		double dWidth  = fabs(m_oRect.fRight - m_oRect.fLeft);

		pAppearance->StartDraw(dWidth, dHeight);
		pAppearance->EndDraw();
	}
	void CWidgetAnnotation::SetAP(const std::wstring& wsValue, unsigned short* pCodes, unsigned int unCount, double dX, double dY, CFontCidTrueType** ppFonts, double* pShifts)
	{
		if (!m_pAppearance)
			m_pAppearance = new CAnnotAppearance(m_pXref, this);
		Add("AP", m_pAppearance);
		CAnnotAppearanceObject* pNormal = m_pAppearance->GetNormal();
		pNormal->DrawSimpleText(wsValue, pCodes, unCount, m_pFont, m_dFontSize, dX, dY, 0, 0, 0, NULL, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop), ppFonts, pShifts);
	}
	CAnnotAppearanceObject* CWidgetAnnotation::StartAP()
	{
		m_pAppearance = new CAnnotAppearance(m_pXref, this);
		if (!m_pAppearance)
			return NULL;
		Add("AP", m_pAppearance);
		CAnnotAppearanceObject* pNormal  = m_pAppearance->GetNormal();
		pNormal->StartDrawText(m_pFont, m_dFontSize, 0, 0, 0, NULL, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));
		return pNormal;
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
		if (nFlag < 0)
			return;
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
		if (!m_pAppearance)
		{
			m_pAppearance = new CAnnotAppearance(m_pXref, this);
			CObjectBase* pAP = Get("AP");
			if (pAP && pAP->GetType() == object_type_DICT)
			{
				CDictObject* pDAP = (CDictObject*)pAP;
				CObjectBase* pAPi = pDAP->Get("N");
				if (pAPi)
				{
					CProxyObject* pNewAPi = new CProxyObject(pAPi->Copy(), true);
					pNewAPi->Get()->SetRef(pAPi->GetObjId(), pAPi->GetGenNo());
					m_pAppearance->Add("N", pNewAPi);
				}
				pAPi = pDAP->Get("D");
				if (pAPi)
				{
					CProxyObject* pNewAPi = new CProxyObject(pAPi->Copy(), true);
					pNewAPi->Get()->SetRef(pAPi->GetObjId(), pAPi->GetGenNo());
					m_pAppearance->Add("D", pNewAPi);
				}
				pAPi = pDAP->Get("R");
				if (pAPi)
				{
					CProxyObject* pNewAPi = new CProxyObject(pAPi->Copy(), true);
					pNewAPi->Get()->SetRef(pAPi->GetObjId(), pAPi->GetGenNo());
					m_pAppearance->Add("R", pNewAPi);
				}
			}
		}

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

		if (isdigit(sV[0]))
			pOwner->Add("V", sV.c_str());
		else
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
		CObjectBase* pAP, *pAPN;
		Add("AS", "Off");
		if (!m_sAP_N_Yes.empty())
		{
			CObjectBase* pObj = GetObjValue("Opt");
			if (pObj && pObj->GetType() == object_type_ARRAY)
			{
				CArrayObject* pArr = (CArrayObject*)pObj;
				for (int i = 0; i < pArr->GetCount(); ++i)
				{
					pObj = pArr->Get(i);
					if (pObj->GetType() == object_type_ARRAY && ((CArrayObject*)pObj)->GetCount() > 0)
						pObj = ((CArrayObject*)pObj)->Get(0);
					if (pObj->GetType() == object_type_STRING)
					{
						CStringObject* pStr = (CStringObject*)pObj;
						const BYTE* pBinary = pStr->GetString();
						if (pStr->GetLength() == m_sAP_N_Yes.length() && !StrCmp((const char*)pBinary, m_sAP_N_Yes.c_str()))
						{
							m_sAP_N_Yes = std::to_string(i);
							SetV(UTF8_TO_U(m_sAP_N_Yes));
							break;
						}
					}
				}
			}
			Add("AS", m_sAP_N_Yes.c_str());
		}
		else if ((pAP = Get("AP")) && pAP->GetType() == object_type_DICT && (pAPN = ((CDictObject*)pAP)->Get("N")) && pAPN->GetType() == object_type_DICT && ((CDictObject*)pAPN)->Get(sV))
			Add("AS", sV.c_str());
	}
	void CCheckBoxWidget::SetFlag(const int& nFlag)
	{
		if (nFlag < 0)
			return;
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
		m_bAPV = false;
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
		m_bAPV = false;
	}
	void CChoiceWidget::SetFlag(const int& nFlag)
	{
		if (nFlag < 0)
			return;
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
		return wsExportV;
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
			pArray->Add(new CStringObject(sValue.c_str(), true));
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
		Add("JS", new CStringObject(sValue.c_str(), true));
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
			pArray->Add(new CStringObject(sValue.c_str(), true));
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
