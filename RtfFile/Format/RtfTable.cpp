/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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
	//массив всевозможных cellx
	std::vector<int> aCellx; // упорядочен по возрастанию
	int nLastCellx = 0;

	int maxCellxFirstRow = 0;

	//m_aArray - строки
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

		int nDelta = 0;// поправка на margin и  indent и spacing
		if (PROP_DEF != oCurRow->m_oProperty.m_nLeft) //для каждого row свой
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

		//добавляем widthBefore
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
			//те свойства, что остались в row не трогаем - они не важны для конвертации в oox
			nLastCellx = nCellx;
		}
		//добавляем widthAfter
		if (0 != nWidthAfter)
			AddToArray(aCellx, nLastCellx + nWidthAfter);

		if (maxCellxFirstRow == 0) maxCellxFirstRow = nLastCellx + nWidthAfter;
	}
	//вычисляем Span
	for (size_t i = 0; i < m_aArray.size(); i++)
	{
		RtfTableRowPtr oCurRow = m_aArray[i];
		//индекс последнего минимального элемента
		int nLastIndex = 0;
		int nLastCellx = 0;

		int nWidthBefore = 0;
		int nWidthAfter = 0;
		if (PROP_DEF != oCurRow->m_oProperty.m_nWidthStartInvCell && mu_Twips == oCurRow->m_oProperty.m_eWidthStartInvCellUnit)
			nWidthBefore = oCurRow->m_oProperty.m_nWidthStartInvCell;
		if (PROP_DEF != oCurRow->m_oProperty.m_nWidthEndInvCell && mu_Twips == oCurRow->m_oProperty.m_eWidthEndInvCellUnit)
			nWidthAfter = oCurRow->m_oProperty.m_nWidthEndInvCell;

		int nDelta = 0;// поправка на margin и  indent и spacing и border
		if (PROP_DEF != oCurRow->m_oProperty.m_nLeft) //для каждого row свой
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

		//добавляем gridBefore
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
		//добавляем gridAfter
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
	//вычисляем gridTable
	for (size_t i = 0; i < (int)aCellx.size(); i++)
	{
		if (i == 0)
			m_aTableGrid.push_back(aCellx[0]);
		else
			m_aTableGrid.push_back(aCellx[i] - aCellx[i - 1]);
	}
}
void RtfTable::CalculateCellx(RtfDocument& oDocument)//todo учитывать margin indent
{
	if (m_aTableGrid.size() == 0 && m_aArray.size() > 0)
	{
		//если отсутствует <w:tblGrid/> делаем пропорционально
		m_oProperty.m_nAutoFit = 1;
		if ((PROP_DEF == m_oProperty.m_nWidth || m_oProperty.m_nWidth <= 0))
		{
			//если не задана ширина таблицы, считаем ее 100%
			// Width = PageWidth - MarginLeft - MarginRight - Gutter
			int nGutter = oDocument.m_oProperty.m_nGutterWidth;
			if (1 == oDocument.m_oProperty.m_bGutterAtTop)//не учитываем если это Top gutter
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
			int nDelta = nLeft;//в left учитывается GrindBefore

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
void RtfTable::AddToArray(std::vector<int>& aArray, int nValue)//todo можно применить то что он упорядоченный
{
	bool bNeedAdd = true;
	for (size_t k = 0; k < aArray.size(); k++)
	{
		if (std::abs(aArray[k] - nValue) < 3)
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
