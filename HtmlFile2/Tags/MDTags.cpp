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

#include "MDTags.h"

#include "../DesktopEditor/xml/include/xmlutils.h"

#include "../src/StringFinder.h"
#include "../Table.h"
#include "../Common/3dParty/html/css/src/CCompiledStyle.h"

#include "../../DesktopEditor/common/Base64.h"
#include "../../DesktopEditor/common/Path.h"
#include "../../DesktopEditor/common/Directory.h"

#include "../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../DesktopEditor/raster/BgraFrame.h"

#include "../../Common/3dParty/html/gumbo-parser/src/gumbo.h"

namespace HTML
{
void InitTagsForMD(std::map<int, std::shared_ptr<ITag>>& mTags, CMDWriter* pWriter)
{
	#define HTML_TAG(tag) GUMBO_TAG_##tag
	#define ADD_TAG(tag_name, tag_class_name) mTags[HTML_TAG(tag_name)] = std::make_shared<C ##tag_class_name##MDTag> (pWriter)

	ADD_TAG(A, Anchor);
	ADD_TAG(BR, Break);
	ADD_TAG(PRE, Preformatted);
	ADD_TAG(H1, Header);
	ADD_TAG(IMG, Image);
	ADD_TAG(HR, HorizontalRule);
	ADD_TAG(BLOCKQUOTE, Blockquote);
	ADD_TAG(OL, List);
	ADD_TAG(LI, ListElement);
	ADD_TAG(CODE, Code);
}

bool CAnchorMDTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	m_pWriter->WriteString(L"[");
	return true;
}

void CAnchorMDTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return;

	m_pWriter->WriteString(L"]");

	std::wstring wsHref, wsTitle;

	arSelectors.back().GetAttributeValue(L"href", wsHref);
	arSelectors.back().GetAttributeValue(L"title", wsTitle);

	m_pWriter->WriteString(L'(' + wsHref);

	if (!wsTitle.empty())
		m_pWriter->WriteString(L" \"" + wsTitle + L'"');

	m_pWriter->WriteString(L")");
}

bool CBreakMDTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	m_pWriter->WriteBreakLine();

	return true;
}

void CBreakMDTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

bool CPreformattedMDTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	if (m_pWriter->InTable())
		m_pWriter->WriteBreakLine();
	else
		m_pWriter->WriteOpenSpecialString(L"```");

	m_pWriter->EnteredPreformatted();

	return true;
}

void CPreformattedMDTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return;

	if (m_pWriter->InTable())
	{
		m_pWriter->OutPreformatted();
		m_pWriter->WriteBreakLine();
		return;
	}

	bool bNeedBreakLine{false};

	for (std::vector<NSCSS::CNode>::const_reverse_iterator itElement{arSelectors.crbegin()}; itElement < arSelectors.crend(); ++itElement)
	{
		if (L"pre" == itElement->m_wsName || L"xmp" == itElement->m_wsName)
		{
			bNeedBreakLine = true;
			break;
		}
	}

	if (bNeedBreakLine)
		m_pWriter->WriteBreakLine();

	m_pWriter->WriteCloseSpecialString(L"```");
	m_pWriter->OutPreformatted();

	if (bNeedBreakLine)
		m_pWriter->WriteBreakLine(false);
}

bool CHeaderMDTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	switch(arSelectors.back().m_wsName[1])
	{
		case L'1' : m_pWriter->WriteString(L"# ",      true); break;
		case L'2' : m_pWriter->WriteString(L"## ",     true); break;
		case L'3' : m_pWriter->WriteString(L"### ",    true); break;
		case L'4' : m_pWriter->WriteString(L"#### ",   true); break;
		case L'5' : m_pWriter->WriteString(L"##### ",  true); break;
		case L'6' : m_pWriter->WriteString(L"###### ", true); break;
		default:
			return false;
	}

	return true;
}

void CHeaderMDTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (Valid())
		m_pWriter->WriteBreakLine();
}

bool ConvertBufferToBase64(BYTE* pBuffer, size_t unSize, std::wstring& wsBase64)
{
	if (nullptr == pBuffer || 0 == unSize)
		return false;

	int nDecodeSize{NSBase64::Base64EncodeGetRequiredLength(unSize)};

	BYTE* pBase64Buffer = new BYTE[nDecodeSize];

	if (nullptr == pBase64Buffer)
		return false;

	if (FALSE == NSBase64::Base64Encode(pBuffer, unSize, pBase64Buffer, &nDecodeSize, 2))
	{
		RELEASEARRAYOBJECTS(pBase64Buffer);
		return false;
	}

	wsBase64.reserve(22 + nDecodeSize);

	wsBase64 = L"data:image/png;base64,";
	wsBase64.append(pBase64Buffer, pBase64Buffer + nDecodeSize);

	RELEASEARRAYOBJECTS(pBase64Buffer);

	return true;
}

