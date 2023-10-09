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

#include "./AnnotField.h"
#include "../MetafileToRenderer.h"

CAnnotFieldInfo::CAnnotFieldInfo() : IAdvancedCommand(AdvancedCommandType::Annotaion)
{
	m_nType = 0;

	m_nFlag      = 0;
	m_nID        = 0;
	m_nAnnotFlag = 0;
	m_nPage      = 0;

	m_dX1 = 0.0;
	m_dY1 = 0.0;
	m_dX2 = 0.0;
	m_dY2 = 0.0;
	m_pBE.first  = 0.0;
	m_pBE.second = 0.0;

	m_oBorder.nType              = 0;
	m_oBorder.dWidth             = 0.0;
	m_oBorder.dDashesAlternating = 0.0;
	m_oBorder.dGaps              = 0.0;

	m_pMarkupPr       = NULL;
	m_pTextPr         = NULL;
	m_pInkPr          = NULL;
	m_pLinePr         = NULL;
	m_pTextMarkupPr   = NULL;
	m_pSquareCirclePr = NULL;
	m_pPolygonLinePr  = NULL;
	m_pPopupPr        = NULL;
	m_pFreeTextPr     = NULL;
	m_pCaretPr        = NULL;
	m_pWidgetPr       = NULL;
}
CAnnotFieldInfo::~CAnnotFieldInfo()
{
	RELEASEOBJECT(m_pMarkupPr);
	RELEASEOBJECT(m_pTextPr);
	RELEASEOBJECT(m_pInkPr);
	RELEASEOBJECT(m_pLinePr);
	RELEASEOBJECT(m_pTextMarkupPr);
	RELEASEOBJECT(m_pSquareCirclePr);
	RELEASEOBJECT(m_pPolygonLinePr);
	RELEASEOBJECT(m_pPopupPr);
	RELEASEOBJECT(m_pFreeTextPr);
	RELEASEOBJECT(m_pCaretPr);
	RELEASEOBJECT(m_pWidgetPr);
}

void CAnnotFieldInfo::SetType(int nType)
{
	switch (nType)
	{
	case 0:
	{
		m_nType = 15;

		RELEASEOBJECT(m_pMarkupPr);
		m_pMarkupPr = new CAnnotFieldInfo::CMarkupAnnotPr();

		RELEASEOBJECT(m_pTextPr);
		m_pTextPr = new CAnnotFieldInfo::CTextAnnotPr();
		break;
	}
	case 2:
	{
		m_nType = 13;

		RELEASEOBJECT(m_pMarkupPr);
		m_pMarkupPr = new CAnnotFieldInfo::CMarkupAnnotPr();

		RELEASEOBJECT(m_pFreeTextPr);
		m_pFreeTextPr = new CAnnotFieldInfo::CFreeTextAnnotPr();
		break;
	}
	case 3:
	{
		m_nType = 9;

		RELEASEOBJECT(m_pMarkupPr);
		m_pMarkupPr = new CAnnotFieldInfo::CMarkupAnnotPr();

		RELEASEOBJECT(m_pLinePr);
		m_pLinePr = new CAnnotFieldInfo::CLineAnnotPr();
		break;
	}
	case 4:
	case 5:
	{
		m_nType = 11;

		RELEASEOBJECT(m_pMarkupPr);
		m_pMarkupPr = new CAnnotFieldInfo::CMarkupAnnotPr();

		RELEASEOBJECT(m_pSquareCirclePr);
		m_pSquareCirclePr = new CAnnotFieldInfo::CSquareCircleAnnotPr();
		break;
	}
	case 6:
	case 7:
	{
		m_nType = 12;

		RELEASEOBJECT(m_pMarkupPr);
		m_pMarkupPr = new CAnnotFieldInfo::CMarkupAnnotPr();

		RELEASEOBJECT(m_pPolygonLinePr);
		m_pPolygonLinePr = new CAnnotFieldInfo::CPolygonLineAnnotPr();
		break;
	}
	case 8:
	case 9:
	case 10:
	case 11:
	{
		m_nType = 10;

		RELEASEOBJECT(m_pMarkupPr);
		m_pMarkupPr = new CAnnotFieldInfo::CMarkupAnnotPr();

		RELEASEOBJECT(m_pTextMarkupPr);
		m_pTextMarkupPr = new CAnnotFieldInfo::CTextMarkupAnnotPr();
		break;
	}
	case 13:
	{
		m_nType = 14;

		RELEASEOBJECT(m_pMarkupPr);
		m_pMarkupPr = new CAnnotFieldInfo::CMarkupAnnotPr();

		RELEASEOBJECT(m_pCaretPr);
		m_pCaretPr = new CAnnotFieldInfo::CCaretAnnotPr();
		break;
	}
	case 14:
	{
		m_nType = 8;

		RELEASEOBJECT(m_pMarkupPr);
		m_pMarkupPr = new CAnnotFieldInfo::CMarkupAnnotPr();

		RELEASEOBJECT(m_pInkPr);
		m_pInkPr = new CAnnotFieldInfo::CInkAnnotPr();
		break;
	}
	case 15:
	{
		m_nType = 24;

		RELEASEOBJECT(m_pPopupPr);
		m_pPopupPr = new CAnnotFieldInfo::CPopupAnnotPr();
		break;
	}
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	{
		m_nType = nType - 26;

		RELEASEOBJECT(m_pWidgetPr);
		m_pWidgetPr = new CAnnotFieldInfo::CWidgetAnnotPr(nType);
		break;
	}
	}
}
bool CAnnotFieldInfo::IsValid() const
{
	return (m_nType != 0);
}

