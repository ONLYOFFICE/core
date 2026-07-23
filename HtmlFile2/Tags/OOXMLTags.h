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

#ifndef OOXMLTAGS_H
#define OOXMLTAGS_H

#include "HTMLTags.h"
#include "../Writers/OOXMLWriter.h"
#include "../Table.h"

#include <unordered_map>

namespace HTML
{
#define CREATE_OOXML_TAG(class_name) CREATE_TAG(class_name##OOXML, COOXMLWriter)

CREATE_OOXML_TAG(Anchor);
CREATE_OOXML_TAG(Abbr);
CREATE_OOXML_TAG(Break);
CREATE_OOXML_TAG(Font);
CREATE_OOXML_TAG(Input);
CREATE_OOXML_TAG(BaseFont);
CREATE_OOXML_TAG(List);
CREATE_OOXML_TAG(ListElement);
CREATE_OOXML_TAG(HTML);

class CDivisionOOXMLTag : public IHTMLTag<COOXMLWriter>
{
	std::stack<UINT> m_arFootnoteIDs;
public:
	CDivisionOOXMLTag(COOXMLWriter *pWriter);

	bool Open (const std::vector<NSCSS::CNode>& arSelectors) override;
	void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

class CImageOOXMLTag : public IHTMLTag<COOXMLWriter>
{
	std::vector<std::wstring> m_arImages;
public:
	CImageOOXMLTag(COOXMLWriter *pWriter);

	bool Open (const std::vector<NSCSS::CNode>& arSelectors) override;
	void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

class CBlockquoteOOXMLTag : public IHTMLTag<COOXMLWriter>
{
	std::map<std::wstring, UINT> m_mDivs;
public:
	CBlockquoteOOXMLTag(COOXMLWriter *pWriter);

	bool Open (const std::vector<NSCSS::CNode>& arSelectors) override;
	void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

class CHorizontalRuleOOXMLTag : public IHTMLTag<COOXMLWriter>
{
	UINT m_unShapeId;
public:
	CHorizontalRuleOOXMLTag(COOXMLWriter *pWriter);

	bool Open (const std::vector<NSCSS::CNode>& arSelectors) override;
	void Close(const std::vector<NSCSS::CNode>& arSelectors) override;
};

void InitTagsForOOXML(std::map<int, std::shared_ptr<ITag>>& mTags, COOXMLWriter* pWriter);

enum class ETableRules
{
	All,
	Groups,
	Cols,
	None,
	Rows
};

class COOXMLTable : public CTableElement
{
public:
	COOXMLTable(TExternalTableData &oExternalData);
	virtual ~COOXMLTable();

	bool PreParse(XmlUtils::CXmlLiteReader& oReader) override;
	void Normalize() override;
	bool Convert(XmlUtils::CXmlLiteReader& oReader, const NSCSS::CNode& oTableNode) override;
private:
	bool ParseCaption(XmlUtils::CXmlLiteReader& oReader, XmlString*& pCaption) override;
	bool ParseColgroup(XmlUtils::CXmlLiteReader& oReader, std::vector<CTableColgroup*>& arColgroups) override;

	bool HaveColgroups() const;
	size_t GetColumnsCount() const;

	struct TTableStyles
	{
		size_t m_unCellSpacing;
		ETableRules m_enRules;

		const COOXMLTable *m_pTable;

		NSCSS::NSProperties::CBorder m_oBorder;
		NSCSS::NSProperties::CIndent m_oPadding;

		TTableStyles()
			: m_unCellSpacing{0}, m_enRules{ETableRules::All}, m_pTable{nullptr}
		{}
	};

	enum class ERowParseMode
	{
		Header,
		Body,
		Foother
	};

	enum class ERowPosition
	{
		First,
		Middle,
		Last
	};

	void OpenTable(XmlString& oXmlString, const NSCSS::CNode& oTableNode, TTableStyles& oTableStyles, const COOXMLTable& oTable);
	void CloseTable(XmlString& oXmlString);

	void OpenRow(XmlString& oXmlString, bool bIsHeader, size_t unMaxHeight, size_t unCellSpacing);
	void CloseRow(XmlString& oXmlString);

	void OpenCell(XmlString& oXmlString);
	void OpenCell(XmlString& oXmlString, const NSCSS::CNode& oCellNode, ITableElementCell* pCell, size_t unColumnIndex, ERowParseMode eRowParseMode, ERowPosition eRowPosition, size_t& unHeight, const TTableStyles& oTableStyles, std::unordered_map<size_t, NSCSS::CNode>& mFillingColumn);
	void CloseCell(XmlString& oXmlString);

	void ConvertTable(XmlUtils::CXmlLiteReader& oReader, COOXMLWriter& oWriter, const COOXMLTable& oTable, const NSCSS::CNode& oTableNode);
	void ConvertMatrix(XmlUtils::CXmlLiteReader& oReader, COOXMLWriter& oWriter, std::vector<NSCSS::CNode>& arSelectors, std::stack<int>& arDepths, const Table& oMatrix, TTableStyles& oTableStyles, ERowParseMode eRowParseMode);
};
}

#endif // OOXMLTAGS_H