bool ConvertFromMetafileBase(MetaFile::IMetaFile* pMetafileReader, UINT unWidth, UINT unHeight, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, std::wstring& wsResultBase64)
{
	NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();
	pGrRenderer->SetFontManager(pMetafileReader->get_FontManager());

	double dX, dY, dW, dH;
	pMetafileReader->GetBounds(&dX, &dY, &dW, &dH);

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

	if (0 == unWidth && 0 == unHeight)
	{
		unWidth  = (int)(dW * 96 / 25.4);
		unHeight = (int)(dH * 96 / 25.4);
	}
	else if (0 == unWidth)
		unWidth = (int)((double)unHeight * dW / dH);
	else if (0 == unHeight)
		unHeight = (int)((double)unWidth * dH / dW);

	double dWidth  = 25.4 * unWidth / 96;
	double dHeight = 25.4 * unHeight / 96;

	BYTE* pBgraData = (BYTE*)malloc(unWidth * unHeight * 4);
	if (!pBgraData)
	{
		double dKoef = 2000.0 / (unWidth > unHeight ? unWidth : unHeight);

		unWidth = (int)(dKoef * unWidth);
		unHeight = (int)(dKoef * unWidth);

		dWidth  = 25.4 * unWidth / 96;
		dHeight = 25.4 * unHeight / 96;

		pBgraData = (BYTE*)malloc(unWidth * unHeight * 4);
	}

	if (!pBgraData)
		return false;

	unsigned int alfa = 0xffffff;
	//The default tone should be transparent, not white
	//memset(pBgraData, 0xff, nWidth * nHeight * 4);
	for (int i = 0; i < unWidth * unHeight; i++)
		((unsigned int*)pBgraData)[i] = alfa;

	CBgraFrame oFrame;
	oFrame.put_Data(pBgraData);
	oFrame.put_Width(unWidth);
	oFrame.put_Height(unHeight);
	oFrame.put_Stride(-4 * unWidth);

	pGrRenderer->CreateFromBgraFrame(&oFrame);
	pGrRenderer->SetSwapRGB(false);
	pGrRenderer->put_Width(dWidth);
	pGrRenderer->put_Height(dHeight);

	pMetafileReader->SetTempDirectory(wsTempDir);
	pMetafileReader->DrawOnRenderer(pGrRenderer, 0, 0, dWidth, dHeight);

	BYTE *pImageBuffer{nullptr};
	int nImageSize;

	bool bResult{false};

	if (oFrame.Encode(pImageBuffer, nImageSize, 4))
	{
		bResult = ConvertBufferToBase64(pImageBuffer, nImageSize, wsResultBase64);
		RELEASEARRAYOBJECTS(pImageBuffer);
	}

	RELEASEINTERFACE(pGrRenderer);

	return bResult;
}

bool ConvertFromMetafile(const std::wstring& wsFilePath, UINT unWidth, UINT unHeight, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, std::wstring& wsResultBase64)
{
	MetaFile::IMetaFile* pMetafileReader = MetaFile::Create(pFonts);

	if (!pMetafileReader->LoadFromFile(wsFilePath.c_str()))
	{
		RELEASEINTERFACE(pMetafileReader);
		return false;
	}

	const bool bResult{ConvertFromMetafileBase(pMetafileReader, unWidth, unHeight, pFonts, wsTempDir, wsResultBase64)};

	RELEASEINTERFACE(pMetafileReader);

	return bResult;
}

bool ConvertFromSVG(const std::wstring& wsSVG, UINT unWidth, UINT unHeight, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, std::wstring& wsResultBase64)
{
	MetaFile::IMetaFile* pSvgReader = MetaFile::Create(pFonts);
	if (!pSvgReader->LoadFromString(wsSVG))
	{
		RELEASEINTERFACE(pSvgReader);
		return false;
	}

	const bool bResult{ConvertFromMetafileBase(pSvgReader, unWidth, unHeight, pFonts, wsTempDir, wsResultBase64)};

	RELEASEINTERFACE(pSvgReader);

	return bResult;
}

bool IsSVGExtention(const std::wstring& wsExtention)
{
	return L"svg" == wsExtention || L"svg+xml" == wsExtention;
}

bool IsMetafileExtention(const std::wstring& wsExtention)
{
	return L"emf" == wsExtention || L"wmf" == wsExtention;
}

bool ConvertFromBase64(const std::wstring& wsBase64, UINT unWidth, UINT unHeight, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, std::wstring& wsResultBase64)
{
	const size_t unStartExtention{wsBase64.find(L"/", 4)};
	if (unStartExtention == std::wstring::npos)
		return false;

	const size_t unEndExtention{wsBase64.find(L";", unStartExtention)};
	if (unEndExtention == std::wstring::npos)
		return false;

	const size_t nBase = wsBase64.find(L"base64", unEndExtention);
	if (nBase == std::wstring::npos)
		return false;

	bool bResult{false};

	const int nOffset = nBase + 7; //Skip "base64,"
	int nSrcLen = (int)(wsBase64.length() - nBase + 1);
	int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);

	if (nDecodeLen != 0)
	{
		BYTE* pImageData = new BYTE[nDecodeLen];

		if (!pImageData || FALSE == NSBase64::Base64Decode(wsBase64.c_str() + nOffset, nSrcLen, pImageData, &nDecodeLen))
			return false;

		if (IsSVGExtention(wsBase64.substr(unStartExtention + 1, unEndExtention - unStartExtention - 1)))
		{
			const std::wstring wsSvg(pImageData, pImageData + nDecodeLen);

			RELEASEARRAYOBJECTS(pImageData);

			bResult = ConvertFromSVG(wsSvg, unWidth, unHeight, pFonts, wsTempDir, wsResultBase64);
		}
		else
		{
			CBgraFrame oFrame;
			oFrame.Decode(pImageData, nDecodeLen);

			RELEASEARRAYOBJECTS(pImageData);

			oFrame.Resize(unWidth, unHeight);

			BYTE *pBuffer{nullptr};
			int nEncodeSize{0};

			oFrame.Encode(pBuffer, nEncodeSize, 4);

			bResult = ConvertBufferToBase64(pBuffer, nEncodeSize, wsResultBase64);

			RELEASEARRAYOBJECTS(pBuffer);
		}
	}

	return bResult;
}

bool ConvertFileToBase64(const std::wstring& wsFilePath, const std::wstring& wsRootPath, UINT unWidth, UINT unHeight, bool bIsAllowExternalLocalFiles, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, std::wstring& wsBase64)
{
	if (wsFilePath.empty() || !NSFile::CFileBinary::Exists(wsFilePath))
		return false;

	if (!bIsAllowExternalLocalFiles && !NSSystemPath::IsPathInDirectory(wsRootPath, wsFilePath))
		return false;

	const std::wstring wsExtention{NSFile::GetFileExtention(wsFilePath)};

	if (IsSVGExtention(wsExtention))
	{
		std::wstring wsSVG;

		if (!NSFile::CFileBinary::ReadAllTextUtf8(wsFilePath, wsSVG))
			return false;

		return ConvertFromSVG(wsSVG, unWidth, unHeight, pFonts, wsTempDir, wsBase64);
	}
	else if(IsMetafileExtention(wsExtention))
		return ConvertFromMetafile(wsFilePath, unWidth, unHeight, pFonts, wsTempDir, wsBase64);

	CBgraFrame oFrame;

	if (!oFrame.OpenFile(wsFilePath))
		return false;

	if (!oFrame.Resize(unWidth, unHeight))
		return false;

	BYTE *pBuffer{nullptr};
	int nEncodeSize{0};

	if (!oFrame.Encode(pBuffer, nEncodeSize, 4))
		return false;

	const bool bResult{ConvertBufferToBase64(pBuffer, nEncodeSize, wsBase64)};

	RELEASEARRAYOBJECTS(pBuffer);

	return bResult;
}