void CAnnotFieldInfo::SetBounds(const double& dX1, const double& dY1, const double& dX2, const double& dY2)
{
	m_dX1 = dX1;
	m_dY1 = dY1;
	m_dX2 = dX2;
	m_dY2 = dY2;
}
void CAnnotFieldInfo::GetBounds(double& dX1, double& dY1, double& dX2, double& dY2) const
{
	dX1 = m_dX1;
	dY1 = m_dY1;
	dX2 = m_dX2;
	dY2 = m_dY2;
}
void CAnnotFieldInfo::SetBorder(BYTE nType, double dWidth, double dDashesAlternating, double dGaps)
{
	m_oBorder.nType = nType;
	m_oBorder.dWidth = dWidth;
	m_oBorder.dDashesAlternating = dDashesAlternating;
	m_oBorder.dGaps = dGaps;
}
void CAnnotFieldInfo::GetBorder(BYTE& nType, double& dWidth, double& dDashesAlternating, double& dGaps)
{
	nType = m_oBorder.nType;
	dWidth = m_oBorder.dWidth;
	dDashesAlternating = m_oBorder.dDashesAlternating;
	dGaps = m_oBorder.dGaps;
}

// Common
bool CAnnotFieldInfo::IsWidget() const
{
	return (m_nType < 8);
}
bool CAnnotFieldInfo::IsButtonWidget() const
{
	return (m_nType == 1 || m_nType == 2 || m_nType == 3);
}
bool CAnnotFieldInfo::IsTextWidget() const
{
	return (m_nType == 4);
}
bool CAnnotFieldInfo::IsChoiceWidget() const
{
	return (m_nType == 5 || m_nType == 6);
}
bool CAnnotFieldInfo::IsSignatureWidget() const
{
	return (m_nType == 7);
}
bool CAnnotFieldInfo::IsMarkup() const
{
	return (m_nType > 6 && m_nType < 24);
}
bool CAnnotFieldInfo::IsText() const
{
	return (m_nType == 15);
}
bool CAnnotFieldInfo::IsInk() const
{
	return (m_nType == 8);
}
bool CAnnotFieldInfo::IsLine() const
{
	return (m_nType == 9);
}
bool CAnnotFieldInfo::IsTextMarkup() const
{
	return (m_nType == 10);
}
bool CAnnotFieldInfo::IsSquareCircle() const
{
	return (m_nType == 11);
}
bool CAnnotFieldInfo::IsPolygonLine() const
{
	return (m_nType == 12);
}
bool CAnnotFieldInfo::IsPopup() const
{
	return (m_nType == 24);
}
bool CAnnotFieldInfo::IsFreeText() const
{
	return (m_nType == 13);
}
bool CAnnotFieldInfo::IsCaret() const
{
	return (m_nType == 14);
}

