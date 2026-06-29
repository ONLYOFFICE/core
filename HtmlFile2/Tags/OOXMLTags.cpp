/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "OOXMLTags.h"

#include "../src/StringFinder.h"
#include "../Table.h"

#include "../../Common/Network/FileTransporter/include/FileTransporter.h"

#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../DesktopEditor/common/Base64.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Path.h"

#include "../../Common/3dParty/html/gumbo-parser/src/gumbo.h"

namespace HTML
{
#define DEFAULT_PAGE_WIDTH  12240 // Value in Twips
#define DEFAULT_PAGE_HEIGHT 15840 // Value in Twips

#define MAX_COLUMNS_IN_TABLE 63
#define MAX_ROWS_IN_TABLE    32767

inline bool ElementInTable(const std::vector<NSCSS::CNode>& arSelectors);

inline bool NotValidExtension(const std::wstring& sExtention);
inline bool IsSVG(const std::wstring& wsExtention);
inline bool IsMetafile(const std::wstring& wsExtention);
bool ReadMetafileData(const std::wstring& wsFilePath, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, const std::wstring& wsImagePath);
bool ReadSVGData(const std::wstring& wsSvg, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, const std::wstring& wsImagePath);
bool ReadBase64(const std::wstring& wsSrc, const std::wstring& wsImagePath, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, std::wstring& wsExtention);
bool GetStatusUsingExternalLocalFiles();
bool CanUseThisPath(const std::wstring& wsPath, const std::wstring& wsSrcPath, const std::wstring& wsCorePath, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, bool bIsAllowExternalLocalFiles);

bool CopyImage(std::wstring wsImageSrc, const std::wstring& wsSrc, const std::wstring& wsDst, bool bIsAllowExternalLocalFiles);
bool UpdateImageData(const std::wstring& wsImagePath, TImageData& oImageData);

const static double HTML_FONTS[7] = {7.5, 10, 12, 13.5, 18, 24, 36};

void InitTagsForOOXML(std::map<int, std::shared_ptr<ITag> >& mTags, COOXMLWriter* pWriter)
{
	#define HTML_TAG(tag) GUMBO_TAG_##tag
	#define ADD_TAG(tag_name, tag_class_name) mTags[HTML_TAG(tag_name)] = std::make_shared<C ##tag_class_name##OOXMLTag> (pWriter)

	ADD_TAG(A,          Anchor);
	ADD_TAG(ABBR,       Abbr);
	ADD_TAG(BR,         Break);
	ADD_TAG(FONT,       Font);
	ADD_TAG(INPUT,      Input);
	ADD_TAG(BASEFONT,   BaseFont);
	ADD_TAG(OL,         List);
	ADD_TAG(LI,         ListElement);
	ADD_TAG(HTML,       HTML);
	ADD_TAG(DIV,        Division);
	ADD_TAG(IMG,        Image);
	ADD_TAG(BLOCKQUOTE, Blockquote);
	ADD_TAG(HR,         HorizontalRule);
}

bool CAnchorOOXMLTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if(!Valid())
		return false;

	std::wstring wsRef, wsAlt, wsName;
	bool bCross = false;

	if (arSelectors.back().GetAttributeValue(L"href", wsRef) && wsRef.find('#') != std::wstring::npos)
		bCross = true;

	if (arSelectors.back().GetAttributeValue(L"name", wsName))
		m_pWriter->WriteEmptyBookmark(wsName);

	arSelectors.back().GetAttributeValue(L"alt", wsAlt);

	std::wstring wsFootnote;

	if (arSelectors.back().m_wsStyle.find(L"mso-footnote-id") != std::wstring::npos)
		wsFootnote = arSelectors.back().m_wsStyle.substr(arSelectors.back().m_wsStyle.rfind(L':') + 1);
	else
	{
		if (arSelectors.back().GetAttributeValue(L"epub:type", wsFootnote) && wsFootnote.find(L"noteref"))
			wsFootnote = L"href";
	}

	bool bFootnote = false;
	if (arSelectors.size() > 1)
	{
		const NSCSS::CNode& oNode = arSelectors[arSelectors.size() - 2];
		bFootnote = oNode.m_wsName == L"p" && oNode.m_wsClass == L"MsoFootnoteText";
	}

	if (bCross)
		m_pWriter->SetHyperlinkData(wsRef, L"", true, wsFootnote, bFootnote);
	else
	{
		std::wstring wsTooltip(wsRef);
		arSelectors.back().GetAttributeValue(L"title", wsTooltip);

		m_pWriter->SetHyperlinkData(wsRef, wsTooltip, false, wsFootnote, bFootnote);
	}

	return true;
}

void CAnchorOOXMLTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return;

	m_pWriter->ClearHyperlinkData();
}

bool CAbbrOOXMLTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if(!Valid())
		return false;

	std::wstring wsTitle;

	if (!arSelectors.back().GetAttributeValue(L"title", wsTitle))
		return false;

	m_pWriter->WritePPr(arSelectors);

	XmlString* pCurrentDocument{m_pWriter->GetCurrentDocument()};

	pCurrentDocument->WriteString(L"<w:r><w:fldChar w:fldCharType=\"begin\"/></w:r><w:r><w:instrText>HYPERLINK \\l \"" + m_pWriter->AddLiteBookmark() + L"\" \\o \"");
	pCurrentDocument->WriteEncodeXmlString(wsTitle);
	pCurrentDocument->WriteString(L"\"</w:instrText></w:r>");
	pCurrentDocument->WriteString(L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>");

	return true;
}

void CAbbrOOXMLTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return;

	m_pWriter->GetCurrentDocument()->WriteString(L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>");
}

bool CBreakOOXMLTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	m_pWriter->Break(arSelectors.back());

	return true;
}

void CBreakOOXMLTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CDivisionOOXMLTag::CDivisionOOXMLTag(COOXMLWriter* pWriter)
	: IHTMLTag(pWriter)
{}

bool CDivisionOOXMLTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	m_pWriter->UpdatePageStyle(arSelectors);

	UINT unMsoFootnote = 0;

	if (L"footnote" == arSelectors.back().GetAttributeValue(L"epub:type"))
		++unMsoFootnote;

	if (std::wstring::npos != arSelectors.back().m_wsStyle.find(L"mso-element:footnote"))
		++unMsoFootnote;

	std::wstring wsFootnoteID;

	if (!arSelectors.back().m_wsId.empty())
	{
		wsFootnoteID = m_pWriter->FindFootnote(arSelectors.back().m_wsId);

		if (!wsFootnoteID.empty())
			++unMsoFootnote;

		if (unMsoFootnote >= 2 && !wsFootnoteID.empty())
		{
			m_pWriter->OpenFootnote(wsFootnoteID);
			m_pWriter->SetCurrentDocument(&m_pWriter->GetNotesXml());
		}
	}

	m_arFootnoteIDs.push(unMsoFootnote);

	return true;
}

void CDivisionOOXMLTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid() || m_arFootnoteIDs.empty())
		return;

	if (m_arFootnoteIDs.top() >= 2)
	{
		m_pWriter->CloseFootnote();
		m_pWriter->RollBackState();
	}

	m_pWriter->CloseP();
	m_arFootnoteIDs.pop();
}


CImageOOXMLTag::CImageOOXMLTag(COOXMLWriter* pWriter)
	: IHTMLTag(pWriter)
{}