bool CImageMDTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	if (L"svg" == arSelectors.back().m_wsName)
	{
		std::wstring wsBase64;

		if (!ConvertFromSVG(arSelectors.back().GetAttributeValue(L"svg-text"), 0, 0, m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsBase64))
			return false;

		m_pWriter->WriteString(L"![](" + wsBase64 + L")");
	}

	std::wstring wsAlt, wsSrc, wsTitle;

	if (!arSelectors.back().GetAttributeValue(L"src", wsSrc) &&
	    !arSelectors.back().GetAttributeValue(L"alt", wsAlt))
		return false;

	NSCSS::NSProperties::CDigit oWidth{arSelectors.back().m_pCompiledStyle->m_oDisplay.GetWidth()};
	NSCSS::NSProperties::CDigit oHeight{arSelectors.back().m_pCompiledStyle->m_oDisplay.GetHeight()};

	std::wstring wsValue;

	if (arSelectors.back().GetAttributeValue(L"width", wsValue))
		oWidth.SetValue(wsValue);

	if (arSelectors.back().GetAttributeValue(L"height", wsValue))
		oHeight.SetValue(wsValue);

	const bool bNeedSetSize{(!oWidth.Empty() && !oWidth.Zero()) || (!oHeight.Empty() && !oHeight.Zero())};
	bool bResult{false};
	std::wstring wsResultBase64;

	// Assume an image in Base64
	if (wsSrc.length() > 4 && wsSrc.substr(0, 4) == L"data" && wsSrc.find(L"/", 4) != std::wstring::npos)
	{
		if (bNeedSetSize)
			bResult = ConvertFromBase64(wsSrc, oWidth.ToInt(NSCSS::UnitMeasure::Pixel), oHeight.ToInt(NSCSS::UnitMeasure::Pixel), m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsResultBase64);
		else
			bResult = true;
	}

	if (!bResult && (wsSrc.length() <= 7 || L"http" != wsSrc.substr(0, 4)))
	{
		wsSrc = NSSystemPath::ShortenPath(wsSrc);

		if (!CanUseThisPath(wsSrc, m_pWriter->GetRootPath(), m_pWriter->GetSrcPath(), GetStatusUsingExternalLocalFiles()))
		{
			wsSrc.clear();
			bResult = true;
		}
	}

	const std::wstring wsBasePath{m_pWriter->GetBasePath()};

	// Assume an image on the network
	if (!bResult &&
	    ((!wsBasePath.empty() && wsBasePath.length() > 4 && wsBasePath.substr(0, 4) == L"http") ||
	      (wsSrc.length() > 4 && wsSrc.substr(0, 4) == L"http")))
	{
		const std::wstring wsDst{NSFile::CFileBinary::CreateTempFileWithUniqueName(m_pWriter->GetTempDir(), L"IMG")};

		bResult = m_pWriter->DownloadExternalFile(wsBasePath + wsSrc, wsDst);

		if (!bResult)
		{
			bResult = true;
			wsSrc.clear();
		}

		if (IsSVGExtention(NSFile::GetFileExtention(wsSrc)))
		{
			std::wstring wsFileData;

			if (NSFile::CFileBinary::ReadAllTextUtf8(wsDst, wsFileData) &&
			    ConvertFromSVG(wsFileData, oWidth.ToInt(NSCSS::UnitMeasure::Pixel), oHeight.ToInt(NSCSS::UnitMeasure::Pixel), m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsResultBase64))
				bResult = true;

			NSFile::CFileBinary::Remove(wsDst);
		}
	}

	// Assume an image at a local path
	if (!bResult)
	{
		const bool bIsAllowExternalLocalFiles{GetStatusUsingExternalLocalFiles()};

		const std::wstring wsRootPath{m_pWriter->GetRootPath()};

		if (!m_pWriter->GetBasePath().empty())
		{
			if (!bResult)
				bResult = ConvertFileToBase64(NSSystemPath::ShortenPath(NSSystemPath::Combine(m_pWriter->GetBasePath(), wsSrc)), wsRootPath, oWidth.ToInt(NSCSS::UnitMeasure::Pixel), oHeight.ToInt(NSCSS::UnitMeasure::Pixel), bIsAllowExternalLocalFiles, m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsResultBase64);
			if (!bResult)
				bResult = ConvertFileToBase64(NSSystemPath::ShortenPath(NSSystemPath::Combine(m_pWriter->GetSrcPath(), NSSystemPath::Combine(m_pWriter->GetBasePath(), wsSrc))), wsRootPath, oWidth.ToInt(NSCSS::UnitMeasure::Pixel), oHeight.ToInt(NSCSS::UnitMeasure::Pixel), bIsAllowExternalLocalFiles, m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsResultBase64);
		}
		if (!bResult)
			bResult = ConvertFileToBase64(NSSystemPath::ShortenPath(NSSystemPath::Combine(m_pWriter->GetSrcPath(), wsSrc)), wsRootPath, oWidth.ToInt(NSCSS::UnitMeasure::Pixel), oHeight.ToInt(NSCSS::UnitMeasure::Pixel), bIsAllowExternalLocalFiles, m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsResultBase64);
		if (!bResult)
			bResult = ConvertFileToBase64(wsSrc, wsRootPath, oWidth.ToInt(NSCSS::UnitMeasure::Pixel), oHeight.ToInt(NSCSS::UnitMeasure::Pixel), bIsAllowExternalLocalFiles, m_pWriter->GetFonts(), m_pWriter->GetTempDir(), wsResultBase64);
	}

	arSelectors.back().GetAttributeValue(L"title", wsTitle);

	m_pWriter->WriteString(L"![" + wsAlt + L"](" + ((bResult) ? wsResultBase64 : wsSrc));

	if (!wsTitle.empty())
		m_pWriter->WriteString(L" \"" + wsTitle + L'"');

	m_pWriter->WriteString(L")");

	return true;
}

void CImageMDTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

bool CHorizontalRuleMDTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	if (m_pWriter->InTable())
	{
		if (m_pWriter->GetParametrs().m_bUseAlternativeHTMLTags)
		{
			m_pWriter->WriteString(L"<hr/>");
			return true;
		}
		return false;
	}

	m_pWriter->WriteBreakLine(false);
	m_pWriter->WriteString(L"---");
	m_pWriter->WriteBreakLine(false);

	return true;
}

void CHorizontalRuleMDTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

bool CBlockquoteMDTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	m_pWriter->WriteBreakLine();
	m_pWriter->EnteredBlockquote();

	return true;
}

void CBlockquoteMDTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return;

	m_pWriter->OutBlockquote();
	m_pWriter->WriteBreakLine();
	m_pWriter->WriteBreakLine(false);
}

bool CListMDTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	m_pWriter->WriteBreakLine();
	m_pWriter->EnteredList(L"ol" == arSelectors.back().m_wsName);

	if (!m_pWriter->InOrederedList())
		return true;

	std::wstring wsIndex;

	if (arSelectors.back().GetAttributeValue(L"start", wsIndex))
		m_pWriter->SetIndexOrderedList(NSStringFinder::ToInt(wsIndex, 1));

	return true;

	return true;
}

void CListMDTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return;

	m_pWriter->OutList();
}

bool CListElementMDTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	if (0 !=  m_pWriter->GetLevelList())
	{
		for (UINT unLevelList = 0; unLevelList < m_pWriter->GetLevelList() - 1; ++unLevelList)
			m_pWriter->WriteString(L"  ");
	}

	if (m_pWriter->InOrederedList())
	{
		m_pWriter->WriteString(std::to_wstring(m_pWriter->GetIndexOrderedList()) + m_pWriter->GetParametrs().m_wchOrderedList + L' ');
		m_pWriter->IncreaseIndexOrderedList();
	}
	else
		m_pWriter->WriteString({m_pWriter->GetParametrs().m_wchUnorderedList, L' '});

	return true;
}

void CListElementMDTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{}

bool CCodeMDTag::Open(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return false;

	m_pWriter->EnteredCode();

	if (m_pWriter->InPreformatted() && !m_pWriter->InTable())
	{
		if (!arSelectors.back().m_wsClass.empty() && arSelectors.back().m_wsClass.size() >= 9 &&
		    0 == arSelectors.back().m_wsClass.compare(0, 9, L"language-"))
			m_pWriter->WriteString(arSelectors.back().m_wsClass.substr(9, arSelectors.back().m_wsClass.size() - 9));
		m_pWriter->WriteBreakLine(false);
	}

	return true;
}

void CCodeMDTag::Close(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!Valid())
		return;

	m_pWriter->OutCode();
}

bool IsTableContainer(const ITableElementCell* pCell)
{
	return nullptr != pCell && ETableElement::TableContainer == pCell->GetType();
}

CMarkdownTable::CMarkdownTable(TExternalTableData &oExternalData)
	: CTableElement(oExternalData)
{}

CMarkdownTable::~CMarkdownTable()
{}

bool CMarkdownTable::PreParse(XmlUtils::CXmlLiteReader& oReader)
{
	return ParseTable(oReader, this);
}

void CMarkdownTable::Normalize()
{
	#define FLATTEN_TABLE(table_variable) table_variable.GetMatrixCells() = Flatten(std::move(table_variable .GetMatrixCells()))

	FLATTEN_TABLE(m_oHeader );
	FLATTEN_TABLE(m_oBody   );
	FLATTEN_TABLE(m_oFoother);

	size_t unMaxColumns{m_oBody.GetColumnSize()};

	if (!m_oHeader.Empty())
		unMaxColumns = (std::max)(unMaxColumns, m_oHeader.GetColumnSize());

	if (!m_oFoother.Empty())
		unMaxColumns = (std::max)(unMaxColumns, m_oFoother.GetColumnSize());

	#define NORMALIZE_NUMBER_COLUMN(table_variable)\
	if (!table_variable.Empty() && unMaxColumns != table_variable.GetColumnSize())\
		table_variable.NormalizeNumberColumns(unMaxColumns)

	NORMALIZE_NUMBER_COLUMN(m_oHeader);
	NORMALIZE_NUMBER_COLUMN(m_oBody);
	NORMALIZE_NUMBER_COLUMN(m_oFoother);
}


typedef std::map<std::pair<size_t, size_t>, NSStringUtils::CStringBuilder*> NestedCells;

inline void WriteRowStart(CMDWriter& oWriter)
{
	oWriter.WriteOpenSpecialString(L"| ");
}

inline void WriteRowEnd(CMDWriter& oWriter)
{
	oWriter.WriteOpenSpecialString(L" |");
	oWriter.WriteBreakLine(false);
}

inline void WriteCellSeparator(CMDWriter& oWriter)
{
	oWriter.WriteOpenSpecialString(L" | ");
}

// TODO: Instead of handling the different parsing behavior for nested tables
// (when a cell contains both a table and data),
// you can set a specific parsing method in HTMLReader and replace
// it with the desired one. As a result,
// everything except the table will be parsed using HTMLReader,
// while reading the nested table will be done using the newly set method.

void ReadNestedCells(XmlUtils::CXmlLiteReader& oReader, std::vector<NSCSS::CNode>& arSelectors, TCurentTablePosition& oPosition, NestedCells& arNestedCells, const Table& oCells, CMDWriter* pWriter, const TExternalTableData& oExternalTableData, NSStringUtils::CStringBuilder& oIntermediateData);

