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

#ifndef MDTAGS_H
#define MDTAGS_H

#include "HTMLTags.h"
#include "../Writers/MDWriter.h"
#include "../Table.h"

#include <map>

namespace HTML
{
#define CREATE_MD_TAG(class_name) CREATE_TAG(class_name##MD, CMDWriter)

CREATE_MD_TAG(Anchor);
CREATE_MD_TAG(Break);
CREATE_MD_TAG(Preformatted);
CREATE_MD_TAG(Header);
CREATE_MD_TAG(Image);
CREATE_MD_TAG(HorizontalRule);
CREATE_MD_TAG(Blockquote);
CREATE_MD_TAG(List);
CREATE_MD_TAG(ListElement);
CREATE_MD_TAG(Code);

void InitTagsForMD(std::map<int, std::shared_ptr<ITag>>& mTags, CMDWriter* pWriter);

struct TElementInfo
{
	size_t m_unRows;
	size_t m_unColumns;

	TElementInfo()
		: m_unRows{0}, m_unColumns{0}
	{}

	TElementInfo(const size_t& unRows, const size_t& m_unColumns)
		: m_unRows{unRows}, m_unColumns{m_unColumns}
	{}
};

class CMarkdownTable : public CTableElement
{
public:
	CMarkdownTable(TExternalTableData &oExternalData);
	virtual ~CMarkdownTable();

	bool PreParse(XmlUtils::CXmlLiteReader& oReader) override;
	void Normalize() override;
	bool Convert(XmlUtils::CXmlLiteReader& oReader, const NSCSS::CNode& oTableNode) override;
private:
	bool ParseCaption(XmlUtils::CXmlLiteReader& oReader, XmlString*& pCaption) override;
	bool ParseColgroup(XmlUtils::CXmlLiteReader& oReader, std::vector<CTableColgroup*>& arColgroups) override;

	bool ConvertMatrix(XmlUtils::CXmlLiteReader& oReader, std::vector<NSCSS::CNode>& arSelectors, const Table& oMatrix, CMDWriter* pWriter, bool bIsHeader = false);

	static Table Flatten(Table&& srcTable);
	static TElementInfo ComputeInfo(const ITableElementCell* pCell);
	static TElementInfo ComputeInfo(const CTableElement* pTable);
};
}

#endif // MDTAGS_H