bool CImageOOXMLTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	if (L"svg" == arSelectors.back().m_wsName)
	{
		const std::wstring wsImagePath{m_pWriter->GetMediaDir() + L'i' + std::to_wstring(m_arImages.size()) + L".png"};

		if (!ReadSVGData(arSelectors.back().GetAttributeValue(L"svg-text"), m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsImagePath))
			return false;

		TImageData oNewImageData;
		if (!UpdateImageData(wsImagePath, oNewImageData))
			return false;

		m_pWriter->WritePPr(arSelectors);
		m_pWriter->WriteImage(oNewImageData, std::to_wstring(m_arImages.size()));
		return true;
	}

	const std::wstring wsAlt{arSelectors.back().GetAttributeValue(L"alt")};
	std::wstring wsSrc{arSelectors.back().GetAttributeValue(L"src")};

	TImageData oImageData;

	std::wstring wsTempValue;
	NSCSS::NSProperties::CDigit oTempDigit;

	#define GET_UNIT_DATA(variable_data, data_variable)\
	if (NSCSS::UnitMeasure::None == data_variable.GetUnitMeasure())\
		variable_data = static_cast<UINT>(NSCSS::CUnitMeasureConverter::ConvertPx(data_variable.ToDouble(), NSCSS::Inch, 96) * 914400);\
	else\
		variable_data = static_cast<UINT>(data_variable.ToDouble(NSCSS::Inch) * 914400)

	#define READ_IMAGE_DATA(name_data, variable_data)\
	if (arSelectors.back().GetAttributeValue(name_data, wsTempValue))\
	{\
		if (oTempDigit.SetValue(wsTempValue))\
		{\
			GET_UNIT_DATA(variable_data, oTempDigit);\
		}\
	}

	READ_IMAGE_DATA(L"width", oImageData.m_unWidth);
	READ_IMAGE_DATA(L"height", oImageData.m_unHeight);
	READ_IMAGE_DATA(L"hspace", oImageData.m_nHSpace);
	READ_IMAGE_DATA(L"vspace", oImageData.m_nVSpace);

	if (nullptr != arSelectors.back().m_pCompiledStyle)
	{
		oTempDigit = arSelectors.back().m_pCompiledStyle->m_oDisplay.GetWidth();

		if (0 == oImageData.m_unWidth && !oTempDigit.Empty())
		{
			GET_UNIT_DATA(oImageData.m_unWidth, oTempDigit);
		}

		oTempDigit = arSelectors.back().m_pCompiledStyle->m_oDisplay.GetHeight();

		if (0 == oImageData.m_unHeight && !oTempDigit.Empty())
		{
			GET_UNIT_DATA(oImageData.m_unHeight, oTempDigit);
		}
	}

	if (wsSrc.empty())
	{
		m_pWriter->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
		return true;
	}

	bool bRes = false;
	std::wstring wsExtention;
	const std::wstring wsImagePath{m_pWriter->GetMediaDir() + L'i' + std::to_wstring(m_arImages.size())};

	// Assume Base64-encoded image
	if (wsSrc.length() > 4 && wsSrc.substr(0, 4) == L"data" && wsSrc.find(L"/", 4) != std::wstring::npos)
		bRes = ReadBase64(wsSrc, wsImagePath, m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsExtention);

	const bool bIsAllowExternalLocalFiles = GetStatusUsingExternalLocalFiles();

	if (!bRes && (wsSrc.length() <= 7 || L"http" != wsSrc.substr(0, 4)))
	{
		wsSrc = NSSystemPath::ShortenPath(wsSrc);

		if (!CanUseThisPath(wsSrc, m_pWriter->GetSrcPath(), m_pWriter->GetCorePath(), bIsAllowExternalLocalFiles))
		{
			m_pWriter->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
			return true;
		}
	}

	// Check extension
	if (!bRes)
	{
		wsExtention = NSFile::GetFileExtention(wsSrc);
		std::transform(wsExtention.begin(), wsExtention.end(), wsExtention.begin(), tolower);

		std::wstring::const_iterator itFound = std::find_if(wsExtention.cbegin(), wsExtention.cend(), [](wchar_t wChar){ return !iswalpha(wChar) && L'+' != wChar; });

		if (wsExtention.cend() != itFound)
			wsExtention.erase(itFound, wsExtention.cend());
	}

	const std::wstring wsBasePath{m_pWriter->GetBasePath()};

	// Assume network image
	if (!bRes &&
	    ((!wsBasePath.empty() && wsBasePath.length() > 4 && wsBasePath.substr(0, 4) == L"http") ||
	      (wsSrc.length() > 4 && wsSrc.substr(0, 4) == L"http")))
	{
		if (!wsExtention.empty() && NotValidExtension(wsExtention))
		{
			m_pWriter->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
			return true;
		}

		const std::wstring wsDst = wsImagePath + L'.' + ((!wsExtention.empty()) ? wsExtention : L"png");

		// gc_allowNetworkRequest check is assumed in kernel_network
		NSNetwork::NSFileTransport::CFileDownloader oDownloadImg(m_pWriter->GetBasePath() + wsSrc, false);
		oDownloadImg.SetFilePath(wsDst);
		bRes = oDownloadImg.DownloadSync();

		if (!bRes)
		{
			m_pWriter->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
			return true;
		}

		if (IsSVG(wsExtention))
		{
			std::wstring wsFileData;

			if (!NSFile::CFileBinary::ReadAllTextUtf8(wsDst, wsFileData) ||
			    !ReadSVGData(wsFileData, m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsImagePath))
				bRes = false;

			NSFile::CFileBinary::Remove(wsDst);
			wsExtention = L"png";
		}
		else if (wsExtention.empty())
		{
			//TODO:: better to detect image format from content
			wsExtention = L"png";
		}
	}

	int nImageId = -1;

	if (!bRes)
	{
		if (NotValidExtension(wsExtention))
		{
			m_pWriter->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
			return true;
		}

		// Check for duplicate
		const std::vector<std::wstring>::const_iterator nFind = std::find(m_arImages.cbegin(), m_arImages.cend(), wsSrc);
		if (nFind != m_arImages.end())
		{
			bRes = true;
			nImageId = nFind - m_arImages.cbegin();
		}
	}

	// Assume image by local path
	if (IsSVG(wsExtention) || IsMetafile(wsExtention))
	{
		const std::wstring wsDst = wsImagePath + L".png";

		if (!m_pWriter->GetBasePath().empty())
		{
			if (!bRes)
				bRes = ReadMetafileData(NSSystemPath::Combine(m_pWriter->GetBasePath(), wsSrc), m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsDst);
			if (!bRes)
				bRes = ReadMetafileData(NSSystemPath::Combine(m_pWriter->GetSrcPath(), NSSystemPath::Combine(m_pWriter->GetBasePath(), wsSrc)), m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsDst);
		}
		if (!bRes)
			bRes = ReadMetafileData(NSSystemPath::Combine(m_pWriter->GetSrcPath(), wsSrc), m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsDst);
		if (!bRes)
			bRes = ReadMetafileData(wsSrc, m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsDst);

		if (bRes)
			wsExtention = L"png";
	}

	if (!bRes)
	{
		const std::wstring wsDst = wsImagePath + L'.' + wsExtention;

		if (!m_pWriter->GetBasePath().empty())
		{
			if (!bRes)
				bRes = CopyImage(NSSystemPath::Combine(m_pWriter->GetBasePath(), wsSrc), m_pWriter->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
			if (!bRes)
				bRes = CopyImage(NSSystemPath::Combine(m_pWriter->GetSrcPath(), NSSystemPath::Combine(m_pWriter->GetBasePath(), wsSrc)), m_pWriter->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
		}
		if (!bRes)
			bRes = CopyImage(NSSystemPath::Combine(m_pWriter->GetSrcPath(), wsSrc), m_pWriter->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
		if (!bRes)
			bRes = CopyImage(wsSrc, m_pWriter->GetSrcPath(), wsDst, bIsAllowExternalLocalFiles);
	}

	if (!bRes)
		m_pWriter->WriteAlternativeImage(wsAlt, wsSrc, oImageData);
	else
	{
		m_pWriter->WritePPr(arSelectors);

		const std::wstring wsImageID{std::to_wstring(m_arImages.size())};
		m_arImages.push_back(wsSrc);

		if (nImageId < 0)
		{
			m_pWriter->WriteImageRels(wsImageID, wsImageID + L'.' + wsExtention);
			m_arImages.push_back(wsSrc);
		}

		if (!oImageData.ZeroSize())
		{
			m_pWriter->WriteImage(oImageData, wsImageID);
			return true;
		}

		TImageData oNewImageData{oImageData};
		if (!UpdateImageData(wsImagePath + L'.' + wsExtention, oNewImageData))
			return false;

		m_pWriter->WriteImage(oNewImageData, wsImageID);
	}

	return true;
}

void CImageOOXMLTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

bool CFontOOXMLTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	std::wstring wsValue;

	if (arSelectors.back().GetAttributeValue(L"color", wsValue))
		arSelectors.back().m_pCompiledStyle->m_oText.SetColor(wsValue, arSelectors.size());

	if (arSelectors.back().GetAttributeValue(L"face", wsValue))
		arSelectors.back().m_pCompiledStyle->m_oFont.SetFamily(wsValue, arSelectors.size());

	if (arSelectors.back().GetAttributeValue(L"size", wsValue))
	{
		int nSize = 3;
		if(!wsValue.empty())
		{
			if(wsValue.front() == L'+')
				nSize += NSStringFinder::ToInt(wsValue.substr(1));
			else if(wsValue.front() == L'-')
				nSize -= NSStringFinder::ToInt(wsValue.substr(1));
			else
				nSize = NSStringFinder::ToInt(wsValue);
		}

		if (nSize < 1 || nSize > 7)
			nSize = 3;

		arSelectors.back().m_pCompiledStyle->m_oFont.SetSize(HTML_FONTS[nSize - 1], NSCSS::UnitMeasure::Point,  arSelectors.size());
	}

	return true;
}

void CFontOOXMLTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

bool CInputOOXMLTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	std::wstring wsValue{arSelectors.back().GetAttributeValue(L"value")};
	const std::wstring wsAlt{arSelectors.back().GetAttributeValue(L"alt")},
	                   wsType{arSelectors.back().GetAttributeValue(L"type")};

	if(wsType == L"hidden")
		return false;

	if(wsValue.empty())
		wsValue = wsAlt;

	if(!wsValue.empty())
	{
		m_pWriter->WritePPr(arSelectors);
		m_pWriter->OpenR();
		m_pWriter->WriteRPr(*(m_pWriter->GetCurrentDocument()), arSelectors);
		m_pWriter->OpenT();
		m_pWriter->GetCurrentDocument()->WriteEncodeXmlString(wsValue + L' ');
		m_pWriter->CloseT();
		m_pWriter->CloseR();
	}

	return true;
}

void CInputOOXMLTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

bool CBaseFontOOXMLTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	std::wstring wsFontStyles, wsValue;

	if (arSelectors.back().GetAttributeValue(L"face", wsValue))
		wsFontStyles += L"font-family:" + wsValue + L';';

	if (arSelectors.back().GetAttributeValue(L"size", wsValue))
	{
		wsFontStyles += L"font-size:";

		switch(NSStringFinder::ToInt(wsValue, 3))
		{
			case 1: wsFontStyles += L"7.5pt;";  break;
			case 2: wsFontStyles += L"10pt;";   break;
			default:
			case 3: wsFontStyles += L"12pt;";   break;
			case 4: wsFontStyles += L"13.5pt;"; break;
			case 5: wsFontStyles += L"18pt;";   break;
			case 6: wsFontStyles += L"24pt;";   break;
			case 7: wsFontStyles += L"36pt;";   break;
		}
	}

	if (arSelectors.back().GetAttributeValue(L"color", wsValue))
		wsFontStyles += L"text-color:" + wsValue + L';';

	if (wsFontStyles.empty())
		return false;

	m_pWriter->SetBaseFont(L"*{" + wsFontStyles + L'}');

	return true;
}

void CBaseFontOOXMLTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

CBlockquoteOOXMLTag::CBlockquoteOOXMLTag(COOXMLWriter* pWriter)
	: IHTMLTag(pWriter)
{}

bool CBlockquoteOOXMLTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	//TODO:: when Blockquote inside Blockquote, need to add <w:divsChild> to the first one
	const std::wstring wsKeyWord{arSelectors.back().m_wsName};

	std::map<std::wstring, UINT>::const_iterator itFound = m_mDivs.find(wsKeyWord);

	if (m_mDivs.end() != itFound)
	{
		m_pWriter->SetDivId(std::to_wstring(itFound->second));
		return true;
	}

	const std::wstring wsId{std::to_wstring(m_mDivs.size() + 1)};

	XmlString &oWebSettings{m_pWriter->GetWebSettingsXml()};

	if (m_mDivs.empty())
		oWebSettings.WriteString(L"<w:divs>");

	NSCSS::CCompiledStyle *pStyle = arSelectors.back().m_pCompiledStyle;

	const bool bInTable = ElementInTable(arSelectors);

	INT nMarLeft   = (!bInTable) ? 720 : 0;
	INT nMarRight  = (!bInTable) ? 720 : 0;
	INT nMarTop    = (!bInTable) ? 100 : 0;
	INT nMarBottom = (!bInTable) ? 100 : 0;

	const NSCSS::NSProperties::CPage *pPageData{m_pWriter->GetPageData()};

	if (!pStyle->m_oMargin.GetLeft().Empty() && !pStyle->m_oMargin.GetLeft().Zero())
		nMarLeft  = pStyle->m_oMargin.GetLeft().ToInt(NSCSS::Twips, pPageData->GetWidth().ToInt(NSCSS::Twips));

	if (!pStyle->m_oMargin.GetRight().Empty() && !pStyle->m_oMargin.GetRight().Zero())
		nMarRight = pStyle->m_oMargin.GetRight().ToInt(NSCSS::Twips, pPageData->GetWidth().ToInt(NSCSS::Twips));

	if (!pStyle->m_oMargin.GetTop().Empty() && !pStyle->m_oMargin.GetTop().Zero())
		nMarTop = pStyle->m_oMargin.GetTop().ToInt(NSCSS::Twips, pPageData->GetHeight().ToInt(NSCSS::Twips));

	if (!pStyle->m_oMargin.GetBottom().Empty() && !pStyle->m_oMargin.GetBottom().Zero())
		nMarBottom = pStyle->m_oMargin.GetBottom().ToInt(NSCSS::Twips, pPageData->GetHeight().ToInt(NSCSS::Twips));

	if (L"blockquote" == wsKeyWord)
	{
		oWebSettings.WriteString(L"<w:div w:id=\"" + wsId + L"\">");
		oWebSettings.WriteString(L"<w:blockQuote w:val=\"1\"/>");
		oWebSettings.WriteString(L"<w:marLeft w:val=\"" + std::to_wstring(nMarLeft) + L"\"/>");
		oWebSettings.WriteString(L"<w:marRight w:val=\"" + std::to_wstring(nMarRight) + L"\"/>");
		oWebSettings.WriteString(L"<w:marTop w:val=\"" + std::to_wstring(nMarTop) + L"\"/>");
		oWebSettings.WriteString(L"<w:marBottom w:val=\"" + std::to_wstring(nMarBottom) + L"\"/>");
		oWebSettings.WriteString(L"<w:divBdr>");
		oWebSettings.WriteString(L"<w:top w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>");
		oWebSettings.WriteString(L"<w:left w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>");
		oWebSettings.WriteString(L"<w:bottom w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>");
		oWebSettings.WriteString(L"<w:right w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>");
		oWebSettings.WriteString(L"</w:divBdr>");
		oWebSettings.WriteString(L"</w:div>");
	}

	m_mDivs.insert(std::make_pair(wsKeyWord, m_mDivs.size() + 1));

	m_pWriter->SetDivId(wsId);

	return true;
}

void CBlockquoteOOXMLTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return;

	m_pWriter->RollBackDivId();
}

CHorizontalRuleOOXMLTag::CHorizontalRuleOOXMLTag(COOXMLWriter* pWriter)
	: IHTMLTag(pWriter), m_unShapeId(1)
{}

bool CHorizontalRuleOOXMLTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	for (const NSCSS::CNode& item : arSelectors)
	{
		if (item.m_wsName == L"div" && item.m_wsStyle == L"mso-element:footnote-list")
			return false;
	}

	NSCSS::NSProperties::CDigit oSize, oWidth;
	NSCSS::NSProperties::CColor oColor;
	bool bShade = true;
	std::wstring wsAlign{L"center"};
	std::wstring wsValue;

	if (arSelectors.back().GetAttributeValue(L"align", wsValue))
	{
		if (NSStringFinder::Equals(L"left", wsValue))
			wsAlign = L"left";
		else if (NSStringFinder::Equals(L"right", wsValue))
			wsAlign = L"right";
		else if (NSStringFinder::Equals(L"center", wsValue))
			wsAlign = L"center";
	}

	if (arSelectors.back().GetAttributeValue(L"color", wsValue))
		oColor.SetValue(wsValue);

	if (arSelectors.back().GetAttributeValue(L"noshade", wsValue))
		bShade = false;

	if (arSelectors.back().GetAttributeValue(L"size", wsValue))
		oSize.SetValue(wsValue);

	if (arSelectors.back().GetAttributeValue(L"width", wsValue))
		oWidth.SetValue(wsValue);

	XmlString& oXmlString{*m_pWriter->GetCurrentDocument()};

	m_pWriter->OpenP();
	oXmlString.WriteString(L"<w:pPr><w:jc w:val=\"" + wsAlign + L"\"/></w:pPr>");
	m_pWriter->OpenR();

	const NSCSS::NSProperties::CPage *pPageData{m_pWriter->GetPageData()};

	const unsigned int unPageWidth{static_cast<unsigned int>((pPageData->GetWidth().ToDouble(NSCSS::Point) - pPageData->GetMargin().GetLeft().ToDouble(NSCSS::Point) - pPageData->GetMargin().GetRight().ToDouble(NSCSS::Point)))};

	std::wstring wsWidth;

	// width is measured in px or %
	if (!oWidth.Empty())
		wsWidth = std::to_wstring(NSCSS::CUnitMeasureConverter::ConvertPx(oWidth.ToDouble(), NSCSS::Point, unPageWidth));
	else
		wsWidth = std::to_wstring(unPageWidth);

	std::wstring wsHeight{L"1.5"};

	// size is measured only in px
	if (!oSize.Empty())
		wsHeight = std::to_wstring(NSCSS::CUnitMeasureConverter::ConvertPx(oSize.ToDouble(), NSCSS::Point, 96));

	oXmlString.WriteString(L"<w:rPr><w:noProof/></w:rPr>");
	oXmlString.WriteString(L"<w:pict>");
	oXmlString.WriteString(L"<v:shape id=\"shape " + std::to_wstring(m_unShapeId++) + L"\" style=\"width:" + wsWidth + L"pt;"
	                        "height:" + wsHeight + L"pt;"
	                        "mso-wrap-distance-left:0.00pt;mso-wrap-distance-top:0.00pt;mso-wrap-distance-right:0.00pt;mso-wrap-distance-bottom:0.00pt;visibility:visible;\" "
	                        "o:hralign=\"" + wsAlign + L"\" o:hrnoshade=\"" + ((bShade) ? L"f" : L"t") + L"\" filled=\"f\" ");

	if (oColor.Empty())
		oXmlString.WriteString(L"stroked=\"f\" o:hr=\"t\" o:hrstd=\"t\" ");
	else
		oXmlString.WriteString(L"stroked=\"t\" o:hr=\"t\" o:hrstd=\"t\" strokecolor=\"#" + oColor.ToHEX() + L"\" strokeweight=\"" + wsHeight + L"pt\" coordsize=\"100,100\" path=\"m0,0l100,0e\" ");

	oXmlString.WriteString(L"/></w:pict>");

	//// Old version
	// oXmlString.WriteString(L"<w:rPr><w:noProof/></w:rPr>");
	// oXmlString.WriteString(L"<mc:AlternateContent><mc:Choice Requires=\"wps\"><w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\">");
	// oXmlString.WriteString(L"<wp:extent cx=\"" + wsWidth + L"\" cy=\"0\"/>");
	// oXmlString.WriteString(L"<wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>");
	// oXmlString.WriteString(L"<wp:docPr id=\"" + std::to_wstring(m_unShapeId) + L"\" name=\"Line " + std::to_wstring(m_unShapeId) + L"\"/>"
	//                                   "<wp:cNvGraphicFramePr/>"
	//                                       "<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">"
	//                                             "<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">"
	//                                                 "<wps:wsp>"
	//                                                     "<wps:cNvSpPr/>"
	//                                                     "<wps:spPr>");
	// oXmlString.WriteString(L"<a:xfrm>"
	//                                   "<a:off x=\"0\" y=\"0\"/>"
	//                                   "<a:ext cx=\"" + wsWidth + L"\" cy=\"0\"/>"
	//                               "</a:xfrm>"
	//                               "<a:custGeom><a:pathLst><a:path>"
	//                                   "<a:moveTo><a:pt x=\"0\" y=\"0\"/></a:moveTo>"
	//                                   "<a:lnTo><a:pt x=\"" + wsWidth + L"\" y=\"0\"/></a:lnTo>"
	//                               "</a:path></a:pathLst></a:custGeom>"
	//                               "<a:ln w=\"" + wsHeight + L"\"><a:solidFill><a:srgbClr val=\"" + ((!oColor.Empty()) ? oColor.ToHEX() : L"808080") + L"\"/></a:solidFill></a:ln>");

	// if (bShade)
	// 	oXmlString.WriteString(L"<a:scene3d><a:camera prst=\"orthographicFront\"/><a:lightRig rig=\"threePt\" dir=\"t\"/></a:scene3d><a:sp3d><a:bevelT prst=\"angle\"/></a:sp3d>");

	// oXmlString.WriteString(L"</wps:spPr><wps:bodyPr/></wps:wsp></a:graphicData></a:graphic></wp:inline></w:drawing></mc:Choice></mc:AlternateContent>");

	m_pWriter->CloseP();

	return true;
}

void CHorizontalRuleOOXMLTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

bool CListOOXMLTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	m_pWriter->CloseP();

	// Numbered list
	if (L"ol" == arSelectors.back().m_wsName)
	{
		const int nStart{NSStringFinder::ToInt(arSelectors.back().GetAttributeValue(L"start"), 1)};

		XmlString& oNumberXml{m_pWriter->GetNumberingXml()};

		const std::wstring wsStart(std::to_wstring(nStart));
		oNumberXml.WriteString(L"<w:abstractNum w:abstractNumId=\"");
		oNumberXml.WriteString(std::to_wstring(m_pWriter->GetListId()));
		oNumberXml.WriteString(L"\"><w:multiLevelType w:val=\"hybridMultilevel\"/><w:lvl w:ilvl=\"0\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%1.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"709\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"1\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%2.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"1429\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"2\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%3.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"2149\" w:hanging=\"180\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"3\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%4.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"2869\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"4\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%5.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"3589\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"5\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%6.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"4309\" w:hanging=\"180\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"6\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%7.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5029\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"7\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%8.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5749\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"8\"><w:start w:val=\"");
		oNumberXml.WriteString(wsStart);
		oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%9.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"6469\" w:hanging=\"180\"/></w:pPr></w:lvl></w:abstractNum>");

		m_pWriter->IncreaseListId();
	}

	return true;
}

void CListOOXMLTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return;

	m_pWriter->CloseP();
}

bool CListElementOOXMLTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	return Valid();
}

void CListElementOOXMLTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return;

	m_pWriter->CloseP();
}

inline bool ElementInTable(const std::vector<NSCSS::CNode>& arSelectors)
{
	return arSelectors.crend() != std::find_if(arSelectors.crbegin(), arSelectors.crend(), [](const NSCSS::CNode& oNode) { return L"table" == oNode.m_wsName; });
}

inline bool NotValidExtension(const std::wstring& sExtention)
{
	return  sExtention != L"bmp" && sExtention != L"emf"  && sExtention != L"emz"  && sExtention != L"eps"  && sExtention != L"fpx" && sExtention != L"gif"  &&
			sExtention != L"jpe" && sExtention != L"jpeg" && sExtention != L"jpg"  && sExtention != L"jfif" && sExtention != L"pct" && sExtention != L"pict" &&
			sExtention != L"png" && sExtention != L"pntg" && sExtention != L"psd"  && sExtention != L"qtif" && sExtention != L"sgi" && sExtention != L"wmz"  &&
			sExtention != L"tga" && sExtention != L"tpic" && sExtention != L"tiff" && sExtention != L"tif"  && sExtention != L"wmf" && !IsSVG(sExtention);
}

inline bool IsSVG(const std::wstring& wsExtention)
{
	return L"svg" == wsExtention || L"svg+xml" == wsExtention;
}

inline bool IsMetafile(const std::wstring& wsExtention)
{
	return L"emf" == wsExtention || L"wmf" == wsExtention;
}

bool ReadMetafileBase(MetaFile::IMetaFile* pReader, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, const std::wstring& wsImagePath)
{
	NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();
	pGrRenderer->SetFontManager(pReader->get_FontManager());

	double dX, dY, dW, dH;
	pReader->GetBounds(&dX, &dY, &dW, &dH);

	if (dW < 0) dW = -dW;
	if (dH < 0) dH = -dH;

	double dOneMaxSize = (double)1000.;

	if (dW > dH && dW > dOneMaxSize)
	{
		dH *= (dOneMaxSize / dW);
		dW = dOneMaxSize;
	}
	else if (dH > dW && dH > dOneMaxSize)
	{
		dW *= (dOneMaxSize / dH);
		dH = dOneMaxSize;
	}

	int nWidth  = static_cast<int>(dW + 0.5);
	int nHeight = static_cast<int>(dH + 0.5);

	double dWidth  = 25.4 * nWidth / 96;
	double dHeight = 25.4 * nHeight / 96;

	BYTE* pBgraData = (BYTE*)malloc(nWidth * nHeight * 4);
	if (!pBgraData)
	{
		double dKoef = 2000.0 / (nWidth > nHeight ? nWidth : nHeight);

		nWidth = (int)(dKoef * nWidth);
		nHeight = (int)(dKoef * nHeight);

		dWidth  = 25.4 * nWidth / 96;
		dHeight = 25.4 * nHeight / 96;

		pBgraData = (BYTE*)malloc(nWidth * nHeight * 4);
	}

	if (!pBgraData)
		return false;

	unsigned int alfa = 0xffffff;
	// default tone should be transparent, not white
	//memset(pBgraData, 0xff, nWidth * nHeight * 4);
	for (int i = 0; i < nWidth * nHeight; i++)
	{
		((unsigned int*)pBgraData)[i] = alfa;
	}
	CBgraFrame oFrame;
	oFrame.put_Data(pBgraData);
	oFrame.put_Width(nWidth);
	oFrame.put_Height(nHeight);
	oFrame.put_Stride(-4 * nWidth);

	pGrRenderer->CreateFromBgraFrame(&oFrame);
	pGrRenderer->SetSwapRGB(false);
	pGrRenderer->put_Width(dWidth);
	pGrRenderer->put_Height(dHeight);

	pReader->SetTempDirectory(wsTempDir);
	pReader->DrawOnRenderer(pGrRenderer, 0, 0, dWidth, dHeight);

	oFrame.SaveFile(wsImagePath, 4);
	oFrame.put_Data(NULL);

	RELEASEINTERFACE(pGrRenderer);

	if (pBgraData)
		free(pBgraData);

	return true;
}

bool ReadMetafileData(const std::wstring& wsFilePath, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, const std::wstring& wsImagePath)
{
	MetaFile::IMetaFile* pMetafileReader = MetaFile::Create(pFonts);
	if (!pMetafileReader->LoadFromFile(wsFilePath.c_str()))
	{
		RELEASEINTERFACE(pMetafileReader);
		return false;
	}

	const bool bResult{ReadMetafileBase(pMetafileReader, pFonts, wsTempDir, wsImagePath)};

	RELEASEINTERFACE(pMetafileReader);

	return bResult;
}

bool ReadSVGData(const std::wstring& wsSvg, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, const std::wstring& wsImagePath)
{
	MetaFile::IMetaFile* pSvgReader = MetaFile::Create(pFonts);
	if (!pSvgReader->LoadFromString(wsSvg))
	{
		RELEASEINTERFACE(pSvgReader);
		return false;
	}

	const bool bResult{ReadMetafileBase(pSvgReader, pFonts, wsTempDir, wsImagePath)};

	RELEASEINTERFACE(pSvgReader);

	return bResult;
}

bool ReadBase64(const std::wstring& wsSrc, const std::wstring& wsImagePath, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, std::wstring& wsExtention)
{
	bool bRes = false;
	size_t nBase = wsSrc.find(L"/", 4);
	nBase++;

	size_t nEndBase = wsSrc.find(L";", nBase);
	if (nEndBase == std::wstring::npos)
		return bRes;

	wsExtention = wsSrc.substr(nBase, nEndBase - nBase);

	if (wsExtention == L"octet-stream")
		wsExtention = L"jpg";

	if (NotValidExtension(wsExtention))
		return bRes;

	nBase = wsSrc.find(L"base64", nEndBase);
	if (nBase == std::wstring::npos)
		return bRes;

	int nOffset = nBase + 7;
	int nSrcLen = (int)(wsSrc.length() - nBase + 1);
	int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
	if (nDecodeLen != 0)
	{
		BYTE* pImageData = new BYTE[nDecodeLen];

		if (!pImageData || FALSE == NSBase64::Base64Decode(wsSrc.c_str() + nOffset, nSrcLen, pImageData, &nDecodeLen))
			return bRes;

		if (IsSVG(wsExtention))
		{
			std::wstring wsSvg(pImageData, pImageData + nDecodeLen);
			bRes = ReadSVGData(wsSvg, pFonts, wsTempDir, wsImagePath + L".png");
			wsExtention = L"png";
		}
		else
		{
			NSFile::CFileBinary oImageWriter;

			if (oImageWriter.CreateFileW(wsImagePath + L'.' + wsExtention))
				bRes = oImageWriter.WriteFile(pImageData, (DWORD)nDecodeLen);

			oImageWriter.CloseFile();
		}

		RELEASEARRAYOBJECTS(pImageData);
	}

	return bRes;
}

bool CopyImage(std::wstring wsImageSrc, const std::wstring& wsSrc, const std::wstring& wsDst, bool bIsAllowExternalLocalFiles)
{
	if (wsImageSrc.empty() || !NSFile::CFileBinary::Exists(wsImageSrc))
		return false;

	bool bRes = false;
	bool bAllow = true;

	if (!bIsAllowExternalLocalFiles)
	{
		wsImageSrc = NSSystemPath::NormalizePath(wsImageSrc);
		const std::wstring wsStartSrc = NSSystemPath::NormalizePath(wsSrc);
		bAllow = wsImageSrc.substr(0, wsStartSrc.length()) == wsStartSrc;
	}
	if (bAllow)
		bRes = NSFile::CFileBinary::Copy(wsSrc, wsDst);

	return bRes;
}