void ReadNestedTable(XmlUtils::CXmlLiteReader& oReader, std::vector<NSCSS::CNode>& arSelectors, TCurentTablePosition& oPosition, NestedCells& arNestedCells, const Table& oCells, CMDWriter* pWriter, const TExternalTableData& oExternalTableData, NSStringUtils::CStringBuilder& oIntermediateData)
{
	TCurentTablePosition oNestedPosition{oPosition};

	oNestedPosition.m_unStartRowIndex    = oPosition.m_unRowIndex;
	oNestedPosition.m_unStartColumnIndex = oPosition.m_unColumnIndex;

	std::vector<NSCSS::CNode> arNestedSelectors{arSelectors.back()};

	ReadNestedCells(oReader, arNestedSelectors, oNestedPosition, arNestedCells, oCells, pWriter, oExternalTableData, oIntermediateData);

	oPosition.m_unRowIndex = oNestedPosition.m_unRowIndex;
}

void ReadNestedCells(XmlUtils::CXmlLiteReader& oReader, std::vector<NSCSS::CNode>& arSelectors, TCurentTablePosition& oPosition, NestedCells& arNestedCells, const Table& oCells, CMDWriter* pWriter, const TExternalTableData& oExternalTableData, NSStringUtils::CStringBuilder& oIntermediateData)
{
	const int nDepth{oReader.GetDepth()};

	while(oReader.ReadNextSiblingNode2(nDepth))
	{
		const std::wstring wsName = oReader.GetName();

		oExternalTableData.GetSubClass(oReader, arSelectors);

		if (L"td" == wsName || L"th" == wsName)
		{
			const ITableElementCell* pCell{nullptr};

			if (oPosition.m_unRowIndex < oCells.size() && oPosition.m_unColumnIndex < oCells[oPosition.m_unRowIndex].size())
				pCell = oCells[oPosition.m_unRowIndex][oPosition.m_unColumnIndex];

			if ((oPosition.m_unRowIndex != oPosition.m_unStartRowIndex || oPosition.m_unColumnIndex != oPosition.m_unStartColumnIndex) &&
			    (nullptr != pCell && ETableElement::FlatTable == pCell->GetType()))
			{
				ReadNestedCells(oReader, arSelectors, oPosition, arNestedCells, oCells, pWriter, oExternalTableData, oIntermediateData);
				arSelectors.pop_back();
				continue;
			}

			NSStringUtils::CStringBuilder *pCellData = new NSStringUtils::CStringBuilder(20);

			if (nullptr != pCellData)
			{
				pCellData->Write(oIntermediateData);
				oIntermediateData.Clear();

				pWriter->SetDataOutput(pCellData);
				oExternalTableData.ReadStream(oReader, arSelectors);
				pWriter->RevertDataOutput();
			}

			if (L"table" == oReader.GetName())
			{
				if (nullptr != pCellData)
				{
					oIntermediateData.WriteBefore(*pCellData);
					delete pCellData;
				}

				const int nCellContentDepth{oReader.GetDepth() - 1};

				ReadNestedTable(oReader, arSelectors, oPosition, arNestedCells, oCells, pWriter, oExternalTableData, oIntermediateData);

				while (oReader.ReadNextSiblingNode2(nCellContentDepth))
				{
					if (L"table" == oReader.GetName())
					{
						ReadNestedTable(oReader, arSelectors, oPosition, arNestedCells, oCells, pWriter, oExternalTableData, oIntermediateData);
						continue;
					}

					pWriter->SetDataOutput(&oIntermediateData);
					oExternalTableData.ReadInside(oReader, arSelectors);
					pWriter->RevertDataOutput();
				}

				arSelectors.pop_back();
				continue;
			}

			while (oPosition.m_unRowIndex < oCells.size() &&
			       oPosition.m_unColumnIndex < oCells[oPosition.m_unRowIndex].size() &&
			       nullptr != oCells[oPosition.m_unRowIndex][oPosition.m_unColumnIndex])
			{
				if (ETableElement::FillingCell == oCells[oPosition.m_unRowIndex][oPosition.m_unColumnIndex]->GetType())
					oPosition.m_unColumnIndex +=  oCells[oPosition.m_unRowIndex][oPosition.m_unColumnIndex]->GetColspan();
				else
					break;
			}

			arNestedCells[{oPosition.m_unRowIndex, oPosition.m_unColumnIndex}] = pCellData;

			if (oReader.MoveToFirstAttribute())
			{
				do
				{
					if (L"colspan" == oReader.GetName())
						oPosition.m_unColumnIndex += NSStringFinder::ToInt(oReader.GetText(), 1) - 1;
				}while (oReader.MoveToNextAttribute());
				oReader.MoveToElement();
			}

			//READ and add to nested cells
			++oPosition.m_unColumnIndex;
		}
		else if (L"tr" == wsName)
		{
			oPosition.m_unColumnIndex = oPosition.m_unStartColumnIndex;

			ReadNestedCells(oReader, arSelectors, oPosition, arNestedCells, oCells, pWriter, oExternalTableData, oIntermediateData);

			++oPosition.m_unRowIndex;
		}
		else if (L"table" == wsName)
			ReadNestedTable(oReader, arSelectors, oPosition, arNestedCells, oCells, pWriter, oExternalTableData, oIntermediateData);
		else if (L"caption" == wsName)
		{
			arSelectors.pop_back();
			continue;
		}
		else if (L"tbody" == wsName || L"thead" == wsName || L"tfoot" == wsName)
			ReadNestedCells(oReader, arSelectors, oPosition, arNestedCells, oCells, pWriter, oExternalTableData, oIntermediateData);
		else
		{
			const size_t unCurrentIntermediateDataSize{oIntermediateData.GetCurSize()};

			pWriter->SetDataOutput(&oIntermediateData);
			oExternalTableData.ReadInside(oReader, arSelectors);
			pWriter->RevertDataOutput();

			if (unCurrentIntermediateDataSize != oIntermediateData.GetCurSize())
				oIntermediateData.WriteString(L" ");

			if (L"table" != oReader.GetName())
			{
				arSelectors.pop_back();
				continue;
			}

			ReadNestedTable(oReader, arSelectors, oPosition, arNestedCells, oCells, pWriter, oExternalTableData, oIntermediateData);

			pWriter->SetDataOutput(&oIntermediateData);

			const int nNewDepth{oReader.GetDepth()};
			while (oReader.ReadNextSiblingNode2(nNewDepth - 1))
				oExternalTableData.ReadInside(oReader, arSelectors);

			pWriter->RevertDataOutput();

			if (0 != oIntermediateData.GetCurSize() && !arNestedCells.empty() && nullptr != arNestedCells.rbegin()->second)
			{
				arNestedCells.rbegin()->second->WriteString(L" ");
				arNestedCells.rbegin()->second->Write(oIntermediateData);
				oIntermediateData.Clear();
			}
		}

		arSelectors.pop_back();
	}
}

