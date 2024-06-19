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

#include "./DocInfo.h"

// CHyperlinkCommand
CHyperlinkCommand::CHyperlinkCommand() : IAdvancedCommand(AdvancedCommandType::Hyperlink) {}
CHyperlinkCommand::~CHyperlinkCommand() {}

double CHyperlinkCommand::GetX() { return m_dX; }
double CHyperlinkCommand::GetY() { return m_dY; }
double CHyperlinkCommand::GetW() { return m_dW; }
double CHyperlinkCommand::GetH() { return m_dH; }

void CHyperlinkCommand::SetX(const double& val) { m_dX = val; }
void CHyperlinkCommand::SetY(const double& val) { m_dY = val; }
void CHyperlinkCommand::SetW(const double& val) { m_dW = val; }
void CHyperlinkCommand::SetH(const double& val) { m_dH = val; }

std::wstring CHyperlinkCommand::CHyperlinkCommand::GetUrl() { return m_sUrl; }
std::wstring CHyperlinkCommand::CHyperlinkCommand::GetToolTip() { return m_sToolTip; }

void CHyperlinkCommand::SetUrl(const std::wstring& val) { m_sUrl = val; }
void CHyperlinkCommand::SetToolTip(const std::wstring& val) { m_sToolTip = val; }

bool CHyperlinkCommand::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector)
{
	m_dX = pReader->ReadDouble();
	m_dY = pReader->ReadDouble();
	m_dW = pReader->ReadDouble();
	m_dH = pReader->ReadDouble();

	m_sUrl = pReader->ReadString();
	m_sToolTip = pReader->ReadString();

	return true;
}

// CLinkCommand
CLinkCommand::CLinkCommand() : IAdvancedCommand(AdvancedCommandType::Link) {}
CLinkCommand::~CLinkCommand() {}

double CLinkCommand::GetX() { return m_dX; }
double CLinkCommand::GetY() { return m_dY; }
double CLinkCommand::GetW() { return m_dW; }
double CLinkCommand::GetH() { return m_dH; }

void CLinkCommand::SetX(const double& val) { m_dX = val; }
void CLinkCommand::SetY(const double& val) { m_dY = val; }
void CLinkCommand::SetW(const double& val) { m_dW = val; }
void CLinkCommand::SetH(const double& val) { m_dH = val; }

int CLinkCommand::GetPage() { return m_nPage; }
double CLinkCommand::GetDestX() { return m_dDestX; }
double CLinkCommand::GetDestY() { return m_dDestY; }

void CLinkCommand::SetPage(const int& val) { m_nPage = val; }
void CLinkCommand::SetDestX(const double& val) { m_dDestX = val; }
void CLinkCommand::SetDestY(const double& val) { m_dDestY = val; }

bool CLinkCommand::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector)
{
	m_dX = pReader->ReadDouble();
	m_dY = pReader->ReadDouble();
	m_dW = pReader->ReadDouble();
	m_dH = pReader->ReadDouble();

	m_dDestX = pReader->ReadDouble();
	m_dDestY = pReader->ReadDouble();
	m_nPage  = pReader->ReadInt();

	return true;
}

// CDocInfoCommand
CDocInfoCommand::CDocInfoCommand() : IAdvancedCommand(AdvancedCommandType::DocInfo) {}
CDocInfoCommand::~CDocInfoCommand() {}

std::wstring CDocInfoCommand::GetTitle() { return m_sTitle; }
std::wstring CDocInfoCommand::GetCreator() { return m_sCreator; }
std::wstring CDocInfoCommand::GetSubject() { return m_sSubject; }
std::wstring CDocInfoCommand::GetKeywords() { return m_sKeywords; }

void CDocInfoCommand::SetTitle(const std::wstring& val) { m_sTitle = val; }
void CDocInfoCommand::SetCreator(const std::wstring& val) { m_sCreator = val; }
void CDocInfoCommand::SetSubject(const std::wstring& val) { m_sSubject = val; }
void CDocInfoCommand::SetKeywords(const std::wstring& val) { m_sKeywords = val; }

bool CDocInfoCommand::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector)
{
	int nFlags = pReader->ReadInt();
	if (nFlags & 1)
		m_sTitle = pReader->ReadString();
	if (nFlags & 2)
		m_sCreator = pReader->ReadString();
	if (nFlags & 4)
		m_sSubject = pReader->ReadString();
	if (nFlags & 8)
		m_sKeywords = pReader->ReadString();
	return true;
}

CShapeStart::CShapeStart() : IAdvancedCommand(AdvancedCommandType::ShapeStart)
{
	m_pImage = NULL;
}
CShapeStart::~CShapeStart()
{
	RELEASEINTERFACE(m_pImage);
}
void CShapeStart::SetShapeXML(const std::string& sShapeXML) { m_sShapeXML = sShapeXML; }
void CShapeStart::SetShapeImage(BYTE* pImgData, int nWidth, int nHeight)
{
	if (pImgData)
	{
		m_pImage = new Aggplus::CImage();
		m_pImage->Create(pImgData, nWidth, nHeight, -4 * nWidth);
	}
}
std::string& CShapeStart::GetShapeXML() { return m_sShapeXML; }
Aggplus::CImage* CShapeStart::GetShapeImage() { return m_pImage; }
bool CShapeStart::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector)
{
	m_sShapeXML = pReader->ReadStringA();
	return true;
}

CEmptyComand::CEmptyComand(AdvancedCommandType nType) : IAdvancedCommand(nType) {}

CPageRotate::CPageRotate() : IAdvancedCommand(AdvancedCommandType::PageRotate) {}
int CPageRotate::GetPageRotate() { return m_nPageRotate; }
bool CPageRotate::Read(NSOnlineOfficeBinToPdf::CBufferReader* pReader, IMetafileToRenderter* pCorrector)
{
	m_nPageRotate = pReader->ReadInt();
	return true;
}

