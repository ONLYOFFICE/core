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
	m_pBE.first  = 0;
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

void CAnnotFieldInfo::GetBounds(double& dX1, double& dY1, double& dX2, double& dY2) const
{
	dX1 = m_dX1;
	dY1 = m_dY1;
	dX2 = m_dX2;
	dY2 = m_dY2;
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
		pRes->nKind = pReader->ReadByte();
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

	m_nID        = pReader->ReadInt();
	m_nAnnotFlag = pReader->ReadInt();
	m_nPage      = pReader->ReadInt();

	m_dX1 = pReader->ReadDouble();
	m_dY1 = pReader->ReadDouble();
	m_dX2 = pReader->ReadDouble();
	m_dY2 = pReader->ReadDouble();

	int nFlags = pReader->ReadInt();
	m_nFlag = nFlags;

	if (nFlags & (1 << 0))
		m_wsNM = pReader->ReadString();
	if (nFlags & (1 << 1))
		m_wsContents = pReader->ReadString();
	if (nFlags & (1 << 2))
	{
		m_pBE.first  = pReader->ReadByte();
		m_pBE.second = pReader->ReadDouble();
	}
	if (nFlags & (1 << 3))
	{
		int n = pReader->ReadInt();
		for (int i = 0; i < n; ++i)
			m_arrC.push_back(pReader->ReadDouble());
	}
	if (nFlags & (1 << 4))
	{
		m_oBorder.nType = pReader->ReadByte();
		m_oBorder.dWidth = pReader->ReadDouble();
		m_oBorder.dDashesAlternating = 0.0;
		m_oBorder.dGaps = 0.0;
		if (m_oBorder.nType == 2)
		{
			m_oBorder.dDashesAlternating = pReader->ReadDouble();
			m_oBorder.dGaps = pReader->ReadDouble();
		}
	}
	if (nFlags & (1 << 5))
		m_wsLM = pReader->ReadString();

	if (IsMarkup())
	{
		CAnnotFieldInfo::CMarkupAnnotPr* pPr = GetMarkupAnnotPr();

		nFlags = pReader->ReadInt();
		pPr->Read(pReader, nFlags);

		if (IsText())
			m_pTextPr->Read(pReader, nFlags);
		else if (IsInk())
			m_pInkPr->Read(pReader);
		else if (IsLine())
			m_pLinePr->Read(pReader, nFlags);
		else if (IsTextMarkup())
			m_pTextMarkupPr->Read(pReader, nType);
		else if (IsSquareCircle())
			m_pSquareCirclePr->Read(pReader, nType, nFlags);
		else if (IsPolygonLine())
			m_pPolygonLinePr->Read(pReader, nType, nFlags);
		else if (IsFreeText())
			m_pFreeTextPr->Read(pReader, nFlags);
		else if (IsCaret())
			m_pCaretPr->Read(pReader, nFlags);
	}
	else if (IsPopup())
		m_pPopupPr->Read(pReader);
	else if (IsWidget())
		m_pWidgetPr->Read(pReader, nType);

	return IsValid();
}