bool CMarkdownTable::Convert(XmlUtils::CXmlLiteReader& oReader, std::vector<NSCSS::CNode>& arTableSelectors)
{
	if (Empty() || arTableSelectors.empty())
		return false;

	CMDWriter *pWriter{dynamic_cast<CMDWriter*>(m_oExternalData.m_pWriter)};

	if (nullptr == pWriter)
		return false;

	const size_t unBaseSelectors{arTableSelectors.size()};

	m_oExternalData.AddStopTag(L"table");

	pWriter->WriteBreakLine(false);

	if (HaveCaption())
		WriteToStringBuilder(*m_pCaption, *pWriter->GetCurrentDocument());

	//Open table
	pWriter->WriteBreakLine();
	pWriter->EnteredTable();

	if (pWriter->InCode())
		pWriter->OutCode();

	if (pWriter->InPreformatted())
	{
		pWriter->WriteBreakLine();
		pWriter->WriteCloseSpecialString(L"```");
		pWriter->WriteBreakLine(false);
		pWriter->OutPreformatted();
	}
	//-----

	//Convert header
	if (!ConvertMatrix(oReader, arTableSelectors, m_oHeader.GetMatrixCells(), pWriter, true))
	{
		WriteRowStart(*pWriter);

		for (size_t unColumnIndex = 0; unColumnIndex < m_oBody.GetColumnSize() - 1; ++unColumnIndex)
			WriteCellSeparator(*pWriter);

		WriteRowEnd(*pWriter);

		WriteRowStart(*pWriter);

		for (size_t unColumnIndex = 0; unColumnIndex < m_oBody.GetColumnSize(); ++unColumnIndex)
		{
			pWriter->WriteString(L"-", true);

			if (unColumnIndex != m_oBody.GetColumnSize() - 1)
				WriteCellSeparator(*pWriter);
		}

		WriteRowEnd(*pWriter);
	}
	//Convert body
	ConvertMatrix(oReader, arTableSelectors, m_oBody.GetMatrixCells(), pWriter);
	//Convert foother
	ConvertMatrix(oReader, arTableSelectors, m_oFoother.GetMatrixCells(), pWriter);

	arTableSelectors.resize(unBaseSelectors);

	m_oExternalData.RemoveStopTag(L"table");

	//Close table
	pWriter->OutTable();
	pWriter->WriteBreakLine(false);

	return true;
}

bool CMarkdownTable::ParseCaption(XmlUtils::CXmlLiteReader& oReader, XmlString*& pCaption)
{
	if (nullptr == m_oExternalData.m_pWriter)
		return false;

	if (nullptr == pCaption)
		pCaption = new XmlString(50);

	std::vector<NSCSS::CNode> arSelectors;
	m_oExternalData.GetSubClass(oReader, arSelectors);

	arSelectors.back().m_pCompiledStyle->m_oText.SetAlign(L"center", 0, true);

	CMDWriter& oWriter{*(CMDWriter*)m_oExternalData.m_pWriter};

	oWriter.SetDataOutput(m_pCaption);
	m_oExternalData.ReadStream(oReader, arSelectors);
	oWriter.WriteBreakLine();
	oWriter.RevertDataOutput();

	return true;
}

bool CMarkdownTable::ParseColgroup(XmlUtils::CXmlLiteReader& oReader, std::vector<CTableColgroup*>& arColgroups)
{
	return true;
}

