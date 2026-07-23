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

#ifndef TABLE_H
#define TABLE_H

#include "../DesktopEditor/xml/include/xmlutils.h"

#include "Writers/IWriter.h"
#include "src/StringFinder.h"

#include <stack>
#include <vector>

namespace HTML
{
#define MAXCOLUMNSINTABLE 63
#define MAXROWSINTABLE    32767

struct TCurentTablePosition
{
	size_t m_unRowIndex;
	size_t m_unColumnIndex;

	size_t m_unStartRowIndex;
	size_t m_unStartColumnIndex;

	TCurentTablePosition(size_t unRowIndex, size_t unColumnIndex, size_t unStartRowIndex, size_t unStartColumnIndex)
		: m_unRowIndex{unRowIndex}, m_unColumnIndex{unColumnIndex},
		  m_unStartRowIndex{unStartRowIndex}, m_unStartColumnIndex{unStartColumnIndex}
	{}
};


enum class ETableElement
{
	FillingCell,
	Cell,
	FlatTable,
	TableContainer
};

// !TODO!:: At the moment, I don't really like the implementation where there is a table inside a cell,
// or when there are multiple nested tables inside a cell

// Perhaps it's worth doing it as follows:
// ITableCell -> CTableCell (A regular cell that does not contain nested tables)
//            -> CTableCellContainer (Stores anything (like default, but has a container for storing nested tables))

class ITableElementCell
{
	size_t m_unColspan;
protected:
	ITableElementCell(size_t unColspan);
public:
	virtual ~ITableElementCell() = default;

	virtual ETableElement GetType() const = 0;

	void SetColspan(size_t unColspan);

	size_t GetColspan() const;
};

class CTableElementCell : public ITableElementCell
{
	ETableElement m_eType;

protected:
	CTableElementCell(bool bIsFilling, size_t unColspan);
public:
	virtual ~CTableElementCell();

	ETableElement GetType() const override;

	void IsFlatTable();

	static CTableElementCell* CreateCell(const size_t& unColspan = 1);
	static CTableElementCell* CreateFillingCell(const size_t& unColspan = 1);
};

class ITag;
class IWriter;

struct TExternalTableData
{
	typedef std::function<bool(XmlUtils::CXmlLiteReader& oReader, std::vector<NSCSS::CNode>& arSelectors)> FuncReadStream;
	typedef std::function<void(XmlUtils::CXmlLiteReader& oReader, std::vector<NSCSS::CNode>& arSelectors)> FuncGetSubClass;
	typedef std::function<bool(XmlUtils::CXmlLiteReader& oReader, std::vector<NSCSS::CNode>& arSelectors)> FuncReadInside;
	typedef std::function<void(const std::wstring& wsTag)>                                                 FuncAddStopTag;
	typedef std::function<void()>                                                                          FuncClearStopTags;

	FuncReadStream    ReadStream;
	FuncGetSubClass   GetSubClass;
	FuncReadInside    ReadInside;
	FuncAddStopTag    AddStopTag;
	FuncClearStopTags ClearStopTags;

	IWriter* m_pWriter{nullptr};
};

typedef std::vector<ITableElementCell*> Row;
typedef std::vector<Row> Table;

class IWriter;

class CTableMatrix
{
public:
	CTableMatrix();
	virtual ~CTableMatrix();

	bool Empty() const;

	void Clear();

	bool SetCell(size_t unRowIndex, size_t unColumnIndex, ITableElementCell* pCell, bool bAutoClean = true);

	void NormalizeNumberColumns(size_t unNumberColumns);

	bool IsFillingCell(size_t unRowIndex, size_t unColumnIndex) const;
	bool IsNotNullCell(size_t unRowIndex, size_t unColumnIndex) const;

	size_t GetRowSize() const;
	size_t GetColumnSize() const;

	const Table& GetMatrixCells() const;
	Table& GetMatrixCells();

	const ITableElementCell* GetCell(size_t unRowIndex, size_t unColumnIndex) const;
	ITableElementCell* GetCell(size_t unRowIndex, size_t unColumnIndex);
protected:
	Table m_arCells;
};

class CTableCol
{
public:
	CTableCol(NSCSS::CNode& oTableColNode);
	~CTableCol();

	UINT GetSpan() const;
	const NSCSS::CCompiledStyle* GetStyle() const;
private:
	UINT m_unSpan;
	NSCSS::CCompiledStyle* m_pStyle;
};

class CTableColgroup
{
public:
	CTableColgroup(const NSCSS::CNode& oTableColgroupNode);
	~CTableColgroup();

	bool Empty() const;

	void AddCol(CTableCol* pCol);

	UINT GetTotalSpans() const;
	const std::vector<CTableCol*>& GetCols() const;
	const NSCSS::CCompiledStyle* GetColStyle(size_t unIndex) const;
private:
	std::vector<CTableCol*> m_arCols;
	UINT                    m_unWidth;
	UINT                    m_unTotalSpans;
};

class CTableElement
{
protected:
	CTableElement(TExternalTableData &oExternalData);
public:
	virtual ~CTableElement();

	void Clear();

	bool Empty() const;
	bool HaveCaption() const;

	virtual bool PreParse(XmlUtils::CXmlLiteReader& oReader) = 0;
	virtual void Normalize() = 0;
	virtual bool Convert(XmlUtils::CXmlLiteReader& oReader, const NSCSS::CNode& oTableNode) = 0;
protected:
	CTableMatrix m_oHeader;
	CTableMatrix m_oBody;
	CTableMatrix m_oFoother;

	XmlString *m_pCaption;

	std::vector<CTableColgroup*> m_arColgroups;

	TExternalTableData m_oExternalData;

