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
#include "../../common/File.h"

// void Set(const BYTE& n) { m_n = n; }
// BYTE Get() const { return m_n; }

// void Set(const int& n) { m_n = n; }
// int Get() const { return m_n; }

// void Set(const double& d) { m_d = d; }
// double Get() const { return m_d; }

// void Set(const std::wstring& ws) { m_ws = ws; }
// const std::wstring& Get() { return m_ws; }

CAnnotFieldInfo::CAnnotFieldInfo() : IAdvancedCommand(AdvancedCommandType::Annotaion)
{
	m_nType = EAnnotType::Unknown;

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

	m_oBorder.nType  = 0;
	m_oBorder.dWidth = 0.0;

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
	m_pStampPr        = NULL;
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
	RELEASEOBJECT(m_pStampPr);
	RELEASEOBJECT(m_pWidgetPr);
}

void CAnnotFieldInfo::CreateMarkup()
{
	RELEASEOBJECT(m_pMarkupPr);
	m_pMarkupPr = new CAnnotFieldInfo::CMarkupAnnotPr();
}
void CAnnotFieldInfo::SetType(int nType)
{
	m_nType = static_cast<EAnnotType>(nType);
	switch (m_nType)
	{
	case EAnnotType::Text:
	{
		CreateMarkup();
		RELEASEOBJECT(m_pTextPr);
		m_pTextPr = new CAnnotFieldInfo::CTextAnnotPr();
		break;
	}
	case EAnnotType::FreeText:
	{
		CreateMarkup();
		RELEASEOBJECT(m_pFreeTextPr);
		m_pFreeTextPr = new CAnnotFieldInfo::CFreeTextAnnotPr();
		break;
	}
	case EAnnotType::Line:
	{
		CreateMarkup();
		RELEASEOBJECT(m_pLinePr);
		m_pLinePr = new CAnnotFieldInfo::CLineAnnotPr();
		break;
	}
	case EAnnotType::Square:
	case EAnnotType::Circle:
	{
		CreateMarkup();
		RELEASEOBJECT(m_pSquareCirclePr);
		m_pSquareCirclePr = new CAnnotFieldInfo::CSquareCircleAnnotPr();
		break;
	}
	case EAnnotType::PolygonLine:
	case EAnnotType::PolyLine:
	{
		CreateMarkup();
		RELEASEOBJECT(m_pPolygonLinePr);
		m_pPolygonLinePr = new CAnnotFieldInfo::CPolygonLineAnnotPr();
		break;
	}
	case EAnnotType::Highlight:
	case EAnnotType::Underline:
	case EAnnotType::Squiggly:
	case EAnnotType::Strikeout:
	{
		CreateMarkup();
		RELEASEOBJECT(m_pTextMarkupPr);
		m_pTextMarkupPr = new CAnnotFieldInfo::CTextMarkupAnnotPr();
		break;
	}
	case EAnnotType::Stamp:
	{
		CreateMarkup();
		RELEASEOBJECT(m_pStampPr);
		m_pStampPr = new CAnnotFieldInfo::CStampAnnotPr();
		break;
	}
	case EAnnotType::Caret:
	{
		CreateMarkup();
		RELEASEOBJECT(m_pCaretPr);
		m_pCaretPr = new CAnnotFieldInfo::CCaretAnnotPr();
		break;
	}
	case EAnnotType::Ink:
	{
		CreateMarkup();
		RELEASEOBJECT(m_pInkPr);
		m_pInkPr = new CAnnotFieldInfo::CInkAnnotPr();
		break;
	}
	case EAnnotType::Popup:
	{
		RELEASEOBJECT(m_pPopupPr);
		m_pPopupPr = new CAnnotFieldInfo::CPopupAnnotPr();
		break;
	}
	case EAnnotType::Widget:
	case EAnnotType::WidgetPushButton:
	case EAnnotType::WidgetRadioButton:
	case EAnnotType::WidgetCheckBox:
	case EAnnotType::WidgetText:
	case EAnnotType::WidgetCombobox:
	case EAnnotType::WidgetListbox:
	case EAnnotType::WidgetSignature:
	{
		RELEASEOBJECT(m_pWidgetPr);
		m_pWidgetPr = new CAnnotFieldInfo::CWidgetAnnotPr(nType);
		break;
	}
	default:
		break;
	}
}
CAnnotFieldInfo::EAnnotType CAnnotFieldInfo::GetType() { return m_nType; }