CAnnotFieldInfo::CWidgetAnnotPr::CWidgetAnnotPr(BYTE nType)
{
	m_pButtonPr    = NULL;
	m_pTextPr      = NULL;
	m_pChoicePr    = NULL;
	m_pSignaturePr = NULL;

	m_nType = nType;
	switch (nType)
	{
	case 26:
	{
		// Unknown widget
		break;
	}
	case 27:
	case 28:
	case 29:
	{
		RELEASEOBJECT(m_pButtonPr);
		m_pButtonPr = new CWidgetAnnotPr::CButtonWidgetPr();
		break;
	}
	case 30:
	{
		RELEASEOBJECT(m_pTextPr);
		m_pTextPr = new CWidgetAnnotPr::CTextWidgetPr();
		break;
	}
	case 31:
	case 32:
	{
		RELEASEOBJECT(m_pChoicePr);
		m_pChoicePr = new CWidgetAnnotPr::CChoiceWidgetPr();
		break;
	}
	case 33:
	{
		RELEASEOBJECT(m_pSignaturePr);
		m_pSignaturePr = new CWidgetAnnotPr::CSignatureWidgetPr();
		break;
	}
	}
}
CAnnotFieldInfo::CWidgetAnnotPr::~CWidgetAnnotPr()
{
	RELEASEOBJECT(m_pButtonPr);
	RELEASEOBJECT(m_pTextPr);
	RELEASEOBJECT(m_pChoicePr);
	RELEASEOBJECT(m_pSignaturePr);

	for (int i = 0; i < m_arrAction.size(); ++i)
		RELEASEOBJECT(m_arrAction[i]);
}

CAnnotFieldInfo::CWidgetAnnotPr::CActionWidget* ReadAction(NSOnlineOfficeBinToPdf::CBufferReader* pReader)
{
	CAnnotFieldInfo::CWidgetAnnotPr::CActionWidget* pRes = new CAnnotFieldInfo::CWidgetAnnotPr::CActionWidget();

	pRes->nActionType = pReader->ReadByte();
	switch (pRes->nActionType)
	{
	case 14: // JavaScript
	{
		pRes->wsStr1 = pReader->ReadString();
		break;
	}
	case 1: // GoTo
	{
		pRes->nInt1 = pReader->ReadInt();
		pRes->nKind = pReader->ReadByte();
		switch (pRes->nKind)
		{
		case 0:
		case 2:
		case 3:
		case 6:
		case 7:
		{
			pRes->nFlags = pReader->ReadByte();
			if (pRes->nFlags & (1 << 0))
				pRes->dD[0] = pReader->ReadDouble();
			if (pRes->nFlags & (1 << 1))
				pRes->dD[1] = pReader->ReadDouble();
			if (pRes->nFlags & (1 << 2))
				pRes->dD[2] = pReader->ReadDouble();
			break;
		}
		case 4:
		{
			pRes->dD[0] = pReader->ReadDouble();
			pRes->dD[1] = pReader->ReadDouble();
			pRes->dD[2] = pReader->ReadDouble();
			pRes->dD[3] = pReader->ReadDouble();
			break;
		}
		case 1:
		case 5:
		default:
		{
			break;
		}
		}
		break;
	}
	case 10: // Named
	{
		pRes->wsStr1 = pReader->ReadString();
		break;
	}
	case 6: // URI
	{
		pRes->wsStr1 = pReader->ReadString();
		break;
	}
	case 9: // Hide
	{
		pRes->nInt1 = pReader->ReadInt();
		int n = pReader->ReadInt();
		for (int i = 0; i < n; ++i)
			pRes->arrStr.push_back(pReader->ReadString());
		break;
	}
	case 12: // ResetForm
	{
		pRes->nInt1 = pReader->ReadInt();
		int n = pReader->ReadInt();
		for (int i = 0; i < n; ++i)
			pRes->arrStr.push_back(pReader->ReadString());
		break;
	}
	}

	if (pReader->ReadByte())
		pRes->pNext = ReadAction(pReader);

	return pRes;
}