void CAnnotFieldInfo::CMarkupAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags)
{
	m_nFlag = nFlags;
	if (nFlags & (1 << 0))
		m_nPopupID = pReader->ReadInt();
	if (nFlags & (1 << 1))
		m_wsT = pReader->ReadString();
	if (nFlags & (1 << 2))
		m_dCA = pReader->ReadDouble();
	if (nFlags & (1 << 3))
		m_wsRC = pReader->ReadString();
	if (nFlags & (1 << 4))
		m_wsCD = pReader->ReadString();
	if (nFlags & (1 << 5))
		m_nIRTID = pReader->ReadInt();
	if (nFlags & (1 << 6))
		m_nRT = pReader->ReadByte();
	if (nFlags & (1 << 7))
		m_wsSubj = pReader->ReadString();
}
void CAnnotFieldInfo::CTextAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags)
{
	m_bOpen = nFlags & (1 << 15);
	if (nFlags & (1 << 16))
		m_nName = pReader->ReadByte();
	if (nFlags & (1 << 17))
		m_nStateModel = pReader->ReadByte();
	if (nFlags & (1 << 18))
		m_nState = pReader->ReadByte();
}
void CAnnotFieldInfo::CInkAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader)
{
	int n = pReader->ReadInt();
	for (int i = 0; i < n; ++i)
	{
		std::vector<double> arrLine;
		int m = pReader->ReadInt();
		for (int j = 0; j < m; ++j)
			arrLine.push_back(pReader->ReadDouble());
		if (!arrLine.empty())
			m_arrInkList.push_back(arrLine);
	}
}
void CAnnotFieldInfo::CLineAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags)
{
	m_dL[0] = pReader->ReadDouble();
	m_dL[1] = pReader->ReadDouble();
	m_dL[2] = pReader->ReadDouble();
	m_dL[3] = pReader->ReadDouble();

	if (nFlags & (1 << 15))
	{
		m_nLE[0] = pReader->ReadByte();
		m_nLE[1] = pReader->ReadByte();
	}
	if (nFlags & (1 << 16))
	{
		int n = pReader->ReadInt();
		for (int i = 0; i < n; ++i)
			m_arrIC.push_back(pReader->ReadDouble());
	}
	if (nFlags & (1 << 17))
		m_dLL = pReader->ReadDouble();
	if (nFlags & (1 << 18))
		m_dLLE = pReader->ReadDouble();
	m_bCap = nFlags & (1 << 19);
	if (nFlags & (1 << 20))
		m_nIT = pReader->ReadByte();
	if (nFlags & (1 << 21))
		m_dLLO = pReader->ReadDouble();
	if (nFlags & (1 << 22))
		m_nCP = pReader->ReadByte();
	if (nFlags & (1 << 23))
	{
		m_dCO[0] = pReader->ReadDouble();
		m_dCO[1] = pReader->ReadDouble();
	}
}
void CAnnotFieldInfo::CTextMarkupAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType)
{
	m_nSubtype = nType;
	int n = pReader->ReadInt();
	for (int i = 0; i < n; ++i)
		m_arrQuadPoints.push_back(pReader->ReadDouble());
}
void CAnnotFieldInfo::CSquareCircleAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType, int nFlags)
{
	m_nSubtype = nType;
	if (nFlags & (1 << 15))
	{
		m_dRD[0] = pReader->ReadDouble();
		m_dRD[1] = pReader->ReadDouble();
		m_dRD[2] = pReader->ReadDouble();
		m_dRD[3] = pReader->ReadDouble();
	}
	if (nFlags & (1 << 16))
	{
		int n = pReader->ReadInt();
		for (int i = 0; i < n; ++i)
			m_arrIC.push_back(pReader->ReadDouble());
	}
}
void CAnnotFieldInfo::CPolygonLineAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType, int nFlags)
{
	int n = pReader->ReadInt();
	for (int i = 0; i < n; ++i)
		m_arrVertices.push_back(pReader->ReadDouble());

	m_nSubtype = nType;
	if (nFlags & (1 << 15))
	{
		m_nLE[0] = pReader->ReadByte();
		m_nLE[1] = pReader->ReadByte();
	}
	if (nFlags & (1 << 16))
	{
		int n = pReader->ReadInt();
		for (int i = 0; i < n; ++i)
			m_arrIC.push_back(pReader->ReadDouble());
	}
	if (nFlags & (1 << 20))
		m_nIT = pReader->ReadByte();
}
void CAnnotFieldInfo::CFreeTextAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags)
{
	m_nQ = pReader->ReadByte();
	if (nFlags & (1 << 15))
	{
		m_dRD[0] = pReader->ReadDouble();
		m_dRD[1] = pReader->ReadDouble();
		m_dRD[2] = pReader->ReadDouble();
		m_dRD[3] = pReader->ReadDouble();
	}
	if (nFlags & (1 << 16))
	{
		int n = pReader->ReadInt();
		for (int i = 0; i < n; ++i)
			m_arrCL.push_back(pReader->ReadDouble());
	}
	if (nFlags & (1 << 17))
		m_wsDS = pReader->ReadString();
	if (nFlags & (1 << 18))
		m_nLE = pReader->ReadByte();
	if (nFlags & (1 << 20))
		m_nIT = pReader->ReadByte();
}
void CAnnotFieldInfo::CCaretAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags)
{
	if (nFlags & (1 << 15))
	{
		m_dRD[0] = pReader->ReadDouble();
		m_dRD[1] = pReader->ReadDouble();
		m_dRD[2] = pReader->ReadDouble();
		m_dRD[3] = pReader->ReadDouble();
	}
	if (nFlags & (1 << 16))
		m_nSy = pReader->ReadByte();
}
void CAnnotFieldInfo::CPopupAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader)
{
	m_nFlag = pReader->ReadInt();
	m_bOpen = m_nFlag & (1 << 0);
	if (m_nFlag & (1 << 1))
		m_nParentID = pReader->ReadInt();
}
void CAnnotFieldInfo::CWidgetAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType)
{
	m_wsFN = pReader->ReadString();
	m_dFS = pReader->ReadDouble();
	m_dFSAP = pReader->ReadDouble();
	m_nFontStyle = pReader->ReadInt();

	int n = pReader->ReadInt();
	for (int i = 0; i < n; ++i)
		m_arrTC.push_back(pReader->ReadDouble());

	m_nQ = pReader->ReadByte();
	int nWidgetFlag = pReader->ReadInt();
	m_nFlag = nWidgetFlag;

	int nFlags = pReader->ReadInt();
	m_nFlags = nFlags;
	if (nFlags & (1 << 0))
		m_wsTU = pReader->ReadString();
	if (nFlags & (1 << 1))
		m_wsDS = pReader->ReadString();
	if (nFlags & (1 << 3))
		m_nH = pReader->ReadByte();
	if (nFlags & (1 << 5))
	{
		int n = pReader->ReadInt();
		for (int i = 0; i < n; ++i)
			m_arrBC.push_back(pReader->ReadDouble());
	}
	if (nFlags & (1 << 6))
		m_nR = pReader->ReadInt();
	if (nFlags & (1 << 7))
	{
		int n = pReader->ReadInt();
		for (int i = 0; i < n; ++i)
			m_arrBG.push_back(pReader->ReadDouble());
	}
	if (nFlags & (1 << 8))
		m_wsDV = pReader->ReadString();
	if (nFlags & (1 << 17))
		m_nParentID = pReader->ReadInt();
	if (nFlags & (1 << 18))
		m_wsT = pReader->ReadString();

	// Action
	int nAction = pReader->ReadInt();
	for (int i = 0; i < nAction; ++i)
	{
		std::wstring wsType = pReader->ReadString();
		CAnnotFieldInfo::CWidgetAnnotPr::CActionWidget* pA = ReadAction(pReader);
		if (pA)
		{
			pA->wsType = wsType;
			m_arrAction.push_back(pA);
		}
	}

	if (nType == 29 || nType == 28 || nType == 27)
		m_pButtonPr->Read(pReader, nType, nFlags);
	else if (nType == 30)
		m_pTextPr->Read(pReader, nFlags, nWidgetFlag);
	else if (nType == 31 || nType == 32)
		m_pChoicePr->Read(pReader, nFlags);
}
void CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType, int nFlags)
{
	int nIFFlags = pReader->ReadInt();
	m_nIFFlag = nIFFlags;
	if (nType == 27)
	{
		if (nFlags & (1 << 10))
			m_wsCA = pReader->ReadString();
		if (nFlags & (1 << 11))
			m_wsRC = pReader->ReadString();
		if (nFlags & (1 << 12))
			m_wsAC = pReader->ReadString();
	}
	else
		m_nStyle = pReader->ReadByte();

	if (nFlags & (1 << 13))
		m_nTP = pReader->ReadByte();
	if (nIFFlags & (1 << 0))
	{
		if (nIFFlags & (1 << 1))
			m_nSW = pReader->ReadByte();
		if (nIFFlags & (1 << 2))
			m_nS = pReader->ReadByte();
		if (nIFFlags & (1 << 3))
		{
			m_dA1 = pReader->ReadDouble();
			m_dA2 = pReader->ReadDouble();
		}
	}
	if (nFlags & (1 << 14))
		m_wsAP_N_Yes = pReader->ReadString();
}
void CAnnotFieldInfo::CWidgetAnnotPr::CTextWidgetPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags, int nWidgetFlag)
{
	if (nFlags & (1 << 9))
		m_wsV = pReader->ReadString();
	if (nFlags & (1 << 10))
		m_nMaxLen = pReader->ReadInt();
	if (nWidgetFlag & (1 << 25))
		m_wsRV = pReader->ReadString();
}
void CAnnotFieldInfo::CWidgetAnnotPr::CChoiceWidgetPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags)
{
	if (nFlags & (1 << 9))
		m_wsV = pReader->ReadString();
	if (nFlags & (1 << 10))
	{
		int n = pReader->ReadInt();
		for (int i = 0; i < n; ++i)
		{
			std::wstring s1 = pReader->ReadString();
			std::wstring s2 = pReader->ReadString();
			m_arrOpt.push_back(std::make_pair(s2, s1));
		}
	}
	if (nFlags & (1 << 11))
		m_nTI = pReader->ReadInt();
}