bool UpdateImageData(const std::wstring& wsImagePath, TImageData& oImageData)
{
	CBgraFrame oBgraFrame;
	if (!oBgraFrame.OpenFile(wsImagePath))
	{
		NSFile::CFileBinary::Remove(wsImagePath);
		return false;
	}

	if (0 != oImageData.m_unWidth || 0 != oImageData.m_unHeight)
	{
		const double dMaxScale = std::max(oImageData.m_unWidth  / oBgraFrame.get_Width(),
		                                  oImageData.m_unHeight / oBgraFrame.get_Height());

		oImageData.m_unWidth  = oBgraFrame.get_Width()  * dMaxScale;
		oImageData.m_unHeight = oBgraFrame.get_Height() * dMaxScale;
	}
	else
	{
		oImageData.m_unWidth  = oBgraFrame.get_Width();
		oImageData.m_unHeight = oBgraFrame.get_Height();

		if (oImageData.m_unWidth > oImageData.m_unHeight)
		{
			int nW = oImageData.m_unWidth * 9525;
			nW = (nW > 7000000 ? 7000000 : nW);
			oImageData.m_unHeight = (int)((double)oImageData.m_unHeight * (double)nW / (double)oImageData.m_unWidth);
			oImageData.m_unWidth = nW;
		}
		else
		{
			int nH = oImageData.m_unHeight * 9525;
			nH = (nH > 8000000 ? 8000000 : nH);
			int nW = (int)((double)oImageData.m_unWidth * (double)nH / (double)oImageData.m_unHeight);
			if (nW > 7000000)
			{
				nW = 7000000;
				oImageData.m_unHeight = (int)((double)oImageData.m_unHeight * (double)nW / (double)oImageData.m_unWidth);
			}
			else
				oImageData.m_unHeight = nH;
			oImageData.m_unWidth = nW;
		}
	}

	return true;
}

std::wstring CreateBorders(const NSCSS::NSProperties::CBorder& oBorder, const NSCSS::NSProperties::CIndent* pPadding = NULL, bool bAddIntermediateLines = false, ETableRules enTableRule = ETableRules::None)
{
	std::wstring wsTable;

	if (oBorder.EqualSides() && (NULL == pPadding || pPadding->Equals()))
	{
		const std::wstring wsBorderStyle = NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetLeftBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetLeft()))));

		wsTable = L"<w:top "    + wsBorderStyle + L"/>" + L"<w:left "   + wsBorderStyle + L"/>" +
		          L"<w:bottom " + wsBorderStyle + L"/>" + L"<w:right "  + wsBorderStyle + L"/>";
	}
	else
	{
		if (oBorder.GetTopBorder().Valid())
			wsTable += L"<w:top "    + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetTopBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetTop()))))       + L"/>";

		if (oBorder.GetLeftBorder().Valid())
			wsTable += L"<w:left "   + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetLeftBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetLeft()))))     + L"/>";

		if (oBorder.GetBottomBorder().Valid())
			wsTable += L"<w:bottom " + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetBottomBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetBottom())))) + L"/>";

		if (oBorder.GetRightBorder().Valid())
			wsTable += L"<w:right "  + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetRightBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetRight()))))   + L"/>";
	}

	if (!bAddIntermediateLines)
		return wsTable;

	if (ETableRules::Rows == enTableRule || ETableRules::All == enTableRule)
	{
		NSCSS::NSProperties::CBorderSide oNewSide(oBorder.GetBottomBorder());
		oNewSide.SetWidth(L"1pt", 0, true);

		wsTable += L"<w:insideH " + NSCSS::CDocumentStyle::CalculateBorderStyle(oNewSide) + L"/>";
	}

	if (ETableRules::Cols == enTableRule || ETableRules::All == enTableRule)
	{
		NSCSS::NSProperties::CBorderSide oNewSide(oBorder.GetRightBorder());
		oNewSide.SetWidth(L"1pt", 0, true);

		wsTable += L"<w:insideV " + NSCSS::CDocumentStyle::CalculateBorderStyle(oNewSide) + L"/>";
	}

	return wsTable;
}

std::wstring CreateBorders(const std::wstring& wsStyle, UINT unSize, UINT unSpace, const std::wstring& wsAuto)
{
	const std::wstring wsBodyBorder{L"w:val=\"" + wsStyle + L"\" w:sz=\"" + std::to_wstring(unSize) + L"\" w:space=\"" + std::to_wstring(unSpace) + L"\" w:color=\"" + wsAuto + L"\""};

	return L"<w:top "    + wsBodyBorder + L"/>" +
	       L"<w:left "   + wsBodyBorder + L"/>" +
	       L"<w:bottom " + wsBodyBorder + L"/>" +
	       L"<w:right "  + wsBodyBorder + L"/>";
}

#define CreateOutsetBorders(enType) CreateBorders(L"outset", 6, 0, L"auto", enType)

std::wstring CreateDefaultBorder(std::wstring wsSideName)
{
	std::transform(wsSideName.begin(), wsSideName.end(), wsSideName.begin(), std::towlower);
	return L"<w:" + wsSideName + L" w:val=\"single\" w:sz=\"1\" w:space=\"0\" w:color=\"auto\"/>";
}

std::wstring CalculateSidesToClean(UINT unColumnNumber, const std::vector<CTableColgroup*>& arColgroups, UINT unMaxColumns)
{
	if (arColgroups.empty())
		return std::wstring();

	UINT unCurrentNumber = 0;

	for (const CTableColgroup* pColgroup : arColgroups)
	{
		for (const CTableCol* pCol : pColgroup->GetCols())
		{
			if (unCurrentNumber + 1 == unCurrentNumber)
				return (1 != pCol->GetSpan()) ? L"<w:right w:val=\"nil\"/>" : std::wstring();

			unCurrentNumber += pCol->GetSpan();

			if (unColumnNumber == unCurrentNumber)
				return (1 != pCol->GetSpan()) ? L"<w:left w:val=\"nil\"/>" : std::wstring();
			else if (unColumnNumber < unCurrentNumber)
				return std::wstring((1 != unColumnNumber) ? L"<w:left w:val=\"nil\"/>" : L"") + std::wstring((unMaxColumns != unColumnNumber) ? L"<w:right w:val=\"nil\"/>" : L"");
		}
	}

	return std::wstring();
}

bool CHTMLOOXMLTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	if (arSelectors.back().m_mAttributes.empty())
		return true;

	std::wstring wsBackground;
	if (arSelectors.back().GetAttributeValue(L"bgcolor", wsBackground))
	{
		NSCSS::NSProperties::CColor oColor;
		oColor.SetValue(wsBackground);

		if (!oColor.Empty() && !oColor.None())
		{
			const std::wstring wsHEXColor{oColor.ToHEX()};

			if (!wsHEXColor.empty())
				m_pWriter->GetCurrentDocument()->WriteString(L"<w:background w:color=\"" + wsHEXColor + L"\"/>");
		}
	}

	return true;
}

void CHTMLOOXMLTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

COOXMLTable::COOXMLTable(TExternalTableData& oExternalData)
	: CTableElement(oExternalData)
{}

COOXMLTable::~COOXMLTable()
{}

bool COOXMLTable::PreParse(XmlUtils::CXmlLiteReader& oReader)
{
	return ParseTable(oReader, this);
}

bool COOXMLTable::HaveColgroups() const
{
	return !m_arColgroups.empty();
}

size_t COOXMLTable::GetColumnsCount() const
{
	return m_oBody.GetColumnSize();
}