bool CAnnotFieldInfo::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector)
{
	BYTE nType = pReader->ReadByte();
	SetType(nType);
	SetID(pReader->ReadInt());
	SetAnnotFlag(pReader->ReadInt());
	SetPage(pReader->ReadInt());

	double dX1 = pReader->ReadDouble();
	double dY1 = pReader->ReadDouble();
	double dX2 = pReader->ReadDouble();
	double dY2 = pReader->ReadDouble();

	SetBounds(dX1, dY1, dX2, dY2);

	int nFlags = pReader->ReadInt();
	SetFlag(nFlags);

	if (nFlags & (1 << 0))
		SetNM(pReader->ReadString());
	if (nFlags & (1 << 1))
		SetContents(pReader->ReadString());
	if (nFlags & (1 << 2))
	{
		BYTE nS = pReader->ReadByte();
		double dI = pReader->ReadDouble();
		SetBE(nS, dI);
	}
	if (nFlags & (1 << 3))
	{
		int n = pReader->ReadInt();
		std::vector<double> arrC;
		for (int i = 0; i < n; ++i)
			arrC.push_back(pReader->ReadDouble());
		SetC(arrC);
	}
	if (nFlags & (1 << 4))
	{
		BYTE nType = pReader->ReadByte();
		double dWidth = pReader->ReadDouble();
		double d1 = 0.0, d2 = 0.0;
		if (nType == 2)
		{
			d1 = pReader->ReadDouble();
			d2 = pReader->ReadDouble();
		}
		SetBorder(nType, dWidth, d1, d2);
	}
	if (nFlags & (1 << 5))
		SetLM(pReader->ReadString());

	if (IsMarkup())
	{
		CAnnotFieldInfo::CMarkupAnnotPr* pPr = GetMarkupAnnotPr();

		nFlags = pReader->ReadInt();
		pPr->SetFlag(nFlags);
		if (nFlags & (1 << 0))
			pPr->SetPopupID(pReader->ReadInt());
		if (nFlags & (1 << 1))
			pPr->SetT(pReader->ReadString());
		if (nFlags & (1 << 2))
			pPr->SetCA(pReader->ReadDouble());
		if (nFlags & (1 << 3))
			pPr->SetRC(pReader->ReadString());
		if (nFlags & (1 << 4))
			pPr->SetCD(pReader->ReadString());
		if (nFlags & (1 << 5))
			pPr->SetIRTID(pReader->ReadInt());
		if (nFlags & (1 << 6))
			pPr->SetRT(pReader->ReadByte());
		if (nFlags & (1 << 7))
			pPr->SetSubj(pReader->ReadString());
	}

	if (IsText())
	{
		CAnnotFieldInfo::CTextAnnotPr* pPr = GetTextAnnotPr();

		pPr->SetOpen(nFlags & (1 << 15));
		if (nFlags & (1 << 16))
			pPr->SetName(pReader->ReadByte());
		if (nFlags & (1 << 17))
			pPr->SetStateModel(pReader->ReadByte());
		if (nFlags & (1 << 18))
			pPr->SetState(pReader->ReadByte());
	}
	else if (IsInk())
	{
		CAnnotFieldInfo::CInkAnnotPr* pPr = GetInkAnnotPr();

		int n = pReader->ReadInt();
		std::vector< std::vector<double> > arrInkList;
		for (int i = 0; i < n; ++i)
		{
			std::vector<double> arrLine;
			int m = pReader->ReadInt();
			for (int j = 0; j < m; ++j)
				arrLine.push_back(pReader->ReadDouble());
			if (!arrLine.empty())
				arrInkList.push_back(arrLine);
		}
		pPr->SetInkList(arrInkList);
	}
	else if (IsLine())
	{
		CAnnotFieldInfo::CLineAnnotPr* pPr = GetLineAnnotPr();

		double dLX1 = pReader->ReadDouble();
		double dLY1 = pReader->ReadDouble();
		double dLX2 = pReader->ReadDouble();
		double dLY2 = pReader->ReadDouble();
		pPr->SetL(dLX1, dLY1, dLX2, dLY2);

		if (nFlags & (1 << 15))
		{
			BYTE nLE1 = pReader->ReadByte();
			BYTE nLE2 = pReader->ReadByte();
			pPr->SetLE(nLE1, nLE2);
		}
		if (nFlags & (1 << 16))
		{
			int n = pReader->ReadInt();
			std::vector<double> arrIC;
			for (int i = 0; i < n; ++i)
				arrIC.push_back(pReader->ReadDouble());
			pPr->SetIC(arrIC);
		}
		if (nFlags & (1 << 17))
			pPr->SetLL(pReader->ReadDouble());
		if (nFlags & (1 << 18))
			pPr->SetLLE(pReader->ReadDouble());
		pPr->SetCap(nFlags & (1 << 19));
		if (nFlags & (1 << 20))
			pPr->SetIT(pReader->ReadByte());
		if (nFlags & (1 << 21))
			pPr->SetLLO(pReader->ReadDouble());
		if (nFlags & (1 << 22))
			pPr->SetCP(pReader->ReadByte());
		if (nFlags & (1 << 23))
		{
			double dCO1 = pReader->ReadDouble();
			double dCO2 = pReader->ReadDouble();
			pPr->SetCO(dCO1, dCO2);
		}
	}
	else if (IsTextMarkup())
	{
		CAnnotFieldInfo::CTextMarkupAnnotPr* pPr = GetTextMarkupAnnotPr();

		pPr->SetSubtype(nType);
		std::vector<double> arrQuadPoints;
		int n = pReader->ReadInt();
		for (int i = 0; i < n; ++i)
			arrQuadPoints.push_back(pReader->ReadDouble());
		pPr->SetQuadPoints(arrQuadPoints);
	}
	else if (IsSquareCircle())
	{
		CAnnotFieldInfo::CSquareCircleAnnotPr* pPr = GetSquareCircleAnnotPr();

		pPr->SetSubtype(nType);
		if (nFlags & (1 << 15))
		{
			double dRD1 = pReader->ReadDouble();
			double dRD2 = pReader->ReadDouble();
			double dRD3 = pReader->ReadDouble();
			double dRD4 = pReader->ReadDouble();
			pPr->SetRD(dRD1, dRD2, dRD3, dRD4);
		}
		if (nFlags & (1 << 16))
		{
			int n = pReader->ReadInt();
			std::vector<double> arrIC;
			for (int i = 0; i < n; ++i)
				arrIC.push_back(pReader->ReadDouble());
			pPr->SetIC(arrIC);
		}
	}
	else if (IsPolygonLine())
	{
		CAnnotFieldInfo::CPolygonLineAnnotPr* pPr = GetPolygonLineAnnotPr();

		int n = pReader->ReadInt();
		std::vector<double> arrVertices;
		for (int i = 0; i < n; ++i)
			arrVertices.push_back(pReader->ReadDouble());
		pPr->SetVertices(arrVertices);

		pPr->SetSubtype(nType);
		if (nFlags & (1 << 15))
		{
			BYTE nLE1 = pReader->ReadByte();
			BYTE nLE2 = pReader->ReadByte();
			pPr->SetLE(nLE1, nLE2);
		}
		if (nFlags & (1 << 16))
		{
			int n = pReader->ReadInt();
			std::vector<double> arrIC;
			for (int i = 0; i < n; ++i)
				arrIC.push_back(pReader->ReadDouble());
			pPr->SetIC(arrIC);
		}
		if (nFlags & (1 << 20))
			pPr->SetIT(pReader->ReadByte());
	}
	else if (IsPopup())
	{
		CAnnotFieldInfo::CPopupAnnotPr* pPr = GetPopupAnnotPr();

		nFlags = pReader->ReadInt();
		pPr->SetFlag(nFlags);
		pPr->SetOpen(nFlags & (1 << 0));
		if (nFlags & (1 << 1))
			pPr->SetParentID(pReader->ReadInt());
	}
	else if (IsFreeText())
	{
		CAnnotFieldInfo::CFreeTextAnnotPr* pPr = GetFreeTextAnnotPr();

		pPr->SetQ(pReader->ReadByte());
		if (nFlags & (1 << 15))
		{
			double dRD1 = pReader->ReadDouble();
			double dRD2 = pReader->ReadDouble();
			double dRD3 = pReader->ReadDouble();
			double dRD4 = pReader->ReadDouble();
			pPr->SetRD(dRD1, dRD2, dRD3, dRD4);
		}
		if (nFlags & (1 << 16))
		{
			int n = pReader->ReadInt();
			std::vector<double> arrCL;
			for (int i = 0; i < n; ++i)
				arrCL.push_back(pReader->ReadDouble());
			pPr->SetCL(arrCL);
		}
		if (nFlags & (1 << 17))
			pPr->SetDS(pReader->ReadString());
		if (nFlags & (1 << 18))
			pPr->SetLE(pReader->ReadByte());
		if (nFlags & (1 << 20))
			pPr->SetIT(pReader->ReadByte());
	}
	else if (IsCaret())
	{
		CAnnotFieldInfo::CCaretAnnotPr* pPr = GetCaretAnnotPr();

		if (nFlags & (1 << 15))
		{
			double dRD1 = pReader->ReadDouble();
			double dRD2 = pReader->ReadDouble();
			double dRD3 = pReader->ReadDouble();
			double dRD4 = pReader->ReadDouble();
			pPr->SetRD(dRD1, dRD2, dRD3, dRD4);
		}
		if (nFlags & (1 << 16))
			pPr->SetSy(pReader->ReadByte());
	}

	if (IsWidget())
	{
		CAnnotFieldInfo::CWidgetAnnotPr* pPr = GetWidgetAnnotPr();

		int n = pReader->ReadInt();
		std::vector<double> arrTC;
		for (int i = 0; i < n; ++i)
			arrTC.push_back(pReader->ReadDouble());
		pPr->SetTC(arrTC);

		pPr->SetQ(pReader->ReadByte());
		int nWidgetFlag = pReader->ReadInt();
		pPr->SetFlag(nWidgetFlag);

		int nFlags = pReader->ReadInt();
		pPr->SetFlags(nFlags);
		if (nFlags & (1 << 0))
			pPr->SetTU(pReader->ReadString());
		if (nFlags & (1 << 1))
			pPr->SetDS(pReader->ReadString());
		if (nFlags & (1 << 3))
			pPr->SetH(pReader->ReadByte());
		if (nFlags & (1 << 5))
		{
			int n = pReader->ReadInt();
			std::vector<double> arrBC;
			for (int i = 0; i < n; ++i)
				arrBC.push_back(pReader->ReadDouble());
			pPr->SetBC(arrBC);
		}
		if (nFlags & (1 << 6))
			pPr->SetR(pReader->ReadInt());
		if (nFlags & (1 << 7))
		{
			int n = pReader->ReadInt();
			std::vector<double> arrBG;
			for (int i = 0; i < n; ++i)
				arrBG.push_back(pReader->ReadDouble());
			pPr->SetBG(arrBG);
		}
		if (nFlags & (1 << 8))
			pPr->SetDV(pReader->ReadString());
		if (nFlags & (1 << 17))
			pPr->SetParentID(pReader->ReadInt());
		if (nFlags & (1 << 18))
			pPr->SetT(pReader->ReadString());

		// Action
		int nAction = pReader->ReadInt();
		for (int i = 0; i < nAction; ++i)
		{
			std::wstring wsType = pReader->ReadString();
			CAnnotFieldInfo::CWidgetAnnotPr::CActionWidget* pA = ReadAction(pReader);
			if (pA)
			{
				pA->wsType = wsType;
				pPr->AddAction(pA);
			}
		}

		if (nType == 29 || nType == 28 || nType == 27)
		{
			CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr* pPr1 = pPr->GetButtonWidgetPr();

			int nIFFlags = pReader->ReadInt();
			pPr1->SetIFFlag(nIFFlags);
			if (nType == 27)
			{
				if (nFlags & (1 << 10))
					pPr1->SetCA(pReader->ReadString());
				if (nFlags & (1 << 11))
					pPr1->SetRC(pReader->ReadString());
				if (nFlags & (1 << 12))
					pPr1->SetAC(pReader->ReadString());
			}
			else
				pPr1->SetS(pReader->ReadByte());

			if (nFlags & (1 << 13))
				pPr1->SetTP(pReader->ReadByte());
			if (nIFFlags & (1 << 0))
			{
				if (nIFFlags & (1 << 1))
					pPr1->SetSW(pReader->ReadByte());
				if (nIFFlags & (1 << 2))
					pPr1->SetS(pReader->ReadByte());
				if (nIFFlags & (1 << 3))
				{
					double d1 = pReader->ReadDouble();
					double d2 = pReader->ReadDouble();
					pPr1->SetA(d1, d2);
				}
			}
			if (nFlags & (1 << 14))
				pPr1->SetAP_N_Yes(pReader->ReadString());

		}
		else if (nType == 30)
		{
			CAnnotFieldInfo::CWidgetAnnotPr::CTextWidgetPr* pPr1 = pPr->GetTextWidgetPr();

			if (nFlags & (1 << 9))
				pPr1->SetV(pReader->ReadString());
			if (nFlags & (1 << 10))
				pPr1->SetMaxLen(pReader->ReadInt());
			if (nWidgetFlag & (1 << 25))
				pPr1->SetRV(pReader->ReadString());
		}
		else if (nType == 31 || nType == 32)
		{
			CAnnotFieldInfo::CWidgetAnnotPr::CChoiceWidgetPr* pPr1 = pPr->GetChoiceWidgetPr();

			if (nFlags & (1 << 9))
				pPr1->SetV(pReader->ReadString());
			if (nFlags & (1 << 10))
			{
				int n = pReader->ReadInt();
				std::vector< std::pair<std::wstring, std::wstring> > arrOpt;
				for (int i = 0; i < n; ++i)
				{
					std::wstring s1 = pReader->ReadString();
					std::wstring s2 = pReader->ReadString();
					arrOpt.push_back(std::make_pair(s2, s1));
				}
				pPr1->SetOpt(arrOpt);
			}
			if (nFlags & (1 << 11))
				pPr1->SetTI(pReader->ReadInt());
		}
	}

	return IsValid();
}
