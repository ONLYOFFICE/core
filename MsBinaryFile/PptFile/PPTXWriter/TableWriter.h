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
#pragma once

#include "../../../OOXML/PPTXFormat/Logic/GraphicFrame.h"
#include "../Drawing/Elements.h"
#include "ImageManager.h"

class TCell
{
public:
	TCell(PPT::CElementPtr ptrShape, int row, int col, CRelsGenerator* pRels, TCell* pParent = nullptr);

	void FillTc(PPTX::Logic::TableCell &oTc, PPT::CTextCFRun *pLastCF);

	enum eMergeDirection {
		none = 0b00,
		vert = 0b01,
		horz = 0b10,
		hove = (horz | vert)
	};

	enum eBorderPossition
	{
		lnL,
		lnR,
		lnT,
		lnB,
		lnTlToBr,
		lnBlToTr
	};
	void setBorder(eBorderPossition borderPos, PPT::CElementPtr ptrBorder);

	eMergeDirection parentDirection() const;
	int getHeight()const;

	void setPParent(TCell *pParent);
	void setPShape(PPT::CElementPtr ptrShape);

	void setRowSpan(int rowSpan);

	void setGridSpan(int gridSpan);

	bool isRealCell()const;

private:
	void FillTxBody(PPTX::Logic::TxBody &oTxBody, PPT::CTextCFRun *pLastCF);
	void FillTcPr(PPTX::Logic::TableCellProperties& oTcPr);
	void SetTcPrInvisibleBorders(PPTX::Logic::TableCellProperties& oTcPr)const;
	void SetTcPrInvisibleBorder(PPTX::Logic::TableCellProperties& oTcPr, eBorderPossition eBP)const;
	static bool isInvisibleBorder(const PPT::CElementPtr ptrBorder);
	bool isInvisibleBorders() const;
	void FillLn(PPTX::Logic::Ln& Ln, eBorderPossition eBP, PPT::CElementPtr ptrBorder);
	void SetLnName(PPTX::Logic::Ln& Ln, eBorderPossition eBP) const;
	void ApplyLn(PPTX::Logic::TableCellProperties& oTcPr, PPTX::Logic::Ln *pLn, eBorderPossition eBP) const;
	void FillMergeDirection(PPTX::Logic::TableCell& oTc);
	void setParentDirection();

private:
	PPT::CElementPtr m_ptrSpElCell;

	std::map<eBorderPossition, PPT::CElementPtr> m_mapBorders;

	// Proto table's coord
	int m_row, m_col;

	int m_rowSpan;
	int m_gridSpan;

	// Parent
	TCell* m_pParent;
	eMergeDirection m_parentDirection;

	// Common
	CRelsGenerator* m_pRels;
};


typedef std::vector<TCell> ProtoTableRow;
typedef std::vector<ProtoTableRow> MProtoTable;


class ProtoTable
{
public:
	ProtoTable(std::vector<PPT::CElementPtr> &arrCells,
		std::vector<PPT::CElementPtr>& arrSpliters,
		CRelsGenerator* pRels);

	static std::vector<int> getWidth(const std::vector<PPT::CElementPtr> &arrCells, long tableWidth = -1, bool isWidth = true);
	static std::vector<int> getHeight(const std::vector<PPT::CElementPtr> &arrCells, long tableHeight = -1, bool isHeight = true);
	static bool checkRowForZeroHeight(const ProtoTableRow& oRow);
	MProtoTable getTable() const;

private:
	void initProtoTable();
	bool fillProtoTable(std::vector<PPT::CElementPtr> &arrCells, std::vector<PPT::CElementPtr> &arrSpliters);
	bool fillCells(std::vector<PPT::CElementPtr> &arrCells);
	void fillBorders(std::vector<PPT::CElementPtr> &arrSpliters);
	bool findCellPos(const int top, const int left, UINT& posRow, UINT& posCol);
	std::list<TCell*> getParentCellFromTable(const UINT posFRow, const UINT posFCol, const UINT posLRow, const UINT posLCol);
	void setBorders(const UINT posFRow, const UINT posFCol, const UINT posLRow, const UINT posLCol, PPT::CElementPtr ptrBorder);

	bool isDefaultBoard(const PPT::CShapeElement *pBorder);

	std::vector<std::vector<PPT::CShapeElement *> >
		getRows(std::vector<PPT::CShapeElement *> &arrCells);

private:
	std::vector<int> m_arrLeft;
	std::vector<int> m_arrTop;
	MProtoTable m_table;
	CRelsGenerator* m_pRels;
};

class TableWriter
{
public:
	TableWriter(PPT::CTableElement *pTableElement, CRelsGenerator* pRels);

	void Convert(PPTX::Logic::GraphicFrame& oGraphicFrame);

private:
	void FillNvGraphicFramePr(PPTX::Logic::NvGraphicFramePr& oNvGFPr);
	void FillXfrm(PPTX::Logic::Xfrm& oXFRM);
	void FillTable(PPTX::Logic::Table &oTable);

	void FillTblPr(PPTX::Logic::TableProperties& oTblPr);
	void FillTblGrid(std::vector<PPTX::Logic::TableCol>& tblGrid,
		std::vector<PPT::CElementPtr> &arrCells);
	void prepareShapes(std::vector<PPT::CElementPtr> &arrCells,
		std::vector<PPT::CElementPtr> &arrSpliters);

	void FillRow(PPTX::Logic::TableRow& oRow, ProtoTableRow &arrCells);

public:
	std::wstring getXmlForGraphicFrame(int ID, int idx)const;
private:
	void CorrectGraphicFrame(PPTX::Logic::GraphicFrame& oGraphicFrame);
	void FillnvPr(PPTX::Logic::NvPr& oNvPr);

private:
	PPT::CTableElement* m_pTableElement;
	CRelsGenerator* m_pRels;
	nullable<ProtoTable> m_nPTable;
};
