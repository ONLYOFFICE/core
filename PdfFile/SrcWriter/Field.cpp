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
#include "Field.h"
#include "Pages.h"
#include "Streams.h"
#include "Document.h"
#include "ResourcesDictionary.h"
#include "Types.h"
#include "Image.h"
#include "Font.h"
#include "Info.h"
#include "EncryptDictionary.h"
#include "Annotation.h"

#include <algorithm>
#include <math.h>

#include "../../DesktopEditor/common/SystemUtils.h"
#include "../../DesktopEditor/common/File.h"

#ifndef BS_DEF_WIDTH
#define BS_DEF_WIDTH 1
#endif

namespace PdfWriter
{

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
	void StreamWriteCM(CStream* pStream, double m11, double m12, double m21, double m22, double tx, double ty)
	{
		pStream->WriteReal(m11);
		pStream->WriteChar(' ');
		pStream->WriteReal(m12);
		pStream->WriteChar(' ');
		pStream->WriteReal(m21);
		pStream->WriteChar(' ');
		pStream->WriteReal(m22);
		pStream->WriteChar(' ');
		pStream->WriteReal(tx);
		pStream->WriteChar(' ');
		pStream->WriteReal(ty);
		pStream->WriteStr(" cm\012");
	}
	void StreamWriteXYMove(CStream* pStream, double x, double y)
	{
		pStream->WriteReal(x);
		pStream->WriteChar(' ');
		pStream->WriteReal(y);
		pStream->WriteStr(" m\012");
	}
	void StreamWriteXYLine(CStream* pStream, double x, double y)
	{
		pStream->WriteReal(x);
		pStream->WriteChar(' ');
		pStream->WriteReal(y);
		pStream->WriteStr(" l\012");
	}
	void StreamWriteXYCurve(CStream* pStream, double x1, double y1, double x2, double y2, double x3, double y3)
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
	void StreamWriteCircle(CStream* pStream, double cx, double cy, double r)
	{
		double bezierCircle = 0.55228475 * r;
		StreamWriteXYMove(pStream, cx + r, cy);
		StreamWriteXYCurve(pStream, cx + r, cy + bezierCircle, cx + bezierCircle, cy + r, cx, cy + r);
		StreamWriteXYCurve(pStream, cx - bezierCircle, cy + r, cx - r, cy + bezierCircle, cx - r, cy);
		StreamWriteXYCurve(pStream, cx - r, cy - bezierCircle, cx - bezierCircle, cy - r, cx, cy - r);
		StreamWriteXYCurve(pStream, cx + bezierCircle, cy - r, cx + r, cy - bezierCircle, cx + r, cy);
	}
	void DrawArrow(CStream* pStream, ELineEndType nType, double x, double y, double dx, double dy, double w)
	{
		double lineEndSize1 = 3, pi = 3.14159265358979323846;
		switch (nType)
		{
		case ELineEndType::Butt:
		{
			w *= lineEndSize1;
			StreamWriteXYMove(pStream, x + w * dy, y - w * dx);
			StreamWriteXYLine(pStream, x - w * dy, y + w * dx);
			pStream->WriteStr("S\012");
			break;
		}
		case ELineEndType::Circle:
		{
			StreamWriteCircle(pStream, x, y, w * lineEndSize1);
			pStream->WriteStr("h\012B\012");
			break;
		}
		case ELineEndType::Diamond:
		{
			w *= lineEndSize1;
			StreamWriteXYMove(pStream, x - w, y);
			StreamWriteXYLine(pStream, x, y + w);
			StreamWriteXYLine(pStream, x + w, y);
			StreamWriteXYLine(pStream, x, y - w);
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
				StreamWriteXYMove(pStream, x + w * cos(d32) * dx + w * sin(d32) * dy, y + w * cos(d32) * dy - w * sin(d32) * dx);
				StreamWriteXYLine(pStream, x, y);
				StreamWriteXYLine(pStream, x + w * cos(d28) * dx - w * sin(d28) * dy, y + w * cos(d28) * dy + w * sin(d28) * dx);
			}
			else
			{
				double dCos = w * cos(pi / 6.0);
				double dSin = w * sin(pi / 6.0);

				StreamWriteXYMove(pStream, x + dCos * dx + dSin * dy, y + dCos * dy - dSin * dx);
				StreamWriteXYLine(pStream, x, y);
				StreamWriteXYLine(pStream, x + dCos * dx - dSin * dy, y + dCos * dy + dSin * dx);
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
			StreamWriteXYMove(pStream, x - dCos * dx + dSin * dy, y - dCos * dy - dSin * dx);
			StreamWriteXYLine(pStream, x, y);
			StreamWriteXYLine(pStream, x - dCos * dx - dSin * dy, y - dCos * dy + dSin * dx);
			pStream->WriteStr(nType == ELineEndType::ROpenArrow ? "S\012" : "b\012");
			break;
		}
		case ELineEndType::Slash:
		{
			w *= lineEndSize1 * lineEndSize1;
			double dCos = w * cos(pi / 6.0);
			double dSin = w * sin(pi / 6.0);
			StreamWriteXYMove(pStream, x + dCos * dy - dSin * dx, y - dCos * dx - dSin * dy);
			StreamWriteXYLine(pStream, x - dCos * dy + dSin * dx, y + dCos * dx + dSin * dy);
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
			StreamWriteXYMove(pStream, ax1, ay1);
			StreamWriteXYLine(pStream, bx1, by1);

			StreamWriteXYMove(pStream, ax2, ay2);
			StreamWriteXYLine(pStream, bx2, by2);
		}

		StreamWriteXYMove(pStream, tx1, ty1);
		StreamWriteXYLine(pStream, tx2, ty2);
		pStream->WriteStr("S\012");

		DrawArrow(pStream, nLE1, tx1, ty1,  dDX,  dDY, dBorderSize);
		DrawArrow(pStream, nLE2, tx2, ty2, -dDX, -dDY, dBorderSize);
	}

	//----------------------------------------------------------------------------------------
	// CFieldBase
	//----------------------------------------------------------------------------------------
	CFieldBase::CFieldBase(CXref* pXref, CDocument* pDocument)
	{
		pXref->Add(this);
		Add("Ff", (unsigned int)0);

		m_pXref     = pXref;
		m_pDocument = pDocument;

		m_nBorderType = 0;
		m_dBorderSize = 0;

		m_bAutoFit = false;

		m_bShd = false;

		m_pMK         = NULL;
		m_pParent     = NULL;
		m_pKids       = NULL;
		m_pAppearance = NULL;
		m_pFocus      = NULL;
		m_pBlur       = NULL;
		m_pFont       = NULL;
	}
	void CFieldBase::SetReadOnlyFlag(bool isReadOnly)
	{
		SetFlag(isReadOnly, 1 << 0);
	}
	void CFieldBase::SetRequiredFlag(bool isRequired)
	{
		SetFlag(isRequired, 1 << 1);
	}
	void CFieldBase::SetNoExportFlag(bool isNoExport)
	{
		SetFlag(isNoExport, 1 << 2);
	}
	void CFieldBase::SetFlag(bool isFlag, int nBit)
	{
		int nFlags = ((CNumberObject*)this->Get("Ff"))->Get();

		if ((nFlags & nBit) && !isFlag)
		{
			nFlags &= 0xffffffff ^ nBit;
		}
		else if (!(nFlags & nBit) && isFlag)
		{
			nFlags |= nBit;
		}

		Add("Ff", nFlags);
	}
	CDictObject* CFieldBase::GetAA()
	{
		CDictObject* pAA = dynamic_cast<CDictObject*>(this->Get("AA"));
		if (pAA)
			return pAA;

		pAA = new CDictObject();
		if (!pAA)
			return NULL;

		Add("AA", pAA);
		return pAA;
	}
	void CFieldBase::AddScriptToAA(const std::string& sKey, const std::string& sScript, CDictObject* pAA)
	{
		if (sScript.empty())
			return;

		if (!pAA)
			pAA = GetAA();

		if (!pAA)
			return;

		CDictObject* pKey = new CDictObject();
		if (!pKey)
			return;

		m_pXref->Add(pKey);
		pAA->Add(sKey.c_str(), pKey);
		pKey->Add("S", "JavaScript");
		pKey->Add("JS", new CStringObject(sScript.c_str(), true, true));
	}
	void CFieldBase::AddPageRect(CPage* pPage, const TRect& oRect)
	{
		if (!pPage)
			return;

		pPage->AddAnnotation(this);

		Add("Type", "Annot");
		Add("Subtype", "Widget");

		Add("P", pPage);

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

			m_oRect.fLeft = oRect.fLeft;
			m_oRect.fTop = oRect.fTop;
			m_oRect.fRight = oRect.fRight;
			m_oRect.fBottom = oRect.fBottom;
		}
		else
		{
			pArray->Add(oRect.fLeft);
			pArray->Add(oRect.fBottom);
			pArray->Add(oRect.fRight);
			pArray->Add(oRect.fTop);

			m_oRect.fLeft = oRect.fLeft;
			m_oRect.fTop = oRect.fBottom;
			m_oRect.fRight = oRect.fRight;
			m_oRect.fBottom = oRect.fTop;
		}

		Add("F", 4);		
	}
	void CFieldBase::SetFieldName(const std::string& sName, bool isSkipCheck)
	{
		if (isSkipCheck || !m_pDocument->CheckFieldName(this, sName))
			Add("T", new CStringObject(sName.c_str()));
	}
	void CFieldBase::SetFieldName(const std::wstring& wsName, bool isSkipCheck) 
	{
		std::string sName = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsName);
		if (isSkipCheck || !m_pDocument->CheckFieldName(this, sName))
			Add("T", new CStringObject(sName.c_str(), true));
	}
	void CFieldBase::ClearKidRecords()
	{
		Remove("T");
		Remove("FT");
		Remove("Ff");
		Remove("V");
	}
	void CFieldBase::SetFieldHint(const std::wstring& wsHint)
	{
		if (wsHint != L"")
		{
			std::string sHint = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsHint);
			Add("TU", new CStringObject(sHint.c_str(), true));
		}
		else
		{
			Remove("TU");
		}
	}
	TRect& CFieldBase::GetRect()
	{
		return m_oRect;
	}
	CResourcesDict* CFieldBase::GetResourcesDict()
	{
		return m_pDocument->GetFieldsResources();
	}
	void CFieldBase::SetDefaultAppearance(CFontDict* pFont, const double& dFontSize, const TRgb& oColor)
	{
		CResourcesDict* pFieldsResources = m_pDocument->GetFieldsResources();

		const char* sFontName = pFieldsResources->GetFontName(pFont);
		if (!sFontName)
			return;

		std::string sDA;
		sDA.append(std::to_string(oColor.r));
		sDA.append(" ");
		sDA.append(std::to_string(oColor.g));
		sDA.append(" ");
		sDA.append(std::to_string(oColor.b));
		sDA.append(" rg /");
		sDA.append(sFontName);

		if (IsAutoFit())
		{
			sDA.append(" 0 Tf");
		}
		else
		{
			sDA.append(" ");
			sDA.append(std::to_string(dFontSize));
			sDA.append(" Tf");
		}

		Add("DA", new CStringObject(sDA.c_str(), false, true));
	}
	void CFieldBase::SetTextAppearance(const std::wstring& wsValue, unsigned short* pCodes, unsigned int unCount, CFontDict* pFont, const TRgb& oColor, const double& dAlpha, double dFontSize, double dX, double dY, CFontCidTrueType** ppFonts, double* pShifts)
	{
		CAnnotAppearance* pAppearance = new CAnnotAppearance(m_pXref, this);
		Add("AP", pAppearance);

		CAnnotAppearanceObject* pNormal = pAppearance->GetNormal();

		CResourcesDict* pFieldsResources = m_pDocument->GetFieldsResources();
		
		const char* sExtGrStateName = NULL;
		if (fabs(dAlpha - 1.0) > 0.001)
		{
			CExtGrState* pExtGrState = m_pDocument->GetFillAlpha(dAlpha);
			sExtGrStateName = pFieldsResources->GetExtGrStateName(pExtGrState);
		}

		pNormal->DrawSimpleText(wsValue, pCodes, unCount, pFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop), ppFonts, pShifts);
		m_pFont = pNormal->GetFont();
	}
	void CFieldBase::StartTextAppearance(CFontDict* pFont, const double& dFontSize, const TRgb& oColor, const double& dAlpha)
	{
		m_pAppearance = new CAnnotAppearance(m_pXref, this);
		if (!m_pAppearance)
			return;

		Add("AP", m_pAppearance);

		CAnnotAppearanceObject* pNormal  = m_pAppearance->GetNormal();
		CResourcesDict* pFieldsResources = GetResourcesDict();

		const char* sExtGrStateName = NULL;
		if (fabs(dAlpha - 1.0) > 0.001)
		{
			CExtGrState* pExtGrState = m_pDocument->GetFillAlpha(dAlpha);
			sExtGrStateName = pFieldsResources->GetExtGrStateName(pExtGrState);
		}

		pNormal->StartDrawText(pFont, dFontSize, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));
		m_pFont = pNormal->GetFont();
	}
	void CFieldBase::AddLineToTextAppearance(const double& dX, const double& dY, unsigned short* pCodes, const unsigned int& unCodesCount, CFontCidTrueType** ppFonts, const double* pShifts)
	{
		if (!m_pAppearance)
			return;

		CAnnotAppearanceObject* pNormal = m_pAppearance->GetNormal();
		pNormal->DrawTextLine(dX, dY, pCodes, unCodesCount, ppFonts, pShifts);
		m_pFont = pNormal->GetFont();
	}
	void CFieldBase::EndTextAppearance()
	{
		if (!m_pAppearance)
			return;

		CAnnotAppearanceObject* pNormal = m_pAppearance->GetNormal();
		pNormal->EndDrawText();
	}
	void CFieldBase::SetTextValue(const std::wstring &wsValue)
	{
		std::string sValue = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsValue);
		Add("V", new CStringObject(sValue.c_str(), true));
	}
	void CFieldBase::SetFieldBorder(const EBorderSubtype& eSubtype, const TRgb& oColor, const double& dWidth, const unsigned short& nDashOn, const unsigned short& nDashOff, const unsigned short& nDashPhase)
	{
		if (dWidth < 0.01)
		{
			m_nBorderType = 0;
			Remove("BS");
			return;
		}

		CDictObject* pBorderStyleDict = new CDictObject();
		if (!pBorderStyleDict)
			return;

		Add("BS", pBorderStyleDict);

		// PDF ридеры воспринимают только целочисленные значения толщины линии
		int nWidth = (int)(dWidth + 0.5);
		if (nWidth < 1)
			nWidth = 1;

		pBorderStyleDict->Add("W", nWidth);

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

		if (!m_pMK)
		{
			m_pMK = new CDictObject();
			Add("MK", m_pMK);
		}

		if (!m_pMK)
			return;

		CArrayObject* pColor = new CArrayObject();
		pColor->Add(oColor.r);
		pColor->Add(oColor.g);
		pColor->Add(oColor.b);
		m_pMK->Add("BC", pColor);

		m_nBorderType  = 1;
		m_dBorderSize  = nWidth;
		m_oBorderColor = oColor;
	}
	bool CFieldBase::HaveBorder() const
	{
		return m_nBorderType == 1;
	}
	const double& CFieldBase::GetBorderSize() const
	{
		return m_dBorderSize;
	}
	const TRgb& CFieldBase::GetBorderColor() const
	{
		return m_oBorderColor;
	}
	const bool& CFieldBase::IsAutoFit() const
	{
		return m_bAutoFit;
	}
	void CFieldBase::SetAutoFit(const bool& isAutoFit)
	{
		m_bAutoFit = isAutoFit;
	}
	bool CFieldBase::HaveShd() const
	{
		return m_bShd;
	}
	void CFieldBase::SetShd(const TRgb& oRgb)
	{
		m_oShdColor = oRgb;

		if (!m_pMK)
		{
			m_pMK = new CDictObject();
			Add("MK", m_pMK);
		}

		if (!m_pMK)
			return;

		CArrayObject* pBG = new CArrayObject();
		if (pBG)
		{
			pBG->Add(oRgb.r);
			pBG->Add(oRgb.g);
			pBG->Add(oRgb.b);
			m_pMK->Add("BG", pBG);
		}
		m_bShd = true;
	}
	const TRgb& CFieldBase::GetShdColor() const
	{
		return m_oShdColor;
	}
	void CFieldBase::SetParent(CFieldBase* pParent)
	{
		m_pParent = pParent;
		Add("Parent", pParent);
	}
	void CFieldBase::AddKid(CFieldBase* pChild)
	{
		if (!m_pKids)
		{
			m_pKids = new CArrayObject();
			Add("Kids", m_pKids);
		}

		m_pKids->Add(pChild);
	}
	int CFieldBase::GetKidsCount() const
	{
		if (!m_pKids)
			return 0;

		return m_pKids->GetCount();
	}
	int CFieldBase::GetFieldFlag() const
	{
		return ((CNumberObject*)Get("Ff"))->Get();
	}
	const char* CFieldBase::GetFieldType() const
	{
		return ((CNameObject*)Get("FT"))->Get();
	}
	void CFieldBase::SetAlign(const EFieldAlignType& eType)
	{
		Add("Q", (int)eType);
	}
	void CFieldBase::SetPlaceHolderText(const std::wstring& wsText, const TRgb& oNormalColor, const TRgb& oPlaceHolderColor)
	{
		m_wsPlaceHolderText = wsText;
		m_oNormalColor      = oNormalColor;
		m_oPlaceHolderColor = oPlaceHolderColor;

		SetPlaceHolderText(std::vector<std::wstring>{wsText}, {oNormalColor}, {oPlaceHolderColor});

		if (m_pParent)
			m_pParent->UpdateKidsPlaceHolder();
	}
	void CFieldBase::SetPlaceHolderText(const std::vector<std::wstring>& vPlaceHolders, const std::vector<TRgb>& vNormalColors, const std::vector<TRgb>& vPlaceHolderColors)
	{
		if (!vPlaceHolders.size() || !vNormalColors.size() || !vPlaceHolderColors.size())
			return;

		std::string sFocus = "\nvar	curColor = color.convert(event.target.textColor, \"RGB\");\nif ((";

		for (unsigned int unIndex = 0, unCount = vPlaceHolders.size(); unIndex < unCount; ++unIndex)
		{
			if (unIndex)
				sFocus += " || ";

			std::string sText = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(vPlaceHolders[unIndex]);
			sFocus += "event.target.value==\"" + sText + "\"";
		}

		sFocus += ") && (";

		for (unsigned int unIndex = 0, unCount = vPlaceHolderColors.size(); unIndex < unCount; ++unIndex)
		{
			if (unIndex)
				sFocus += " || ";

			const TRgb& oColor = vPlaceHolderColors[unIndex];

			sFocus += "(Math.abs(curColor[1] - " + std::to_string(oColor.r) +
					") < 0.005 && Math.abs(curColor[2] - " + std::to_string(oColor.g) +
					") < 0.005 && Math.abs(curColor[3] - " + std::to_string(oColor.b) +
					") < 0.005)";
		}

		sFocus += "))\n{	event.target.value = \"\";\n	event.target.textColor =[\"RGB\", " +
				std::to_string(vNormalColors[vNormalColors.size() - 1].r) + ", " +
				std::to_string(vNormalColors[vNormalColors.size() - 1].g) + ", " +
				std::to_string(vNormalColors[vNormalColors.size() - 1].b) + "];\n}";



		std::string sText = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(vPlaceHolders[vPlaceHolders.size() - 1]);
		const TRgb& oColor = vPlaceHolderColors[vPlaceHolderColors.size() - 1];
		std::string sBlur = "\nif (event.target.value == \"\")\n{\n	event.target.textColor =[\"RGB\", " +
			std::to_string(oColor.r) + ", " +
			std::to_string(oColor.g) + ", " +
			std::to_string(oColor.b) + "];\nevent.target.value = \"" + sText + "\";\n}";

		if (!m_pFocus || !m_pBlur)
		{
			CDictObject* pAA = GetAA();
			if (!pAA)
				return;

			CDictObject* pFocus = new CDictObject();
			CDictObject* pBlur = new CDictObject();
			if (!pFocus || !pBlur)
				return;

			m_pXref->Add(pFocus);
			m_pXref->Add(pBlur);

			m_pFocus = pFocus;
			m_pBlur  = pBlur;

			pAA->Add("Bl", pBlur);
			pAA->Add("Fo", pFocus);
		}

		m_pFocus->Add("S", "JavaScript");
		m_pFocus->Add("JS", new CStringObject(sFocus.c_str(), true, true));

		m_pBlur->Add("S", "JavaScript");
		m_pBlur->Add("JS", new CStringObject(sBlur.c_str(), true, true));
	}
	void CFieldBase::UpdateKidsPlaceHolder()
	{
		std::vector<std::wstring> vText;
		std::vector<TRgb> vPlaceHolderColor, vNormalColor;

		if (m_pKids)
		{
			for (unsigned int unKidIndex = 0, unKidsCount = GetKidsCount(); unKidIndex < unKidsCount; ++unKidIndex)
			{
				CFieldBase* pKid = (CFieldBase*)m_pKids->Get(unKidIndex);

				const std::wstring& wsText = pKid->GetPlaceHolderText();

				if (wsText.empty())
					continue;

				const TRgb& oPColor = pKid->GetPlaceHolderColor();
				const TRgb& oNColor = pKid->GetNormalColor();

				bool isAdd = true;
				for (unsigned int unIndex = 0, unCount = vText.size(); unIndex < unCount; ++unIndex)
				{
					if (vText[unIndex] == wsText)
					{
						isAdd = false;
						break;
					}
				}

				if (isAdd)
					vText.push_back(wsText);

				isAdd = true;
				for (unsigned int unIndex = 0, unCount = vPlaceHolderColor.size(); unIndex < unCount; ++unIndex)
				{
					if (vPlaceHolderColor[unIndex] == oPColor)
					{
						isAdd = false;
						break;
					}
				}

				if (isAdd)
					vPlaceHolderColor.push_back(oPColor);

				isAdd = true;
				for (unsigned int unIndex = 0, unCount = vNormalColor.size(); unIndex < unCount; ++unIndex)
				{
					if (vNormalColor[unIndex] == oNColor)
					{
						isAdd = false;
						break;
					}
				}

				if (isAdd)
					vNormalColor.push_back(oNColor);
			}

			for (unsigned int unKidIndex = 0, unKidsCount = GetKidsCount(); unKidIndex < unKidsCount; ++unKidIndex)
			{
				CFieldBase* pKid = (CFieldBase*)m_pKids->Get(unKidIndex);
				pKid->SetPlaceHolderText(vText, vNormalColor, vPlaceHolderColor);
			}
		}
	}
	const std::wstring& CFieldBase::GetPlaceHolderText()
	{
		return m_wsPlaceHolderText;
	}
	const TRgb& CFieldBase::GetNormalColor()
	{
		return m_oNormalColor;
	}
	const TRgb& CFieldBase::GetPlaceHolderColor()
	{
		return m_oPlaceHolderColor;
	}
	void CFieldBase::SetFormat(const CFormFieldInfo::CTextFormFormat* pFormat)
	{
		if (!pFormat || pFormat->IsEmpty())
			return;

		std::string scriptK, scriptF, scriptV;

		const TRgb& oNormalColor = GetNormalColor();

		const std::string prefix = "var curColor = color.convert(event.target.textColor, \"RGB\");\nif (Math.abs(curColor[1] - " + std::to_string(oNormalColor.r) +
			") < 0.005 && Math.abs(curColor[2] - " + std::to_string(oNormalColor.g) +
			") < 0.005 && Math.abs(curColor[3] - " + std::to_string(oNormalColor.b) +
			") < 0.005)\n{";
		const std::string postfix = "}";

		if (pFormat->GetSymbolsCount())
		{
			scriptK += "\nvar s=String.fromCharCode(";
			scriptV += "\nvar s=String.fromCharCode(";
			for (unsigned int unIndex = 0, unCount = pFormat->GetSymbolsCount(); unIndex < unCount; ++unIndex)
			{
				if (unIndex)
				{
					scriptK += ", ";
					scriptV += ", ";
				}

				scriptK += std::to_string(pFormat->GetSymbol(unIndex));
				scriptV += std::to_string(pFormat->GetSymbol(unIndex));
			}
			scriptK += ");";
			scriptV += ");";

			scriptK += "\nfor (var i = 0, l = event.change.length; i < l; ++i){\nvar c = event.change.charAt(i);\nif(-1 == s.indexOf(c)){\nevent.rc = false;\nbreak;\n}\n}";
			scriptV += "\nfor (var i = 0, l = event.value.length; i < l; ++i){\nvar c = event.value.charAt(i);\nif(-1 == s.indexOf(c)){\nevent.rc = false;\nbreak;\n}\n}";
		}

		if (pFormat->IsDigit())
		{
			scriptK += "\nif(false !== event.rc)\n{for(var i = 0, l = event.change.length; i < l; ++i){var c = event.change.charCodeAt(i);if(c < 48 || c > 57){event.rc = false;break;}}}";
			scriptV += "\nfor(var i = 0, l = event.value.length; i < l; ++i){var c = event.value.charCodeAt(i);if(c < 48 || c > 57){event.rc = false;break;}}";
		}
		else if (pFormat->IsLetter())
		{
			scriptK += "\nvar r=/^[^\\d.!?\\/\\\\+\\-\\x20'\":;\\(\\)\\[\\]\\{\\}=_@#$%^&*]+$/;\nif(false !== event.rc)\n{for(var i = 0, l = event.change.length; i < l; ++i){var c = event.change.charCodeAt(i);if(!r.test(String.fromCharCode(c))){event.rc = false;break;}}}";
			scriptV += "\nvar r=/^[^\\d.!?\\/\\\\+\\-\\x20'\":;\\(\\)\\[\\]\\{\\}=_@#$%^&*]+$/;\nfor(var i = 0, l = event.value.length; i < l; ++i){var c = event.value.charCodeAt(i);if(!r.test(String.fromCharCode(c))){event.rc = false;break;}}";
		}
		else if (pFormat->IsMask())
		{
			scriptK += "\nAFSpecial_KeystrokeEx(\"" + NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pFormat->GetMask()) + "\");";
		}
		else if (pFormat->IsRegExp())
		{
			scriptV += "\nvar r=/" + NSFile::CUtf8Converter::GetUtf8StringFromUnicode(pFormat->GetRegExp()) + "/;";
			scriptV += "\nif(event.value)event.rc=!!event.value.match(r);";
		}

		CDictObject* pAA = GetAA();

		if (!scriptK.empty())
		{
			scriptK = prefix + scriptK + postfix;
			AddScriptToAA("K", scriptK, pAA);
		}

		if (!scriptV.empty())
		{
			scriptV = prefix + scriptV + postfix;
			AddScriptToAA("V", scriptV, pAA);
		}

		if (!scriptF.empty())
		{
			scriptV = prefix + scriptF + postfix;
			AddScriptToAA("F", scriptF, pAA);
		}
	}
	CFontDict* CFieldBase::GetFont()
	{
		return m_pFont;
	}
	//----------------------------------------------------------------------------------------
	// CTextField
	//----------------------------------------------------------------------------------------
	CTextField::CTextField(CXref* pXref, CDocument* pDocument) : CFieldBase(pXref, pDocument)
	{
		Add("FT", "Tx");

		m_bAutoFit = false;
	}
	void CTextField::SetMultilineFlag(bool isMultiLine)
	{
		SetFlag(isMultiLine, 1 << 12);
	}
	void CTextField::SetPasswordFlag(bool isPassword) 
	{
		SetFlag(isPassword, 1 << 13);
	}
	void CTextField::SetFileSelectFlag(bool isFileSelect)
	{
		SetFlag(isFileSelect, 1 << 20);
	}
	void CTextField::SetDoNotSpellCheckFlag(bool isDoNotSpellCheck)
	{
		SetFlag(isDoNotSpellCheck, 1 << 22);
	}
	void CTextField::SetDoNotScrollFlag(bool isDoNotScroll)
	{
		SetFlag(isDoNotScroll, 1 << 23);
	}
	void CTextField::SetCombFlag(bool isComb)
	{
		SetFlag(isComb, 1 << 24);
	}
	void CTextField::SetRichTextFlag(bool isRichText)
	{
		SetFlag(isRichText, 1 << 25);
	}
	void CTextField::SetMaxLen(int nMaxLen)
	{
		if (nMaxLen > 0)
		{
			if (m_pParent)
				m_pParent->Add("MaxLen", nMaxLen);
			else
				Add("MaxLen", nMaxLen);
		}
	}
	int CTextField::GetMaxLen() const
	{
		CNumberObject* oMaxLen = (CNumberObject*)Get("MaxLen");
		if (oMaxLen)
			return oMaxLen->Get();

		return 0;
	}
	bool CTextField::IsCombFlag() const
	{
		int nFlags = ((CNumberObject*)this->Get("Ff"))->Get();
		return (nFlags & (1 << 24));
	}
	//----------------------------------------------------------------------------------------
	// CChoiceField
	//----------------------------------------------------------------------------------------
	CChoiceField::CChoiceField(CXref* pXref, CDocument* pDocument) : CFieldBase(pXref, pDocument)
	{
		m_pOpt = NULL;

		Add("FT", "Ch");
	}
	void CChoiceField::SetComboFlag(const bool& isCombo)
	{
		SetFlag(isCombo, 1 << 17);
	}
	void CChoiceField::SetEditFlag(const bool& isEdit)
	{
		SetFlag(isEdit, 1 << 18);
	}
	void CChoiceField::SetSortFlag(const bool& isSort)
	{
		SetFlag(isSort, 1 << 19);
	}
	void CChoiceField::SetMultiSelectFlag(const bool& isMultiSelect)
	{
		SetFlag(isMultiSelect, 1 << 21);
	}
	void CChoiceField::SetDoNotSpellCheck(const bool& isDoNotSpellCheck)
	{
		SetFlag(isDoNotSpellCheck, 1 << 22);
	}
	void CChoiceField::AddOption(const std::wstring& wsOption, const bool& bPushBack)
	{
		if (!m_pOpt)
		{
			m_pOpt = new CArrayObject();
			Add("Opt", m_pOpt);
		}

		if (!m_pOpt)
			return;

		std::string sOption = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsOption);
		m_pOpt->Add(new CStringObject(sOption.c_str(), true), bPushBack);
	}
	void CChoiceField::SetSelectedIndex(const unsigned int& unSelectedIndex)
	{
		if (m_pParent)
		{
			CChoiceField* pParent = dynamic_cast<CChoiceField*>(m_pParent);
			if (pParent)
			{
				pParent->SetSelectedIndex(unSelectedIndex);
				return;
			}
		}

		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		pArray->Add(unSelectedIndex);
		Add("I", pArray);

		if (m_pKids)
		{
			for (unsigned int unIndex = 0, unCount = m_pKids->GetCount(); unIndex < unCount; ++unIndex)
			{
				CFieldBase* pKid = dynamic_cast<CFieldBase*>(m_pKids->Get(unIndex));
				if (pKid)
					pKid->Remove("I");
			}
		}
	}
	void CChoiceField::UpdateSelectedIndexToParent()
	{
		if (m_pParent)
		{
			CObjectBase* pSelectedArray = Get("I");
			if (pSelectedArray)
				m_pParent->Add("I", pSelectedArray->Copy());

			Remove("I");

			CObjectBase* pValue = Get("V");
			if (pValue)
				m_pParent->Add("V", pValue->Copy());

			Remove("V");

			CObjectBase* pOpt = Get("Opt");
			if (pOpt)
				m_pParent->Add("Opt", pOpt->Copy());

			Remove("Opt");
		}
	}
	void CChoiceField::SetPlaceHolderText(const std::wstring& wsText, const TRgb& oNormalColor, const TRgb& oPlaceHolderColor)
	{
		CDictObject* pAA = GetAA();
		if (!pAA)
			return;

		CDictObject* pFocus = new CDictObject();
		CDictObject* pBlur = new CDictObject();
		if (!pFocus || !pBlur)
			return;

		bool bCanEdit = GetFieldFlag() & (1 << 18);

		
		std::string sText = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsText);
		std::string sFocus, sBlur;

		if (!bCanEdit)
		{
			CDictObject* pChange = new CDictObject();
			if (!pChange)
			{
				delete pFocus;
				delete pBlur;
				return;
			}

			// Если текст плейсхолдера не добавить как опцию, тогда AdobeAcrobat не дает выставлять такое текстовое значение
			AddOption(wsText, false);

			sFocus = "event.target.textColor = [\"RGB\", " +
				std::to_string(oNormalColor.r) + ", " +
				std::to_string(oNormalColor.g) + ", " +
				std::to_string(oNormalColor.b) + "];";

			sBlur = "\nif (event.target.value == \"" + sText + "\")\n	event.target.textColor = [\"RGB\", " +
				std::to_string(oPlaceHolderColor.r) + ", " +
				std::to_string(oPlaceHolderColor.g) + ", " +
				std::to_string(oPlaceHolderColor.b) + "];\nelse\n	event.target.textColor = [\"RGB\", " +
				std::to_string(oNormalColor.r) + ", " +
				std::to_string(oNormalColor.g) + ", " +
				std::to_string(oNormalColor.b) + "];";

			std::string sChange = "\n	if (event.value == \"" + sText + "\")\n		event.target.textColor =[\"RGB\", " +
				std::to_string(oPlaceHolderColor.r) + ", " +
				std::to_string(oPlaceHolderColor.g) + ", " +
				std::to_string(oPlaceHolderColor.b) + "];\n	else\n		event.target.textColor = [\"RGB\", " +
				std::to_string(oNormalColor.r) + ", " +
				std::to_string(oNormalColor.g) + ", " +
				std::to_string(oNormalColor.b) + "];\n}";

			m_pXref->Add(pChange);
			pChange->Add("S", "JavaScript");
			pChange->Add("JS", new CStringObject(sChange.c_str(), true, true));
			pAA->Add("K", pChange);
		}
		else
		{
			sFocus = "var curColor = color.convert(event.target.textColor, \"RGB\");\nevent.target.textColor = [\"RGB\", " +
				std::to_string(oNormalColor.r) + ", " +
				std::to_string(oNormalColor.g) + ", " +
				std::to_string(oNormalColor.b) + "];\nif (event.target.value == \"" + \
				sText + "\" && Math.abs(curColor[1] - " + std::to_string(oPlaceHolderColor.r) +
				") < 0.005 && Math.abs(curColor[2] - " + std::to_string(oPlaceHolderColor.g) +
				") < 0.005 && Math.abs(curColor[3] - " + std::to_string(oPlaceHolderColor.b) +
				") < 0.005)\n	event.target.value = \"\";";

			sBlur = "\nif (event.target.value == \"\")\n{\n	event.target.textColor = [\"RGB\", " +
				std::to_string(oPlaceHolderColor.r) + ", " +
				std::to_string(oPlaceHolderColor.g) + ", " +
				std::to_string(oPlaceHolderColor.b) + "];\nevent.target.value = \"" + sText + "\";\n}";
		}

		m_pXref->Add(pFocus);
		pFocus->Add("S", "JavaScript");
		pFocus->Add("JS", new CStringObject(sFocus.c_str(), true, true));
		pAA->Add("Fo", pFocus);

		m_pXref->Add(pBlur);
		pBlur->Add("S", "JavaScript");
		pBlur->Add("JS", new CStringObject(sBlur.c_str(), true, true));
		pAA->Add("Bl", pBlur);
	}
	//----------------------------------------------------------------------------------------
	// CCheckBoxField
	//----------------------------------------------------------------------------------------
	CCheckBoxField::CCheckBoxField(CXref* pXref, CDocument* pDocument, CRadioGroupField* pGroup, const char* sYesName) : CFieldBase(pXref, pDocument)
	{
		Add("FT", "Btn");

		m_pGroup = pGroup;

		if (pGroup)
			Add("Parent", pGroup);

		m_sYesName = sYesName ? sYesName : "Yes";
		Add("AS", "Off");
	}
	void CCheckBoxField::SetAppearance(const std::wstring& wsYesValue, unsigned short* pYesCodes, unsigned int unYesCount, CFontDict* pYesFont,
									   const std::wstring& wsOffValue, unsigned short* pOffCodes, unsigned int unOffCount, CFontDict* pOffFont,
									   const TRgb& oColor, const double& dAlpha, double dFontSize, double dX, double dY)
	{
		CCheckBoxAnnotAppearance* pAppearance = new CCheckBoxAnnotAppearance(m_pXref, this, m_sYesName.c_str());
		Add("AP", pAppearance);

		if (!m_pMK)
		{
			m_pMK = new CDictObject();
			m_pXref->Add(m_pMK);
			Add("MK", m_pMK);
		}

		if (!m_nBorderType)
		{
			CArrayObject* pArray = new CArrayObject();
			pArray->Add(0);
			m_pMK->Add("BC", pArray);
		}

		if (!m_bShd)
		{
			CArrayObject* pArray = new CArrayObject();
			pArray->Add(1);
			m_pMK->Add("BG", pArray);
		}
		
		CResourcesDict* pFieldsResources = m_pDocument->GetFieldsResources();

		const char* sFontName = pFieldsResources->GetFontName(pYesFont);
		if (!sFontName)
			return;

		std::string sDA;
		sDA.append(std::to_string(oColor.r));
		sDA.append(" ");
		sDA.append(std::to_string(oColor.g));
		sDA.append(" ");
		sDA.append(std::to_string(oColor.b));
		sDA.append(" rg /");
		sDA.append(sFontName);
		sDA.append(" ");
		sDA.append(std::to_string(0));
		sDA.append(" Tf");
		Add("DA", new CStringObject(sDA.c_str()));

		const char* sExtGrStateName = NULL;
		if (fabs(dAlpha - 1.0) > 0.001)
		{
			CExtGrState* pExtGrState = m_pDocument->GetFillAlpha(dAlpha);
			sExtGrStateName = pFieldsResources->GetExtGrStateName(pExtGrState);
		}


		pAppearance->GetYesN()->DrawSimpleText(wsYesValue, pYesCodes, unYesCount, pYesFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));
		pAppearance->GetOffN()->DrawSimpleText(wsOffValue, pOffCodes, unOffCount, pOffFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));

		pAppearance->GetYesD()->DrawSimpleText(wsYesValue, pYesCodes, unYesCount, pYesFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));
		pAppearance->GetOffD()->DrawSimpleText(wsOffValue, pOffCodes, unOffCount, pOffFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, fabs(m_oRect.fRight - m_oRect.fLeft), fabs(m_oRect.fBottom - m_oRect.fTop));
	}
	void CCheckBoxField::SetAppearance(const int& nType, const TRgb& oColor, const double& dAlpha, double dFontSize, double dX, double dY)
	{
		if (!m_pMK)
		{
			m_pMK = new CDictObject();
			m_pXref->Add(m_pMK);
			Add("MK", m_pMK);
		}

		CArrayObject* pArray = new CArrayObject();
		pArray->Add(3);
		m_pMK->Add("BC", pArray);

		pArray = new CArrayObject();
		pArray->Add(1);
		pArray->Add(1);
		pArray->Add(1);
		m_pMK->Add("BG", pArray);

		CCheckBoxAnnotAppearance* pAppearance = new CCheckBoxAnnotAppearance(m_pXref, this, m_sYesName.c_str());
		Add("AP", pAppearance);

		CFontDict* pFont = (CFontDict*)m_pDocument->GetDefaultCheckboxFont();
		CResourcesDict* pFieldsResources = m_pDocument->GetFieldsResources();

		const char* sFontName = pFieldsResources->GetFontName(pFont);
		if (!sFontName)
			return;

		std::string sDA;
		sDA.append(std::to_string(oColor.r));
		sDA.append(" ");
		sDA.append(std::to_string(oColor.g));
		sDA.append(" ");
		sDA.append(std::to_string(oColor.b));
		sDA.append(" rg /");
		sDA.append(sFontName);
		sDA.append(" ");
		sDA.append(std::to_string(0));
		sDA.append(" Tf");
		Add("DA", new CStringObject(sDA.c_str()));

		const char* sExtGrStateName = NULL;
		if (fabs(dAlpha - 1.0) > 0.001)
		{
			CExtGrState* pExtGrState = m_pDocument->GetFillAlpha(dAlpha);
			sExtGrStateName = pFieldsResources->GetExtGrStateName(pExtGrState);
		}

		double dW = fabs(m_oRect.fRight - m_oRect.fLeft);
		double dH = fabs(m_oRect.fBottom - m_oRect.fTop);

		dFontSize = dH * 31.014 / 33.8712;
		dX        = 5.4407 * dW / 37.119000;
		dY        = 6.4375 * dH / 33.8712;

		SetFieldBorder(border_subtype_Solid, TRgb(0.0, 0.0, 0.0), 1, 0, 0, 0);
		Remove("BS");

		if (1 == nType)
		{
			m_pMK->Add("CA", new CStringObject("4"));

			pAppearance->GetYesN()->DrawSimpleText(L"4", NULL, 0, pFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, dW, dH);
			pAppearance->GetOffN()->DrawSimpleText(L"", NULL, 0, pFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, dW, dH);
		}
		else if (2 == nType)
		{
			m_pMK->Add("CA", new CStringObject("1"));

			pAppearance->GetYesN()->DrawSimpleText(L"1", NULL, 0, pFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, dW, dH);
			pAppearance->GetOffN()->DrawSimpleText(L"", NULL, 0, pFont, dFontSize, dX, dY, oColor.r, oColor.g, oColor.b, sExtGrStateName, dW, dH);
		}

	}
	void CCheckBoxField::SetValue(const bool& isYes)
	{
		const char* sValue = (isYes ? m_sYesName.c_str() : "Off");
		Add("AS", sValue);
		Add("V", sValue);
	}	
	//----------------------------------------------------------------------------------------
	// CRadioGroupField
	//----------------------------------------------------------------------------------------
	CRadioGroupField::CRadioGroupField(CXref* pXref, CDocument* pDocument) : CFieldBase(pXref, pDocument)
	{
		Add("FT", "Btn");
		SetFlag(true, 1 << 14);
		SetFlag(true, 1 << 15);

		m_pKids = new CArrayObject();

		if (m_pKids)
			Add("Kids", m_pKids);
	}
	CCheckBoxField* CRadioGroupField::CreateKid(const wchar_t* wsChoiceName)
	{
		if (!m_pKids)
			return NULL;

		std::string sName = wsChoiceName ? NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsChoiceName) : "Choice" + std::to_string(m_pKids->GetCount() + 1);
		CCheckBoxField* pKid = new CCheckBoxField(m_pXref, m_pDocument, this, sName.c_str());
		m_pKids->Add(pKid);
		return pKid;
	}
	void CRadioGroupField::SetFieldName(const std::wstring& wsFieldName, bool isSkipCheck)
	{
		m_wsFieldName = wsFieldName;
		CFieldBase::SetFieldName(wsFieldName, true);
	}
	const std::wstring& CRadioGroupField::GetFieldName() const
	{
		return m_wsFieldName;
	}
	//----------------------------------------------------------------------------------------
	// CPictureField
	//----------------------------------------------------------------------------------------
	CPictureField::CPictureField(CXref* pXref, CDocument* pDocument) : CFieldBase(pXref, pDocument)
	{
		m_pMK = NULL;
		m_pIF = NULL;

		Add("FT", "Btn");
		SetFlag(true, 1 << 16);
		Add("H", "I");

		CDictObject* pAction = new CDictObject();		
		if (pAction)
		{
			pXref->Add(pAction);

			Add("A", pAction);

			pAction->Add("Type", "Action");
			pAction->Add("S", "JavaScript");
			pAction->Add("JS", new CStringObject("event.target.buttonImportIcon();"));
		}

		m_pMK = new CDictObject();
		if (!m_pMK)
			return;

		pXref->Add(m_pMK);
		Add("MK", m_pMK);

		m_pMK->Add("R", 0);
		m_pMK->Add("TP", 1);

		m_pIF = new CDictObject();
		if (!m_pIF)
			return;

		m_pMK->Add("IF", m_pIF);

		SetScaleType(EScaleType::Always);
		SetRespectBorders(false);
		SetConstantProportions(true);
		SetShift(0.5, 0.5);

		m_pResources = NULL;
	}
	void CPictureField::SetFieldName(const std::string& sName, bool isSkipCheck)
	{
		std::string _sName = sName + "_af_image";
		CFieldBase::SetFieldName(_sName, isSkipCheck);
	}
	void CPictureField::SetFieldName(const std::wstring& wsName, bool isSkipCheck)
	{
		std::string sName = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsName) + "_af_image";
		CFieldBase::SetFieldName(sName, isSkipCheck);
	}
	void CPictureField::SetAppearance(CImageDict* pImage)
	{
		CAnnotAppearance* pAppearance = new CAnnotAppearance(m_pXref, this);
		Add("AP", pAppearance);

		CAnnotAppearanceObject* pNormal = pAppearance->GetNormal();
		CResourcesDict* pFieldsResources = GetResourcesDict();

		std::string sDA = "0.909 0.941 0.992 rg";
		Add("DA", new CStringObject(sDA.c_str()));

		if (pImage)
		{			
			TRect oRect = GetRect();

			double dH = fabs(oRect.fTop - oRect.fBottom);
			double dW = fabs(oRect.fRight - oRect.fLeft);

			double dOriginW = pImage->GetWidth() * 72 / 96.0;
			double dOriginH = pImage->GetHeight() * 72 / 96.0;

			bool bNeedScale = (EScaleType::Always == m_eScaleType
				|| (EScaleType::Bigger == m_eScaleType && (dOriginH > dH || dOriginW > dW))
				|| (EScaleType::Smaller == m_eScaleType && dOriginH < dH && dOriginW < dW));

			double dDstW = dOriginW;
			double dDstH = dOriginH;
			double dDstX = 0;
			double dDstY = 0;

			if (m_bRespectBorders && HaveBorder())
			{
				double dBorderSize = GetBorderSize();
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

			CXObject* pForm = new CXObject();
			CStream* pStream = new CMemoryStream();
			pForm->SetStream(m_pXref, pStream);
			
#ifndef FILTER_FLATE_DECODE_DISABLED
			if (m_pDocument->GetCompressionMode() & COMP_TEXT)
				pForm->SetFilter(STREAM_FILTER_FLATE_DECODE);
#endif
			CArrayObject* pBBox = new CArrayObject();
			pForm->Add("BBox", pBBox);
			pBBox->Add(0);
			pBBox->Add(0);
			pBBox->Add(dOriginW);
			pBBox->Add(dOriginH);
			pForm->Add("FormType", 1);
			CArrayObject* pFormMatrix = new CArrayObject();
			pForm->Add("Matrix", pFormMatrix);
			pFormMatrix->Add(1);
			pFormMatrix->Add(0);
			pFormMatrix->Add(0);
			pFormMatrix->Add(1);
			pFormMatrix->Add(0);
			pFormMatrix->Add(0);
			pForm->Add("Name", "FRM");

			CDictObject* pFormRes = new CDictObject();
			CArrayObject* pFormResProcset = new CArrayObject();
			pFormRes->Add("ProcSet", pFormResProcset);
			pFormResProcset->Add(new CNameObject("PDF"));
			pFormResProcset->Add(new CNameObject("ImageC"));
			CDictObject* pFormResXObject = new CDictObject();
			pFormRes->Add("XObject", pFormResXObject);
			pFormResXObject->Add("Img", pImage);
			pForm->Add("Resources", pFormRes);

			pForm->Add("Subtype", "Form");
			pForm->Add("Type", "XObject");			

			pStream->WriteStr("q\012");
			pStream->WriteReal(dOriginW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(dOriginH);
			pStream->WriteStr(" 0 0 cm\012/Img Do\012Q");

			pFieldsResources->AddXObjectWithName("FRM", pForm);
			pNormal->DrawPicture("FRM", dDstX, dDstY, dDstW / dOriginW, dDstH / dOriginH, m_bRespectBorders);
		}
		else
		{
			pNormal->DrawPicture();
		}		
	}
	void CPictureField::SetScaleType(const EScaleType& eType)
	{
		if (!m_pIF)
			return;

		switch (eType)
		{
			case EScaleType::Always: m_pIF->Add("SW", "A"); break;
			case EScaleType::Bigger: m_pIF->Add("SW", "B"); break;
			case EScaleType::Smaller: m_pIF->Add("SW", "S"); break;
			case EScaleType::Never: m_pIF->Add("SW", "N"); break;
		}		

		m_eScaleType = eType;
	}
	void CPictureField::SetConstantProportions(const bool& bConstant)
	{
		if (!m_pIF)
			return;

		if (bConstant)
			m_pIF->Add("S", "P");
		else
			m_pIF->Add("S", "A");

		m_bConstantProportions = bConstant;
	}
	void CPictureField::SetRespectBorders(const bool& bRespectBorders)
	{
		if (!m_pIF)
			return;

		m_pIF->Add("FB", !bRespectBorders);

		m_bRespectBorders = bRespectBorders;
	}
	void CPictureField::SetShift(const double& dX, const double& dY)
	{
		if (!m_pIF)
			return;

		CArrayObject* pA = new CArrayObject();
		if (pA)
		{
			m_pIF->Add("A", pA);
			pA->Add(dX);
			pA->Add(dY);
		}

		m_dShiftX = dX;
		m_dShiftY = dY;
	}
	CResourcesDict* CPictureField::GetResourcesDict()
	{
		if (!m_pResources)
			m_pResources = new CResourcesDict(m_pXref, false, true);

		return m_pResources;
	}
	//----------------------------------------------------------------------------------------
	// CSignatureField
	//----------------------------------------------------------------------------------------
	CSignatureField::CSignatureField(CXref* pXref, CDocument* pDocument) : CFieldBase(pXref, pDocument)
	{
		// Словарь сигнатур
		m_pSig = new CSignatureDict(pXref);
		if (!m_pSig)
			return;
		Add("V", m_pSig);
		Add("FT", "Sig");
		m_pResources = NULL;
	}
	void CSignatureField::SetName(const std::wstring& wsValue)
	{
		if (!m_pSig || wsValue.empty())
			return;
		m_pSig->SetName(U_TO_UTF8(wsValue));
	}
	void CSignatureField::SetReason(const std::wstring &wsValue)
	{
		if (!m_pSig || wsValue.empty())
			return;
		m_pSig->SetReason(U_TO_UTF8(wsValue));
	}
	void CSignatureField::SetContact(const std::wstring &wsValue)
	{
		if (!m_pSig || wsValue.empty())
			return;
		m_pSig->SetContact(U_TO_UTF8(wsValue));
	}
	void CSignatureField::SetCert()
	{

	}
	void CSignatureField::SetDate(bool bDate)
	{
		if (!m_pSig || !bDate)
			return;
		m_pSig->SetDate();
	}
	void CSignatureField::SetAppearance(CImageDict* pImage)
	{
		CAnnotAppearance* pAppearance = new CAnnotAppearance(m_pXref, this);
		Add("AP", pAppearance);

		CAnnotAppearanceObject* pNormal = pAppearance->GetNormal();
		CResourcesDict* pFieldsResources = GetResourcesDict();

		std::string sDA = "0.909 0.941 0.992 rg";
		Add("DA", new CStringObject(sDA.c_str()));

		if (pImage)
		{
			TRect oRect = GetRect();

			double dH = fabs(oRect.fTop - oRect.fBottom);
			double dW = fabs(oRect.fRight - oRect.fLeft);

			double dOriginW = pImage->GetWidth() * 72 / 96.0;
			double dOriginH = pImage->GetHeight() * 72 / 96.0;

			double dDstW = dOriginW;
			double dDstH = dOriginH;
			double dDstX = 0;
			double dDstY = 0;

			if (HaveBorder())
			{
				double dBorderSize = GetBorderSize();
				dDstX += 2 * dBorderSize;
				dDstY += 2 * dBorderSize;
				dH -= 4 * dBorderSize;
				dW -= 4 * dBorderSize;
			}

			double dScaleKoef = fmin(dW / dOriginW, dH / dOriginH);
			dDstW = dScaleKoef * dOriginW;
			dDstH = dScaleKoef * dOriginH;

			dDstX += (dW - dDstW) * 0.5;
			dDstY += (dH - dDstH) * 0.5;

			CXObject* pForm = new CXObject();
			CStream* pStream = new CMemoryStream();
			pForm->SetStream(m_pXref, pStream);

#ifndef FILTER_FLATE_DECODE_DISABLED
			if (m_pDocument->GetCompressionMode() & COMP_TEXT)
				pForm->SetFilter(STREAM_FILTER_FLATE_DECODE);
#endif
			CArrayObject* pBBox = new CArrayObject();
			pForm->Add("BBox", pBBox);
			pBBox->Add(0);
			pBBox->Add(0);
			pBBox->Add(dOriginW);
			pBBox->Add(dOriginH);
			pForm->Add("FormType", 1);
			CArrayObject* pFormMatrix = new CArrayObject();
			pForm->Add("Matrix", pFormMatrix);
			pFormMatrix->Add(1);
			pFormMatrix->Add(0);
			pFormMatrix->Add(0);
			pFormMatrix->Add(1);
			pFormMatrix->Add(0);
			pFormMatrix->Add(0);
			pForm->Add("Name", "FRM");

			CDictObject* pFormRes = new CDictObject();
			CArrayObject* pFormResProcset = new CArrayObject();
			pFormRes->Add("ProcSet", pFormResProcset);
			pFormResProcset->Add(new CNameObject("PDF"));
			pFormResProcset->Add(new CNameObject("ImageC"));
			CDictObject* pFormResXObject = new CDictObject();
			pFormRes->Add("XObject", pFormResXObject);
			pFormResXObject->Add("Img", pImage);
			pForm->Add("Resources", pFormRes);

			pForm->Add("Subtype", "Form");
			pForm->Add("Type", "XObject");

			pStream->WriteStr("q\012");
			pStream->WriteReal(dOriginW);
			pStream->WriteStr(" 0 0 ");
			pStream->WriteReal(dOriginH);
			pStream->WriteStr(" 0 0 cm\012/Img Do\012Q");

			pFieldsResources->AddXObjectWithName("FRM", pForm);
			pNormal->DrawPicture("FRM", dDstX, dDstY, dDstW / dOriginW, dDstH / dOriginH, true);
		}
		else
		{
			pNormal->DrawPicture();
		}
	}
	CResourcesDict* CSignatureField::GetResourcesDict()
	{
		if (!m_pResources)
			m_pResources = new CResourcesDict(m_pXref, false, true);

		return m_pResources;
	}
	//----------------------------------------------------------------------------------------
	// CAnnotAppearance
	//----------------------------------------------------------------------------------------
	CDateTimeField::CDateTimeField(CXref* pXref, CDocument* pDocument) : CFieldBase(pXref, pDocument)
	{
		Add("FT", "Tx");
		SetFormat("d-mmm-yyyy");
	}
	void CDateTimeField::SetFormat(const std::wstring& wsFormat)
	{
		SetFormat(NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsFormat));
	}
	void CDateTimeField::SetFormat(const std::string& sFormat)
	{
		std::string script = "AFDate_FormatEx(\"" + sFormat + "\");";
		AddScriptToAA("F", script);
		script = "AFDate_KeystrokeEx(\"" + sFormat + "\");";
		AddScriptToAA("K", script);
	}
	//----------------------------------------------------------------------------------------
	// CAnnotAppearance
	//----------------------------------------------------------------------------------------
	CAnnotAppearance::CAnnotAppearance(CXref* pXref, CFieldBase* pField)
	{
		m_pXref  = pXref;
		m_pField = pField;
		m_pAnnot = NULL;

		m_pNormal   = new CAnnotAppearanceObject(pXref, pField);
		m_pRollover = NULL;
		m_pDown     = NULL;

		Add("N", m_pNormal);
	}
	CAnnotAppearance::CAnnotAppearance(CXref* pXref, CAnnotation* pAnnot)
	{
		m_pXref  = pXref;
		m_pAnnot = pAnnot;
		m_pField = NULL;

		m_pNormal   = NULL;
		m_pRollover = NULL;
		m_pDown     = NULL;
	}
	CAnnotAppearanceObject* CAnnotAppearance::GetNormal(CResourcesDict* pResources)
	{
		if (!m_pNormal)
		{
			if (m_pField)
				m_pNormal = new CAnnotAppearanceObject(m_pXref, m_pField);
			else if (m_pAnnot)
				m_pNormal = new CAnnotAppearanceObject(m_pXref, m_pAnnot, pResources);
			if (m_pXref)
				Add("N", m_pNormal);
		}

		return m_pNormal;
	}
	CAnnotAppearanceObject* CAnnotAppearance::GetRollover(CResourcesDict* pResources)
	{
		if (!m_pRollover)
		{
			if (m_pField)
				m_pRollover = new CAnnotAppearanceObject(m_pXref, m_pField);
			else if (m_pAnnot)
				m_pRollover = new CAnnotAppearanceObject(m_pXref, m_pAnnot, pResources);
			Add("R", m_pRollover);
		}

		return m_pRollover;
	}
	CAnnotAppearanceObject* CAnnotAppearance::GetDown(CResourcesDict* pResources)
	{
		if (!m_pDown)
		{
			if (m_pField)
				m_pDown = new CAnnotAppearanceObject(m_pXref, m_pField);
			else if (m_pAnnot)
				m_pDown = new CAnnotAppearanceObject(m_pXref, m_pAnnot, pResources);
			Add("D", m_pDown);
		}

		return m_pDown;
	}
	//----------------------------------------------------------------------------------------
	// CCheckBoxAnnotAppearance
	//----------------------------------------------------------------------------------------
	CCheckBoxAnnotAppearance::CCheckBoxAnnotAppearance(CXref* pXref, CFieldBase* pField, const char* sYesName)
	{
		m_pXref  = pXref;

		m_pYesN = new CAnnotAppearanceObject(pXref, pField);
		m_pOffN = new CAnnotAppearanceObject(pXref, pField);
		m_pYesD = new CAnnotAppearanceObject(pXref, pField);
		m_pOffD = new CAnnotAppearanceObject(pXref, pField);

		CDictObject* pDictN = new CDictObject();
		Add("N", pDictN);
		pDictN->Add(sYesName ? sYesName : "Yes", m_pYesN);
		pDictN->Add("Off", m_pOffN);
		
		CDictObject* pDictD = new CDictObject();
		Add("D", pDictD);
		pDictD->Add(sYesName ? sYesName : "Yes", m_pYesD);
		pDictD->Add("Off", m_pOffD);
	}
	CCheckBoxAnnotAppearance::CCheckBoxAnnotAppearance(CXref* pXref, CAnnotation* pAnnot, const char* sYesName)
	{
		m_pXref  = pXref;

		m_pYesN = new CAnnotAppearanceObject(pXref, pAnnot, new CResourcesDict(pXref, true, false));
		m_pOffN = new CAnnotAppearanceObject(pXref, pAnnot, new CResourcesDict(pXref, true, false));
		m_pYesD = new CAnnotAppearanceObject(pXref, pAnnot, new CResourcesDict(pXref, true, false));
		m_pOffD = new CAnnotAppearanceObject(pXref, pAnnot, new CResourcesDict(pXref, true, false));

		CDictObject* pDictN = new CDictObject();
		Add("N", pDictN);
		pDictN->Add(sYesName ? sYesName : "Yes", m_pYesN);
		pDictN->Add("Off", m_pOffN);

		CDictObject* pDictD = new CDictObject();
		Add("D", pDictD);
		pDictD->Add(sYesName ? sYesName : "Yes", m_pYesD);
		pDictD->Add("Off", m_pOffD);
	}
	CAnnotAppearanceObject* CCheckBoxAnnotAppearance::GetYesN()
	{
		return m_pYesN;
	}
	CAnnotAppearanceObject* CCheckBoxAnnotAppearance::GetOffN()
	{
		return m_pOffN;
	}
	CAnnotAppearanceObject* CCheckBoxAnnotAppearance::GetYesD()
	{
		return m_pYesD;
	}
	CAnnotAppearanceObject* CCheckBoxAnnotAppearance::GetOffD()
	{
		return m_pOffD;
	}
	//----------------------------------------------------------------------------------------
	// CAnnotAppearanceObject
	//----------------------------------------------------------------------------------------
	void CAnnotAppearanceObject::Init(CXref* pXref, CResourcesDict* pResources)
	{
		m_pXref     = pXref ? pXref : NULL;
		m_pFont     = NULL;
		m_dFontSize = 10.0;
		m_bStart    = true;

		if (m_pXref)
		{
			m_pStream = new CMemoryStream();
			SetStream(m_pXref, m_pStream);
#ifndef FILTER_FLATE_DECODE_DISABLED
			SetFilter(STREAM_FILTER_FLATE_DECODE);
#endif
		}

		Add("Type", "XObject");
		Add("Subtype", "Form");
		Add("Resources", pResources);
	}
	CAnnotAppearanceObject::CAnnotAppearanceObject(CXref* pXref, CFieldBase* pField)
	{
		Init(pXref, pField->GetResourcesDict());
		m_pField = pField;
		m_pAnnot = NULL;

		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return;

		Add("BBox", pArray);
		pArray->Add(0);
		pArray->Add(0);
		pArray->Add(fabs(pField->GetRect().fRight - pField->GetRect().fLeft));
		pArray->Add(fabs(pField->GetRect().fBottom - pField->GetRect().fTop));
	}
	CAnnotAppearanceObject::CAnnotAppearanceObject(CXref* pXRef, CAnnotation* pAnnot, CResourcesDict* pResources)
	{
		Init(pXRef, pResources ? pResources : pAnnot->GetDocument()->GetFieldsResources());
		m_pAnnot = pAnnot;
		m_pField = NULL;

		if (pAnnot->GetAnnotationType() == EAnnotType::AnnotWidget)
		{
			CArrayObject* pArray = new CArrayObject();
			if (!pArray)
				return;
			Add("BBox", pArray);

			pArray->Add(0);
			pArray->Add(0);
			pArray->Add(fabs(pAnnot->GetRect().fRight - pAnnot->GetRect().fLeft));
			pArray->Add(fabs(pAnnot->GetRect().fBottom - pAnnot->GetRect().fTop));
		}
	}
	void CAnnotAppearanceObject::DrawSimpleText(const std::wstring& wsText, unsigned short* pCodes, unsigned int unCount, CFontDict* pFont, double dFontSize, double dX, double dY, double dR, double dG, double dB, const char* sExtGStateName, double dWidth, double dHeight, CFontCidTrueType** ppFonts, double* pShifts)
	{
		if (!m_pStream || !pFont)
			return;

		StartDrawText(pFont, dFontSize, dR, dG, dB, sExtGStateName, dWidth, dHeight);

		if (pCodes)
			DrawTextLine(dX, dY, pCodes, unCount, ppFonts, pShifts);
		else
			DrawTextLine(dX, dY, wsText);

		EndDrawText();		
	}
	void CAnnotAppearanceObject::DrawPicture(const char* sImageName, const double& dX, const double& dY, const double& dImageW, const double& dImageH, const bool& bRespectBorder)
	{
		if (!m_pStream)
			return;

		CWidgetAnnotation* pAnnot = NULL;
		if (m_pAnnot)
			pAnnot = (CWidgetAnnotation*)m_pAnnot;

		double dW = 0, dH = 0;
		if (m_pField || pAnnot)
		{
			TRect oRect = m_pField ? m_pField->GetRect() : pAnnot->GetRect();
			dW = fabs(oRect.fRight - oRect.fLeft);
			dH = fabs(oRect.fBottom - oRect.fTop);
		}

		m_pStream->WriteStr("q\012");

		if ((m_pField && m_pField->HaveShd()) || (pAnnot && pAnnot->HaveBG()))
		{
			if (m_pField)
			{
				TRgb oColor = m_pField->GetShdColor();
				m_pStream->WriteReal(oColor.r);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(oColor.g);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(oColor.b);
				m_pStream->WriteStr(" rg\012");
			}
			else
			{
				m_pStream->WriteStr(pAnnot->GetBGforAP().c_str());
				m_pStream->WriteStr("\012");
			}

			m_pStream->WriteStr("1 0 0 1 0 0 cm\012");
			m_pStream->WriteStr("0 0 ");
			m_pStream->WriteReal(fmax(dW, 0.0));
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(fmax(dH, 0.0));
			m_pStream->WriteStr(" re\012f\012");
		}
		else if (!sImageName)
		{
			m_pStream->WriteStr("0.909 0.941 0.992 rg\0121 0 0 1 0 0 cm\012");
			m_pStream->WriteStr("0 0 ");
			m_pStream->WriteReal(fmax(dW, 0.0));
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(fmax(dH, 0.0));
			m_pStream->WriteStr(" re\012f\012");
		}

		if ((m_pField && m_pField->HaveBorder()) || (pAnnot && pAnnot->HaveBorder()))
		{
			double dBorderSize = m_pField ? m_pField->GetBorderSize() : pAnnot->GetBorderWidth();

			BYTE nType = 0;
			if (pAnnot)
			{
				nType = (BYTE)pAnnot->GetBorderType();
				switch (nType)
				{
				case 1: // Beveled
				case 3: // Inset
				{
					m_pStream->WriteStr(nType == 1 ? "1 g\012" : "0.501953 g\012");

					m_pStream->WriteReal(dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dBorderSize);
					m_pStream->WriteStr(" m\012");

					m_pStream->WriteReal(dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dH - dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(dW - dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dH - dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(dW - 2 * dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dH - 2 * dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(2 * dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dH - 2 * dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(2 * dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(2 * dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteStr("f\012");

					if (nType == 1 && pAnnot->HaveBG())
					{
						m_pStream->WriteStr(pAnnot->GetBGforAP(-0.25).c_str());
						m_pStream->WriteStr("\012");
					}
					else
						m_pStream->WriteStr("0.75293 g\012");

					m_pStream->WriteReal(dW - dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dH - dBorderSize);
					m_pStream->WriteStr(" m\012");

					m_pStream->WriteReal(dW - dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(2 * dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(2 * dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(dW - 2 * dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(2 * dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(dW - 2 * dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dH - 2 * dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteStr("f\012");
					break;
				}
				case 2: // Dashed
				{
					m_pStream->WriteStr(pAnnot->GetBorderDash().c_str());
					break;
				}
				default: break;
				}
			}

			if (m_pField)
			{
				TRgb oColor = m_pField->GetBorderColor();
				m_pStream->WriteReal(oColor.r);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(oColor.g);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(oColor.b);
				m_pStream->WriteStr(" RG\012");
			}
			else
			{
				m_pStream->WriteStr(pAnnot->GetBCforAP().c_str());
				m_pStream->WriteStr("\012");
			}

			m_pStream->WriteReal(dBorderSize);
			m_pStream->WriteStr(" w\0120 j\0120 J\012");

			if (nType == 4) // Underline
			{
				m_pStream->WriteInt(0);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(dBorderSize / 2);
				m_pStream->WriteStr(" m\012");

				m_pStream->WriteReal(dW);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(dBorderSize / 2);
				m_pStream->WriteStr(" l\012S\012");
			}
			else
			{
				m_pStream->WriteReal(dBorderSize / 2);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(dBorderSize / 2);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(std::max(dW - dBorderSize, 0.0));
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(std::max(dH - dBorderSize, 0.0));
				m_pStream->WriteStr(" re\012S\012");
			}

			if (bRespectBorder && sImageName)
			{
				m_pStream->WriteReal(2 * dBorderSize);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(2 * dBorderSize);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(fmax(dW - 4 * dBorderSize, 0.0));
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(fmax(dH - 4 * dBorderSize, 0.0));
				m_pStream->WriteStr(" re\012W\012n\012");
			}

		}
		else if (!sImageName)
		{
			m_pStream->WriteStr("0.909 0.941 0.992 RG\012");
			m_pStream->WriteStr("0.5 0.5 ");
			m_pStream->WriteReal(fmax(dW - 1, 0.0));
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(fmax(dH - 1, 0.0));
			m_pStream->WriteStr(" re\012s\012");
		}

		if (sImageName)
		{
			m_pStream->WriteReal(dImageW);
			m_pStream->WriteStr(" 0 0 ");
			m_pStream->WriteReal(dImageH);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dX);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dY);
			m_pStream->WriteStr(" cm\012");

			m_pStream->WriteEscapeName(sImageName);
			m_pStream->WriteStr(" Do\012");
		}

		m_pStream->WriteStr("Q");
	}	
	void CAnnotAppearanceObject::StartDrawText(CFontDict* pFont, const double& dFontSize, const double& dR, const double& dG, const double& dB, const char* sExtGStateName, const double& dWidth, const double& dHeight)
	{
		CResourcesDict* pResources = dynamic_cast<CResourcesDict*>(Get("Resources"));
		if (!m_pStream || !pResources)
			return;

		CWidgetAnnotation* pAnnot = NULL;
		if (m_pAnnot)
			pAnnot = (CWidgetAnnotation*)m_pAnnot;

		m_pStream->WriteEscapeName("Tx");
		m_pStream->WriteStr(" BMC\012");

		if ((m_pField && m_pField->HaveShd()) || (pAnnot && pAnnot->HaveBG()))
		{
			m_pStream->WriteStr("q\012");
			if (m_pField)
			{
				TRgb oColor = m_pField->GetShdColor();
				m_pStream->WriteReal(oColor.r);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(oColor.g);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(oColor.b);
				m_pStream->WriteStr(" rg\012");
			}
			else
			{
				m_pStream->WriteStr(pAnnot->GetBGforAP().c_str());
				m_pStream->WriteStr("\012");
			}

			m_pStream->WriteStr("1 0 0 1 0 0 cm\012");
			m_pStream->WriteStr("0 0 ");
			m_pStream->WriteReal(fmax(dWidth, 0.0));
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(fmax(dHeight, 0.0));
			m_pStream->WriteStr(" re\012f\012");
			m_pStream->WriteStr("Q\012");
		}

		double dBorderSize       = 0;
		double dBorderSizeStyle  = 0;

		if (pAnnot && pAnnot->HaveBorder())
		{
			dBorderSize      = pAnnot->GetBorderWidth();
			dBorderSizeStyle = dBorderSize;

			if (pAnnot->GetBorderType() == EBorderType::Beveled || pAnnot->GetBorderType() == EBorderType::Inset)
				dBorderSizeStyle *= 2;
		}

		if ((m_pField && m_pField->HaveBorder()) || (pAnnot && pAnnot->HaveBorder() && pAnnot->HaveBC()))
		{
			m_pStream->WriteStr("q\012");

			dBorderSize      = m_pField ? m_pField->GetBorderSize() : pAnnot->GetBorderWidth();
			dBorderSizeStyle = dBorderSize;

			BYTE nType = 0;
			if (pAnnot)
			{
				nType = (BYTE)pAnnot->GetBorderType();
				switch (nType)
				{
				case 1: // Beveled
				case 3: // Inset
				{
					dBorderSizeStyle *= 2;

					m_pStream->WriteStr(nType == 1 ? "1 g\012" : "0.501953 g\012");

					m_pStream->WriteReal(dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dBorderSize);
					m_pStream->WriteStr(" m\012");

					m_pStream->WriteReal(dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dHeight - dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(dWidth - dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dHeight - dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(dWidth - 2 * dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dHeight - 2 * dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(2 * dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dHeight - 2 * dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(2 * dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(2 * dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteStr("f\012");

					if (nType == 1 && pAnnot->HaveBG())
					{
						m_pStream->WriteStr(pAnnot->GetBGforAP(-0.25).c_str());
						m_pStream->WriteStr("\012");
					}
					else
						m_pStream->WriteStr("0.75293 g\012");

					m_pStream->WriteReal(dWidth - dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dHeight - dBorderSize);
					m_pStream->WriteStr(" m\012");

					m_pStream->WriteReal(dWidth - dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(2 * dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(2 * dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(dWidth - 2 * dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(2 * dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteReal(dWidth - 2 * dBorderSize);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(dHeight - 2 * dBorderSize);
					m_pStream->WriteStr(" l\012");

					m_pStream->WriteStr("f\012");
					break;
				}
				case 2: // Dashed
				{
					m_pStream->WriteStr(pAnnot->GetBorderDash().c_str());
					break;
				}
				default: break;
				}
			}

			if (m_pField)
			{
				TRgb oColor = m_pField->GetBorderColor();
				m_pStream->WriteReal(oColor.r);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(oColor.g);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(oColor.b);
				m_pStream->WriteStr(" RG\012");
			}
			else
			{
				m_pStream->WriteStr(pAnnot->GetBCforAP().c_str());
				m_pStream->WriteStr("\012");
			}

			m_pStream->WriteReal(dBorderSize);
			m_pStream->WriteStr(" w\0120 j\0120 J\012");

			if (nType == 4) // Underline
			{
				m_pStream->WriteInt(0);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(dBorderSize / 2);
				m_pStream->WriteStr(" m\012");

				m_pStream->WriteReal(dWidth);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(dBorderSize / 2);
				m_pStream->WriteStr(" l\012S\012");
			}
			else
			{
				m_pStream->WriteReal(dBorderSize / 2);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(dBorderSize / 2);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(std::max(dWidth - dBorderSize, 0.0));
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(std::max(dHeight - dBorderSize, 0.0));
				m_pStream->WriteStr(" re\012S\012");
			}

			CTextField* pTextField = dynamic_cast<CTextField*>(m_pField);
			CTextWidget* pAnnot = dynamic_cast<CTextWidget*>(m_pAnnot);
			if ((pTextField && pTextField->IsCombFlag()) || (pAnnot && pAnnot->IsCombFlag() && (nType == 0 || nType == 2)))
			{
				int nMaxLen = pTextField ? pTextField->GetMaxLen() : pAnnot->GetMaxLen();
				if (nMaxLen > 1)
				{
					double dStep = dWidth / nMaxLen;
					double dX = dStep;
					for (int nIndex = 0; nIndex < nMaxLen - 1; ++nIndex)
					{
						m_pStream->WriteReal(dX);
						m_pStream->WriteChar(' ');
						m_pStream->WriteInt(0);
						m_pStream->WriteChar(' ');
						m_pStream->WriteStr(" m\012");
						m_pStream->WriteReal(dX);
						m_pStream->WriteChar(' ');
						m_pStream->WriteReal(dHeight);
						m_pStream->WriteChar(' ');
						m_pStream->WriteStr(" l\012S\012");

						dX += dStep;
					}
				}
			}

			m_pStream->WriteStr("Q\012");
		}

		m_pStream->WriteStr("q\012");

		if (pAnnot && pAnnot->GetWidgetType() == WidgetPushbutton && !((CPushButtonWidget*)pAnnot)->GetRespectBorder())
		{
			m_pStream->WriteStr("0 0 ");
			m_pStream->WriteReal(std::max(dWidth, 0.0));
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(std::max(dHeight, 0.0));
		}
		else
		{
			m_pStream->WriteReal(dBorderSizeStyle);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dBorderSizeStyle);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(std::max(dWidth - dBorderSizeStyle * 2, 0.0));
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(std::max(dHeight - dBorderSizeStyle * 2, 0.0));
		}
		m_pStream->WriteStr(" re\012W\012n\012");

		if (pAnnot && pAnnot->GetWidgetType() == WidgetListbox)
		{
			CChoiceWidget* pAnnot = dynamic_cast<CChoiceWidget*>(m_pAnnot);
			double dBaseLine = pAnnot->GetListBoxHeight();
			std::vector<int> arrIndex = pAnnot->GetListBoxIndex();
			m_pStream->WriteStr("0.60 0.75 0.85 rg\012");
			for (int i = 0; i < arrIndex.size(); ++i)
			{
				double dH = dHeight - dBorderSizeStyle - dBaseLine * (double)(arrIndex[i] + 1);
				if (dH < 0)
					break;
				m_pStream->WriteReal(dBorderSizeStyle);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(dH);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(std::max(dWidth - dBorderSizeStyle * 2.0, 0.0));
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(dBaseLine);
				m_pStream->WriteStr(" re\012f\012");
			}
		}

		m_pStream->WriteStr("BT\012");

		m_dFontSize = std::min(1000.0, std::max(0.0, dFontSize));
		if (m_pField)
		{
			m_pStream->WriteReal(dR);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dG);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dB);
			m_pStream->WriteStr(" rg\012");

			if (sExtGStateName)
			{
				m_pStream->WriteEscapeName(sExtGStateName);
				m_pStream->WriteStr(" gs\012");
			}

			m_pStream->WriteEscapeName(pResources->GetFontName(pFont));
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(m_dFontSize);
			m_pStream->WriteStr(" Tf\012");
		}
		else if (pFont)
			m_pStream->WriteStr(pAnnot->GetDAforAP(pFont).c_str());

		m_bStart = true;
		m_pFont  = pFont;
	}
	void CAnnotAppearanceObject::StartDraw(const double& dWidth, const double& dHeight)
	{
		CWidgetAnnotation* pAnnot = dynamic_cast<CWidgetAnnotation*>(m_pAnnot);
		if (!m_pStream || !pAnnot)
			return;

		DrawBackground(dWidth, dHeight);
		DrawBorder(dWidth, dHeight);

		m_pStream->WriteStr("q\012");

		double dBorderSize = pAnnot->GetBorderWidth();
		EBorderType nType = pAnnot->GetBorderType();
		if (nType == EBorderType::Beveled || nType == EBorderType::Inset)
			dBorderSize *= 2;

		if (pAnnot->GetWidgetType() == WidgetPushbutton && !((CPushButtonWidget*)pAnnot)->GetRespectBorder())
			dBorderSize = 0;

		m_pStream->WriteReal(dBorderSize);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dBorderSize);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(std::max(dWidth  - dBorderSize * 2, 0.0));
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(std::max(dHeight - dBorderSize * 2, 0.0));
		m_pStream->WriteStr(" re\012W\012n\012");

		if (pAnnot && pAnnot->GetWidgetType() == WidgetListbox)
		{
			CChoiceWidget* pAnnot = dynamic_cast<CChoiceWidget*>(m_pAnnot);
			double dBaseLine = pAnnot->GetListBoxHeight();
			std::vector<int> arrIndex = pAnnot->GetListBoxIndex();
			m_pStream->WriteStr("0.60 0.75 0.85 rg\012");
			for (int i = 0; i < arrIndex.size(); ++i)
			{
				double dH = dHeight - dBorderSize - dBaseLine * (double)(arrIndex[i] + 1);
				if (dH < 0)
					break;
				m_pStream->WriteReal(dBorderSize);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(dH);
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(std::max(dWidth - dBorderSize * 2.0, 0.0));
				m_pStream->WriteChar(' ');
				m_pStream->WriteReal(dBaseLine);
				m_pStream->WriteStr(" re\012f\012");
			}
		}
	}
	void CAnnotAppearanceObject::StartText(CFontDict* pFont, const double& dFontSize)
	{
		CWidgetAnnotation* pAnnot  = dynamic_cast<CWidgetAnnotation*>(m_pAnnot);
		if (!m_pStream || !pAnnot)
			return;

		m_dFontSize = std::min(1000.0, std::max(0.0, dFontSize));
		m_pFont  = pFont;
		m_bStart = true;

		m_pStream->WriteStr("BT\012");
		if (pFont)
			m_pStream->WriteStr(pAnnot->GetDAforAP(pFont).c_str());
	}
	void CAnnotAppearanceObject::DrawPictureInline(const char* sImageName, const double& dX, const double& dY, const double& dW, const double& dH, const bool& bRespectBorder)
	{
		CWidgetAnnotation* pAnnot  = dynamic_cast<CWidgetAnnotation*>(m_pAnnot);
		if (!m_pStream || !pAnnot || !sImageName)
			return;

		m_pStream->WriteStr("q\012");

		if (bRespectBorder)
		{
			TRect oRect = pAnnot->GetRect();
			double dWidth  = fabs(oRect.fRight - oRect.fLeft);
			double dHeight = fabs(oRect.fBottom - oRect.fTop);

			double dBorderSize = pAnnot->GetBorderWidth();
			EBorderType nType = pAnnot->GetBorderType();
			if (nType == EBorderType::Beveled || nType == EBorderType::Inset)
				dBorderSize *= 2;

			m_pStream->WriteReal(2 * dBorderSize);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(2 * dBorderSize);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(fmax(dWidth  - 4 * dBorderSize, 0.0));
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(fmax(dHeight - 4 * dBorderSize, 0.0));
			m_pStream->WriteStr(" re\012W\012n\012");
		}

		m_pStream->WriteReal(dW);
		m_pStream->WriteStr(" 0 0 ");
		m_pStream->WriteReal(dH);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dX);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dY);
		m_pStream->WriteStr(" cm\012");

		m_pStream->WriteEscapeName(sImageName);
		m_pStream->WriteStr(" Do\012");

		m_pStream->WriteStr("Q\012");
	}
	void CAnnotAppearanceObject::EndText()
	{
		m_pStream->WriteStr("ET\012");
	}
	void CAnnotAppearanceObject::EndDraw()
	{
		m_pStream->WriteStr("Q\012");
	}
	void CAnnotAppearanceObject::DrawBackground(const double& dWidth, const double& dHeight)
	{
		CWidgetAnnotation* pAnnot  = dynamic_cast<CWidgetAnnotation*>(m_pAnnot);
		if (!pAnnot || !pAnnot->HaveBG())
			return;

		m_pStream->WriteStr("q\012");
		m_pStream->WriteStr(pAnnot->GetBGforAP().c_str());
		m_pStream->WriteStr("\012");
		m_pStream->WriteStr("1 0 0 1 0 0 cm\012");
		m_pStream->WriteStr("0 0 ");
		m_pStream->WriteReal(fmax(dWidth, 0.0));
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(fmax(dHeight, 0.0));
		m_pStream->WriteStr(" re\012f\012");
		m_pStream->WriteStr("Q\012");
	}
	void CAnnotAppearanceObject::DrawBorder(const double& dWidth, const double& dHeight)
	{
		CWidgetAnnotation* pAnnot  = dynamic_cast<CWidgetAnnotation*>(m_pAnnot);
		if (!pAnnot || !pAnnot->HaveBorder())
			return;

		m_pStream->WriteStr("q\012");

		double dBorderSize = pAnnot->GetBorderWidth();
		EBorderType nType = pAnnot->GetBorderType();

		switch (nType)
		{
		case EBorderType::Beveled:
		case EBorderType::Inset:
		{
			m_pStream->WriteStr(nType == EBorderType::Beveled ? "1 g\012" : "0.501953 g\012");

			m_pStream->WriteReal(dBorderSize);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dBorderSize);
			m_pStream->WriteStr(" m\012");

			m_pStream->WriteReal(dBorderSize);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dHeight - dBorderSize);
			m_pStream->WriteStr(" l\012");

			m_pStream->WriteReal(dWidth - dBorderSize);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dHeight - dBorderSize);
			m_pStream->WriteStr(" l\012");

			m_pStream->WriteReal(dWidth - 2 * dBorderSize);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dHeight - 2 * dBorderSize);
			m_pStream->WriteStr(" l\012");

			m_pStream->WriteReal(2 * dBorderSize);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dHeight - 2 * dBorderSize);
			m_pStream->WriteStr(" l\012");

			m_pStream->WriteReal(2 * dBorderSize);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(2 * dBorderSize);
			m_pStream->WriteStr(" l\012");

			m_pStream->WriteStr("f\012");

			if (nType == EBorderType::Beveled && pAnnot->HaveBG())
			{
				m_pStream->WriteStr(pAnnot->GetBGforAP(-0.25).c_str());
				m_pStream->WriteStr("\012");
			}
			else
				m_pStream->WriteStr("0.75293 g\012");

			m_pStream->WriteReal(dWidth - dBorderSize);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dHeight - dBorderSize);
			m_pStream->WriteStr(" m\012");

			m_pStream->WriteReal(dWidth - dBorderSize);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dBorderSize);
			m_pStream->WriteStr(" l\012");

			m_pStream->WriteReal(dBorderSize);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dBorderSize);
			m_pStream->WriteStr(" l\012");

			m_pStream->WriteReal(2 * dBorderSize);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(2 * dBorderSize);
			m_pStream->WriteStr(" l\012");

			m_pStream->WriteReal(dWidth - 2 * dBorderSize);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(2 * dBorderSize);
			m_pStream->WriteStr(" l\012");

			m_pStream->WriteReal(dWidth - 2 * dBorderSize);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dHeight - 2 * dBorderSize);
			m_pStream->WriteStr(" l\012");

			m_pStream->WriteStr("f\012");
			break;
		}
		case EBorderType::Dashed:
		{
			m_pStream->WriteStr(pAnnot->GetBorderDash().c_str());
			break;
		}
		default: break;
		}

		m_pStream->WriteStr(pAnnot->GetBCforAP().c_str());
		m_pStream->WriteStr("\012");
		m_pStream->WriteReal(dBorderSize);
		m_pStream->WriteStr(" w\0120 j\0120 J\012");

		if (nType == EBorderType::Underline)
		{
			m_pStream->WriteInt(0);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dBorderSize / 2);
			m_pStream->WriteStr(" m\012");

			m_pStream->WriteReal(dWidth);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dBorderSize / 2);
			m_pStream->WriteStr(" l\012S\012");
		}
		else
		{
			m_pStream->WriteReal(dBorderSize / 2);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dBorderSize / 2);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(std::max(dWidth - dBorderSize, 0.0));
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(std::max(dHeight - dBorderSize, 0.0));
			m_pStream->WriteStr(" re\012S\012");
		}

		m_pStream->WriteStr("Q\012");
	}
	void CAnnotAppearanceObject::DrawTextLine(const double& dX, const double& dY, const unsigned short* pCodes, const unsigned int& unCount, CFontCidTrueType** ppFonts, const double* pShifts)
	{
		CResourcesDict* pResources = dynamic_cast<CResourcesDict*>(Get("Resources"));
		if (!pResources)
			return;

		if (pCodes && pShifts)
		{
			double _dX = dX + pShifts[0], _dY = dY;

			if (!m_bStart)
			{
				_dX -= m_dCurX;
				_dY -= m_dCurY;

				m_dCurX += _dX;
				m_dCurY += _dY;
			}
			else
			{
				m_dCurX = _dX;
				m_dCurY = _dY;
			}

			m_pStream->WriteReal(_dX);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(_dY);
			m_pStream->WriteStr(" Td\012");

			for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
			{
				if (ppFonts)
				{
					CFontDict* pFont = (CFontDict*)(ppFonts[unIndex]);
					if (m_pFont != pFont)
					{
						m_pStream->WriteEscapeName(pResources->GetFontName((CFontDict*)(ppFonts[unIndex])));
						m_pStream->WriteChar(' ');
						m_pStream->WriteReal(m_dFontSize);
						m_pStream->WriteStr(" Tf\012");
						m_pFont = pFont;
					}
				}

				unsigned char unByte[2] = {static_cast<unsigned char>((pCodes[unIndex] >> 8) & 0xFF), static_cast<unsigned char>(pCodes[unIndex] & 0xFF)};
				m_pStream->WriteChar('[');
				m_pStream->WriteChar('<');
				m_pStream->WriteBinary(unByte, 2, NULL);
				m_pStream->WriteChar('>');
				m_pStream->WriteStr("]TJ\012");

				if (unIndex != unCount - 1)
				{
					m_pStream->WriteReal(pShifts[unIndex + 1]);
					m_pStream->WriteChar(' ');
					m_pStream->WriteReal(0.0);
					m_pStream->WriteStr(" Td\012");
				}
			}
		}
		else
		{
			double _dX = dX, _dY = dY;

			if (!m_bStart)
			{
				_dX -= m_dCurX;
				_dY -= m_dCurY;

				m_dCurX += _dX;
				m_dCurY += _dY;
			}
			else
			{
				m_dCurX = _dX;
				m_dCurY = _dY;
			}

			m_pStream->WriteReal(_dX);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(_dY);
			m_pStream->WriteStr(" Td\012");

			for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
			{
				if (ppFonts)
				{
					CFontDict* pFont = (CFontDict*)(ppFonts[unIndex]);
					if (m_pFont != pFont)
					{
						m_pStream->WriteEscapeName(pResources->GetFontName((CFontDict*)(ppFonts[unIndex])));
						m_pStream->WriteChar(' ');
						m_pStream->WriteReal(m_dFontSize);
						m_pStream->WriteStr(" Tf\012");
						m_pFont = pFont;
					}
				}

				unsigned char unByte[2] = {static_cast<unsigned char>((pCodes[unIndex] >> 8) & 0xFF), static_cast<unsigned char>(pCodes[unIndex] & 0xFF)};
				m_pStream->WriteChar('<');
				m_pStream->WriteBinary(unByte, 2, NULL);
				m_pStream->WriteChar('>');
				m_pStream->WriteStr(" Tj\012");
			}
		}

		m_bStart = false;
	}
	void CAnnotAppearanceObject::DrawTextLine(const double &dX, const double &dY, const std::wstring& wsText)
	{
		m_pStream->WriteReal(dX);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dY);
		m_pStream->WriteStr(" Td\012");

		std::string sText = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsText);
		m_pStream->WriteEscapeText((BYTE*)(sText.c_str()), sText.length());

		m_pStream->WriteStr(" Tj\012");
	}
	void CAnnotAppearanceObject::EndDrawText()
	{
		m_pStream->WriteStr("ET\012");
		m_pStream->WriteStr("Q\012EMC\012");
	}
	void CAnnotAppearanceObject::AddBBox(double dX, double dY, double dW, double dH)
	{
		CArrayObject* pArray = new CArrayObject();
		Add("BBox", pArray);
		pArray->Add(dX);
		pArray->Add(dY);
		pArray->Add(dW);
		pArray->Add(dH);
	}
	void CAnnotAppearanceObject::AddMatrix(double sx, double shy, double shx, double sy, double tx, double ty)
	{
		CArrayObject* pArray = new CArrayObject();
		Add("Matrix", pArray);
		pArray->Add(sx);
		pArray->Add(shy);
		pArray->Add(shx);
		pArray->Add(sy);
		pArray->Add(tx);
		pArray->Add(ty);
	}
	void CAnnotAppearanceObject::DrawTextCommentN(const std::string& sColor)
	{
		CExtGrState* pExtGrState = m_pAnnot->GetDocument()->GetExtGState(0.6, 0.6);
		const char* sExtGrStateName =  m_pAnnot->GetDocument()->GetFieldsResources()->GetExtGrStateName(pExtGrState);

		m_pStream->WriteStr("q 1 1 1 rg 0 i 1 w 4 M 1 j 0 J [] 0 d ");
		if (sExtGrStateName)
		{
			m_pStream->WriteEscapeName(sExtGrStateName);
			m_pStream->WriteStr(" gs ");
		}
		m_pStream->WriteStr("1 0 0 1 9 5.0908 cm 7.74 12.616 m -7.74 12.616 l -8.274 12.616 -8.707 12.184 -8.707 11.649 c -8.707 -3.831 l -8.707 -4.365 -8.274 -4.798 -7.74 -4.798 c ");
		m_pStream->WriteStr("7.74 -4.798 l 8.274 -4.798 8.707 -4.365 8.707 -3.831 c 8.707 11.649 l 8.707 12.184 8.274 12.616 7.74 12.616 c h f Q 0 G ");
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 i 0.60 w 4 M 1 j 0 J [0 100] 1 d 1 0 0 1 9 5.0908 cm 1 0 m -2.325 -2.81 l -2.325 0 l -5.72 0 l -5.72 8.94 l 5.51 8.94 l 5.51 0 l 1 0 l -3.50 5.01 m ");
		m_pStream->WriteStr("-3.50 5.59 l 3.29 5.59 l 3.29 5.01 l -3.50 5.01 l -3.50 3.34 m -3.50 3.92 l 2.27 3.92 l 2.27 3.34 l -3.50 3.34 l 7.74 12.616 m -7.74 12.616 l ");
		m_pStream->WriteStr("-8.274 12.616 -8.707 12.184 -8.707 11.649 c -8.707 -3.831 l -8.707 -4.365 -8.274 -4.798 -7.74 -4.798 c 7.74 -4.798 l ");
		m_pStream->WriteStr("8.274 -4.798 8.707 -4.365 8.707 -3.831 c 8.707 11.649 l 8.707 12.184 8.274 12.616 7.74 12.616 c b");
	}
	void CAnnotAppearanceObject::DrawTextCommentR(const std::string& sColor)
	{
		m_pStream->WriteStr("0 G ");
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 i 0.60 w 4 M 1 j 0 J [0 100] 1 d 1 0 0 1 9 5.0908 cm 4.1 1.71 m -0.54 -2.29 l -0.54 1.71 l -5.5 1.71 l -5.5 14.42 l 10.5 14.42 l 10.5 1.71 l 4.1 1.71 l ");
		m_pStream->WriteStr("-2.33 9.66 m 7.34 9.66 l 7.34 8.83 l -2.33 8.83 l -2.33 9.66 l -2.33 7.28 m 5.88 7.28 l 5.88 6.46 l -2.33 6.46 l -2.33 7.28 l 14.9 23.1235 m -14.9 23.1235 l ");
		m_pStream->WriteStr("-14.9 -20.345 l 14.9 -20.345 l 14.9 23.1235 l b");
	}
	void CAnnotAppearanceObject::DrawTextCheck(const std::string& sColor)
	{
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 7.1836 1.2061 cm 0 0 m 6.691 11.152 11.31 14.196 v 10.773 15.201 9.626 16.892 8.155 17.587 c ");
		m_pStream->WriteStr("2.293 10.706 -0.255 4.205 y -4.525 9.177 l -6.883 5.608 l h b");
	}
	void CAnnotAppearanceObject::DrawTextCheckmark()
	{
		m_pStream->WriteStr("q 0.396 0.396 0.396 rg 1 0 0 1 13.5151 16.5 cm 0 0 m -6.7 -10.23 l -8.81 -7 l -13.22 -7 l -6.29 -15 l 4.19 0 l h f Q");
	}
	void CAnnotAppearanceObject::DrawTextCircle(const std::string& sColor)
	{
		CExtGrState* pExtGrState = m_pAnnot->GetDocument()->GetExtGState(0.6, 0.6);
		const char* sExtGrStateName =  m_pAnnot->GetDocument()->GetFieldsResources()->GetExtGrStateName(pExtGrState);

		m_pStream->WriteStr("q 1 1 1 rg 0 i 1 w 4 M 1 j 0 J [] 0 d ");
		if (sExtGrStateName)
		{
			m_pStream->WriteEscapeName(sExtGrStateName);
			m_pStream->WriteStr(" gs ");
		}
		m_pStream->WriteStr("1 0 0 1 9.999 3.6387 cm 0 0 m -3.513 0 -6.36 2.85 -6.36 6.363 c -6.36 9.875 -3.513 12.724 0 12.724 c 3.514 12.724 6.363 9.875 6.363 6.363 c ");
		m_pStream->WriteStr("6.363 2.85 3.514 0 0 0 c h f Q ");
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 9.999 3.6387 cm 0 0 m -3.513 0 -6.36 2.85 -6.36 6.363 c -6.36 9.875 -3.513 12.724 0 12.724 c ");
		m_pStream->WriteStr("3.514 12.724 6.363 9.875 6.363 6.363 c 6.363 2.85 3.514 0 0 0 c 0 16.119 m -5.388 16.119 -9.756 11.751 -9.756 6.363 c -9.756 0.973 -5.388 -3.395 0 -3.395 c ");
		m_pStream->WriteStr("5.391 -3.395 9.757 0.973 9.757 6.363 c 9.757 11.751 5.391 16.119 0 16.119 c b");
	}
	void CAnnotAppearanceObject::DrawTextCross(const std::string& sColor)
	{
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 18.6924 3.1357 cm 0 0 m -6.363 6.364 l 0 12.728 l -2.828 15.556 l -9.192 9.192 l -15.556 15.556 l ");
		m_pStream->WriteStr("-18.384 12.728 l -12.02 6.364 l -18.384 0 l -15.556 -2.828 l -9.192 3.535 l -2.828 -2.828 l h b");
	}
	void CAnnotAppearanceObject::DrawTextCrossHairs(const std::string& sColor)
	{
		CExtGrState* pExtGrState = m_pAnnot->GetDocument()->GetExtGState(0.6, 0.6);
		const char* sExtGrStateName =  m_pAnnot->GetDocument()->GetFieldsResources()->GetExtGrStateName(pExtGrState);

		m_pStream->WriteStr("q 1 1 1 rg 0 i 1 w 4 M 1 j 0 J [] 0 d ");
		if (sExtGrStateName)
		{
			m_pStream->WriteEscapeName(sExtGrStateName);
			m_pStream->WriteStr(" gs ");
		}
		m_pStream->WriteStr("1 0 0 1 9.9771 1.9443 cm 0 0 m -4.448 0 -8.053 3.604 -8.053 8.053 c -8.053 12.5 -4.448 16.106 0 16.106 c 4.447 16.106 8.054 12.5 8.054 8.053 c ");
		m_pStream->WriteStr("8.054 3.604 4.447 0 0 0 c h f Q ");
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 G 0 i 0.61 w 4 M 0 j 0 J [] 0 d q 1 0 0 1 9.9771 1.9443 cm 0 0 m -4.448 0 -8.053 3.604 -8.053 8.053 c -8.053 12.5 -4.448 16.106 0 16.106 c ");
		m_pStream->WriteStr("4.447 16.106 8.054 12.5 8.054 8.053 c 8.054 3.604 4.447 0 0 0 c 0 17.716 m -5.336 17.716 -9.663 13.39 -9.663 8.053 c -9.663 2.716 -5.336 -1.61 0 -1.61 c ");
		m_pStream->WriteStr("5.337 -1.61 9.664 2.716 9.664 8.053 c 9.664 13.39 5.337 17.716 0 17.716 c b Q q 1 0 0 1 10.7861 14.8325 cm 0 0 m -1.611 0 l -1.611 -4.027 l -5.638 -4.027 l ");
		m_pStream->WriteStr("-5.638 -5.638 l -1.611 -5.638 l -1.611 -9.665 l 0 -9.665 l 0 -5.638 l 4.026 -5.638 l 4.026 -4.027 l 0 -4.027 l h b Q");
	}
	void CAnnotAppearanceObject::DrawTextHelp(const std::string& sColor)
	{
		CExtGrState* pExtGrState = m_pAnnot->GetDocument()->GetExtGState(0.6, 0.6);
		const char* sExtGrStateName =  m_pAnnot->GetDocument()->GetFieldsResources()->GetExtGrStateName(pExtGrState);

		m_pStream->WriteStr("q 1 1 1 rg 0 i 1 w 4 M 1 j 0 J [] 0 d ");
		if (sExtGrStateName)
		{
			m_pStream->WriteEscapeName(sExtGrStateName);
			m_pStream->WriteStr(" gs ");
		}
		m_pStream->WriteStr("1 0 0 1 12.1465 10.5137 cm -2.146 9.403 m -7.589 9.403 -12.001 4.99 -12.001 -0.453 c -12.001 -5.895 -7.589 -10.309 -2.146 -10.309 c ");
		m_pStream->WriteStr("3.296 -10.309 7.709 -5.895 7.709 -0.453 c 7.709 4.99 3.296 9.403 -2.146 9.403 c h f Q ");
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 12.1465 10.5137 cm 0 0 m -0.682 -0.756 -0.958 -1.472 -0.938 -2.302 c -0.938 -2.632 l -3.385 -2.632 l ");
		m_pStream->WriteStr("-3.403 -2.154 l -3.459 -1.216 -3.147 -0.259 -2.316 0.716 c -1.729 1.433 -1.251 2.022 -1.251 2.647 c -1.251 3.291 -1.674 3.715 -2.594 3.751 c ");
		m_pStream->WriteStr("-3.202 3.751 -3.937 3.531 -4.417 3.2 c -5.041 5.205 l -4.361 5.591 -3.274 5.959 -1.968 5.959 c 0.46 5.959 1.563 4.616 1.563 3.089 c ");
		m_pStream->WriteStr("1.563 1.691 0.699 0.771 0 0 c -2.227 -6.863 m -2.245 -6.863 l -3.202 -6.863 -3.864 -6.146 -3.864 -5.189 c -3.864 -4.196 -3.182 -3.516 -2.227 -3.516 c ");
		m_pStream->WriteStr("-1.233 -3.516 -0.589 -4.196 -0.57 -5.189 c -0.57 -6.146 -1.233 -6.863 -2.227 -6.863 c -2.146 9.403 m -7.589 9.403 -12.001 4.99 -12.001 -0.453 c ");
		m_pStream->WriteStr("-12.001 -5.895 -7.589 -10.309 -2.146 -10.309 c 3.296 -10.309 7.709 -5.895 7.709 -0.453 c 7.709 4.99 3.296 9.403 -2.146 9.403 c b");
	}
	void CAnnotAppearanceObject::DrawTextInsert(const std::string& sColor)
	{
		m_pStream->WriteStr("0 G ");
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 i 0.59 w 4 M 0 j 0 J [] 0 d 1 0 0 1 8.5386 19.8545 cm 0 0 m -8.39 -19.719 l 8.388 -19.719 l h B");
	}
	void CAnnotAppearanceObject::DrawTextKey(const std::string& sColor)
	{
		CExtGrState* pExtGrState = m_pAnnot->GetDocument()->GetExtGState(0.6, 0.6);
		const char* sExtGrStateName =  m_pAnnot->GetDocument()->GetFieldsResources()->GetExtGrStateName(pExtGrState);

		m_pStream->WriteStr("q 1 1 1 rg 0 i 1 w 4 M 1 j 0 J [] 0 d ");
		if (sExtGrStateName)
		{
			m_pStream->WriteEscapeName(sExtGrStateName);
			m_pStream->WriteStr(" gs ");
		}
		m_pStream->WriteStr("1 0 0 1 6.5 12.6729 cm 0.001 5.138 m -2.543 5.138 -4.604 3.077 -4.604 0.534 c -4.604 -1.368 -3.449 -3.001 -1.802 -3.702 c ");
		m_pStream->WriteStr("-1.802 -4.712 l -0.795 -5.719 l -1.896 -6.82 l -0.677 -8.039 l -1.595 -8.958 l -0.602 -9.949 l -1.479 -10.829 l -0.085 -12.483 l 1.728 -10.931 l ");
		m_pStream->WriteStr("1.728 -3.732 l 1.737 -3.728 1.75 -3.724 1.76 -3.721 c 3.429 -3.03 4.604 -1.385 4.604 0.534 c 4.604 3.077 2.542 5.138 0.001 5.138 c f Q ");
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 6.5 12.6729 cm 0 0 m -1.076 0 -1.95 0.874 -1.95 1.95 c -1.95 3.028 -1.076 3.306 0 3.306 c ");
		m_pStream->WriteStr("1.077 3.306 1.95 3.028 1.95 1.95 c 1.95 0.874 1.077 0 0 0 c 0.001 5.138 m -2.543 5.138 -4.604 3.077 -4.604 0.534 c -4.604 -1.368 -3.449 -3.001 -1.802 -3.702 c ");
		m_pStream->WriteStr("-1.802 -4.712 l -0.795 -5.719 l -1.896 -6.82 l -0.677 -8.039 l -1.595 -8.958 l -0.602 -9.949 l -1.479 -10.829 l -0.085 -12.483 l 1.728 -10.931 l 1.728 -3.732 l ");
		m_pStream->WriteStr("1.737 -3.728 1.75 -3.724 1.76 -3.721 c 3.429 -3.03 4.604 -1.385 4.604 0.534 c 4.604 3.077 2.542 5.138 0.001 5.138 c b");
	}
	void CAnnotAppearanceObject::DrawTextNewParagraph(const std::string& sColor)
	{
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d q 1 0 0 1 6.4995 20 cm 0 0 m -6.205 -12.713 l 6.205 -12.713 l h b Q q 1 0 0 1 1.1909 6.2949 cm 0 0 m 1.278 0 l ");
		m_pStream->WriteStr("1.353 0 1.362 -0.02 1.391 -0.066 c 2.128 -1.363 3.78 -4.275 3.966 -4.713 c 3.985 -4.713 l 3.976 -4.453 3.957 -3.91 3.957 -3.137 c 3.957 -0.076 l ");
		m_pStream->WriteStr("3.957 -0.02 3.976 0 4.041 0 c 4.956 0 l 5.021 0 5.04 -0.029 5.04 -0.084 c 5.04 -6.049 l 5.04 -6.113 5.021 -6.133 4.947 -6.133 c 3.695 -6.133 l ");
		m_pStream->WriteStr("3.621 -6.133 3.611 -6.113 3.574 -6.066 c 3.052 -4.955 1.353 -2.063 0.971 -1.186 c 0.961 -1.186 l 0.999 -1.68 0.999 -2.146 1.008 -3.025 c 1.008 -6.049 l ");
		m_pStream->WriteStr("1.008 -6.104 0.989 -6.133 0.933 -6.133 c 0.009 -6.133 l -0.046 -6.133 -0.075 -6.123 -0.075 -6.049 c -0.075 -0.066 l -0.075 -0.02 -0.056 0 0 0 c f Q q ");
		m_pStream->WriteStr("1 0 0 1 9.1367 3.0273 cm 0 0 m 0.075 0 0.215 -0.008 0.645 -0.008 c 1.4 -0.008 2.119 0.281 2.119 1.213 c 2.119 1.969 1.633 2.381 0.737 2.381 c ");
		m_pStream->WriteStr("0.354 2.381 0.075 2.371 0 2.361 c h -1.146 3.201 m -1.146 3.238 -1.129 3.268 -1.082 3.268 c -0.709 3.275 0.02 3.285 0.729 3.285 c ");
		m_pStream->WriteStr("2.613 3.285 3.248 2.314 3.258 1.232 c 3.258 -0.27 2.007 -0.914 0.607 -0.914 c 0.327 -0.914 0.057 -0.914 0 -0.904 c 0 -2.789 l ");
		m_pStream->WriteStr("0 -2.836 -0.019 -2.865 -0.074 -2.865 c -1.082 -2.865 l -1.119 -2.865 -1.146 -2.846 -1.146 -2.799 c h f Q");
	}
	void CAnnotAppearanceObject::DrawTextNote(const std::string& sColor)
	{
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 G 0 i 0.61 w 4 M 0 j 0 J [] 0 d q 1 0 0 1 16.959 1.3672 cm 0 0 m 0 -0.434 -0.352 -0.785 -0.784 -0.785 c -14.911 -0.785 l ");
		m_pStream->WriteStr("-15.345 -0.785 -15.696 -0.434 -15.696 0 c -15.696 17.266 l -15.696 17.699 -15.345 18.051 -14.911 18.051 c -0.784 18.051 l -0.352 18.051 0 17.699 0 17.266 c ");
		m_pStream->WriteStr("h b Q q 1 0 0 1 4.4023 13.9243 cm 0 0 m 9.418 0 l S Q q 1 0 0 1 4.4019 11.2207 cm 0 0 m 9.418 0 l S Q q 1 0 0 1 4.4023 8.5176 cm 0 0 m 9.418 0 l S Q q ");
		m_pStream->WriteStr("1 0 0 1 4.4023 5.8135 cm 0 0 m 9.418 0 l S Q");
	}
	void CAnnotAppearanceObject::DrawTextParagraph(const std::string& sColor)
	{
		CExtGrState* pExtGrState = m_pAnnot->GetDocument()->GetExtGState(0.6, 0.6);
		const char* sExtGrStateName =  m_pAnnot->GetDocument()->GetFieldsResources()->GetExtGrStateName(pExtGrState);

		m_pStream->WriteStr("q 1 1 1 rg 0 i 1 w 4 M 1 j 0 J [] 0 d ");
		if (sExtGrStateName)
		{
			m_pStream->WriteEscapeName(sExtGrStateName);
			m_pStream->WriteStr(" gs ");
		}
		m_pStream->WriteStr("1 0 0 1 19.6973 10.0005 cm 0 0 m 0 -5.336 -4.326 -9.662 -9.663 -9.662 c -14.998 -9.662 -19.324 -5.336 -19.324 0 c -19.324 5.335 -14.998 9.662 -9.663 9.662 c ");
		m_pStream->WriteStr("-4.326 9.662 0 5.335 0 0 c h f Q ");
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d q 1 0 0 1 19.6973 10.0005 cm 0 0 m 0 -5.336 -4.326 -9.662 -9.663 -9.662 c -14.998 -9.662 -19.324 -5.336 -19.324 0 c ");
		m_pStream->WriteStr("-19.324 5.335 -14.998 9.662 -9.663 9.662 c -4.326 9.662 0 5.335 0 0 c h S Q q 1 0 0 1 11.6787 2.6582 cm 0 0 m -1.141 0 l -1.227 0 -1.244 0.052 -1.227 0.139 c ");
		m_pStream->WriteStr("-0.656 1.157 -0.52 2.505 -0.52 3.317 c -0.52 3.594 l -2.833 3.783 -5.441 4.838 -5.441 8.309 c -5.441 10.778 -3.714 12.626 -0.57 13.024 c ");
		m_pStream->WriteStr("-0.535 13.508 -0.381 14.129 -0.242 14.389 c -0.207 14.44 -0.174 14.475 -0.104 14.475 c 1.088 14.475 l 1.156 14.475 1.191 14.458 1.175 14.372 c ");
		m_pStream->WriteStr("1.105 14.095 0.881 13.127 0.881 12.402 c 0.881 9.431 0.932 7.324 0.95 4.06 c 0.95 2.298 0.708 0.813 0.189 0.07 c 0.155 0.034 0.103 0 0 0 c b Q");
	}
	void CAnnotAppearanceObject::DrawTextRightArrow(const std::string& sColor)
	{
		CExtGrState* pExtGrState = m_pAnnot->GetDocument()->GetExtGState(0.6, 0.6);
		const char* sExtGrStateName =  m_pAnnot->GetDocument()->GetFieldsResources()->GetExtGrStateName(pExtGrState);

		m_pStream->WriteStr("q 1 1 1 rg 0 i 1 w 4 M 1 j 0 J [] 0 d ");
		if (sExtGrStateName)
		{
			m_pStream->WriteEscapeName(sExtGrStateName);
			m_pStream->WriteStr(" gs ");
		}
		m_pStream->WriteStr("1 0 0 1 3.7856 11.1963 cm 6.214 -10.655 m 11.438 -10.655 15.673 -6.42 15.673 -1.196 c 15.673 4.027 11.438 8.262 6.214 8.262 c ");
		m_pStream->WriteStr("0.991 8.262 -3.244 4.027 -3.244 -1.196 c -3.244 -6.42 0.991 -10.655 6.214 -10.655 c h f Q ");
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 G 0 i 0.59 w 4 M 0 j 0 J [] 0 d 1 0 0 1 3.7856 11.1963 cm 0 0 m 8.554 0 l 6.045 2.51 l 7.236 3.702 l 12.135 -1.197 l 7.236 -6.096 l 6.088 -4.949 l ");
		m_pStream->WriteStr("8.644 -2.394 l 0 -2.394 l h 6.214 -10.655 m 11.438 -10.655 15.673 -6.42 15.673 -1.196 c 15.673 4.027 11.438 8.262 6.214 8.262 c ");
		m_pStream->WriteStr("0.991 8.262 -3.244 4.027 -3.244 -1.196 c -3.244 -6.42 0.991 -10.655 6.214 -10.655 c b");
	}
	void CAnnotAppearanceObject::DrawTextRightPointer(const std::string& sColor)
	{
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 G 0.59 w 4 M 0 j 0 J [] 0 d 1 0 0 1 1.1871 17.0000 cm 0 0 m 4.703 -8.703 l 0 -17 l 18.813 -8.703 l b");
	}
	void CAnnotAppearanceObject::DrawTextStar(const std::string& sColor)
	{
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 9.999 18.8838 cm 0 0 m 3.051 -6.178 l 9.867 -7.168 l 4.934 -11.978 l 6.099 -18.768 l 0 -15.562 l -6.097 -18.768 l ");
		m_pStream->WriteStr("-4.933 -11.978 l -9.866 -7.168 l -3.048 -6.178 l b");
	}
	void CAnnotAppearanceObject::DrawTextUpArrow(const std::string& sColor)
	{
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 1.1007 6.7185 cm 0 0 m 4.009 0 l 4.009 -6.719 l 11.086 -6.719 l 11.086 0 l 14.963 0 l 7.499 13.081 l b");
	}
	void CAnnotAppearanceObject::DrawTextUpLeftArrow(const std::string& sColor)
	{
		m_pStream->WriteStr(sColor.c_str());
		m_pStream->WriteStr(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 2.8335 1.7627 cm 0 0 m -2.74 15.16 l 12.345 12.389 l 9.458 9.493 l 14.027 4.91 l 7.532 -1.607 l 2.964 2.975 l b");
	}
	void CAnnotAppearanceObject::DrawLine()
	{
		AddBBox(m_pAnnot->GetRect().fLeft, m_pAnnot->GetRect().fBottom, m_pAnnot->GetRect().fRight, m_pAnnot->GetRect().fTop);
		AddMatrix(1, 0, 0, 1, -m_pAnnot->GetRect().fLeft, -m_pAnnot->GetRect().fBottom);

		if (m_pAnnot->GetBorderType() == EBorderType::Dashed)
			m_pStream->WriteStr(m_pAnnot->GetBorderDash().c_str());

		double dBorderSize = m_pAnnot->GetBorderWidth();
		m_pStream->WriteReal(dBorderSize);
		m_pStream->WriteStr(" w\012");

		m_pStream->WriteStr(m_pAnnot->GetColorName("IC", false).c_str());

		m_pStream->WriteStr(m_pAnnot->GetColorName("C", true).c_str());

		CObjectBase* pObj = m_pAnnot->Get("CA");
		if (pObj && pObj->GetType() == object_type_REAL)
		{
			float dAlpha = ((CRealObject*)pObj)->Get();
			if (dAlpha != 1)
			{
				CExtGrState* pExtGrState = m_pAnnot->GetDocument()->GetExtGState(dAlpha, dAlpha);
				const char* sExtGrStateName =  m_pAnnot->GetDocument()->GetFieldsResources()->GetExtGrStateName(pExtGrState);
				if (sExtGrStateName)
				{
					m_pStream->WriteEscapeName(sExtGrStateName);
					m_pStream->WriteStr(" gs\012");
				}
			}
		}

		double dLL = 0, dLLE = 0, dLLO = 0;
		pObj = m_pAnnot->Get("LL");
		if (pObj && pObj->GetType() == object_type_REAL)
			dLL = ((CRealObject*)pObj)->Get();
		pObj = m_pAnnot->Get("LLE");
		if (pObj && pObj->GetType() == object_type_REAL)
			dLLE = ((CRealObject*)pObj)->Get();
		pObj = m_pAnnot->Get("LLO");
		if (pObj && pObj->GetType() == object_type_REAL)
			dLLO = ((CRealObject*)pObj)->Get();

		CLineAnnotation* pAnnot = (CLineAnnotation*)m_pAnnot;
		DrawLineArrow(m_pStream, dBorderSize, pAnnot->dL[0], pAnnot->dL[1], pAnnot->dL[2], pAnnot->dL[3], pAnnot->m_nLE1, pAnnot->m_nLE2, dLL, dLLE, dLLO);
	}
	void CAnnotAppearanceObject::DrawCheckBoxCircle(bool bSet, bool bN)
	{
		CCheckBoxWidget* pAnnot = dynamic_cast<CCheckBoxWidget*>(m_pAnnot);
		if (!pAnnot)
			return;

		double dBorder = 1;
		EBorderType nBorderType = EBorderType::Inset;
		if (m_pAnnot->HaveBorder())
		{
			dBorder = m_pAnnot->GetBorderWidth();
			nBorderType = m_pAnnot->GetBorderType();
		}
		double dW = m_pAnnot->GetRect().fRight - m_pAnnot->GetRect().fLeft;
		double dH = std::abs(m_pAnnot->GetRect().fBottom - m_pAnnot->GetRect().fTop);
		double dCX = dW / 2.0, dCY = dH / 2.0;
		double dR = std::min(dW, dH) / 2.0;

		// Задний фон
		std::string sBG;
		if (!bN && nBorderType != EBorderType::Beveled)
		{
			sBG = pAnnot->GetBGforAP();
			if (sBG == "1 g")
				sBG = "0.749023 -0.250977 -0.250977 rg";
			else if (sBG.empty())
				sBG = "0.749023 g";
			else
				sBG = pAnnot->GetBGforAP(-0.250977);
		}
		else
			sBG = pAnnot->GetBGforAP();
		if (!sBG.empty())
		{
			m_pStream->WriteStr(sBG.c_str());
			m_pStream->WriteStr("\012q\012");
			m_pStream->WriteStr("1 0 0 1 ");
			m_pStream->WriteReal(dCX);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dCY);
			m_pStream->WriteStr(" cm\012");
			StreamWriteCircle(m_pStream, 0, 0, dR);
			m_pStream->WriteStr("f\012Q\012");
		}

		// Граница
		if (dBorder != 1)
		{
			m_pStream->WriteReal(dBorder);
			m_pStream->WriteStr(" w\012");
		}
		if (nBorderType == EBorderType::Dashed)
			m_pStream->WriteStr(m_pAnnot->GetBorderDash().c_str());
		m_pStream->WriteStr(pAnnot->GetBCforAP().c_str());
		m_pStream->WriteStr("\012q\012");
		m_pStream->WriteStr("1 0 0 1 ");
		m_pStream->WriteReal(dCX);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dCY);
		m_pStream->WriteStr(" cm\012");

		StreamWriteCircle(m_pStream, 0, 0, dR - dBorder / 2.0);
		m_pStream->WriteStr("s\012Q\012");

		if (nBorderType == EBorderType::Beveled || nBorderType == EBorderType::Inset)
		{
			double ca = cos(45.0 / 180.0 * M_PI);
			double cx = 0, cy = 0, r = dR - dBorder * 1.5;
			double bezierCircle = 0.55228475 * r;

			if (nBorderType == EBorderType::Inset)
				m_pStream->WriteStr(bN ? "0.501953 G" : "0 G");
			else // Beveled
				m_pStream->WriteStr(bN ? "1 G" : pAnnot->GetBGforAP(-0.250977, true).c_str());

			m_pStream->WriteStr("\012q\012");
			StreamWriteCM(m_pStream, ca, ca, -ca, ca, dCX, dCY);
			StreamWriteXYMove(m_pStream, cx + r, cy);
			StreamWriteXYCurve(m_pStream, cx + r, cy + bezierCircle, cx + bezierCircle, cy + r, cx, cy + r);
			StreamWriteXYCurve(m_pStream, cx - bezierCircle, cy + r, cx - r, cy + bezierCircle, cx - r, cy);
			m_pStream->WriteStr("S\012Q\012");

			if (nBorderType == EBorderType::Inset)
				m_pStream->WriteStr(bN ? "0.75293 G" : "1 G");
			else // Beveled
				m_pStream->WriteStr(bN ? pAnnot->GetBGforAP(-0.250977, true).c_str() : "1 G");

			m_pStream->WriteStr("\012q\012");
			StreamWriteCM(m_pStream, ca, ca, -ca, ca, dCX, dCY);
			StreamWriteXYMove(m_pStream, cx - r, cy);
			StreamWriteXYCurve(m_pStream, cx - r, cy - bezierCircle, cx - bezierCircle, cy - r, cx, cy - r);
			StreamWriteXYCurve(m_pStream, cx + bezierCircle, cy - r, cx + r, cy - bezierCircle, cx + r, cy);
			m_pStream->WriteStr("S\012Q\012");
		}

		// Установлен
		if (!bSet)
			return;
		double dShift = dBorder / 2.0;
		if (nBorderType == EBorderType::Beveled || nBorderType == EBorderType::Inset)
			dShift *= 2.0;
		m_pStream->WriteStr("0 g\012q\012");
		m_pStream->WriteStr("1 0 0 1 ");
		m_pStream->WriteReal(dCX);
		m_pStream->WriteChar(' ');
		m_pStream->WriteReal(dCY);
		m_pStream->WriteStr(" cm\012");
		StreamWriteCircle(m_pStream, 0, 0, dR / 2.0 - dShift);
		m_pStream->WriteStr("f\012Q\012");
	}
	void CAnnotAppearanceObject::DrawCheckBoxSquare(bool bSet, bool bN)
	{
		CCheckBoxWidget* pAnnot = dynamic_cast<CCheckBoxWidget*>(m_pAnnot);
		if (!pAnnot)
			return;

		double dBorder = 1;
		EBorderType nBorderType = EBorderType::Inset;
		if (m_pAnnot->HaveBorder())
		{
			dBorder = m_pAnnot->GetBorderWidth();
			nBorderType = m_pAnnot->GetBorderType();
		}
		double dW = m_pAnnot->GetRect().fRight - m_pAnnot->GetRect().fLeft;
		double dH = std::abs(m_pAnnot->GetRect().fBottom - m_pAnnot->GetRect().fTop);

		// Задний фон
		m_pStream->WriteStr("q\012");
		std::string sBG;
		if (!bN && nBorderType != EBorderType::Beveled)
		{
			sBG = pAnnot->GetBGforAP(-0.250977);
			if (sBG.empty())
				sBG = "0.749023 g";
		}
		else
			sBG = pAnnot->GetBGforAP();
		if (!sBG.empty())
		{
			m_pStream->WriteStr(sBG.c_str());
			m_pStream->WriteStr("\012");
			StreamWriteRect(m_pStream, 0, 0, dW, dH);
			m_pStream->WriteStr("f\012");
		}

		// Граница
		if (nBorderType == EBorderType::Beveled || nBorderType == EBorderType::Inset)
		{
			if (nBorderType == EBorderType::Inset)
				m_pStream->WriteStr(bN ? "0.501953 g" : "0 g");
			else // Beveled
				m_pStream->WriteStr(bN ? "1 g" : pAnnot->GetBGforAP(-0.250977).c_str());
			m_pStream->WriteStr("\012");

			StreamWriteXYMove(m_pStream, dBorder, dBorder);
			StreamWriteXYLine(m_pStream, dBorder, dH - dBorder);
			StreamWriteXYLine(m_pStream, dW - dBorder, dH - dBorder);
			StreamWriteXYLine(m_pStream, dW - dBorder * 2.0, dH - dBorder * 2.0);
			StreamWriteXYLine(m_pStream, dBorder * 2.0, dH - dBorder * 2.0);
			StreamWriteXYLine(m_pStream, dBorder * 2.0, dBorder * 2.0);
			m_pStream->WriteStr("f\012");

			if (nBorderType == EBorderType::Inset)
				m_pStream->WriteStr(bN ? "0.75293 g" : "1 g");
			else // Beveled
				m_pStream->WriteStr(bN ? pAnnot->GetBGforAP(-0.250977).c_str() : "1 g");
			m_pStream->WriteStr("\012");

			StreamWriteXYMove(m_pStream, dW - dBorder, dH - dBorder);
			StreamWriteXYLine(m_pStream, dW - dBorder, dBorder);
			StreamWriteXYLine(m_pStream, dBorder, dBorder);
			StreamWriteXYLine(m_pStream, dBorder * 2.0, dBorder * 2.0);
			StreamWriteXYLine(m_pStream, dW - dBorder * 2.0, dBorder * 2.0);
			StreamWriteXYLine(m_pStream, dW - dBorder * 2.0, dH - dBorder * 2.0);
			m_pStream->WriteStr("f\012");
		}

		if (dBorder != 1)
		{
			m_pStream->WriteReal(dBorder);
			m_pStream->WriteStr(" w\012");
		}
		if (nBorderType == EBorderType::Dashed)
			m_pStream->WriteStr(m_pAnnot->GetBorderDash().c_str());
		m_pStream->WriteStr(pAnnot->GetBCforAP().c_str());
		m_pStream->WriteStr("\012");

		if (nBorderType == EBorderType::Underline)
		{
			StreamWriteXYMove(m_pStream, 0, dBorder / 2.0);
			StreamWriteXYLine(m_pStream, dW, dBorder / 2.0);
		}
		else
			StreamWriteRect(m_pStream, dBorder / 2.0, dBorder / 2.0, dW - dBorder, dH - dBorder);
		m_pStream->WriteStr("s\012Q\012");

		// Установлен
		if (!bSet)
			return;
		double dDiff = std::abs(dW - dH) / 2.0;
		double dShift = dBorder;
		if (nBorderType == EBorderType::Beveled || nBorderType == EBorderType::Inset)
			dShift *= 2;
		bool bW = dW > dH;
		double dCX = dW / 2.0, dCY = dH / 2.0;
		double dC = std::min(dW, dH) / 2.0;

		ECheckBoxStyle nStyle = pAnnot->GetStyle();
		switch (nStyle)
		{
		case ECheckBoxStyle::Check:
		{
			m_pStream->WriteStr("0 g\012q\012");
			m_pStream->WriteStr("1 0 0 1 ");
			m_pStream->WriteReal((bW ? dDiff : 0) + dShift);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal((bW ? 0 : dDiff) + dShift);
			m_pStream->WriteStr(" cm\012");

			double dScale = (std::min(dW, dH) - dShift * 2.0) / 20.0;
			StreamWriteXYMove(m_pStream, 5.2381 * dScale, 11.2 * dScale);
			StreamWriteXYLine(m_pStream, 4 * dScale, 8.2 * dScale);
			StreamWriteXYLine(m_pStream, 7.71429 * dScale, 4 * dScale);
			StreamWriteXYCurve(m_pStream, 12.0476 * dScale, 10.6 * dScale, 13.2857 * dScale, 11.8 * dScale, 17 * dScale, 16 * dScale);
			StreamWriteXYCurve(m_pStream, 14.5238 * dScale, 16 * dScale, 9.77778 * dScale, 11.2 * dScale, 7.71429 * dScale, 8.2 * dScale);
			StreamWriteXYLine(m_pStream, 5.2381 * dScale, 11.2 * dScale);

			m_pStream->WriteStr("f\012Q\012");
			break;
		}
		case ECheckBoxStyle::Cross:
		{
			double dCross = dBorder;
			if (nBorderType == EBorderType::Beveled || nBorderType == EBorderType::Inset)
				dCross *= 2;
			m_pStream->WriteStr("q\012");
			StreamWriteRect(m_pStream, dCross, dCross, dW - dCross, dH - dCross);
			m_pStream->WriteStr("W\012n\0120 G\0121 w\012");

			double x1 = dShift + 1 + (bW ? dDiff : 0);
			double y1 = dShift + 1 + (bW ? 0 : dDiff);
			double x2 = dW - dShift - 1 - (bW ? dDiff : 0);
			double y2 = dH - dShift - 1 - (bW ? 0 : dDiff);
			StreamWriteXYMove(m_pStream, x1, y2);
			StreamWriteXYLine(m_pStream, x2, y1);
			StreamWriteXYMove(m_pStream, x2, y2);
			StreamWriteXYLine(m_pStream, x1, y1);

			m_pStream->WriteStr("s\012Q\012");
			break;
		}
		case ECheckBoxStyle::Diamond:
		{
			double dSq = dC - dShift * 2.0 - 1;
			m_pStream->WriteStr("0 g\012q\012");
			m_pStream->WriteStr("1 0 0 1 ");
			m_pStream->WriteReal(dCX);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dCY);
			m_pStream->WriteStr(" cm\012");
			StreamWriteXYMove(m_pStream, -dSq, 0);
			StreamWriteXYLine(m_pStream, 0, dSq);
			StreamWriteXYLine(m_pStream, dSq, 0);
			StreamWriteXYLine(m_pStream, 0, -dSq);
			m_pStream->WriteStr("f\012Q\012");
			break;
		}
		case ECheckBoxStyle::Circle:
		{
			double dR = dC - dShift * 2.0 - 1;

			m_pStream->WriteStr("0 g\012q\012");
			m_pStream->WriteStr("1 0 0 1 ");
			m_pStream->WriteReal(dCX);
			m_pStream->WriteChar(' ');
			m_pStream->WriteReal(dCY);
			m_pStream->WriteStr(" cm\012");
			StreamWriteCircle(m_pStream, 0, 0, dR);
			m_pStream->WriteStr("f\012Q\012");
			break;
		}
		case ECheckBoxStyle::Star:
		{
			double dROuter = dC - dShift * 2.0 - 1;
			double dRInner = dROuter / 2.5;
			int nPoints = 5;

			m_pStream->WriteStr("0 g\012q\012");
			for (int i = 0; i < nPoints * 2; ++i)
			{
				double dR = i % 2 == 0 ? dROuter : dRInner;
				double dAngle = M_PI / nPoints * i;
				double dX = dCX - dR * std::sin(dAngle);
				double dY = dCY + dR * std::cos(dAngle);
				if (i == 0)
					StreamWriteXYMove(m_pStream, dX, dY);
				else
					StreamWriteXYLine(m_pStream, dX, dY);
			}
			m_pStream->WriteStr("f\012Q\012");
			break;
		}
		case ECheckBoxStyle::Square:
		{
			double dSq = std::min(dW, dH) * 2.0 / 3.0 - dShift * 2.0 - 1;

			m_pStream->WriteStr("0 g\012q\012");
			StreamWriteRect(m_pStream, dCX - dSq / 2.0, dCY - dSq / 2.0, dSq, dSq);
			m_pStream->WriteStr("f\012Q\012");
			break;
		}
		}
	}
}
