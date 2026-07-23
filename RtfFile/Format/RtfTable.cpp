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

#include "RtfTable.h"
#include "RtfDocument.h"

int RtfTable::GetType( )
{
	return TYPE_RTF_TABLE;
}
RtfTable::RtfTable()
{
}
RtfTable::RtfTable(const RtfTable& oTabl)
{
}
RtfTable& RtfTable::operator=(const RtfTable& oTabl)
{
	return *this;
}
std::wstring RtfTable::RenderToOOX(RenderParameter oRenderParameter)
{
	bool bRowsBidi = false;
	for (size_t i = 0; i < m_aArray.size(); i++)
	{
		RtfTableRowPtr &rowPr = m_aArray[i];
		if ((rowPr) && (rowPr->m_oProperty.m_nRightToLeft == 1))
		{
			bRowsBidi = true;
			break;
		}
	}

	if (bRowsBidi && m_oProperty.m_bBidi == PROP_DEF)
		m_oProperty.m_bBidi = 1;

	std::wstring sResult = L"<w:tbl>";
	sResult += m_oProperty.RenderToOOX(oRenderParameter);
	sResult += L"<w:tblGrid>";

	for (size_t i = 0; i < (int)m_aTableGrid.size(); i++)
	{
		if (m_aTableGrid[i] > 0)
			sResult += L"<w:gridCol w:w=\"" + std::to_wstring(m_aTableGrid[i]) + L"\"/>";
		else
			sResult += L"<w:gridCol/>";
	}
	sResult += L"</w:tblGrid>";

	for (size_t i = 0; i < m_aArray.size(); i++)
	{
		sResult += m_aArray[i]->RenderToOOX(oRenderParameter);
	}
	sResult += L"</w:tbl>";
	return sResult;
}
std::wstring RtfTable::RenderToRtf(RenderParameter oRenderParameter)
{
	std::wstring result;

	result += L"\n";
	for (size_t i = 0; i < m_aArray.size(); i++)
	{
		result += m_aArray[i]->RenderToRtf(oRenderParameter);
	}
	result += L"\n";
	return result;
}
void RtfTable::CalculateGridProp()
{
	//array of all possible cellx values
	std::vector<int> aCellx; // sorted in ascending order
	int nLastCellx = 0;

	int maxCellxFirstRow = 0;

	//m_aArray - rows
	for (size_t nCurRow = 0; nCurRow < m_aArray.size(); nCurRow++)
	{
		nLastCellx = 0;
		RtfTableRowPtr oCurRow = m_aArray[nCurRow];

		int nWidthBefore = 0;
		int nWidthAfter = 0;

		if (PROP_DEF != oCurRow->m_oProperty.m_nWidthStartInvCell && mu_Twips == oCurRow->m_oProperty.m_eWidthStartInvCellUnit)
			nWidthBefore = oCurRow->m_oProperty.m_nWidthStartInvCell;
		if (PROP_DEF != oCurRow->m_oProperty.m_nWidthEndInvCell && mu_Twips == oCurRow->m_oProperty.m_eWidthEndInvCellUnit)
			nWidthAfter = oCurRow->m_oProperty.m_nWidthEndInvCell;

		int nDelta = 0;// adjustment for margin, indent and spacing
		if (PROP_DEF != oCurRow->m_oProperty.m_nLeft) //different for each row
			nDelta = -oCurRow->m_oProperty.m_nLeft;
		else
		{
			if (PROP_DEF != m_oProperty.nTableIndent && 3 == m_oProperty.eTableIndentUnit)
				nDelta -= m_oProperty.nTableIndent;
			if (PROP_DEF != m_oProperty.m_nDefCellMarLeft && 3 == m_oProperty.m_eDefCellMarLeftUnit)
				nDelta += m_oProperty.m_nDefCellMarLeft;
			if (PROP_DEF != m_oProperty.m_nDefCellSpLeft && 3 == m_oProperty.m_eDefCellSpLeftUnit)
				nDelta += 2 * m_oProperty.m_nDefCellSpLeft;
			if (PROP_DEF != oCurRow->m_oProperty.m_nWidthStartInvCell && mu_Twips == oCurRow->m_oProperty.m_eWidthStartInvCellUnit)
				nDelta -= oCurRow->m_oProperty.m_nWidthStartInvCell;
		}

		//add widthBefore
		if (0 != nWidthBefore)
		{
			AddToArray(aCellx, nWidthBefore);
			nLastCellx = nWidthBefore + nDelta;
		}

		int nCellx = 0;
		for (int nCurCell = 0; nCurCell < oCurRow->GetCount(); nCurCell++)
		{
			RtfTableCellPtr oCurCell = oCurRow->operator [](nCurCell);

			int nCellx = nWidthBefore + nDelta + oCurCell->m_oProperty.m_nCellx;
			if (nCellx > maxCellxFirstRow && maxCellxFirstRow > 0)
				nCellx = maxCellxFirstRow;
			AddToArray(aCellx, nCellx);
			//don't touch the properties that remain in row - they are not important for oox conversion
			nLastCellx = nCellx;
		}
		//add widthAfter
		if (0 != nWidthAfter)
			AddToArray(aCellx, nLastCellx + nWidthAfter);

		if (maxCellxFirstRow == 0) maxCellxFirstRow = nLastCellx + nWidthAfter;
	}
	//calculate Span
	for (size_t i = 0; i < m_aArray.size(); i++)
	{
		RtfTableRowPtr oCurRow = m_aArray[i];
		//index of the last minimum element
		int nLastIndex = 0;
		int nLastCellx = 0;

		int nWidthBefore = 0;
		int nWidthAfter = 0;
		if (PROP_DEF != oCurRow->m_oProperty.m_nWidthStartInvCell && mu_Twips == oCurRow->m_oProperty.m_eWidthStartInvCellUnit)
			nWidthBefore = oCurRow->m_oProperty.m_nWidthStartInvCell;
		if (PROP_DEF != oCurRow->m_oProperty.m_nWidthEndInvCell && mu_Twips == oCurRow->m_oProperty.m_eWidthEndInvCellUnit)
			nWidthAfter = oCurRow->m_oProperty.m_nWidthEndInvCell;

		int nDelta = 0;// adjustment for margin, indent, spacing and border
		if (PROP_DEF != oCurRow->m_oProperty.m_nLeft) //different for each row
			nDelta = -oCurRow->m_oProperty.m_nLeft;
		else
		{
			if (PROP_DEF != m_oProperty.nTableIndent && 3 == m_oProperty.eTableIndentUnit)
				nDelta -= m_oProperty.nTableIndent;
			if (PROP_DEF != m_oProperty.m_nDefCellMarLeft && 3 == m_oProperty.m_eDefCellMarLeftUnit)
				nDelta += m_oProperty.m_nDefCellMarLeft;
			if (PROP_DEF != m_oProperty.m_nDefCellSpLeft && 3 == m_oProperty.m_eDefCellSpLeftUnit)
				nDelta += 2 * m_oProperty.m_nDefCellSpLeft;
			if (PROP_DEF != oCurRow->m_oProperty.m_nWidthStartInvCell && mu_Twips == oCurRow->m_oProperty.m_eWidthStartInvCellUnit)
				nDelta -= oCurRow->m_oProperty.m_nWidthStartInvCell;
		}

		//add gridBefore
		if (0 != nWidthBefore)
		{
			for (int k = nLastIndex; k < (int)aCellx.size(); k++)
			{
				if (aCellx[k] == nWidthBefore)
				{
					oCurRow->m_oProperty.m_nGridBefore = k - nLastIndex + 1;
					nLastIndex = k + 1;
					break;
				}
			}
		}
		for (int j = 0; j < oCurRow->GetCount(); j++)
		{
			RtfTableCellPtr oCurCell = oCurRow->operator [](j);
			int nCellx = nWidthBefore + nDelta + oCurCell->m_oProperty.m_nCellx;

			for (int k = nLastIndex; k < (int)aCellx.size(); k++)
			{
				if (aCellx[k] == nCellx)
				{
					oCurCell->m_oProperty.m_nSpan = k - nLastIndex + 1;
					int nWidth;
					if (0 == nLastIndex)
						nWidth = aCellx[k];
					else
						nWidth = aCellx[k] - aCellx[nLastIndex - 1];
					oCurCell->m_oProperty.m_nWidth = nWidth;
					oCurCell->m_oProperty.m_eWidthUnit = mu_Twips;
					nLastIndex = k + 1;
					break;
				}
			}
			nLastCellx = nCellx;

		}
		//add gridAfter
		if (0 != nWidthAfter)
			for (int k = nLastIndex; k < (int)aCellx.size(); k++)
			{
				if (aCellx[k] == nLastCellx + nWidthAfter)
				{
					m_aArray[i]->m_oProperty.m_nGridAfter = k - nLastIndex + 1;
					nLastIndex = k + 1;
					break;
				}
			}
	}
	//calculate gridTable
	for (size_t i = 0; i < (int)aCellx.size(); i++)
	{
		if (i == 0)
			m_aTableGrid.push_back(aCellx[0]);
		else
			m_aTableGrid.push_back(aCellx[i] - aCellx[i - 1]);
	}
}
void RtfTable::CalculateCellx(RtfDocument& oDocument)//todo consider margin indent
{
	if (m_aTableGrid.size() == 0 && m_aArray.size() > 0)
	{
		//if <w:tblGrid/> is missing, make proportional
		m_oProperty.m_nAutoFit = 1;
		if ((PROP_DEF == m_oProperty.m_nWidth || m_oProperty.m_nWidth <= 0))
		{
			//if table width is not specified, consider it 100%
			// Width = PageWidth - MarginLeft - MarginRight - Gutter
			int nGutter = oDocument.m_oProperty.m_nGutterWidth;
			if (1 == oDocument.m_oProperty.m_bGutterAtTop)//don't consider if it's Top gutter
				nGutter = 0;
			m_oProperty.m_nWidth = oDocument.m_oProperty.m_nPaperWidth - oDocument.m_oProperty.m_nMarginLeft - oDocument.m_oProperty.m_nMarginRight - nGutter;
			m_oProperty.m_eWidthUnit = mu_Twips;
		}

		for (size_t i = 0; i < m_aArray.size(); i++)
		{
			RtfTableRowPtr oCurRow = m_aArray[i];
			int nCellCount = oCurRow->GetCount();

			if (oCurRow->m_oProperty.GetCount() < nCellCount)
				nCellCount = oCurRow->m_oProperty.GetCount();

			if (nCellCount > 0)
			{
				int nCellWidth = m_oProperty.m_nWidth / nCellCount;
				int nCurCellX = 0;

				for (int j = 0; j < nCellCount; j++)
				{
					nCurCellX += nCellWidth;

					RtfTableCellPtr oCellPtr = (*oCurRow)[j];
					oCellPtr->m_oProperty.m_nCellx = nCurCellX;
					oCurRow->m_oProperty[j].m_nCellx = nCurCellX;
				}
			}
		}
	}
	else
	{
		for (size_t i = 0; i < m_aArray.size(); i++)
		{
			RtfTableRowPtr oCurRow = m_aArray[i];

			int nLeft = 0;
			if (PROP_DEF != m_oProperty.nTableIndent && 3 == m_oProperty.eTableIndentUnit)
				nLeft += m_oProperty.nTableIndent;
			if (PROP_DEF != m_oProperty.m_nDefCellMarLeft && 3 == m_oProperty.m_eDefCellMarLeftUnit)
				nLeft -= m_oProperty.m_nDefCellMarLeft;
			if (PROP_DEF != m_oProperty.m_nDefCellSpLeft && 3 == m_oProperty.m_eDefCellSpLeftUnit)
				nLeft += 2 * m_oProperty.m_nDefCellSpLeft;
			int nDelta = nLeft;//GrindBefore is accounted for in left

							   //if( PROP_DEF != oCurRow->m_oProperty.m_nGridBefore )
							   //{
							   //	int nGridBefore = oCurRow->m_oProperty.m_nGridBefore;
							   //	if( (int)m_aTableGrid.size() > nGridBefore - 1)
							   //	{
							   //		int nWidthBefore = 0;
							   //		for (int k = 0; k < nGridBefore ; k++ )
							   //			nWidthBefore += m_aTableGrid[k];
							   //		oCurRow->m_oProperty.m_nWidthStartInvCell = nWidthBefore;
							   //		oCurRow->m_oProperty.m_eMUStartInvCell = mu_Twips;
							   //		nLeft += nWidthBefore;
							   //	}
							   //}
							   //if( PROP_DEF != oCurRow->m_oProperty.m_nGridAfter )
							   //{
							   //	int nGridAfter = oCurRow->m_oProperty.m_nGridAfter;
							   //	if( (int)m_aTableGrid.size() > nGridAfter - 1)
							   //	{
							   //		int nWidthAfter = 0;
							   //		
							   //		for( int k = (int)m_aTableGrid.size() - 1; k >= (int)m_aTableGrid.size() - 1 - nGridAfter; k-- )
							   //			nWidthAfter += m_aTableGrid[k];
							   //		oCurRow->m_oProperty.m_nWidthEndInvCell = nWidthAfter;
							   //		oCurRow->m_oProperty.m_eMUEndInvCell = mu_Twips;
							   //	}
							   //}

			if (0 != nLeft)
				oCurRow->m_oProperty.m_nLeft = nLeft;

			int nCurWidth = 0;
			int nCurIndex = 0;
			for (int j = 0; j < oCurRow->m_oProperty.GetCount(); j++)
			{
				RtfTableCellPtr oCurCell = oCurRow->operator [](j);

				int nSpan = 1;

				if (PROP_DEF != oCurCell->m_oProperty.m_nSpan)
					nSpan = oCurCell->m_oProperty.m_nSpan;

				if (j == 0 && PROP_DEF != oCurRow->m_oProperty.m_nGridBefore)
					nCurIndex += oCurRow->m_oProperty.m_nGridBefore;
				//if( j == oCurRow->m_oProperty.size() - 1 && PROP_DEF != oCurRow->m_oProperty.m_nGridAfter )
				//	nSpan += oCurRow->m_oProperty.m_nGridAfter;

				for (int k = nCurIndex; k < nCurIndex + nSpan && k < (int)m_aTableGrid.size(); k++)
					nCurWidth += m_aTableGrid[k];
				nCurIndex = nCurIndex + nSpan;
				//if( j == 0 )
				oCurRow->m_oProperty[j].m_nCellx = nLeft + nCurWidth;
				//else
				//	oCurRow->m_oProperty[j].m_nCellx = nCurWidth;

			}
		}
	}
}
void RtfTable::AddToArray(std::vector<int>& aArray, int nValue)//todo can use the fact that it's sorted
{
	bool bNeedAdd = true;
	for (size_t k = 0; k < aArray.size(); k++)
	{
		if (std::abs(aArray[k] - nValue) < 1)
		{
			bNeedAdd = false;
			break;
		}
		else if (aArray[k] > nValue)
		{
			bNeedAdd = false;
			aArray.insert(aArray.begin() + k, nValue);
			break;
		}
	}
	if (true == bNeedAdd)
	{
		aArray.push_back(nValue);
	}
}