void CAnnotFieldInfo::GetBounds(double& dX1, double& dY1, double& dX2, double& dY2)
{
	dX1 = m_dX1;
	dY1 = m_dY1;
	dX2 = m_dX2;
	dY2 = m_dY2;
}
void CAnnotFieldInfo::GetBorder(BYTE& nType, double& dWidth, std::vector<double>& arrDash)
{
	nType = m_oBorder.nType;
	dWidth = m_oBorder.dWidth;
	arrDash = m_oBorder.arrDash;
}
int  CAnnotFieldInfo::GetFlag()      const { return m_nFlag; }
int  CAnnotFieldInfo::GetID()        const { return m_nID; }
int  CAnnotFieldInfo::GetAnnotFlag() const { return m_nAnnotFlag; }
int  CAnnotFieldInfo::GetPage()      const { return m_nPage; }
int CAnnotFieldInfo::GetCopyAP()     const { return m_nCopyAP; }
void CAnnotFieldInfo::GetBE(BYTE& nS, double& dI) { nS = m_pBE.first; dI = m_pBE.second; }
BYTE* CAnnotFieldInfo::GetRender(LONG& nLen)
{
	nLen = m_nRenderLen;
	return m_pRender;
}
const std::wstring& CAnnotFieldInfo::GetNM() { return m_wsNM; }
const std::wstring& CAnnotFieldInfo::GetLM() { return m_wsLM; }
const std::wstring& CAnnotFieldInfo::GetOUserID() { return m_wsOUserID; }
const std::wstring& CAnnotFieldInfo::GetOMetadata() { return m_wsOMetadata; }
const std::wstring& CAnnotFieldInfo::GetContents() { return m_wsContents; }
const std::vector<double>& CAnnotFieldInfo::GetC() { return m_arrC; }

bool CAnnotFieldInfo::IsWidget() const
{
	return (m_nType >= 26);
}
bool CAnnotFieldInfo::IsButtonWidget() const
{
	return (m_nType == 27 || m_nType == 28 || m_nType == 29);
}
bool CAnnotFieldInfo::IsTextWidget() const
{
	return (m_nType == 30);
}
bool CAnnotFieldInfo::IsChoiceWidget() const
{
	return (m_nType == 31 || m_nType == 32);
}
bool CAnnotFieldInfo::IsSignatureWidget() const
{
	return (m_nType == 33);
}
bool CAnnotFieldInfo::IsMarkup() const
{
	return ((m_nType >= 0 && m_nType <= 17 && m_nType != 1 && m_nType != 15) || m_nType == 25);
}
bool CAnnotFieldInfo::IsText() const
{
	return (m_nType == 0);
}
bool CAnnotFieldInfo::IsInk() const
{
	return (m_nType == 14);
}
bool CAnnotFieldInfo::IsLine() const
{
	return (m_nType == 3);
}
bool CAnnotFieldInfo::IsTextMarkup() const
{
	return (m_nType >= 8 && m_nType <= 11);
}
bool CAnnotFieldInfo::IsSquareCircle() const
{
	return (m_nType == 4 || m_nType == 5);
}
bool CAnnotFieldInfo::IsPolygonLine() const
{
	return (m_nType == 6 || m_nType == 7);
}
bool CAnnotFieldInfo::IsPopup() const
{
	return (m_nType == 15);
}
bool CAnnotFieldInfo::IsFreeText() const
{
	return (m_nType == 2);
}
bool CAnnotFieldInfo::IsCaret() const
{
	return (m_nType == 13);
}
bool CAnnotFieldInfo::IsStamp() const
{
	return (m_nType == 12);
}