	const NSCSS::CCompiledStyle* GetColStyle(size_t unColIndex) const;

	virtual bool ParseCaption(XmlUtils::CXmlLiteReader& oReader, XmlString*& pCaption) = 0;
	virtual bool ParseColgroup(XmlUtils::CXmlLiteReader& oReader, std::vector<CTableColgroup*>& arColgroups) = 0;
	template<typename T>
	inline bool ParseTable(XmlUtils::CXmlLiteReader& oReader, T* pTable);
	template<typename T>
	inline bool ParseMatrix(XmlUtils::CXmlLiteReader& oReader, CTableMatrix* pMatrix, size_t& unRowIndex, size_t& unColumnIndex);
};

class CTableContainer : public ITableElementCell
{
public:
	CTableContainer();
	virtual ~CTableContainer();

	ETableElement GetType() const override;

	void AddTable(CTableElement* pTable);
	const std::vector<CTableElement*>& GetTables() const;
private:
	std::vector<CTableElement*> m_arTables;
};

struct TTableData
{
	CTableMatrix m_oHeader;
	CTableMatrix m_oBody;
	CTableMatrix m_oFoother;

	XmlString *m_pCaption;

	std::vector<CTableColgroup*> m_arColgroups;
};

template<typename T>
inline bool CTableElement::ParseTable(XmlUtils::CXmlLiteReader& oReader, T* pTable)
{
	const int nDeath{oReader.GetDepth()};
	std::wstring wsName;

	while(oReader.ReadNextSiblingNode(nDeath))
	{
		wsName = oReader.GetName();

		size_t unRowIndex{0}, unColumnIndex{0};

		if(L"thead" == wsName)
			ParseMatrix<T>(oReader, &pTable->m_oHeader, unRowIndex, unColumnIndex);
		if(L"tbody" == wsName)
			ParseMatrix<T>(oReader, &pTable->m_oBody, unRowIndex, unColumnIndex);
		else if(L"tfoot" == wsName)
			ParseMatrix<T>(oReader, &pTable->m_oFoother, unRowIndex, unColumnIndex);
		else if (L"caption" == wsName)
			ParseCaption(oReader, pTable->m_pCaption);
		else if (L"colgroup" == wsName)
			ParseColgroup(oReader, pTable->m_arColgroups);
	}

	return true;
}

template<typename T>
inline bool CTableElement::ParseMatrix(XmlUtils::CXmlLiteReader& oReader, CTableMatrix* pMatrix, size_t& unRowIndex, size_t& unColumnIndex)
{
	const std::wstring wsElementName{oReader.GetName()};

	if (L"table" == wsElementName)
	{
		T* pNewTable{new T(m_oExternalData)};

		if (nullptr == pNewTable)
			return false;

		ITableElementCell* pTableElement{pMatrix->GetCell(unRowIndex - 1, unColumnIndex - 1)};
		CTableContainer *pContainer{nullptr};

		if (nullptr != pTableElement && ETableElement::TableContainer == pTableElement->GetType())
			pContainer = dynamic_cast<CTableContainer*>(pTableElement);
		else
		{
			pContainer = new CTableContainer();
			pMatrix->SetCell(unRowIndex - 1, unColumnIndex - 1, pContainer);
		}

		if (nullptr == pContainer)
			return false;

		pContainer->AddTable(pNewTable);

		ParseTable<T>(oReader, pNewTable);
	}
	else if (L"tr" == wsElementName)
	{
		const int nDepth{oReader.GetDepth()};
		++unRowIndex;
		unColumnIndex = 0;

		while (oReader.ReadNextSiblingNode(nDepth))
		{
			if (L"td" != oReader.GetName() && L"th" != oReader.GetName())
				continue;

			ParseMatrix<T>(oReader, pMatrix, unRowIndex, unColumnIndex);
		}
	}
	else if (L"td" == wsElementName || L"th" == wsElementName)
	{
		++unColumnIndex;

		while (pMatrix->IsFillingCell(unRowIndex - 1, unColumnIndex - 1))
			++unColumnIndex;

		size_t unRowspan{1}, unColspan{1};

		if (oReader.MoveToFirstAttribute())
		{
			do
			{
				if (L"rowspan" == oReader.GetName())
					unRowspan = NSStringFinder::ToInt(oReader.GetText(), 1);
				else if (L"colspan" == oReader.GetName())
					unColspan = NSStringFinder::ToInt(oReader.GetText(), 1);
			}while (oReader.MoveToNextAttribute());
			oReader.MoveToElement();
		}

		if (!pMatrix->SetCell(unRowIndex - 1, unColumnIndex - 1, CTableElementCell::CreateCell(unColspan)))
			return false;

		for (size_t unRow = 1; unRow < unRowspan; ++unRow)
			pMatrix->SetCell(unRowIndex + unRow - 1, unColumnIndex - 1, CTableElementCell::CreateFillingCell(unColspan));

		const int nDepth{oReader.GetDepth()};
		while (oReader.ReadNextSiblingNode(nDepth))
			ParseMatrix<T>(oReader, pMatrix, unRowIndex, unColumnIndex);
	}
	else if (oReader.IsEmptyNode())
		return false;
	else
	{
		const int nDepth{oReader.GetDepth()};
		while (oReader.ReadNextSiblingNode(nDepth))
			ParseMatrix<T>(oReader, pMatrix, unRowIndex, unColumnIndex);
	}

	return true;
}

bool MoveToNextTableCell(XmlUtils::CXmlLiteReader& oReader, std::vector<NSCSS::CNode>& arSelectors, std::stack<int>& arDepths, TExternalTableData::FuncGetSubClass& GetSubClass);
}

#endif // TABLE_H