bool CMarkdownTable::ConvertMatrix(XmlUtils::CXmlLiteReader& oReader, std::vector<NSCSS::CNode>& arSelectors, const Table& oMatrix, CMDWriter* pWriter, bool bIsHeader)
{
	if (oMatrix.empty() || nullptr == pWriter)
		return false;

	std::stack<int> arDepths({0});

	const size_t unMinDepths{arDepths.size()};
	const size_t unBaseSelectors{arSelectors.size()};

	NestedCells arNestedCells;
	ITableElementCell* pTableCell{nullptr};

	for (size_t unRowIndex = 0; unRowIndex < oMatrix.size(); ++unRowIndex)
	{
		WriteRowStart(*pWriter);

		for (size_t unColumnIndex = 0; unColumnIndex < oMatrix[unRowIndex].size();)
		{
			pTableCell = oMatrix[unRowIndex][unColumnIndex];

			if (nullptr != pTableCell && ETableElement::FillingCell != pTableCell->GetType())
			{
				if (!arNestedCells.empty())
				{
					NestedCells::const_iterator itElement{arNestedCells.find({unRowIndex, unColumnIndex})};

					if (arNestedCells.end() != itElement)
					{
						if (nullptr != itElement->second)
						{
							m_oExternalData.m_pWriter->GetCurrentDocument()->Write(*itElement->second);
							delete itElement->second;

							if (oMatrix[unRowIndex].size() - 1 != unColumnIndex)
								WriteCellSeparator(*pWriter);
						}
						arNestedCells.erase(itElement);

						++unColumnIndex;
						continue;
					}
				}

				if (!MoveToNextTableCell(oReader, arSelectors, arDepths, m_oExternalData.GetSubClass, unMinDepths))
				{
					if (oMatrix[unRowIndex].size() - 1 != unColumnIndex)
						WriteCellSeparator(*pWriter);

					++unColumnIndex;
					continue;
				}

				const size_t unDepthSize{arDepths.size()};
				const size_t unCellSelectors{arSelectors.size()};

				if (ETableElement::FlatTable == pTableCell->GetType())
				{
					TCurentTablePosition oPosition(unRowIndex, unColumnIndex, unRowIndex, unColumnIndex);
					std::vector<NSCSS::CNode> arNestedSelectors;

					const int nDepth{oReader.GetDepth()};

					NSStringUtils::CStringBuilder oTempData(100);
					bool bSetedDataOutput{false};

					arDepths.push(nDepth);

					const size_t unContainerDepths{arDepths.size()};

					NSStringUtils::CStringBuilder oIntermediateData(100);

					while (true)
					{
						bool bMoved{false};

						while (true)
						{
							if (oReader.ReadNextSiblingNode2(arDepths.top()))
							{
								bMoved = true;
								break;
							}

							if (arDepths.size() == unContainerDepths)
								break;

							arDepths.pop();
						}

						if (!bMoved)
							break;

						if (L"table" == oReader.GetName())
						{
							if (bSetedDataOutput)
							{
								if (!arNestedCells.empty() && 0 != oTempData.GetCurSize())
								{
									arNestedCells.rbegin()->second->WriteString(L" ");
									arNestedCells.rbegin()->second->Write(oTempData);
									oTempData.Clear();
								}

								pWriter->RevertDataOutput();
								bSetedDataOutput = false;
							}

							m_oExternalData.GetSubClass(oReader, arNestedSelectors);
							ReadNestedCells(oReader, arNestedSelectors, oPosition, arNestedCells, oMatrix, pWriter, m_oExternalData, oIntermediateData);

							oPosition.m_unColumnIndex = oPosition.m_unStartColumnIndex;
							oPosition.m_unStartRowIndex = oPosition.m_unRowIndex;

							if (0 != oTempData.GetCurSize() && !arNestedCells.empty())
							{
								oTempData.WriteString(L" ");
								arNestedCells.begin()->second->WriteBefore(oTempData);
								oTempData.Clear();
							}

							if (0 != oIntermediateData.GetCurSize() && !arNestedCells.empty())
							{
								arNestedCells.rbegin()->second->WriteString(L" ");
								arNestedCells.rbegin()->second->Write(oIntermediateData);
								oIntermediateData.Clear();
							}

							while (arDepths.size() > unContainerDepths)
								arDepths.pop();

							for (int nCurrentDepth = nDepth + 1; nCurrentDepth < oReader.GetDepth(); ++nCurrentDepth)
								arDepths.push(nCurrentDepth);
						}
						else
						{
							if (!bSetedDataOutput)
							{
								pWriter->SetDataOutput(&oTempData);
								bSetedDataOutput = true;
							}

							m_oExternalData.ReadInside(oReader, arSelectors);
						}
					}

					if (bSetedDataOutput)
					{
						if (!arNestedCells.empty())
						{
							arNestedCells.rbegin()->second->WriteString(L" ");
							arNestedCells.rbegin()->second->Write(oTempData);
						}

						pWriter->RevertDataOutput();
					}
				}
				else
				{
					m_oExternalData.ReadStream(oReader, arSelectors);
					if (oMatrix[unRowIndex].size() - 1 != unColumnIndex)
						WriteCellSeparator(*pWriter);

					++unColumnIndex;
				}

				while (arDepths.size() > unDepthSize)
					arDepths.pop();

				arSelectors.resize(unCellSelectors - 1);
			}
			else
			{
				if (oMatrix[unRowIndex].size() - 1 != unColumnIndex)
					WriteCellSeparator(*pWriter);

				++unColumnIndex;
			}
		}

		WriteRowEnd(*pWriter);

		if (bIsHeader && 0 == unRowIndex)
		{
			WriteRowStart(*pWriter);

			for (size_t unColumnIndex = 0; unColumnIndex < m_oBody.GetColumnSize(); ++unColumnIndex)
			{
				pWriter->WriteString(L"-", true);

				if (unColumnIndex != m_oBody.GetColumnSize() - 1)
					WriteCellSeparator(*pWriter);
			}

			WriteRowEnd(*pWriter);
		}
	}

	arSelectors.resize(unBaseSelectors);

	return true;
}