void COOXMLTable::OpenTable(XmlString& oXmlString, const NSCSS::CNode& oTableNode, TTableStyles& oTableStyles, const COOXMLTable& oTable)
{
	oXmlString.WriteNodeBegin(L"w:tbl");
	oXmlString.WriteNodeBegin(L"w:tblPr");

	const NSCSS::CCompiledStyle *pStyle{oTableNode.m_pCompiledStyle};

	if (nullptr != pStyle)
	{
		const NSCSS::NSProperties::CDigit& oWidth{pStyle->m_oDisplay.GetWidth()};

		if (!oWidth.Empty() && !oWidth.Zero())
		{
			if (NSCSS::UnitMeasure::Percent == oWidth.GetUnitMeasure())
				oXmlString += L"<w:tblW w:w=\"" + std::to_wstring(oWidth.ToInt(NSCSS::UnitMeasure::Percent, 5000)) + L"\" w:type=\"pct\"/>";
			else
				oXmlString += L"<w:tblW w:w=\"" + std::to_wstring(oWidth.ToInt(NSCSS::UnitMeasure::Twips)) + L"\" w:type=\"dxa\"/>";
		}
		else
			oXmlString += L"<w:tblW w:w=\"0\" w:type=\"auto\"/>";

		const NSCSS::NSProperties::CIndent& oMargin{pStyle->m_oMargin};

		if (!oMargin.GetLeft().Empty() && !oMargin.GetLeft().Zero())
		{
			if (NSCSS::UnitMeasure::Percent == oMargin.GetLeft().GetUnitMeasure())
				oXmlString += L"<w:tblInd w:w=\"" + std::to_wstring(oMargin.GetLeft().ToInt(NSCSS::UnitMeasure::Percent, 5000)) + L"\" w:type=\"pct\"/>";
			else
				oXmlString += L"<w:tblInd w:w=\"" + std::to_wstring(oMargin.GetLeft().ToInt(NSCSS::UnitMeasure::Twips)) + L"\" w:type=\"dxa\"/>";
		}

		if (!pStyle->m_oDisplay.GetHAlign().Empty())
			oXmlString += L"<w:jc w:val=\"" + pStyle->m_oDisplay.GetHAlign().ToWString() + L"\"/>";

		std::wstring wsValue;

		if (pStyle->m_oBorder.GetCollapse() == NSCSS::NSProperties::BorderCollapse::Collapse)
			oTableStyles.m_unCellSpacing = 0;
		else if (oTableNode.GetAttributeValue(L"cellspacing", wsValue))
			oTableStyles.m_unCellSpacing = NSStringFinder::ToInt(wsValue);
		else if (pStyle->m_oBorder.GetCollapse() == NSCSS::NSProperties::BorderCollapse::Separate)
			oTableStyles.m_unCellSpacing = 15;

		if (0 < oTableStyles.m_unCellSpacing && pStyle->m_oBorder.GetCollapse() != NSCSS::NSProperties::BorderCollapse::Collapse)
			oXmlString += L"<w:tblCellSpacing w:w=\"" + std::to_wstring(oTableStyles.m_unCellSpacing) + L"\" w:type=\"dxa\"/>";

		if (oTableNode.GetAttributeValue(L"border", wsValue))
		{
			const int nWidth = NSStringFinder::ToInt(wsValue);

			if (0 < nWidth)
			{
				oTableStyles.m_enRules = ETableRules::All;

				if (oTableStyles.m_oBorder.Empty())
				{
					oTableStyles.m_oBorder.SetStyle(L"outset",  0, true);
					oTableStyles.m_oBorder.SetWidth(nWidth,     NSCSS::UnitMeasure::Point, 0, true);
					oTableStyles.m_oBorder.SetColor(L"auto",    0, true);
				}
			}
			else if (pStyle->m_oBorder.Empty())
			{
				oTableStyles.m_oBorder.SetNone(0, true);
				oTableStyles.m_enRules = ETableRules::All;
			}
		}
		else
			oTableStyles.m_oBorder = pStyle->m_oBorder;

		if (oTableNode.GetAttributeValue(L"rules", wsValue))
		{
			if (NSStringFinder::Equals(wsValue, L"all"))
				oTableStyles.m_enRules = ETableRules::All;
			else if (NSStringFinder::Equals(wsValue, L"groups"))
				oTableStyles.m_enRules = ETableRules::Groups;
			else if (NSStringFinder::Equals(wsValue, L"cols"))
				oTableStyles.m_enRules = ETableRules::Cols;
			else if (NSStringFinder::Equals(wsValue, L"none"))
				oTableStyles.m_enRules = ETableRules::None;
			else if (NSStringFinder::Equals(wsValue, L"rows"))
				oTableStyles.m_enRules = ETableRules::Rows;
		}

		std::wstring wsFrame;
		oTableNode.GetAttributeValue(L"frame", wsFrame);

		if (!wsFrame.empty() && oTableStyles.m_oBorder.Empty())
		{
			#define SetDefaultBorderSide(side) \
				oTableStyles.m_oBorder.SetStyle##side(L"solid", 0, true); \
				oTableStyles.m_oBorder.SetWidth##side(1,        NSCSS::UnitMeasure::Point, 0, true); \
				oTableStyles.m_oBorder.SetColor##side(L"black", 0, true)

			if (NSStringFinder::Equals(L"border", wsFrame))
			{
				SetDefaultBorderSide();
			}
			else if (NSStringFinder::Equals(L"above", wsFrame))
			{
				SetDefaultBorderSide(TopSide);
			}
			else if (NSStringFinder::Equals(L"below", wsFrame))
			{
				SetDefaultBorderSide(BottomSide);
			}
			else if (NSStringFinder::Equals(L"hsides", wsFrame))
			{
				SetDefaultBorderSide(TopSide);
				SetDefaultBorderSide(BottomSide);
			}
			else if (NSStringFinder::Equals(L"vsides", wsFrame))
			{
				SetDefaultBorderSide(LeftSide);
				SetDefaultBorderSide(RightSide);
			}
			else if (NSStringFinder::Equals(L"rhs", wsFrame))
			{
				SetDefaultBorderSide(RightSide);
			}
			else if (NSStringFinder::Equals(L"lhs", wsFrame))
			{
				SetDefaultBorderSide(LeftSide);
			}
		}

		if (!oTableStyles.m_oBorder.Empty() && !oTableStyles.m_oBorder.Zero())
			oXmlString += L"<w:tblBorders>" + CreateBorders(oTableStyles.m_oBorder, NULL, true, (ETableRules::Groups == oTableStyles.m_enRules && !HaveColgroups()) ? ETableRules::Cols : oTableStyles.m_enRules) + L"</w:tblBorders>";

		oTableStyles.m_oPadding = pStyle->m_oPadding;

		if (oTableNode.GetAttributeValue(L"cellpadding", wsValue))
			oTableStyles.m_oPadding.SetValues(wsValue + L"px", 0, true);

		if (!oTableStyles.m_oPadding.Empty() && !oTableStyles.m_oPadding.Zero())
		{
			const int nTopPadding    = std::max(0, oTableStyles.m_oPadding.GetTop()   .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
			const int nLeftPadding   = std::max(0, oTableStyles.m_oPadding.GetLeft()  .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH ));
			const int nBottomPadding = std::max(0, oTableStyles.m_oPadding.GetBottom().ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
			const int nRightPadding  = std::max(0, oTableStyles.m_oPadding.GetRight() .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH ));

			oXmlString.WriteNodeBegin(L"w:tblCellMar");

			if (0 != nTopPadding)
				oXmlString += L"<w:top w:w=\""    + std::to_wstring(nTopPadding)    + L"\" w:type=\"dxa\"/>";

			if (0 != nLeftPadding)
				oXmlString += L"<w:left w:w=\""   + std::to_wstring(nLeftPadding)   + L"\" w:type=\"dxa\"/>";

			if (0 != nBottomPadding)
				oXmlString += L"<w:bottom w:w=\"" + std::to_wstring(nBottomPadding) + L"\" w:type=\"dxa\"/>";

			if (0 != nRightPadding)
				oXmlString += L"<w:right w:w=\""  + std::to_wstring(nRightPadding)  + L"\" w:type=\"dxa\"/>";

			oXmlString.WriteNodeEnd(L"w:tblCellMar");
		}
		else
			oXmlString += L"<w:tblCellMar><w:top w:w=\"15\" w:type=\"dxa\"/><w:left w:w=\"15\" w:type=\"dxa\"/><w:bottom w:w=\"15\" w:type=\"dxa\"/><w:right w:w=\"15\" w:type=\"dxa\"/></w:tblCellMar>";

	}

	oXmlString += L"<w:tblLook w:val=\"04A0\" w:noVBand=\"1\" w:noHBand=\"0\" w:lastColumn=\"0\" w:firstColumn=\"1\" w:lastRow=\"0\" w:firstRow=\"1\"/>";
	oXmlString.WriteNodeEnd(L"w:tblPr");

	if (oTable.HaveCaption())
	{
		oXmlString.WriteNodeBegin(L"w:tr");
		oXmlString.WriteNodeBegin(L"w:tc");
		oXmlString.WriteNodeBegin(L"w:tcPr");
		oXmlString += L"<w:tcW w:w=\"0\" w:type=\"auto\"/>";
		oXmlString += L"<w:gridSpan w:val=\"" + std::to_wstring(oTable.GetColumnsCount()) + L"\"/>";
		oXmlString += L"<w:tcBorders><w:top w:val=\"nil\"/><w:left w:val=\"nil\"/><w:bottom w:val=\"nil\"/><w:right w:val=\"nil\"/></w:tcBorders>";
		oXmlString += L"<w:vAlign w:val=\"center\"/>";
		oXmlString += L"<w:hideMark/>";
		oXmlString.WriteNodeEnd(L"w:tcPr");
		WriteToStringBuilder(*oTable.m_pCaption, oXmlString);
		oXmlString.WriteNodeEnd(L"w:tc");
		oXmlString.WriteNodeEnd(L"w:tr");
	}
}

void COOXMLTable::CloseTable(XmlString& oXmlString)
{
	oXmlString.WriteNodeEnd(L"w:tbl");
}

void COOXMLTable::OpenRow(XmlString& oXmlString, bool bIsHeader, size_t unMaxHeight, size_t unCellSpacing)
{
	oXmlString.WriteNodeBegin(L"w:tr");
	oXmlString.WriteNodeBegin(L"w:trPr");

	if (bIsHeader)
		oXmlString += L"<w:tblHeader/>";

	if (0 < unMaxHeight)
		oXmlString += L"<w:trHeight w:val=\"" + std::to_wstring(unMaxHeight) + L"\"/>";

	if (0 < unCellSpacing)
		oXmlString += L"<w:tblCellSpacing w:w=\"" + std::to_wstring(unCellSpacing) + L"\" w:type=\"dxa\"/>";

	oXmlString.WriteNodeEnd(L"w:trPr");
}

void COOXMLTable::CloseRow(XmlString& oXmlString)
{
	oXmlString.WriteNodeEnd(L"w:tr");
}

void COOXMLTable::OpenCell(XmlString& oXmlString)
{
	oXmlString.WriteNodeBegin(L"w:tc");
}