CAnnotFieldInfo::CMarkupAnnotPr*       CAnnotFieldInfo::GetMarkupAnnotPr()       { return m_pMarkupPr; }
CAnnotFieldInfo::CTextAnnotPr*         CAnnotFieldInfo::GetTextAnnotPr()         { return m_pTextPr; }
CAnnotFieldInfo::CInkAnnotPr*          CAnnotFieldInfo::GetInkAnnotPr()          { return m_pInkPr; }
CAnnotFieldInfo::CLineAnnotPr*         CAnnotFieldInfo::GetLineAnnotPr()         { return m_pLinePr; }
CAnnotFieldInfo::CTextMarkupAnnotPr*   CAnnotFieldInfo::GetTextMarkupAnnotPr()   { return m_pTextMarkupPr; }
CAnnotFieldInfo::CSquareCircleAnnotPr* CAnnotFieldInfo::GetSquareCircleAnnotPr() { return m_pSquareCirclePr; }
CAnnotFieldInfo::CPolygonLineAnnotPr*  CAnnotFieldInfo::GetPolygonLineAnnotPr()  { return m_pPolygonLinePr; }
CAnnotFieldInfo::CPopupAnnotPr*        CAnnotFieldInfo::GetPopupAnnotPr()        { return m_pPopupPr; }
CAnnotFieldInfo::CFreeTextAnnotPr*     CAnnotFieldInfo::GetFreeTextAnnotPr()     { return m_pFreeTextPr; }
CAnnotFieldInfo::CCaretAnnotPr*        CAnnotFieldInfo::GetCaretAnnotPr()        { return m_pCaretPr; }
CAnnotFieldInfo::CStampAnnotPr*        CAnnotFieldInfo::GetStampAnnotPr()        { return m_pStampPr; }
CAnnotFieldInfo::CWidgetAnnotPr*       CAnnotFieldInfo::GetWidgetAnnotPr()       { return m_pWidgetPr; }

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
		m_arrC.reserve(n);
		for (int i = 0; i < n; ++i)
			m_arrC.push_back(pReader->ReadDouble());
	}
	if (nFlags & (1 << 4))
	{
		m_oBorder.nType = pReader->ReadByte();
		m_oBorder.dWidth = pReader->ReadDouble();
		if (m_oBorder.nType == 2)
		{
			int n = pReader->ReadInt();
			m_oBorder.arrDash.reserve(n);
			for (int i = 0; i < n; ++i)
				m_oBorder.arrDash.push_back(pReader->ReadDouble());
		}
	}
	if (nFlags & (1 << 5))
		m_wsLM = pReader->ReadString();
	if (nFlags & (1 << 6))
	{
		m_nRenderLen = pReader->ReadInt() - 4;
		m_pRender = pReader->GetCurrentBuffer();
		pReader->Skip(m_nRenderLen);
	}
	if (nFlags & (1 << 7))
		m_wsOUserID = pReader->ReadString();
	if (nFlags & (1 << 8))
		m_nCopyAP = pReader->ReadInt();
	if (nFlags & (1 << 9))
		m_wsOMetadata = pReader->ReadString();

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
		else if (IsStamp())
			m_pStampPr->Read(pReader, nFlags);
	}
	else if (IsPopup())
		m_pPopupPr->Read(pReader);
	else if (IsWidget())
		m_pWidgetPr->Read(pReader, nType);

	return m_nType != -1;
}

CAnnotFieldInfo::CMarkupAnnotPr::~CMarkupAnnotPr()
{
	for (int i = 0; i < m_arrRC.size(); ++i)
		RELEASEOBJECT(m_arrRC[i]);
}
BYTE   CAnnotFieldInfo::CMarkupAnnotPr::GetRT()      const { return m_nRT; }
int    CAnnotFieldInfo::CMarkupAnnotPr::GetFlag()    const { return m_nFlag; }
int    CAnnotFieldInfo::CMarkupAnnotPr::GetPopupID() const { return m_nPopupID; }
int    CAnnotFieldInfo::CMarkupAnnotPr::GetIRTID()   const { return m_nIRTID; }
double CAnnotFieldInfo::CMarkupAnnotPr::GetCA()      const { return m_dCA; }
const std::wstring& CAnnotFieldInfo::CMarkupAnnotPr::GetT()    { return m_wsT; }
const std::wstring& CAnnotFieldInfo::CMarkupAnnotPr::GetCD()   { return m_wsCD; }
const std::wstring& CAnnotFieldInfo::CMarkupAnnotPr::GetSubj() { return m_wsSubj; }
const std::vector<CAnnotFieldInfo::CMarkupAnnotPr::CFontData*>& CAnnotFieldInfo::CMarkupAnnotPr::GetRC() { return m_arrRC; }
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
	{
		int nFont = pReader->ReadInt();
		m_arrRC.reserve(nFont);
		for (int i = 0; i < nFont; ++i)
		{
			CFontData* pFont = new CFontData();
			pFont->nAlignment = pReader->ReadByte();
			pFont->nFontFlag = pReader->ReadInt();
			if (pFont->nFontFlag & (1 << 5))
				pFont->dVAlign = pReader->ReadDouble();
			if (pFont->nFontFlag & (1 << 6))
				pFont->sActualFont = pReader->ReadString();
			pFont->dFontSise = pReader->ReadDouble();
			pFont->dColor[0] = pReader->ReadDouble();
			pFont->dColor[1] = pReader->ReadDouble();
			pFont->dColor[2] = pReader->ReadDouble();
			pFont->sFontFamily = pReader->ReadString();
			pFont->sText = pReader->ReadString();
			m_arrRC.push_back(pFont);
		}
	}
	if (nFlags & (1 << 4))
		m_wsCD = pReader->ReadString();
	if (nFlags & (1 << 5))
		m_nIRTID = pReader->ReadInt();
	if (nFlags & (1 << 6))
		m_nRT = pReader->ReadByte();
	if (nFlags & (1 << 7))
		m_wsSubj = pReader->ReadString();
}