Table CMarkdownTable::Flatten(Table&& srcTable)
{
	if (srcTable.empty())
		return {};

	const size_t unRows{srcTable.size()};

	size_t unColumns{0}, unRowColumns{0};

	for (const Row& oRow : srcTable)
	{
		for (ITableElementCell* pCell : oRow)
			unRowColumns += (nullptr != pCell) ? ((CTableElementCell*)pCell)->GetColspan() : 1;

		unColumns = (std::max)(unColumns, unRowColumns);

		unRowColumns = 0;
	}

	size_t unColspan{1};

	for (Row& oRow : srcTable)
	{
		for (size_t unColumnIndex = 0; unColumnIndex < oRow.size(); ++ unColumnIndex)
		{
			if (nullptr == oRow[unColumnIndex])
				continue;

			unColspan = oRow[unColumnIndex]->GetColspan();

			if (1 != unColspan)
				oRow.insert(oRow.begin() + unColumnIndex + 1, unColspan - 1, nullptr);

			unColumnIndex += unColspan - 1;
		}

		if (oRow.size() != unColumns)
			oRow.resize(unColumns);
	}

	std::vector<std::vector<TElementInfo>> oInfos{unRows, std::vector<TElementInfo>{unColumns}};

	for (size_t unRowIndex = 0; unRowIndex < unRows; ++unRowIndex)
		for (size_t unColumnIndex = 0; unColumnIndex < unColumns; ++unColumnIndex)
			oInfos[unRowIndex][unColumnIndex] = ComputeInfo(srcTable[unRowIndex][unColumnIndex]);

	std::vector<size_t> arRowHeights(unRows, 1);
	std::vector<size_t> arColumnWidths(unColumns, 1);

	for (size_t unRowIndex = 0; unRowIndex < unRows; ++unRowIndex)
	{
		for (size_t unColumnIndex = 0; unColumnIndex < unColumns; ++unColumnIndex)
		{
			arRowHeights[unRowIndex] = (std::max)(arRowHeights[unRowIndex], oInfos[unRowIndex][unColumnIndex].m_unRows);
			arColumnWidths[unColumnIndex] = (std::max)(arColumnWidths[unColumnIndex], oInfos[unRowIndex][unColumnIndex].m_unColumns);
		}
	}

	std::vector<size_t> arRowStart   (unRows    + 1, 0);
	std::vector<size_t> arColumnStart(unColumns + 1, 0);

	for (size_t unRowIndex = 0; unRowIndex < unRows; ++unRowIndex)
		arRowStart[unRowIndex + 1] = arRowStart[unRowIndex] + arRowHeights[unRowIndex];

	for (size_t unColumnIndex = 0; unColumnIndex < unColumns; ++unColumnIndex)
		arColumnStart[unColumnIndex + 1] = arColumnStart[unColumnIndex] + arColumnWidths[unColumnIndex];

	const size_t unTotalRows   {arRowStart[unRows]};
	const size_t unTotalColumns{arColumnStart[unColumns]};

	Table oResult{unTotalRows, Row{unTotalColumns, nullptr}};

	size_t unBaseRow, unBaseColumn;

	for (size_t unRowIndex = 0; unRowIndex < unRows; ++unRowIndex)
	{
		for (size_t unColumnIndex = 0; unColumnIndex < unColumns; ++unColumnIndex)
		{
			ITableElementCell*& pCell{srcTable[unRowIndex][unColumnIndex]};

			if (nullptr == pCell)
				continue;

			unBaseRow    = arRowStart[unRowIndex];
			unBaseColumn = arColumnStart[unColumnIndex];

			if (!IsTableContainer(pCell))
			{
				oResult[unBaseRow][unBaseColumn] = pCell;
				pCell = nullptr;
				continue;
			}


			CTableContainer* pContainer{dynamic_cast<CTableContainer*>(pCell)};
			std::vector<CTableElement*> arTables{pContainer->GetTables()};
			size_t unCurrentRowOffset{0};

			pCell = nullptr;

			for (CTableElement* pTable : arTables)
			{
				TElementInfo oSubInfo{ComputeInfo(pTable)};

				Table& oChild{dynamic_cast<CMarkdownTable*>(pTable)->m_oBody.GetMatrixCells()};
				Table oFlatChild{Flatten(std::move(oChild))};

				const size_t unChildRows{(std::min)(oSubInfo.m_unRows, oFlatChild.size())};

				for (size_t unChildRowIndex = 0; unChildRowIndex < unChildRows; ++unChildRowIndex)
				{
					const Row& arChildRow{oFlatChild[unChildRowIndex]};
					const size_t unChildColumns{(std::min)(oSubInfo.m_unColumns, arChildRow.size())};

					for (size_t unChildColumnIndex = 0; unChildColumnIndex < unChildColumns; ++unChildColumnIndex)
						oResult[unBaseRow + unCurrentRowOffset + unChildRowIndex][unBaseColumn + unChildColumnIndex] = arChildRow[unChildColumnIndex];
				}

				CTableElementCell* pFlatCell{dynamic_cast<CTableElementCell*>(oResult[unBaseRow + unCurrentRowOffset][unBaseColumn])};

				if(nullptr != pFlatCell)
					pFlatCell->IsFlatTable();

				unCurrentRowOffset += oSubInfo.m_unRows;
			}

			delete pContainer;
		}
	}

	return oResult;
}

TElementInfo CMarkdownTable::ComputeInfo(const ITableElementCell* pCell)
{
	if (IsTableContainer(pCell))
	{
		const std::vector<CTableElement*> arTables{((CTableContainer*)pCell)->GetTables()};

		TElementInfo oElementInfo, oTempInfo;

		for (CTableElement* pTable : arTables)
		{
			oTempInfo = ComputeInfo(pTable);

			oElementInfo.m_unColumns = std::max(oElementInfo.m_unColumns, oTempInfo.m_unColumns);
			oElementInfo.m_unRows += oTempInfo.m_unRows;
		}

		return oElementInfo;
	}

	return {1, 1};
}

TElementInfo CMarkdownTable::ComputeInfo(const CTableElement* pTable)
{
	if (nullptr == pTable)
		return {1, 1};

	const CMarkdownTable *pMarkdownTable{dynamic_cast<const CMarkdownTable*>(pTable)};

	if (nullptr == pMarkdownTable)
		return {1, 1};

	const Table& arCells{pMarkdownTable->m_oBody.GetMatrixCells()};

	const size_t unRows{arCells.size()};

	size_t unColumns{0}, unRowColumns{0};

	for (const Row& arRow : arCells)
	{
		for (const ITableElementCell* pCell : arRow)
			unRowColumns += (nullptr != pCell) ? pCell->GetColspan() : 1;

		unColumns = (std::max)(unColumns, unRowColumns);

		unRowColumns = 0;
	}

	if (0 == unRows || 0 == unColumns)
		return {1, 1};

	std::vector<size_t> arRowHeights(unRows, 1);
	std::vector<size_t> arColumnWidths(unColumns, 1);

	for (size_t unRowIndex = 0; unRowIndex < unRows; ++unRowIndex)
	{
		size_t unColumnIndex{0};

		for (const ITableElementCell* pCell : arCells[unRowIndex])
		{
			if (unColumnIndex >= unColumns)
				break;

			const TElementInfo oInfo{IsTableContainer(pCell) ? ComputeInfo(pCell) : TElementInfo{1, 1}};

			arRowHeights[unRowIndex] = (std::max)(arRowHeights[unRowIndex], oInfo.m_unRows);
			arColumnWidths[unColumnIndex] = (std::max)(arColumnWidths[unColumnIndex], oInfo.m_unColumns);

			unColumnIndex += (nullptr != pCell) ? pCell->GetColspan() : 1;
		}
	}

	size_t unTotalRows{0}, unTotalColumns{0};

	for (size_t unHeight : arRowHeights)
		unTotalRows += unHeight;

	for (size_t unWidth : arColumnWidths)
		unTotalColumns += unWidth;

	return {unTotalRows, unTotalColumns};
}
}
