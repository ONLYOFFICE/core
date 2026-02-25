#include "Table.h"

#include "Common.h"
#include "src/StringFinder.h"

namespace HTML
{
#define MAX_STRING_BLOCK_SIZE (size_t)10485760

#define RELEASE_VECTOR_PTR(vector_object, object_type) \
	for (object_type* pElement : vector_object) \
		RELEASEOBJECT(pElement)

#define FIRST_ELEMENT 0x00000001
#define LAST_ELEMENT  0x00000002
#define MID_ELEMENT   0x00000004

#define PARSE_MODE_HEADER  0x00000100
#define PARSE_MODE_BODY    0x00000200
#define PARSE_MODE_FOOTHER 0x00000400

#define COL_POSITION_MASK 0x0000000F
#define ROW_POSITION_MASK 0x000000F0
#define PARSE_MODE_MASK   0x00000F00

#define DEFAULT_PAGE_WIDTH  12240 // Значение в Twips
#define DEFAULT_PAGE_HEIGHT 15840 // Значение в Twips

TTableRowStyle::TTableRowStyle()
	: m_unMaxIndex(0), m_unMaxHeight(0), m_bIsHeader(false)
{}

bool TTableRowStyle::Empty() const
{
	return 0 == m_unMaxHeight && false == m_bIsHeader;
}

TTableCellStyle::TTableCellStyle()
{}

bool TTableCellStyle::Empty()
{
	return m_oWidth.Empty() && m_oHeight.Empty() && m_oBorder.Empty() && m_oPadding.Empty() && m_wsVAlign.empty() && m_wsVAlign.empty();
}

void TTableCellStyle::Copy(const TTableCellStyle* pTableCellStyle)
{
	if (NULL == pTableCellStyle)
		return;

	m_oWidth      = pTableCellStyle->m_oWidth;
	m_oHeight     = pTableCellStyle->m_oHeight;
	m_oBorder     = pTableCellStyle->m_oBorder;
	m_oPadding    = pTableCellStyle->m_oPadding;
	m_oBackground = pTableCellStyle->m_oBackground;

	m_wsHAlign    = pTableCellStyle->m_wsHAlign;
	m_wsVAlign    = pTableCellStyle->m_wsVAlign;
}

TTableCellStyle& TTableCellStyle::operator+=(const TTableCellStyle* pCellStyle)
{
	if (NULL == pCellStyle)
		return *this;

	m_oWidth += pCellStyle->m_oWidth;
	m_oHeight += pCellStyle->m_oHeight;
	m_oBorder += pCellStyle->m_oBorder;
	m_oPadding += pCellStyle->m_oPadding;
	m_oBackground += pCellStyle->m_oBackground;

	if (m_wsHAlign.empty())
		m_wsHAlign = pCellStyle->m_wsHAlign;

	if (m_wsVAlign.empty())
		m_wsVAlign = pCellStyle->m_wsVAlign;

	return *this;
}

CStorageTableCell::CStorageTableCell()
	: m_unColspan(1), m_unRowSpan(1), m_bIsMerged(false), m_bIsEmpty(false), m_oData(30)
{}

CStorageTableCell::CStorageTableCell(UINT unColspan, UINT unRowspan, bool bIsMerged, bool bIsEmpty)
	: m_unColspan(unColspan), m_unRowSpan(unRowspan), m_bIsMerged(bIsMerged),
	  m_bIsEmpty(bIsEmpty)
{}

CStorageTableCell::CStorageTableCell(CStorageTableCell& oCell)
	: m_unColspan(oCell.m_unColspan), m_unRowSpan(oCell.m_unRowSpan), m_bIsMerged(oCell.m_bIsMerged),
	  m_bIsEmpty(oCell.m_bIsEmpty), m_oStyles(oCell.m_oStyles)
{
	WriteToStringBuilder(oCell.m_oData, m_oData);
}

bool CStorageTableCell::Empty() const
{
	return m_bIsEmpty;
}

bool CStorageTableCell::Merged() const
{
	return m_bIsMerged;
}

CStorageTableCell* CStorageTableCell::Copy()
{
	return new CStorageTableCell(*this);
}

CStorageTableCell* CStorageTableCell::CreateEmpty(UINT unColspan, bool m_bIsMerged, const TTableCellStyle* pStyle)
{
	CStorageTableCell *pCell = new CStorageTableCell(unColspan, 1, m_bIsMerged, true);

	pCell->m_oStyles.Copy(pStyle);

	return pCell;
}

CStorageTableCell* CStorageTableCell::CreateEmpty(const TTableCellStyle* pStyle)
{
	CStorageTableCell *pCell = new CStorageTableCell(1, 1, false, true);

	pCell->m_oStyles.Copy(pStyle);

	return pCell;
}

void CStorageTableCell::SetColspan(UINT unColspan, UINT unCurrentIndex)
{
	if (MAXCOLUMNSINTABLE - 1 != unCurrentIndex)
		m_unColspan = std::min(MAXCOLUMNSINTABLE - 1 - unCurrentIndex, unColspan);
	else
		m_unColspan = 1;
}

UINT CStorageTableCell::GetColspan() const
{
	return m_unColspan;
}

void CStorageTableCell::SetRowspan(UINT unRowspan)
{
	m_unRowSpan = unRowspan;
}

UINT CStorageTableCell::GetRowspan() const
{
	return m_unRowSpan;
}

NSStringUtils::CStringBuilder* CStorageTableCell::GetData()
{
	return &m_oData;
}

const TTableCellStyle* CStorageTableCell::GetStyles() const
{
	return &m_oStyles;
}

TTableCellStyle* CStorageTableCell::GetStyles()
{
	return &m_oStyles;
}

void CStorageTableCell::SetWidth(const NSCSS::NSProperties::CDigit& oWidth)
{
	m_oStyles.m_oWidth = oWidth;
}

void CStorageTableCell::SetHeight(const NSCSS::NSProperties::CDigit& oHeight)
{
	m_oStyles.m_oHeight = oHeight;
}

UINT CStorageTableCell::GetWidth() const
{
	return m_oStyles.m_oWidth.ToInt(NSCSS::Twips, DEFAULT_PAGE_WIDTH);
}

UINT CStorageTableCell::GetHeight() const
{
	return m_oStyles.m_oHeight.ToInt(NSCSS::Twips, DEFAULT_PAGE_HEIGHT);
}

void CStorageTableCell::SetBorder(const NSCSS::NSProperties::CBorder& oBorder)
{
	m_oStyles.m_oBorder = oBorder;
}

void CStorageTableCell::ClearTopBorder()
{
	m_oStyles.m_oBorder.SetTopSide(L"none", 0, true);
}

void CStorageTableCell::ClearLeftBorder()
{
	m_oStyles.m_oBorder.SetLeftSide(L"none", 0, true);
}

void CStorageTableCell::ClearBottomBorder()
{
	m_oStyles.m_oBorder.SetBottomSide(L"none", 0, true);
}

void CStorageTableCell::ClearRightBorder()
{
	m_oStyles.m_oBorder.SetRightSide(L"none", 0, true);
}

void CStorageTableCell::SetPadding(const NSCSS::NSProperties::CIndent& oPadding)
{
	m_oStyles.m_oPadding = oPadding;
}

void CStorageTableCell::SetHAlign(const std::wstring& wsAlign)
{
	m_oStyles.m_wsHAlign = wsAlign;
}

void CStorageTableCell::SetVAlign(const std::wstring& wsAlign)
{
	m_oStyles.m_wsVAlign = wsAlign;
}

void CStorageTableCell::SetBackground(const NSCSS::NSProperties::CColor& oColor)
{
	m_oStyles.m_oBackground = oColor;
}

CStorageTableRow::CStorageTableRow()
{}

CStorageTableRow::~CStorageTableRow()
{
	for (CStorageTableCell* pCell : m_arCells)
		RELEASEOBJECT(pCell);
}

void CStorageTableRow::AddCell(CStorageTableCell* pCell)
{
	InsertCell(pCell, -1);
}

void CStorageTableRow::InsertCell(CStorageTableCell* pCell, int nPosition)
{
	if (NULL == pCell)
		return;

	if (nPosition < 0)
	{
		std::vector<CStorageTableCell*>::iterator itFoundEmpty = std::find_if(m_arCells.begin(), m_arCells.end(), [](CStorageTableCell* pCell) { return pCell->Empty() && !pCell->Merged(); });

		if (m_arCells.end() != itFoundEmpty)
		{
			--m_oStyles.m_unMaxIndex;
			delete *itFoundEmpty;
			*itFoundEmpty = pCell;

			if (1 != pCell->GetColspan())
			{
				++itFoundEmpty;
				UINT unColspan = pCell->GetColspan() - 1;

				while (m_arCells.end() != itFoundEmpty && (*itFoundEmpty)->Empty() && unColspan > 0)
				{
					--m_oStyles.m_unMaxIndex;
					--unColspan;
					delete (*itFoundEmpty);
					itFoundEmpty = m_arCells.erase(itFoundEmpty);
				}

				if (unColspan != 0)
					pCell->SetColspan(pCell->GetColspan() - unColspan, MAXCOLUMNSINTABLE);
			}
		}
		else
			m_arCells.push_back(pCell);
	}
	else if (nPosition >= m_arCells.size())
	{
		const UINT unMissingCount = nPosition - m_arCells.size();

		for (UINT unIndex = 0; unIndex < unMissingCount; ++unIndex)
			m_arCells.push_back(CStorageTableCell::CreateEmpty());

		m_oStyles.m_unMaxIndex += unMissingCount;

		m_arCells.push_back(pCell);
	}
	else if (m_arCells[nPosition]->Empty())
	{
		delete m_arCells[nPosition];
		--m_oStyles.m_unMaxIndex;
		m_arCells[nPosition] = pCell;

		if (1 != pCell->GetColspan())
		{
			++nPosition;
			UINT unDeleteCount =  pCell->GetColspan() - 1;
			while (nPosition < m_arCells.size() && m_arCells[nPosition]->Empty() && !m_arCells[nPosition]->Merged() && unDeleteCount > 0)
			{
				delete m_arCells[nPosition];
				--m_oStyles.m_unMaxIndex;
				m_arCells.erase(m_arCells.begin() + nPosition);
				--unDeleteCount;
			}

			if (0 != unDeleteCount)
				pCell->SetColspan(pCell->GetColspan() - unDeleteCount, MAXCOLUMNSINTABLE);
		}
	}
	else
		m_arCells.insert(m_arCells.begin() + nPosition, pCell);

	m_oStyles.m_unMaxIndex += pCell->GetColspan();

	if (1 == pCell->GetColspan() && 1 == pCell->GetRowspan())
		m_oStyles.m_unMaxHeight = std::max(m_oStyles.m_unMaxHeight, pCell->GetHeight());
}

UINT CStorageTableRow::GetIndex() const
{
	return m_oStyles.m_unMaxIndex;
}

UINT CStorageTableRow::GetCount() const
{
	return m_arCells.size();
}

CStorageTableCell* CStorageTableRow::operator[](UINT unIndex)
{
	if (unIndex >= m_arCells.size())
		return NULL;

	return m_arCells[unIndex];
}

bool CStorageTableRow::Empty() const
{
	return m_arCells.empty();
}

const TTableRowStyle& CStorageTableRow::GetStyles() const
{
	return m_oStyles;
}

const std::vector<CStorageTableCell*>& CStorageTableRow::GetCells() const
{
	return m_arCells;
}

CTableCol::CTableCol(UINT unSpan)
	: m_unSpan(unSpan)
{}

CTableCol::CTableCol(const NSCSS::CNode& oTableColNode)
	: m_unSpan(1)
{
	m_unSpan = NSStringFinder::ToInt(oTableColNode.GetAttributeValue(L"span"));
}

UINT CTableCol::GetSpan() const
{
	return m_unSpan;
}

TTableCellStyle* CTableCol::GetStyle()
{
	return &m_oStyle;
}

const TTableCellStyle* CTableCol::GetStyle() const
{
	return &m_oStyle;
}

CTableColgroup::CTableColgroup(NSCSS::CNode& oTableColgroupNode)
	: m_unWidth(0)
{
	m_unWidth = NSStringFinder::ToInt(oTableColgroupNode.GetAttributeValue(L"width"), 0);
}

CTableColgroup::~CTableColgroup()
{
	RELEASE_VECTOR_PTR(m_arCols, CTableCol)
}

bool CTableColgroup::Empty() const
{
	return m_arCols.empty();
}

void CTableColgroup::AddCol(CTableCol* pCol)
{
	if (NULL != pCol)
		m_arCols.push_back(pCol);
}

const std::vector<CTableCol*>& CTableColgroup::GetCols() const
{
	return m_arCols;
}

TTableStyles::TTableStyles()
	: m_nCellSpacing(-1), m_enRules(None)
{}

bool TTableStyles::Empty() const
{
	return m_oPadding.Empty() && m_oMargin.Empty() && m_oBorder.Empty() && m_oWidth.Empty() && -1 == m_nCellSpacing && m_wsAlign.empty();
}

CStorageTable::CStorageTable()
	: m_unMaxColumns(0)
{}

CStorageTable::~CStorageTable()
{
	for (std::vector<CStorageTableRow*>& arHeaders : m_arHeaders)
		RELEASE_VECTOR_PTR(arHeaders, CStorageTableRow)

	RELEASE_VECTOR_PTR(m_arFoother, CStorageTableRow)
	RELEASE_VECTOR_PTR(m_arRows, CStorageTableRow)
	RELEASE_VECTOR_PTR(m_arColgroups, CTableColgroup)
}

CStorageTableRow* CStorageTable::operator[](UINT unIndex)
{
	if (unIndex < m_arRows.size())
		return m_arRows[unIndex];

	return NULL;
}

bool CStorageTable::Empty() const
{
	return m_arHeaders.empty() && m_arRows.empty() && m_arFoother.empty();
}

bool CStorageTable::HaveCaption()
{
	return 0 != m_oCaption.GetCurSize();
}

bool CStorageTable::HaveColgroups() const
{
	return !m_arColgroups.empty();
}

bool CStorageTable::HaveHeader() const
{
	return !m_arHeaders.empty();
}

UINT CStorageTable::GetRowCount() const
{
	return m_arRows.size();
}

const TTableStyles& CStorageTable::GetTableStyles() const
{
	return m_oStyles;
}

const TTableCellStyle* CStorageTable::GetColStyle(UINT unColumnNumber) const
{
	if (m_arColgroups.empty())
		return NULL;

	UINT unCurrentNumber = 0;

	for (const CTableColgroup* pColgroup : m_arColgroups)
	{
		for (const CTableCol* pCol : pColgroup->GetCols())
		{
			unCurrentNumber += pCol->GetSpan();

			if (unCurrentNumber >= unColumnNumber)
				return pCol->GetStyle();
		}
	}

	return NULL;
}

void CStorageTable::AddRows(std::vector<CStorageTableRow*>& arRows, ERowParseMode eParseMode)
{
	if (arRows.empty())
		return;

	if (ERowParseMode::Foother == eParseMode && !m_arFoother.empty())
		eParseMode = ERowParseMode::Header;

	if (ERowParseMode::Header == eParseMode)
		m_arHeaders.push_back({});

	for (CStorageTableRow* pRow : arRows)
		AddRow(pRow, eParseMode);
}

void CStorageTable::AddRow(CStorageTableRow* pRow, ERowParseMode eParseMode)
{
	if (NULL == pRow)
		return;

	for (UINT unIndex = 0; unIndex < pRow->GetCount(); ++unIndex)
	{
		if (unIndex >= m_arMinColspan.size())
			m_arMinColspan.push_back((*pRow)[unIndex]->GetColspan());
		else if ((*pRow)[unIndex]->GetColspan() < m_arMinColspan[unIndex])
			m_arMinColspan[unIndex] = (*pRow)[unIndex]->GetColspan();
	}

	switch (eParseMode)
	{
		default:
		case ERowParseMode::Body:
		{
			m_arRows.push_back(pRow);
			break;
		}
		case ERowParseMode::Header:
		{
			if (m_arHeaders.empty())
				m_arHeaders.push_back({});

			m_arHeaders.back().push_back(pRow);
			break;
		}
		case ERowParseMode::Foother:
		{
			m_arFoother.push_back(pRow);
			break;
		}
	}
}

NSStringUtils::CStringBuilder* CStorageTable::GetCaptionData()
{
	return &m_oCaption;
}

void CStorageTable::SetPadding(const NSCSS::NSProperties::CIndent& oPadding)
{
	m_oStyles.m_oPadding = oPadding;
}

const NSCSS::NSProperties::CIndent& CStorageTable::GetPadding() const
{
	return m_oStyles.m_oPadding;
}

void CStorageTable::SetMargin(const NSCSS::NSProperties::CIndent& oMargin)
{
	m_oStyles.m_oMargin = oMargin;
}

void CStorageTable::SetBorder(const NSCSS::NSProperties::CBorder& oBorder)
{
	m_oStyles.m_oBorder = oBorder;
}

void CStorageTable::SetWidth(const NSCSS::NSProperties::CDigit& oWidth)
{
	m_oStyles.m_oWidth = oWidth;
}

void CStorageTable::SetCellSpacing(int nCellSpacing)
{
	m_oStyles.m_nCellSpacing = nCellSpacing;
}

void CStorageTable::SetAlign(const std::wstring& wsValue)
{
	m_oStyles.m_wsAlign = wsValue;
}

void CStorageTable::SetRules(const std::wstring& wsValue)
{
	if (wsValue.empty())
		return;

	if (NSStringFinder::Equals(wsValue, L"all"))
		m_oStyles.m_enRules = TTableStyles::ETableRules::All;
	else if (NSStringFinder::Equals(wsValue, L"groups"))
		m_oStyles.m_enRules = TTableStyles::ETableRules::Groups;
	else if (NSStringFinder::Equals(wsValue, L"cols"))
		m_oStyles.m_enRules = TTableStyles::ETableRules::Cols;
	else if (NSStringFinder::Equals(wsValue, L"none"))
		m_oStyles.m_enRules = TTableStyles::ETableRules::None;
	else if (NSStringFinder::Equals(wsValue, L"rows"))
		m_oStyles.m_enRules = TTableStyles::ETableRules::Rows;
}

void CStorageTable::AddColgroup(CTableColgroup* pElement)
{
	if (NULL != pElement)
		m_arColgroups.push_back(pElement);
}

void CStorageTable::RecalculateMaxColumns()
{
	for (const std::vector<CStorageTableRow*>& arHeaders : m_arHeaders)
		for (const CStorageTableRow* pHeader : arHeaders)
			m_unMaxColumns = std::max(m_unMaxColumns, pHeader->GetIndex());

	for (const CStorageTableRow* pRow : m_arRows)
		m_unMaxColumns = std::max(m_unMaxColumns, pRow->GetIndex());

	for (const CStorageTableRow* pFoother : m_arFoother)
		m_unMaxColumns = std::max(m_unMaxColumns, pFoother->GetIndex());
}

void CStorageTable::Shorten()
{
	UINT unIndex      = 0;
	CStorageTableCell* pCell = NULL;

	UINT unMaxIndex = 0; //Максимальный индекс без учета строк, где имеется только 1 ячейка

	for (const CStorageTableRow* pRow : m_arRows)
	{
		if (1 < pRow->GetCount())
			unMaxIndex = std::max(unMaxIndex, pRow->GetIndex());
	}

	while (unIndex < m_arMinColspan.size())
	{
		for (CStorageTableRow* pRow : m_arRows)
		{
			if (0 != unMaxIndex && 1 == pRow->GetCount() && pRow->GetIndex() > unMaxIndex)
			{
				pCell = (*pRow)[unIndex];

				if (NULL == pCell)
					continue;

				pCell->SetColspan(unMaxIndex, MAXCOLUMNSINTABLE);
				continue;
			}

			if (1 == m_arMinColspan[unIndex])
				break;

			pCell = (*pRow)[unIndex];

			if (NULL == pCell)
				continue;

			if (1 < pCell->GetColspan() && pCell->GetColspan() > m_arMinColspan[unIndex])
			{
				pCell->SetColspan(m_arMinColspan[unIndex], MAXCOLUMNSINTABLE);
				continue;
			}

			if ((*pRow)[unIndex]->GetColspan() == m_arMinColspan[unIndex] + 1)
				(*pRow)[unIndex]->SetColspan(2, MAXCOLUMNSINTABLE);
			else if ((*pRow)[unIndex]->GetColspan() > m_arMinColspan[unIndex])
				(*pRow)[unIndex]->SetColspan((*pRow)[unIndex]->GetColspan() - m_arMinColspan[unIndex], MAXCOLUMNSINTABLE);
		}

		++unIndex;
	}
}

void CStorageTable::CompleteTable()
{
	UINT unMaxIndex = 0;

	for (CStorageTableRow* pRow : m_arRows)
		unMaxIndex = std::max(unMaxIndex, pRow->GetIndex());

	for (CStorageTableRow* pRow : m_arRows)
	{
		if (NULL == pRow || 0 == pRow->GetCount())
			continue;

		for (UINT unIndex = pRow->GetIndex(); unIndex < unMaxIndex; ++unIndex)
			pRow->InsertCell(CStorageTableCell::CreateEmpty(), unIndex);
	}

	RecalculateMaxColumns();
}

const std::vector<std::vector<CStorageTableRow*>>& CStorageTable::GetHeaders() const
{
	return m_arHeaders;
}

const std::vector<CStorageTableRow*>& CStorageTable::GetFoothers() const
{
	return m_arFoother;
}

const std::vector<CStorageTableRow*>& CStorageTable::GetRows() const
{
	return m_arRows;
}

const std::vector<CTableColgroup*> CStorageTable::GetColgroups() const
{
	return m_arColgroups;
}

UINT CStorageTable::GetMaxColumns() const
{
	return m_unMaxColumns;
}

}