CAnnotFieldDelete::CAnnotFieldDelete() : IAdvancedCommand(AdvancedCommandType::DeleteAnnot) {}
bool CAnnotFieldDelete::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector)
{
	m_nID = pReader->ReadInt();
	return true;
}

CWidgetsInfo::CWidgetsInfo() : IAdvancedCommand(AdvancedCommandType::WidgetsInfo) {}
CWidgetsInfo::~CWidgetsInfo()
{
	for (int i = 0; i < m_arrParents.size(); ++i)
		RELEASEOBJECT(m_arrParents[i]);
}
bool CWidgetsInfo::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector)
{
	int n = pReader->ReadInt();
	for (int i = 0; i < n; ++i)
		m_arrCO.push_back(pReader->ReadString());

	n = pReader->ReadInt();
	for (int i = 0; i < n; ++i)
	{
		CParent* pParent = new CParent();
		pParent->nID = pReader->ReadInt();
		int nFlags   = pReader->ReadInt();
		pParent->nFlags = nFlags;
		if (nFlags & (1 << 0))
			pParent->sName = pReader->ReadString();
		if (nFlags & (1 << 1))
			pParent->sV    = pReader->ReadString();
		if (nFlags & (1 << 2))
			pParent->sDV   = pReader->ReadString();
		if (nFlags & (1 << 3))
			pParent->nParentID = pReader->ReadInt();
		m_arrParents.push_back(pParent);
	}

	return true;
}