void COOXMLTable::OpenCell(XmlString& oXmlString, const NSCSS::CNode& oCellNode, ITableElementCell* pCell, size_t unColumnIndex, ERowParseMode eRowParseMode, ERowPosition eRowPosition, size_t& unHeight, const TTableStyles& oTableStyles, std::unordered_map<size_t, NSCSS::CNode>& mFillingColumn)
{
	OpenCell(oXmlString);

	if (nullptr == pCell)
		return;

	oXmlString.WriteNodeBegin(L"w:tcPr");

	if (ERowParseMode::Header == eRowParseMode)
		oXmlString += L"<w:tblHeader/>";

	const NSCSS::CCompiledStyle *pColStyle{(nullptr != oTableStyles.m_pTable) ? oTableStyles.m_pTable->GetColStyle(unColumnIndex) : nullptr};

	const NSCSS::NSProperties::CDigit* pWidth{&oCellNode.m_pCompiledStyle->m_oDisplay.GetWidth()};

	if (nullptr != pColStyle && pWidth->LessSignificantThen(pColStyle->m_oDisplay.GetWidth()))
		pWidth = &pColStyle->m_oDisplay.GetWidth();

	if (!pWidth->Empty())
	{
		if (NSCSS::UnitMeasure::Percent == pWidth->GetUnitMeasure())
			oXmlString += L"<w:tcW w:w=\"" + std::to_wstring(pWidth->ToInt(NSCSS::UnitMeasure::Percent, 5000)) + L"\" w:type=\"pct\"/>";
		else
		{
			if (!pWidth->Zero())
			{
				int nWidth;
				if (NSCSS::UnitMeasure::None != pWidth->GetUnitMeasure())
					nWidth = pWidth->ToInt(NSCSS::UnitMeasure::Twips);
				else
					nWidth = static_cast<int>(NSCSS::CUnitMeasureConverter::ConvertPx(pWidth->ToDouble(), NSCSS::UnitMeasure::Twips, 96) + 0.5);

				oXmlString += L"<w:tcW w:w=\"" + std::to_wstring(nWidth) + L"\" w:type=\"dxa\"/>";
			}
			else
				oXmlString += L"<w:tcW w:w=\"6\" w:type=\"dxa\"/>";
		}
	}
	else
		oXmlString += L"<w:tcW w:w=\"0\" w:type=\"auto\"/>";

	if (1 != pCell->GetColspan())
		oXmlString += L"<w:gridSpan w:val=\"" + std::to_wstring(pCell->GetColspan()) + L"\"/>";

	const NSCSS::NSProperties::CBorder* pBorder{&oCellNode.m_pCompiledStyle->m_oBorder};

	if (nullptr != pColStyle && !pColStyle->m_oBorder.Empty() && !pColStyle->m_oBorder.Zero())
		pBorder = &pColStyle->m_oBorder;

	if (!pBorder->Empty() && !pBorder->Zero() && *pBorder != oTableStyles.m_oBorder)
		oXmlString += L"<w:tcBorders>" + CreateBorders(*pBorder, &oCellNode.m_pCompiledStyle->m_oPadding) + L"</w:tcBorders>";
	else if (ETableRules::Groups == oTableStyles.m_enRules)
	{
		std::wstring wsBorders;

		if (nullptr != oTableStyles.m_pTable && oTableStyles.m_pTable->HaveColgroups())
			wsBorders += CalculateSidesToClean(unColumnIndex, oTableStyles.m_pTable->m_arColgroups, oTableStyles.m_pTable->GetColumnsCount());

		if (ERowParseMode::Header == eRowParseMode && ERowPosition::Last == eRowPosition)
			wsBorders += CreateDefaultBorder(L"bottom");
		else if (ERowParseMode::Foother == eRowParseMode && ERowPosition::First == eRowPosition)
			wsBorders += CreateDefaultBorder(L"top");

		if (!wsBorders.empty())
			oXmlString += L"<w:tcBorders>" + wsBorders + L"</w:tcBorders>";
	}

	std::wstring wsValue;

	if (ETableElement::FillingCell == pCell->GetType())
		oXmlString += L"<w:vMerge w:val=\"continue\"/>";
	else if (oCellNode.GetAttributeValue(L"rowspan", wsValue))
	{
		const int nRowspan{NSStringFinder::ToInt(wsValue, 1)};

		if (1 < nRowspan)
		{
			oXmlString += L"<w:vMerge w:val=\"restart\"/>";
			mFillingColumn[unColumnIndex] = oCellNode;
			mFillingColumn[unColumnIndex].m_pCompiledStyle = new NSCSS::CCompiledStyle(*oCellNode.m_pCompiledStyle);
		}
	}

	const NSCSS::NSProperties::CColor* pBackground{&oCellNode.m_pCompiledStyle->m_oBackground.GetColor()};

	if (nullptr != pColStyle && pBackground->LessSignificantThen(pColStyle->m_oBackground.GetColor()))
		pBackground = &pColStyle->m_oBackground.GetColor();

	if (!pBackground->Empty())
	{
		const std::wstring wsShdFill{(NSCSS::NSProperties::ColorNone == pBackground->GetType()) ? L"auto" : pBackground->ToWString()};
		oXmlString += L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" + wsShdFill + L"\"/>";
	}

	if (!oCellNode.m_pCompiledStyle->m_oText.GetAlign().Empty())
		oXmlString += L"<w:vAlign w:val=\"" + oCellNode.m_pCompiledStyle->m_oText.GetAlign().ToWString() + L"\"/>";
	else
		oXmlString += L"<w:vAlign w:val=\"center\"/>";

	NSCSS::NSProperties::CIndent oPadding{oCellNode.m_pCompiledStyle->m_oPadding};

	if (nullptr != pColStyle && !pColStyle->Empty())
		oPadding += pColStyle->m_oPadding;

	if (!oPadding.Empty() && oTableStyles.m_oPadding != oPadding)
	{
		const int nTopPadding    = std::max(oTableStyles.m_oPadding.GetTop()   .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT),
		                                    oPadding.GetTop()   .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
		const int nLeftPadding   = std::max(oTableStyles.m_oPadding.GetLeft()  .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH),
		                                    oPadding.GetLeft()  .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH));
		const int nBottomPadding = std::max(oTableStyles.m_oPadding.GetBottom().ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT),
		                                    oPadding.GetBottom().ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
		const int nRightPadding  = std::max(oTableStyles.m_oPadding.GetRight() .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH),
		                                    oPadding.GetRight() .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH));

		oXmlString += L"<w:tcMar>"
		                   "<w:top w:w=\""    + std::to_wstring(nTopPadding)    + L"\" w:type=\"dxa\"/>"
		                   "<w:left w:w=\""   + std::to_wstring(nLeftPadding)   + L"\" w:type=\"dxa\"/>"
		                   "<w:bottom w:w=\"" + std::to_wstring(nBottomPadding) + L"\" w:type=\"dxa\"/>"
		                   "<w:right w:w=\""  + std::to_wstring(nRightPadding)  + L"\" w:type=\"dxa\"/>"
		               "</w:tcMar>";
	}

	oXmlString += L"<w:hideMark/>";
	oXmlString.WriteNodeEnd(L"w:tcPr");
}

void COOXMLTable::CloseCell(XmlString& oXmlString)
{
	oXmlString.WriteNodeEnd(L"w:tc");
}