CAnnotFieldInfo::CTextAnnotPr::CTextAnnotPr() : m_bOpen(false), m_nName(2), m_nState(7), m_nStateModel(2) {}
bool CAnnotFieldInfo::CTextAnnotPr::IsOpen()        const { return m_bOpen; }
BYTE CAnnotFieldInfo::CTextAnnotPr::GetName()       const { return m_nName; }
BYTE CAnnotFieldInfo::CTextAnnotPr::GetState()      const { return m_nState; }
BYTE CAnnotFieldInfo::CTextAnnotPr::GetStateModel() const { return m_nStateModel; }
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

const std::vector< std::vector<double> >& CAnnotFieldInfo::CInkAnnotPr::GetInkList() { return m_arrInkList; }
void CAnnotFieldInfo::CInkAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader)
{
	int n = pReader->ReadInt();
	for (int i = 0; i < n; ++i)
	{
		std::vector<double> arrLine;
		int m = pReader->ReadInt();
		arrLine.reserve(m);
		for (int j = 0; j < m; ++j)
			arrLine.push_back(pReader->ReadDouble());
		if (!arrLine.empty())
			m_arrInkList.push_back(arrLine);
	}
}

bool CAnnotFieldInfo::CLineAnnotPr::IsCap()    const { return m_bCap; }
BYTE CAnnotFieldInfo::CLineAnnotPr::GetIT()    const { return m_nIT; }
BYTE CAnnotFieldInfo::CLineAnnotPr::GetCP()    const { return m_nCP; }
double CAnnotFieldInfo::CLineAnnotPr::GetLL()  const { return m_dLL; }
double CAnnotFieldInfo::CLineAnnotPr::GetLLE() const { return m_dLLE; }
double CAnnotFieldInfo::CLineAnnotPr::GetLLO() const { return m_dLLO; }
void CAnnotFieldInfo::CLineAnnotPr::GetLE(BYTE& nLE1, BYTE& nLE2) { nLE1 = m_nLE[0]; nLE2 = m_nLE[1]; }
void CAnnotFieldInfo::CLineAnnotPr::GetL(double& dL1, double& dL2, double& dL3, double& dL4) { dL1 = m_dL[0]; dL2 = m_dL[1]; dL3 = m_dL[2]; dL4 = m_dL[3]; }
void CAnnotFieldInfo::CLineAnnotPr::GetCO(double& dCO1, double& dCO2) { dCO1 = m_dCO[0]; dCO2 = m_dCO[1]; }
const std::vector<double>& CAnnotFieldInfo::CLineAnnotPr::GetIC() { return m_arrIC; }
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
		m_arrIC.reserve(n);
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

BYTE CAnnotFieldInfo::CTextMarkupAnnotPr::GetSubtype() const { return m_nSubtype; }
const std::vector<double>& CAnnotFieldInfo::CTextMarkupAnnotPr::GetQuadPoints() { return m_arrQuadPoints; }
void CAnnotFieldInfo::CTextMarkupAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType)
{
	m_nSubtype = nType;
	int n = pReader->ReadInt();
	m_arrQuadPoints.reserve(n);
	for (int i = 0; i < n; ++i)
		m_arrQuadPoints.push_back(pReader->ReadDouble());
}

BYTE CAnnotFieldInfo::CSquareCircleAnnotPr::GetSubtype() const { return m_nSubtype; }
void CAnnotFieldInfo::CSquareCircleAnnotPr::GetRD(double& dRD1, double& dRD2, double& dRD3, double& dRD4) { dRD1 = m_dRD[0]; dRD2 = m_dRD[1]; dRD3 = m_dRD[2]; dRD4 = m_dRD[3]; }
const std::vector<double>& CAnnotFieldInfo::CSquareCircleAnnotPr::GetIC() { return m_arrIC; }
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
		m_arrIC.reserve(n);
		for (int i = 0; i < n; ++i)
			m_arrIC.push_back(pReader->ReadDouble());
	}
}

BYTE CAnnotFieldInfo::CPolygonLineAnnotPr::GetIT()      const { return m_nIT; }
BYTE CAnnotFieldInfo::CPolygonLineAnnotPr::GetSubtype() const { return m_nSubtype; }
void CAnnotFieldInfo::CPolygonLineAnnotPr::GetLE(BYTE& nLE1, BYTE& nLE2) { nLE1 = m_nLE[0]; nLE2 = m_nLE[1]; }
const std::vector<double>& CAnnotFieldInfo::CPolygonLineAnnotPr::GetIC() { return m_arrIC; }
const std::vector<double>& CAnnotFieldInfo::CPolygonLineAnnotPr::GetVertices() { return m_arrVertices; }
void CAnnotFieldInfo::CPolygonLineAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType, int nFlags)
{
	int n = pReader->ReadInt();
	m_arrVertices.reserve(n);
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
		m_arrIC.reserve(n);
		for (int i = 0; i < n; ++i)
			m_arrIC.push_back(pReader->ReadDouble());
	}
	if (nFlags & (1 << 20))
		m_nIT = pReader->ReadByte();
}

