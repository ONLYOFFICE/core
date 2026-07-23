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

#include "Table.h"

#include "Common.h"
#include "src/CCompiledStyle.h"

namespace HTML
{
ITableElementCell::ITableElementCell(size_t unColspan)
	: m_unColspan(unColspan)
{}

void ITableElementCell::SetColspan(size_t unColspan)
{
	if (0 < unColspan)
		m_unColspan = unColspan;
}

size_t ITableElementCell::GetColspan() const
{
	return m_unColspan;
}

CTableElementCell::CTableElementCell(bool bIsFilling, size_t unColspan)
	: ITableElementCell(unColspan), m_eType{(bIsFilling) ? ETableElement::FillingCell : ETableElement::Cell}
{}

CTableElementCell::~CTableElementCell()
{}

ETableElement CTableElementCell::GetType() const
{
	return m_eType;
}

void CTableElementCell::IsFlatTable()
{
	m_eType = ETableElement::FlatTable;
}

CTableElementCell* CTableElementCell::CreateCell(const size_t& unColspan)
{
	return new CTableElementCell(false, unColspan);
}

CTableElementCell* CTableElementCell::CreateFillingCell(const size_t& unColspan)
{
	return new CTableElementCell(true, unColspan);
}

CTableMatrix::CTableMatrix()
{}

CTableMatrix::~CTableMatrix()
{
	for (Row& oRow : m_arCells)
	{
		for (ITableElementCell* pCell : oRow)
		{
			if (nullptr != pCell)
				delete pCell;
		}
	}
}

bool CTableMatrix::Empty() const
{
	return m_arCells.empty();
}

void CTableMatrix::Clear()
{
	m_arCells.clear();
}

bool CTableMatrix::SetCell(size_t unRowIndex, size_t unColumnIndex, ITableElementCell* pCell, bool bAutoClean)
{
	if (nullptr == pCell)
		return false;

	if (m_arCells.size() <= unRowIndex)
	{
		if (!m_arCells.empty())
		{
			const size_t unMaxColumn{m_arCells.back().size()};
			const size_t unOldSize{m_arCells.size()};
			m_arCells.resize(unRowIndex + 1);

			for (size_t unIndex = unOldSize; unIndex < m_arCells.size(); ++unIndex)
				m_arCells[unIndex].resize(unMaxColumn);
		}
		else
			m_arCells.resize(unRowIndex + 1);
	}

	if (m_arCells[unRowIndex].size() <= unColumnIndex)
	{
		m_arCells[unRowIndex].resize(unColumnIndex + 1);
		m_arCells[unRowIndex][unColumnIndex] = pCell;
		return true;
	}

	if (nullptr != m_arCells[unRowIndex][unColumnIndex])
	{
		pCell->SetColspan(m_arCells[unRowIndex][unColumnIndex]->GetColspan());

		if (bAutoClean)
			delete m_arCells[unRowIndex][unColumnIndex];
	}

	m_arCells[unRowIndex][unColumnIndex] = pCell;

	return true;
}

void CTableMatrix::NormalizeNumberColumns(size_t unNumberColumns)
{
	if (m_arCells.empty())
		return;

	for (Row& oRow : m_arCells)
		if (unNumberColumns > oRow.size())
			oRow.resize(unNumberColumns);
}

bool CTableMatrix::IsFillingCell(size_t unRowIndex, size_t unColumnIndex) const
{
	if (unRowIndex >= m_arCells.size())
		return false;

	if (unColumnIndex >= m_arCells[unRowIndex].size())
		return false;

	return (nullptr != m_arCells[unRowIndex][unColumnIndex]) ? ETableElement::FillingCell == m_arCells[unRowIndex][unColumnIndex]->GetType() : false;
}

bool CTableMatrix::IsNotNullCell(size_t unRowIndex, size_t unColumnIndex) const
{
	if (unRowIndex >= m_arCells.size())
		return false;

	if (unColumnIndex >= m_arCells[unRowIndex].size())
		return false;

	return nullptr != m_arCells[unRowIndex][unColumnIndex];
}

size_t CTableMatrix::GetRowSize() const
{
	return m_arCells.size();
}

size_t CTableMatrix::GetColumnSize() const
{
	return (m_arCells.empty()) ? 0 : m_arCells.front().size();
}

Table& CTableMatrix::GetMatrixCells()
{
	return m_arCells;
}

const Table& CTableMatrix::GetMatrixCells() const
{
	return m_arCells;
}

const ITableElementCell* CTableMatrix::GetCell(size_t unRowIndex, size_t unColumnIndex) const
{
	if (unRowIndex >= m_arCells.size())
		return nullptr;

	if (unColumnIndex >= m_arCells[unRowIndex].size())
		return nullptr;

	return m_arCells[unRowIndex][unColumnIndex];
}

ITableElementCell* CTableMatrix::GetCell(size_t unRowIndex, size_t unColumnIndex)
{
	if (unRowIndex >= m_arCells.size())
		return nullptr;

	if (unColumnIndex >= m_arCells[unRowIndex].size())
		return nullptr;

	return m_arCells[unRowIndex][unColumnIndex];
}

CTableElement::CTableElement(TExternalTableData &oExternalData)
	: m_pCaption(nullptr), m_oExternalData(oExternalData)
{}

CTableElement::~CTableElement()
{
	Clear();
}

void CTableElement::Clear()
{
	if (nullptr != m_pCaption)
	{
		delete m_pCaption;
		m_pCaption = nullptr;
	}

	for (CTableColgroup* pColgroup : m_arColgroups)
		if (nullptr != pColgroup)
			delete pColgroup;

	m_arColgroups.clear();
	m_oHeader.Clear();
	m_oBody.Clear();
	m_oFoother.Clear();
}

bool CTableElement::Empty() const
{
	return m_oHeader.Empty() && m_oBody.Empty() && m_oFoother.Empty();
}

bool CTableElement::HaveCaption() const
{
	return nullptr != m_pCaption && 0 != m_pCaption->GetCurSize();
}

const NSCSS::CCompiledStyle* CTableElement::GetColStyle(size_t unColIndex) const
{
	if (m_arColgroups.empty())
		return nullptr;

	const NSCSS::CCompiledStyle* pColStyle{nullptr};
	size_t unSkipSpans{0};

	for (const CTableColgroup* pColgroup : m_arColgroups)
	{
		pColStyle = pColgroup->GetColStyle(unColIndex - unSkipSpans);

		if (nullptr != pColStyle)
			return pColStyle;

		unSkipSpans += pColgroup->GetTotalSpans();
	}

	return nullptr;
}

bool MoveToNextTableCell(XmlUtils::CXmlLiteReader& oReader, std::vector<NSCSS::CNode>& arSelectors, std::stack<int>& arDepths, TExternalTableData::FuncGetSubClass& GetSubClass)
{
	while (!arDepths.empty() && !oReader.ReadNextSiblingNode2(arDepths.top()))
	{
		arDepths.pop();
		arSelectors.pop_back();
	}

	if (arDepths.empty())
		return false;

	do
	{
		const std::wstring wsName = oReader.GetName();

		if (L"#text" == wsName || L"colgroup" == wsName || L"caption" == wsName)
			return MoveToNextTableCell(oReader, arSelectors, arDepths, GetSubClass);

		GetSubClass(oReader, arSelectors);

		if (L"td" == wsName || L"th" == wsName)
			return true;
		else if (L"table" == wsName)
			return false;

		arDepths.push(oReader.GetDepth());

		return MoveToNextTableCell(oReader, arSelectors, arDepths, GetSubClass);
	}while(oReader.ReadNextSiblingNode2(arDepths.top()));

	return false;
}

CTableContainer::CTableContainer()
	: ITableElementCell(1)
{}

CTableContainer::~CTableContainer()
{
	for (const CTableElement* pTable : m_arTables)
		if (nullptr != pTable)
			delete pTable;
}

ETableElement CTableContainer::GetType() const
{
	return ETableElement::TableContainer;
}

void CTableContainer::AddTable(CTableElement* pTable)
{
	if (nullptr != pTable)
		m_arTables.push_back(pTable);
}

const std::vector<CTableElement*>& CTableContainer::GetTables() const
{
	return m_arTables;
}

CTableCol::CTableCol(NSCSS::CNode& oTableColNode)
	: m_pStyle(oTableColNode.m_pCompiledStyle)
{
	oTableColNode.m_pCompiledStyle = nullptr;
	m_unSpan = NSStringFinder::ToInt(oTableColNode.GetAttributeValue(L"span"), 1);
}

CTableCol::~CTableCol()
{
	if (nullptr != m_pStyle)
		delete m_pStyle;
}

UINT CTableCol::GetSpan() const
{
	return m_unSpan;
}

const NSCSS::CCompiledStyle* CTableCol::GetStyle() const
{
	return m_pStyle;
}

CTableColgroup::CTableColgroup(const NSCSS::CNode& oTableColgroupNode)
	: m_unTotalSpans(0)
{
	m_unWidth = NSStringFinder::ToInt(oTableColgroupNode.GetAttributeValue(L"width"), 0);
}

CTableColgroup::~CTableColgroup()
{
	for (CTableCol* pCol : m_arCols)
		if (nullptr != pCol)
			delete pCol;
}

bool CTableColgroup::Empty() const
{
	return m_arCols.empty();
}

void CTableColgroup::AddCol(CTableCol* pCol)
{
	if (nullptr == pCol)
		return;

	m_arCols.push_back(pCol);
	m_unTotalSpans += pCol->GetSpan();
}

UINT CTableColgroup::GetTotalSpans() const
{
	return m_unTotalSpans;
}

const std::vector<CTableCol*>& CTableColgroup::GetCols() const
{
	return m_arCols;
}

const NSCSS::CCompiledStyle* CTableColgroup::GetColStyle(size_t unIndex) const
{
	if (m_arCols.empty())
		return nullptr;

	size_t unCurrentIndex{0};
	for (const CTableCol* pCol : m_arCols)
	{
		if (unCurrentIndex <= unIndex && (unCurrentIndex + pCol->GetSpan()) > unIndex)
			return pCol->GetStyle();

		unCurrentIndex += pCol->GetSpan();
	}

	return nullptr;
}
}