void COOXMLTable::ConvertTable(XmlUtils::CXmlLiteReader& oReader, COOXMLWriter& oWriter, const COOXMLTable& oTable, const NSCSS::CNode& oTableNode)
{
	std::vector<NSCSS::CNode> arTableSelectors{oTableNode};

	XmlString& oCurrentDocument{*oWriter.GetCurrentDocument()};
	TTableStyles oTableStyles;

	oTableStyles.m_pTable = &oTable;

	OpenTable(oCurrentDocument, oTableNode, oTableStyles, oTable);

	std::stack<int> arDepths({oReader.GetDepth()});

	bool bConvertedHeader{false}, bConvertedBody{false}, bConvertedFoother{false};

	#define CONVERT_MATRIX(matrix_variable)\
	{\
	if (!bConverted##matrix_variable)\
	{\
		ConvertMatrix(oReader, oWriter, arTableSelectors, arDepths, oTable.m_o##matrix_variable.GetMatrixCells(), oTableStyles, ERowParseMode::matrix_variable);\
		bConverted##matrix_variable = true;\
	}\
	}

	while(oReader.ReadNextSiblingNode2(arDepths.top()))
	{
		const std::wstring sName = oReader.GetName();
		m_oExternalData.GetSubClass(oReader, arTableSelectors);

		if(sName == L"thead")
			CONVERT_MATRIX(Header)
		else if(sName == L"tbody")
			CONVERT_MATRIX(Body)
		else if(sName == L"tfoot")
			CONVERT_MATRIX(Foother)
		else
			arTableSelectors.pop_back();
	}

	CloseTable(oCurrentDocument);
	oWriter.WriteEmptyParagraph(true);
}

void COOXMLTable::ConvertMatrix(XmlUtils::CXmlLiteReader& oReader, COOXMLWriter& oWriter, std::vector<NSCSS::CNode>& arSelectors, std::stack<int>& arDepths, const Table& oMatrix, TTableStyles& oTableStyles, ERowParseMode eRowParseMode)
{
	if (oMatrix.empty())
		return;

	arDepths.push(oReader.GetDepth());

	ITableElementCell* pTableCell{nullptr};
	size_t unCellHeight{0}, unMaxCellHeight{0};
	std::unordered_map<size_t, NSCSS::CNode> mFillingColumn;

	for (size_t unRowIndex = 0; unRowIndex < oMatrix.size(); ++unRowIndex)
	{
		XmlString oCellsData;
		oWriter.SetDataOutput(&oCellsData);
		bool bEmptyRow{true};

		for (size_t unColumnIndex = 0; unColumnIndex < oMatrix[unRowIndex].size(); ++unColumnIndex)
		{
			pTableCell = oMatrix[unRowIndex][unColumnIndex];

			if (nullptr == pTableCell)
			{
				if (unColumnIndex < MAX_COLUMNS_IN_TABLE)
					OpenCell(oCellsData);

				oWriter.WriteEmptyParagraph();

				if (unColumnIndex < MAX_COLUMNS_IN_TABLE - 1)
					CloseCell(oCellsData);

				continue;
			}

			if (ETableElement::FillingCell == pTableCell->GetType())
			{
				if (unColumnIndex < MAX_COLUMNS_IN_TABLE)
				{
					OpenCell(oCellsData, mFillingColumn.at(unColumnIndex), (CTableElementCell*)pTableCell, unColumnIndex, eRowParseMode,
					         ((0 == unRowIndex) ? ERowPosition::First : ((oMatrix.size() - 1 == unRowIndex) ? ERowPosition::Last : ERowPosition::Middle)),
					         unCellHeight, oTableStyles, mFillingColumn);
				}

				oWriter.WriteEmptyParagraph();

				if (unColumnIndex < MAX_COLUMNS_IN_TABLE - 1)
					CloseCell(oCellsData);

				continue;
			}

			MoveToNextTableCell(oReader, arSelectors, arDepths, m_oExternalData.GetSubClass);

			bEmptyRow = false;

			if (ETableElement::TableContainer == pTableCell->GetType())
			{
				if (unColumnIndex < MAX_COLUMNS_IN_TABLE)
					OpenCell(oCellsData, arSelectors.back(), (CTableElementCell*)pTableCell, unColumnIndex, eRowParseMode,
					         ((0 == unRowIndex) ? ERowPosition::First : ((oMatrix.size() - 1 == unRowIndex) ? ERowPosition::Last : ERowPosition::Middle)),
					         unCellHeight, oTableStyles, mFillingColumn);

				std::vector<CTableElement*> arTables{((CTableContainer*)pTableCell)->GetTables()};
				std::vector<CTableElement*>::const_iterator itTable = arTables.cbegin();

				const size_t unDepthSize{arDepths.size()};

				arDepths.push(oReader.GetDepth());

				m_oExternalData.AddStopTag(L"table");

				#define READ_TABLE()\
					m_oExternalData.GetSubClass(oReader, arSelectors);\
					oWriter.CloseP();\
					ConvertTable(oReader, oWriter, *(COOXMLTable*)*itTable++, arSelectors.back());\
					oWriter.WriteEmptyParagraph(true);\
					arSelectors.pop_back()

				while (oReader.ReadNextSiblingNode2(arDepths.top()))
				{
					if (L"table" == oReader.GetName())
					{
						READ_TABLE();
					}
					else
					{
						m_oExternalData.ReadInside(oReader, arSelectors);

						if (L"table" == oReader.GetName())
						{
							READ_TABLE();
						}
					}
				}

				while (arDepths.size() > unDepthSize)
					arDepths.pop();

				arSelectors.resize(unDepthSize);

				m_oExternalData.ClearStopTags();
			}
			else
			{
				if (unColumnIndex < MAX_COLUMNS_IN_TABLE)
					OpenCell(oCellsData, arSelectors.back(), (CTableElementCell*)pTableCell, unColumnIndex, eRowParseMode,
					         ((0 == unRowIndex) ? ERowPosition::First : ((oMatrix.size() - 1 == unRowIndex) ? ERowPosition::Last : ERowPosition::Middle)),
					         unCellHeight, oTableStyles, mFillingColumn);

				unMaxCellHeight = (std::max)(unMaxCellHeight, unCellHeight);

				const size_t unSelectorsSize{arSelectors.size()};

				m_oExternalData.ReadStream(oReader, arSelectors);

				arSelectors.resize(unSelectorsSize);

				arSelectors.pop_back();
			}

			oWriter.CloseP();

			if (unColumnIndex < MAX_COLUMNS_IN_TABLE - 1)
				CloseCell(oCellsData);
		}

		if (oMatrix[unRowIndex].size() >= MAX_COLUMNS_IN_TABLE)
			CloseCell(oCellsData);

		oWriter.RevertDataOutput();

		OpenRow(*oWriter.GetCurrentDocument(), ERowParseMode::Header == eRowParseMode, unMaxCellHeight, 0);
		WriteToStringBuilder(oCellsData, *oWriter.GetCurrentDocument());
		CloseRow(*oWriter.GetCurrentDocument());

		if (!bEmptyRow)
		{
			arSelectors.pop_back();
			arDepths.pop();
		}
	}

	arSelectors.pop_back();
	arDepths.pop();
}

inline void CalculateMaxRowSize(const Table& oMatrix, size_t& unMaxRowSize)
{
	for (const Row& oRow : oMatrix)
	{
		size_t unCurrentSize{0};
		for (ITableElementCell* pCell : oRow)
		{
			if (nullptr == pCell || ETableElement::TableContainer == pCell->GetType())
				unCurrentSize += 1;
			else
				unCurrentSize += ((CTableElementCell*)pCell)->GetColspan();
		}
		unMaxRowSize = (std::max)(unMaxRowSize, unCurrentSize);
	}
}

inline void NormalizeMatrix(Table& oMatrix, const size_t& unMaxMatrixColumns, const size_t& unMaxColumns)
{
	for (Row& oRow : oMatrix)
	{
		for (ITableElementCell* pCell : oRow)
		{
			if (nullptr == pCell)
				continue;

			if (ETableElement::TableContainer == pCell->GetType())
			{
				for (CTableElement* pTable : ((CTableContainer*)pCell)->GetTables())
					if (nullptr != pTable)
						pTable->Normalize();
			}
		}

		if (unMaxMatrixColumns != unMaxColumns)
			oRow.resize(oRow.size() + (unMaxColumns - unMaxMatrixColumns));
	}
}

void COOXMLTable::Normalize()
{
	size_t unMaxColumns{0}, unHeaderRowSize{0}, unBodyRowSize{0}, unFootherRowSize{0};

	CalculateMaxRowSize(m_oHeader.GetMatrixCells(),  unHeaderRowSize);
	CalculateMaxRowSize(m_oBody.GetMatrixCells(),    unBodyRowSize);
	CalculateMaxRowSize(m_oFoother.GetMatrixCells(), unFootherRowSize);

	unMaxColumns = (std::max)((std::max)(unHeaderRowSize, unBodyRowSize), unFootherRowSize);

	NormalizeMatrix(m_oHeader.GetMatrixCells(),  unHeaderRowSize, unMaxColumns);
	NormalizeMatrix(m_oBody.GetMatrixCells(),    unBodyRowSize, unMaxColumns);
	NormalizeMatrix(m_oFoother.GetMatrixCells(), unFootherRowSize, unMaxColumns);
}

bool COOXMLTable::Convert(XmlUtils::CXmlLiteReader& oReader, const NSCSS::CNode& oTableNode)
{
	if (Empty())
		return false;

	COOXMLWriter* pWriter{dynamic_cast<COOXMLWriter*>(m_oExternalData.m_pWriter)};

	if (nullptr == pWriter)
		return false;

	ConvertTable(oReader, *pWriter, *this, oTableNode);

	return true;
}

bool COOXMLTable::ParseCaption(XmlUtils::CXmlLiteReader& oReader, XmlString*& pCaption)
{
	if (nullptr == m_oExternalData.m_pWriter)
		return false;

	if (nullptr == pCaption)
		pCaption = new XmlString(200);

	std::vector<NSCSS::CNode> arSelectors;
	m_oExternalData.GetSubClass(oReader, arSelectors);

	arSelectors.back().m_pCompiledStyle->m_oText.SetAlign(L"center", 0, true);

	COOXMLWriter& oWriter{*(COOXMLWriter*)m_oExternalData.m_pWriter};

	oWriter.SetDataOutput(m_pCaption);
	oWriter.WritePPr(arSelectors);
	m_oExternalData.ReadStream(oReader, arSelectors);
	oWriter.CloseP();
	oWriter.RevertDataOutput();

	return true;
}

bool COOXMLTable::ParseColgroup(XmlUtils::CXmlLiteReader& oReader, std::vector<CTableColgroup*>& arColgroups)
{
	std::vector<NSCSS::CNode> arNodes;

	m_oExternalData.GetSubClass(oReader, arNodes);

	CTableColgroup *pColgroup = new CTableColgroup(arNodes.back());

	if (NULL == pColgroup)
		return false;

	const int nDeath = oReader.GetDepth();
	if (!oReader.IsEmptyNode() && oReader.ReadNextSiblingNode2(nDeath))
	{
		do
		{
			if (L"col" != oReader.GetName())
				continue;

			m_oExternalData.GetSubClass(oReader, arNodes);

			CTableCol *pCol = new CTableCol(arNodes.back());

			if (NULL == pCol)
				continue;

			arNodes.pop_back();
			pColgroup->AddCol(pCol);
		} while(oReader.ReadNextSiblingNode2(nDeath));
	}

	// if(pColgroup->Empty())
	// {
	// 	std::map<std::wstring, std::wstring>::const_iterator itFound = arNodes.begin()->m_mAttributes.find(L"span");

	// 	CTableCol *pCol = new CTableCol((arNodes.begin()->m_mAttributes.cend() != itFound) ? NSStringFinder::ToInt(itFound->second, 1) : 1);

	// 	if (NULL == pCol)
	// 		return;

	// 	CalculateCellStyles(pCol->GetStyle(), arNodes);

	// 	pColgroup->AddCol(pCol);
	// }

	arColgroups.push_back(pColgroup);

	return true;
}
}