BYTE CAnnotFieldInfo::CFreeTextAnnotPr::GetQ()  const { return m_nQ; }
BYTE CAnnotFieldInfo::CFreeTextAnnotPr::GetIT() const { return m_nIT; }
BYTE CAnnotFieldInfo::CFreeTextAnnotPr::GetLE() const { return m_nLE; }
int CAnnotFieldInfo::CFreeTextAnnotPr::GetRotate() { return m_nRotate; }
const std::wstring& CAnnotFieldInfo::CFreeTextAnnotPr::GetDS() { return m_wsDS; }
void CAnnotFieldInfo::CFreeTextAnnotPr::GetRD(double& dRD1, double& dRD2, double& dRD3, double& dRD4) { dRD1 = m_dRD[0]; dRD2 = m_dRD[1]; dRD3 = m_dRD[2]; dRD4 = m_dRD[3]; }
const std::vector<double>& CAnnotFieldInfo::CFreeTextAnnotPr::GetCL() { return m_arrCL; }
const std::vector<double>& CAnnotFieldInfo::CFreeTextAnnotPr::GetIC() { return m_arrIC; }
void CAnnotFieldInfo::CFreeTextAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags)
{
	m_nQ = pReader->ReadByte();
	m_nRotate = pReader->ReadInt();
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
		m_arrCL.reserve(n);
		for (int i = 0; i < n; ++i)
			m_arrCL.push_back(pReader->ReadDouble());
	}
	if (nFlags & (1 << 17))
		m_wsDS = pReader->ReadString();
	if (nFlags & (1 << 18))
		m_nLE = pReader->ReadByte();
	if (nFlags & (1 << 20))
		m_nIT = pReader->ReadByte();
	if (nFlags & (1 << 21))
	{
		int n = pReader->ReadInt();
		m_arrIC.reserve(n);
		for (int i = 0; i < n; ++i)
			m_arrIC.push_back(pReader->ReadDouble());
	}
}

BYTE CAnnotFieldInfo::CCaretAnnotPr::GetSy() const { return m_nSy; }
void CAnnotFieldInfo::CCaretAnnotPr::GetRD(double& dRD1, double& dRD2, double& dRD3, double& dRD4) { dRD1 = m_dRD[0]; dRD2 = m_dRD[1]; dRD3 = m_dRD[2]; dRD4 = m_dRD[3]; }
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

double CAnnotFieldInfo::CStampAnnotPr::GetRotate() { return m_nRotate; }
const std::wstring& CAnnotFieldInfo::CStampAnnotPr::GetName() { return m_wsName; }
void CAnnotFieldInfo::CStampAnnotPr::GetInRect(double& dRD1, double& dRD2, double& dRD3, double& dRD4) { dRD1 = m_dInRect[0]; dRD2 = m_dInRect[1]; dRD3 = m_dInRect[2]; dRD4 = m_dInRect[3]; }
void CAnnotFieldInfo::CStampAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags)
{
	m_wsName = pReader->ReadString();
	m_nRotate = pReader->ReadDouble();
	m_dInRect[0] = pReader->ReadDouble();
	m_dInRect[1] = pReader->ReadDouble();
	m_dInRect[2] = pReader->ReadDouble();
	m_dInRect[3] = pReader->ReadDouble();
}

bool CAnnotFieldInfo::CPopupAnnotPr::IsOpen()      const { return m_bOpen; }
int  CAnnotFieldInfo::CPopupAnnotPr::GetFlag()     const { return m_nFlag; }
int  CAnnotFieldInfo::CPopupAnnotPr::GetParentID() const { return m_nParentID; }
void CAnnotFieldInfo::CPopupAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader)
{
	m_nFlag = pReader->ReadInt();
	m_bOpen = m_nFlag & (1 << 0);
	if (m_nFlag & (1 << 1))
		m_nParentID = pReader->ReadInt();
}

