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

#ifndef OOXMLWRITER_H
#define OOXMLWRITER_H

#include "../../Common/3dParty/html/css/src/xhtml/CDocumentStyle.h"
#include "../../Common/3dParty/html/css/src/CCssCalculator.h"
#include "../HTMLParameters.h"
#include "IWriter.h"

#include <stack>

namespace HTML
{
struct TImageData
{
	UINT m_unWidth;
	UINT m_unHeight;

	int m_nHSpace;
	int m_nVSpace;

	std::wstring m_wsAlign;

	TImageData()
		: m_unWidth(0), m_unHeight(0), m_nHSpace(0), m_nVSpace(0), m_wsAlign(L"left")
	{}

	bool ZeroSize() const
	{
		return 0 == m_unWidth || 0 == m_unHeight;
	}

	bool ZeroSpaces() const
	{
		return 0 == m_nHSpace && 0 == m_nVSpace;
	}
};

class COOXMLWriter : public IWriter
{
	const std::wstring *m_pDstPath;  // Destination directory

	XmlString m_oStylesXml;   // styles.xml
	XmlString m_oDocXmlRels;  // document.xml.rels
	XmlString m_oNoteXmlRels; // footnotes.xml.rels
	XmlString m_oDocXml;      // document.xml
	XmlString m_oNoteXml;     // footnotes.xml
	XmlString m_oNumberXml;   // numbering.xml
	XmlString m_oWebSettings; // webSettings.xml

	struct TCustomProperties
	{
		XmlString m_oXml;     // custom.xml
		UINT m_unIds;
	} m_oCustom;

	NSCSS::CDocumentStyle m_oXmlStyle;      // OOXML style
	NSCSS::NSProperties::CPage m_oPageData; // Page styles

	NSCSS::CCssCalculator *m_pStylesCalculator;
	THTMLParameters *m_pHTMLParameters;

	struct TState
	{
		bool m_bInP;         // <w:p> open?
		bool m_bInR;         // <w:r> open?
		bool m_bInT;         // <w:t> open?
		bool m_bWasPStyle;   // <w:pStyle> written?
		bool m_bWasSpace;    // Was space?

		bool m_bInHyperlink; // <w:hyperlink> open?
		std::wstring m_wsTooltip;
		std::wstring m_wsHref;
		std::wstring m_wsFootnote;
		bool m_bIsFootnote;
		bool m_bISCrossHyperlink;

		XmlString *m_pCurrentDocument; //Current write location
		bool m_bRemoveCurrentDocument;

		TState(XmlString *pCurrentDocument)
			: m_bInP(false), m_bInR(false), m_bInT(false),
			  m_bWasPStyle(false), m_bWasSpace(true), m_bInHyperlink(false),
			  m_bIsFootnote(false), m_bISCrossHyperlink(false),
			  m_pCurrentDocument(pCurrentDocument), m_bRemoveCurrentDocument(false)
		{}

		~TState()
		{
			if (m_bRemoveCurrentDocument && nullptr != m_pCurrentDocument)
				delete m_pCurrentDocument;
		}

		void CreateNewCurrentDocument()
		{
			m_pCurrentDocument = new XmlString();
			m_bRemoveCurrentDocument = true;
		}
	};

	std::stack<TState> m_arStates;

	int m_nFootnoteId;  // Footnote ID
	int m_nHyperlinkId; // Hyperlink ID
	int m_nListId;      // List ID
	int m_nElementId;   // ID for other elements

	bool m_bBanUpdatePageData; // Prohibit page data update?

	std::stack<std::wstring> m_arDivId;
	bool m_bWasDivs;

	std::map<std::wstring, std::wstring> m_mFootnotes; // Footnotes
	std::map<std::wstring, UINT>         m_mBookmarks; // Bookmarks
	using anchors_map = std::map<std::wstring, std::wstring>;
	anchors_map                          m_mAnchors; // Map of anchors with individual ids
public:
	COOXMLWriter(THTMLParameters* pHTMLParameters = nullptr, NSCSS::CCssCalculator* pCSSCalculator = nullptr);

	void SetDstDirectory (const std::wstring& wsPath);

	void Begin(const std::wstring& wsDst) override;
	void End(const std::wstring& wsDst) override;

	bool OpenP();
	bool OpenR();
	bool OpenT();
	void OpenHyperlink();

	void CloseP();
	void CloseR();
	void CloseT();
	void CloseHyperlink();

	void PageBreak() override;

	void BeginBlock() override;
	void EndBlock(bool bAddBlock) override;

	void SetDataOutput(XmlString* pOutputData) override;
	void RevertDataOutput() override;

	void SaveState();
	void RollBackState();

	void SetCurrentDocument(XmlString* pNewDocument);

	void Break(const NSCSS::CNode& oTagNode);

	void SetHyperlinkData(const std::wstring& wsRef, const std::wstring& wsTooltip, bool bIsCross, const std::wstring& wsFootnote, bool bIsFootnote);
	void ClearHyperlinkData();

	void OpenCrossHyperlink(const std::wstring& wsRef);
	void OpenExternalHyperlink(const std::wstring& wsRef, const std::wstring& wsTooltip);
	void CloseCrossHyperlink(const std::vector<NSCSS::CNode>& arSelectors, std::wstring wsFootnote, const std::wstring& wsRef);
	void CloseExternalHyperlink();

	std::wstring WritePPr(const std::vector<NSCSS::CNode>& arSelectors);
	std::wstring WriteRPr(XmlString& oXml, const std::vector<NSCSS::CNode>& arSelectors);

	bool WriteText(std::wstring wsText, const std::vector<NSCSS::CNode>& arSelectors) override;
	void WriteEmptyParagraph(bool bVahish = false, bool bInP = false) override;
	void WriteMetadata(const std::wstring& wsName, const std::wstring& wsValue) override;
	void WriteSpace();

	void WriteEmptyBookmark(const std::wstring& wsId);
	std::wstring AddLiteBookmark();
	std::wstring WriteBookmark(const std::wstring& wsId);
	std::wstring AddAnchor(const std::wstring& wsAnchorValue);

	void WriteImage(const TImageData& oImageData, const std::wstring& wsId);
	void WriteAlternativeImage(const std::wstring& wsAlt, const std::wstring& wsSrc, const TImageData& oImageData);
	void WriteEmptyImage(int nWidth, int nHeight, const std::wstring& wsName = L"", const std::wstring& wsDescr = L"");
	void WriteImageRels(const std::wstring& wsImageId, const std::wstring& wsImageName);

	std::wstring GetStyle(const NSCSS::CCompiledStyle& oStyle, bool bParagraphStyle);

	void UpdatePageStyle(const std::vector<NSCSS::CNode>& arSelectors);
	void SetBaseFont(const std::wstring& wsFontStyles);
	void SetDivId(const std::wstring& wsDivId);
	void RollBackDivId();

	void IncreaseListId();
	int GetListId() const;

	std::wstring FindFootnote(const std::wstring& wsId);
	void OpenFootnote(const std::wstring& wsFootnoteID);
	void CloseFootnote();

	XmlString& GetStylesXml();
	XmlString& GetDocRelsXml();
	XmlString& GetNotesRelsXml();
	XmlString& GetDocumentXml();
	XmlString& GetNotesXml();
	XmlString& GetNumberingXml();
	XmlString& GetWebSettingsXml();
	XmlString* GetCurrentDocument() const override;

	const NSCSS::NSProperties::CPage* GetPageData() const;

	std::wstring GetMediaDir() const;
};
}

#endif // OOXMLWRITER_H