BYTE CAnnotFieldInfo::CWidgetAnnotPr::GetQ()    const { return m_nQ; }
BYTE CAnnotFieldInfo::CWidgetAnnotPr::GetH()    const { return m_nH; }
BYTE CAnnotFieldInfo::CWidgetAnnotPr::GetType() const { return m_nType; }
int  CAnnotFieldInfo::CWidgetAnnotPr::GetR()    const { return m_nR; }
int  CAnnotFieldInfo::CWidgetAnnotPr::GetFlag()      const { return m_nFlag; }
int  CAnnotFieldInfo::CWidgetAnnotPr::GetFlags()     const { return m_nFlags; }
int  CAnnotFieldInfo::CWidgetAnnotPr::GetParentID()  const { return m_nParentID; }
int  CAnnotFieldInfo::CWidgetAnnotPr::GetFontStyle() const { return m_nFontStyle; }
double CAnnotFieldInfo::CWidgetAnnotPr::GetFontSize()   const { return m_dFS; }
double CAnnotFieldInfo::CWidgetAnnotPr::GetFontSizeAP() const { return m_dFSAP; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::GetTU() { return m_wsTU; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::GetDS() { return m_wsDS; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::GetDV() { return m_wsDV; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::GetT()  { return m_wsT; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::GetFontName()  { return m_wsFN; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::GetFontKey()   { return m_wsFK; }
const std::vector<double>& CAnnotFieldInfo::CWidgetAnnotPr::GetTC() { return m_arrTC; }
const std::vector<double>& CAnnotFieldInfo::CWidgetAnnotPr::GetBC() { return m_arrBC; }
const std::vector<double>& CAnnotFieldInfo::CWidgetAnnotPr::GetBG() { return m_arrBG; }
const std::vector<CAnnotFieldInfo::CWidgetAnnotPr::CActionWidget*>& CAnnotFieldInfo::CWidgetAnnotPr::GetActions() { return m_arrAction; }
CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr*    CAnnotFieldInfo::CWidgetAnnotPr::GetButtonWidgetPr()    { return m_pButtonPr; }
CAnnotFieldInfo::CWidgetAnnotPr::CTextWidgetPr*      CAnnotFieldInfo::CWidgetAnnotPr::GetTextWidgetPr()      { return m_pTextPr; }
CAnnotFieldInfo::CWidgetAnnotPr::CChoiceWidgetPr*    CAnnotFieldInfo::CWidgetAnnotPr::GetChoiceWidgetPr()    { return m_pChoicePr; }
CAnnotFieldInfo::CWidgetAnnotPr::CSignatureWidgetPr* CAnnotFieldInfo::CWidgetAnnotPr::GetSignatureWidgetPr() { return m_pSignaturePr; }
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

CAnnotFieldInfo::CWidgetAnnotPr::CActionWidget::CActionWidget() : pNext(NULL) {}
CAnnotFieldInfo::CWidgetAnnotPr::CActionWidget::~CActionWidget() { RELEASEOBJECT(pNext); }
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
		pRes->arrStr.reserve(n);
		for (int i = 0; i < n; ++i)
			pRes->arrStr.push_back(pReader->ReadString());
		break;
	}
	case 12: // ResetForm
	{
		pRes->nInt1 = pReader->ReadInt();
		int n = pReader->ReadInt();
		pRes->arrStr.reserve(n);
		for (int i = 0; i < n; ++i)
			pRes->arrStr.push_back(pReader->ReadString());
		break;
	}
	}

	if (pReader->ReadByte())
		pRes->pNext = ReadAction(pReader);

	return pRes;
}
void CAnnotFieldInfo::CWidgetAnnotPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType)
{
	m_wsFN = pReader->ReadString();
	m_dFS = pReader->ReadDouble();
	m_dFSAP = pReader->ReadDouble();
	m_nFontStyle = pReader->ReadInt();

	int n = pReader->ReadInt();
	m_arrTC.reserve(n);
	for (int i = 0; i < n; ++i)
		m_arrTC.push_back(pReader->ReadDouble());

	m_nQ = 0;
	if (nType != 29 && nType != 28 && nType != 27)
		m_nQ = pReader->ReadByte();
	int nWidgetFlag = pReader->ReadInt();
	m_nFlag = nWidgetFlag;

	int nFlags = pReader->ReadInt();
	m_nFlags = nFlags;
	if (nFlags & (1 << 0))
		m_wsTU = pReader->ReadString();
	if (nFlags & (1 << 1))
		m_wsDS = pReader->ReadString();
	if (nFlags & (1 << 2))
		m_wsFK = pReader->ReadString();
	if (nFlags & (1 << 3))
		m_nH = pReader->ReadByte();
	if (nFlags & (1 << 5))
	{
		int n = pReader->ReadInt();
		m_arrBC.reserve(n);
		for (int i = 0; i < n; ++i)
			m_arrBC.push_back(pReader->ReadDouble());
	}
	if (nFlags & (1 << 6))
		m_nR = pReader->ReadInt();
	if (nFlags & (1 << 7))
	{
		int n = pReader->ReadInt();
		m_arrBG.reserve(n);
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

BYTE CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::GetS() const  { return m_nS; }
BYTE CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::GetTP() const { return m_nTP; }
BYTE CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::GetSW() const { return m_nSW; }
BYTE CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::GetStyle()  const { return m_nStyle; }
int  CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::GetIFFlag() const { return m_nIFFlag; }
int  CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::GetI()      const { return m_nI; }
int  CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::GetRI()     const { return m_nRI; }
int  CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::GetIX()     const { return m_nIX; }
void CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::GetA(double& dA1, double& dA2) { dA1 = m_dA1; dA2 = m_dA2; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::GetV()  { return m_wsV; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::GetCA() { return m_wsCA; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::GetRC() { return m_wsRC; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::GetAC() { return m_wsAC; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::GetAP_N_Yes() { return m_wsAP_N_Yes; }
void CAnnotFieldInfo::CWidgetAnnotPr::CButtonWidgetPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, BYTE nType, int nFlags)
{
	if (nType == 27)
	{
		m_nIFFlag = pReader->ReadInt();

		if (nFlags & (1 << 10))
			m_wsCA = pReader->ReadString();
		if (nFlags & (1 << 11))
			m_wsRC = pReader->ReadString();
		if (nFlags & (1 << 12))
			m_wsAC = pReader->ReadString();
		if (nFlags & (1 << 13))
			m_nTP = pReader->ReadByte();

		if (m_nIFFlag & (1 << 0))
		{
			if (m_nIFFlag & (1 << 1))
				m_nSW = pReader->ReadByte();
			if (m_nIFFlag & (1 << 2))
				m_nS = pReader->ReadByte();
			if (m_nIFFlag & (1 << 3))
			{
				m_dA1 = pReader->ReadDouble();
				m_dA2 = pReader->ReadDouble();
			}
		}

		if (m_nIFFlag & (1 << 5))
			m_nI = pReader->ReadInt();
		if (m_nIFFlag & (1 << 6))
			m_nRI = pReader->ReadInt();
		if (m_nIFFlag & (1 << 7))
			m_nIX = pReader->ReadInt();
	}
	else
	{
		if (nFlags & (1 << 9))
			m_wsV = pReader->ReadString();
		m_nStyle = pReader->ReadByte();
		if (nFlags & (1 << 14))
			m_wsAP_N_Yes = pReader->ReadString();
	}
}

int CAnnotFieldInfo::CWidgetAnnotPr::CTextWidgetPr::GetMaxLen() const { return m_nMaxLen; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::CTextWidgetPr::GetV()   { return m_wsV; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::CTextWidgetPr::GetRV()  { return m_wsRV; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::CTextWidgetPr::GetAPV() { return m_wsAPV; }
BYTE* CAnnotFieldInfo::CWidgetAnnotPr::CTextWidgetPr::GetRender(LONG& nLen)
{
	nLen = m_nRenderLen;
	return m_pRender;
}
void CAnnotFieldInfo::CWidgetAnnotPr::CTextWidgetPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags, int nWidgetFlag)
{
	if (nFlags & (1 << 9))
		m_wsV = pReader->ReadString();
	if (nFlags & (1 << 10))
		m_nMaxLen = pReader->ReadInt();
	if (nFlags & (1 << 11))
		m_wsRV = pReader->ReadString();
	if (nFlags & (1 << 12))
		m_wsAPV = pReader->ReadString();
	if (nFlags & (1 << 13))
	{
		m_nRenderLen = pReader->ReadInt() - 4;
		m_pRender = pReader->GetCurrentBuffer();
		pReader->Skip(m_nRenderLen);
	}
}

int CAnnotFieldInfo::CWidgetAnnotPr::CChoiceWidgetPr::GetTI() const { return m_nTI; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::CChoiceWidgetPr::GetV() { return m_wsV; }
const std::wstring& CAnnotFieldInfo::CWidgetAnnotPr::CChoiceWidgetPr::GetAPV() { return m_wsAPV; }
const std::vector<int>& CAnnotFieldInfo::CWidgetAnnotPr::CChoiceWidgetPr::GetI() { return m_arrI; }
const std::vector<std::wstring>& CAnnotFieldInfo::CWidgetAnnotPr::CChoiceWidgetPr::GetArrV() { return m_arrV; }
const std::vector< std::pair<std::wstring, std::wstring> >& CAnnotFieldInfo::CWidgetAnnotPr::CChoiceWidgetPr::GetOpt() { return m_arrOpt; }
BYTE* CAnnotFieldInfo::CWidgetAnnotPr::CChoiceWidgetPr::GetRender(LONG& nLen)
{
	nLen = m_nRenderLen;
	return m_pRender;
}
void CAnnotFieldInfo::CWidgetAnnotPr::CChoiceWidgetPr::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, int nFlags)
{
	if (nFlags & (1 << 9))
		m_wsV = pReader->ReadString();
	if (nFlags & (1 << 10))
	{
		int n = pReader->ReadInt();
		m_arrOpt.reserve(n);
		for (int i = 0; i < n; ++i)
		{
			std::wstring s1 = pReader->ReadString();
			std::wstring s2 = pReader->ReadString();
			m_arrOpt.push_back(std::make_pair(s1, s2));
		}
	}
	if (nFlags & (1 << 11))
		m_nTI = pReader->ReadInt();
	if (nFlags & (1 << 12))
		m_wsAPV = pReader->ReadString();
	if (nFlags & (1 << 13))
	{
		int n = pReader->ReadInt();
		m_arrV.reserve(n);
		for (int i = 0; i < n; ++i)
			m_arrV.push_back(pReader->ReadString());
	}
	if (nFlags & (1 << 14))
	{
		int n = pReader->ReadInt();
		m_arrI.reserve(n);
		for (int i = 0; i < n; ++i)
			m_arrI.push_back(pReader->ReadInt());
	}
	if (nFlags & (1 << 15))
	{
		m_nRenderLen = pReader->ReadInt() - 4;
		m_pRender = pReader->GetCurrentBuffer();
		pReader->Skip(m_nRenderLen);
	}
}

CAnnotFieldDelete::CAnnotFieldDelete() : IAdvancedCommand(AdvancedCommandType::DeleteAnnot) {}
CAnnotFieldDelete::~CAnnotFieldDelete() {}
int CAnnotFieldDelete::GetID() { return m_nID; }
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
const std::vector< std::pair<int, int> >& CWidgetsInfo::GetCO() { return m_arrCO; }
const std::vector<std::wstring>& CWidgetsInfo::GetButtonImg() { return m_arrButtonImg; }
const std::vector<CWidgetsInfo::CParent*>& CWidgetsInfo::GetParents() { return m_arrParents; }
void CWidgetsInfo::ChangeCO(int i, int nNum, int nGen)
{
	if (i < 0 || i > m_arrCO.size() - 1)
		return;
	m_arrCO[i] = std::make_pair(nNum, nGen);
}
bool CWidgetsInfo::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector)
{
	int n = pReader->ReadInt();
	m_arrCO.reserve(n);
	for (int i = 0; i < n; ++i)
		m_arrCO.push_back(std::make_pair(pReader->ReadInt(), -1));

	n = pReader->ReadInt();
	m_arrParents.reserve(n);
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
		{
			int n = pReader->ReadInt();
			pParent->arrI.reserve(n);
			for (int i = 0; i < n; ++i)
				pParent->arrI.push_back(pReader->ReadInt());
		}
		if (nFlags & (1 << 4))
			pParent->nParentID = pReader->ReadInt();
		if (nFlags & (1 << 5))
		{
			int n = pReader->ReadInt();
			pParent->arrV.reserve(n);
			for (int i = 0; i < n; ++i)
				pParent->arrV.push_back(pReader->ReadString());
		}
		if (nFlags & (1 << 6))
		{
			int n = pReader->ReadInt();
			pParent->arrOpt.reserve(n);
			for (int i = 0; i < n; ++i)
			{
				std::wstring s1 = pReader->ReadString();
				std::wstring s2 = pReader->ReadString();
				pParent->arrOpt.push_back(std::make_pair(s1, s2));
			}
		}
		if (nFlags & (1 << 7))
			pParent->nFieldFlag = pReader->ReadInt();
		if (nFlags & (1 << 8))
		{
			// Action
			int nAction = pReader->ReadInt();
			for (int i = 0; i < nAction; ++i)
			{
				std::wstring wsType = pReader->ReadString();
				CAnnotFieldInfo::CWidgetAnnotPr::CActionWidget* pA = ReadAction(pReader);
				if (pA)
				{
					pA->wsType = wsType;
					pParent->arrAction.push_back(pA);
				}
			}
		}
		if (nFlags & (1 << 9))
			pParent->nMaxLen = pReader->ReadInt();
		if (nFlags & (1 << 10))
			pParent->sTU = pReader->ReadString();
		m_arrParents.push_back(pParent);
	}

	n = pReader->ReadInt();
	m_arrButtonImg.reserve(n);
	for (int i = 0; i < n; ++i)
	{
		std::string sImagePath = pReader->ReadStringA();
		std::wstring sImage = UTF8_TO_U(sImagePath);
		if (sImagePath.find("data:") != 0 && !sImagePath.empty())
			sImage = pCorrector->GetImagePath(sImage);
		m_arrButtonImg.push_back(sImage);
	}

	return true;
}
