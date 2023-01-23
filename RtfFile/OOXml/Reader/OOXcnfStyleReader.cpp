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

#include "OOXcnfStyleReader.h"

CcnfStyle::CcnfStyle()
{
	SetDefault();
}
void CcnfStyle::SetDefault()
{
	bFirstRow = false;
	bLastRow = false;
	bFirstCol = false;
	bLastCol = false;
	bOddVBand = false;
	bEvenVBand = false;
	bOddHBand = false;
	bEvenHBand = false;
	bFirstRowFirstCol = false;
	bFirstRowLastCol = false;
	bLastRowFirstCol = false;
	bLastRowLastCol = false;
}
CcnfStyle CcnfStyle::Normalize()
{
	CcnfStyle oResult;
	if( true == bFirstRowFirstCol )
		oResult.bFirstRowFirstCol = true;
	else if( true == bFirstRowLastCol )
		oResult.bFirstRowLastCol = true;
	else if( true == bLastRowFirstCol )
		oResult.bLastRowFirstCol = true;
	else if( true == bLastRowLastCol )
		oResult.bLastRowLastCol = true;
	else if( true == bLastRowLastCol )
		oResult.bLastRowLastCol = true;
	else if( true == bFirstCol )
		oResult.bFirstCol = true;
	else if( true == bLastCol )
		oResult.bLastCol = true;
	else if( true == bFirstRow )
		oResult.bFirstRow = true;
	else if( true == bLastRow )
		oResult.bLastRow = true;
	else if( true == bOddHBand )
		oResult.bOddHBand = true;
	else if( true == bEvenHBand )
		oResult.bEvenHBand = true;
	else if( true == bOddVBand )
		oResult.bOddVBand = true;
	else if( true == bEvenVBand )
		oResult.bEvenVBand = true;
	return oResult;
}
void CcnfStyle::Merge( CcnfStyle& ocnfStyle )
{
	if( true == ocnfStyle.bFirstRowFirstCol )
		bFirstRowFirstCol = true;
	if( true == ocnfStyle.bFirstRowLastCol )
		bFirstRowLastCol = true;
	if( true == ocnfStyle.bLastRowFirstCol )
		bLastRowFirstCol = true;
	if( true == ocnfStyle.bLastRowLastCol )
		bLastRowLastCol = true;
	if( true == ocnfStyle.bFirstCol )
		bFirstCol = true;
	if( true == ocnfStyle.bLastCol )
		bLastCol = true;
	if( true == ocnfStyle.bFirstRow )
		bFirstRow = true;
	if( true == ocnfStyle.bLastRow )
		bLastRow = true;
	if( true == ocnfStyle.bOddHBand )
		bOddHBand = true;
	if( true == ocnfStyle.bEvenHBand )
		bEvenHBand = true;
	if( true == ocnfStyle.bOddVBand )
		bOddVBand = true;
	if( true == ocnfStyle.bEvenVBand )
		bEvenVBand = true;
}
bool CcnfStyle::operator <( CcnfStyle& ocnfStyle )
{
	return ToInt() < ocnfStyle.ToInt();
}
int CcnfStyle::ToInt()
{
	int nResult = 0;
	if( true == bFirstRowFirstCol || true == bFirstRowLastCol || true == bLastRowFirstCol || true == bLastRowLastCol )
		nResult |= 1<<4;
	if( true == bFirstRow || true == bLastRow )
		nResult |= 1<<3;
	if( true == bFirstCol || true == bLastCol )
		nResult |= 1<<2;
	if( true == bOddHBand || true == bEvenHBand )
		nResult |= 1<<1;
	if( true == bOddVBand || true == bEvenVBand )
		nResult |= 1;
	return nResult;
}
RtfTableStylePtr CcnfStyle::ApplyTableStyle( RtfTableStylePtr oInputStyle )
{
	if( NULL == oInputStyle )
		return oInputStyle;

	RtfTableStylePtr oOutputStyle ( new RtfTableStyle() );

	oOutputStyle->Merge( oInputStyle );
	if( true == bOddVBand && NULL != oInputStyle->m_oBandVerOdd )
		oOutputStyle->Merge( oInputStyle->m_oBandVerOdd );
	if( true == bEvenVBand && NULL != oInputStyle->m_oBandVerEven )
		oOutputStyle->Merge( oInputStyle->m_oBandVerEven );
	if( true == bOddHBand && NULL != oInputStyle->m_oBandHorOdd )
		oOutputStyle->Merge( oInputStyle->m_oBandHorOdd );
	if( true == bEvenHBand && NULL != oInputStyle->m_oBandHorEven )
		oOutputStyle->Merge( oInputStyle->m_oBandHorEven );
	if( true == bLastCol && NULL != oInputStyle->m_oLastCol )
		oOutputStyle->Merge( oInputStyle->m_oLastCol );
	if( true == bFirstCol && NULL != oInputStyle->m_oFirstCol )
		oOutputStyle->Merge( oInputStyle->m_oFirstCol );
	if( true == bLastRow && NULL != oInputStyle->m_oLastRow )
		oOutputStyle->Merge( oInputStyle->m_oLastRow );
	if( true == bFirstRow && NULL != oInputStyle->m_oFirstRow )
		oOutputStyle->Merge( oInputStyle->m_oFirstRow );
	if( true == bFirstRowFirstCol && NULL != oInputStyle->m_oNWCell )
		oOutputStyle->Merge( oInputStyle->m_oNWCell );
	if( true == bFirstRowLastCol && NULL != oInputStyle->m_oNECell )
		oOutputStyle->Merge( oInputStyle->m_oNECell );
	if( true == bLastRowFirstCol && NULL != oInputStyle->m_oSWCell )
		oOutputStyle->Merge( oInputStyle->m_oSWCell );
	if( true == bLastRowLastCol && NULL != oInputStyle->m_oSECell )
		oOutputStyle->Merge( oInputStyle->m_oSECell );

	return oOutputStyle;
}
void CcnfStyle::ApplyTableStyleToCellBorder( RtfTableStylePtr oInputStyle, RtfCellProperty& oOutputProperty, int nCurX, int nCurY, int nWidth, int nHeight )
{
	if( NULL == oInputStyle )
		return;
	int nRowsInBand = 1;
	int nColsInBand = 1;
	if( PROP_DEF != oInputStyle->m_oTableProp.m_nRowBandSize )
		nRowsInBand = oInputStyle->m_oTableProp.m_nRowBandSize;
	if( PROP_DEF != oInputStyle->m_oTableProp.m_nColBandSize )
		nColsInBand = oInputStyle->m_oTableProp.m_nColBandSize;

	//обнуляем
	oOutputProperty.m_oBorderDiagonalLR = RtfBorder();
	oOutputProperty.m_oBorderDiagonalRL = RtfBorder();
	oOutputProperty.m_oBorderLeft = RtfBorder();
	oOutputProperty.m_oBorderTop = RtfBorder();
	oOutputProperty.m_oBorderRight = RtfBorder();
	oOutputProperty.m_oBorderBottom = RtfBorder();
	oOutputProperty.m_oBorderInsideH = RtfBorder();
	oOutputProperty.m_oBorderInsideV = RtfBorder();
	//От всей таблицы
	ApplyTableStyleToCellBorder( oInputStyle, oOutputProperty, nCurX, nCurY, 0, nWidth - 1, 0, nHeight - 1 );

	//От OddVBand
	if( 1 != oInputStyle->m_oTableProp.m_bAutoNoColBand && nColsInBand > 0 )
	{
		int nBoundLeft = 0;
		int nBoundRight = nWidth - 1;
		if( 1 == oInputStyle->m_oTableProp.m_bAutoFirstCol )
			nBoundLeft ++;
		if( 1 == oInputStyle->m_oTableProp.m_bAutoLastCol )
			nBoundRight --;
		if( nBoundLeft <= nBoundRight && nBoundLeft <= nCurX && nCurX <= nBoundRight )
		{
			int nDelta = nCurX - nBoundLeft;//получаем количество столбцов между
			int nPlace = nDelta % nColsInBand;//получаем номер внутри band
			//будем считать что флаги расставлены правильно
			if( true == bOddVBand && NULL != oInputStyle->m_oBandVerOdd )
				ApplyOneStyleToCellBorder( oInputStyle->m_oBandVerOdd, oOutputProperty, nCurX, nCurY, nCurX - nPlace, nCurX - nPlace + nColsInBand - 1, 0, nHeight - 1 );
			if( true == bEvenVBand && NULL != oInputStyle->m_oBandVerEven )
				ApplyOneStyleToCellBorder( oInputStyle->m_oBandVerEven, oOutputProperty, nCurX, nCurY, nCurX - nPlace, nCurX - nPlace + nColsInBand - 1, 0, nHeight - 1 );
		}
	}

	if( 1 != oInputStyle->m_oTableProp.m_bAutoNoRowBand && nRowsInBand > 0 )
	{
		int nBoundTop = 0;
		int nBoundBottom = nHeight - 1;
		if( 1 == oInputStyle->m_oTableProp.m_bAutoFirstRow )
			nBoundTop ++;
		if( 1 == oInputStyle->m_oTableProp.m_bAutoLastRow )
			nBoundBottom --;
		if( nBoundTop <= nBoundBottom && nBoundTop <= nCurY && nCurY <= nBoundBottom )
		{
			int nDelta = nCurY - nBoundTop;//получаем количество столбцов между
			int nPlace = nDelta % nRowsInBand;//получаем номер внутри band
			//будем считать что флаги расставлены правильно
			if( true == bOddHBand && NULL != oInputStyle->m_oBandHorOdd )
				ApplyOneStyleToCellBorder( oInputStyle->m_oBandHorOdd, oOutputProperty, nCurX, nCurY, 0, nWidth - 1, nCurY - nPlace, nCurY - nPlace + nRowsInBand - 1 );
			if( true == bEvenHBand && NULL != oInputStyle->m_oBandHorEven )
				ApplyOneStyleToCellBorder( oInputStyle->m_oBandHorEven, oOutputProperty, nCurX, nCurY, 0, nWidth - 1, nCurY - nPlace, nCurY - nPlace + nRowsInBand - 1 );
		}
	}
	if( true == bLastCol )
		ApplyOneStyleToCellBorder( oInputStyle->m_oLastCol, oOutputProperty, nCurX, nCurY, nWidth - 1, nWidth - 1, 0, nHeight - 1 );
	if( true == bFirstCol )
		ApplyOneStyleToCellBorder( oInputStyle->m_oFirstCol, oOutputProperty, nCurX, nCurY, 0, 0, 0, nHeight - 1 );
	if( true == bLastRow )
		ApplyOneStyleToCellBorder( oInputStyle->m_oLastRow, oOutputProperty, nCurX, nCurY, 0, nWidth - 1, nHeight - 1, nHeight - 1 );
	if( true == bFirstRow )
		ApplyOneStyleToCellBorder( oInputStyle->m_oFirstRow, oOutputProperty, nCurX, nCurY, 0, nWidth - 1, 0, 0 );
	if( true == bFirstRowFirstCol )
		ApplyOneStyleToCellBorder( oInputStyle->m_oNWCell, oOutputProperty, nCurX, nCurY, 0, 0, 0, 0 );
	if( true == bFirstRowLastCol )
		ApplyOneStyleToCellBorder( oInputStyle->m_oNECell, oOutputProperty, nCurX, nCurY, nWidth - 1, nWidth - 1, 0, 0 );
	if( true == bLastRowFirstCol )
		ApplyOneStyleToCellBorder( oInputStyle->m_oSWCell, oOutputProperty, nCurX, nCurY, 0, 0, nHeight - 1, nHeight - 1 );
	if( true == bLastRowLastCol )
		ApplyOneStyleToCellBorder( oInputStyle->m_oSECell, oOutputProperty, nCurX, nCurY, nWidth - 1, nWidth - 1, nHeight - 1, nHeight - 1 );
}
void CcnfStyle::ApplyTableStyleToCellBorder( RtfTableStylePtr oInputStyle, RtfCellProperty& oOutputProperty, int nCurX, int nCurY, int nBoundLeft, int nBoundRight, int nBoundTop, int nBoundBottom )
{
	if( NULL == oInputStyle )
		return;
	if( !(nBoundLeft <= nCurX && nCurX <= nBoundRight && nBoundTop <= nCurY && nCurY <= nBoundBottom) )
		return;

	if( nBoundLeft == nCurX )
	{
		oOutputProperty.m_oBorderLeft.Merge( oInputStyle->m_oTableProp.m_oBorderLeft );

		if( nBoundRight == nCurX )
			oOutputProperty.m_oBorderRight.Merge( oInputStyle->m_oTableProp.m_oBorderRight );
		else //if (oInputStyle->m_oTableProp.m_bAutoNoColBand != 1)
			oOutputProperty.m_oBorderRight.Merge( oInputStyle->m_oTableProp.m_oBorderVert );
	}
	else if( nBoundRight == nCurX )
	{
		oOutputProperty.m_oBorderRight.Merge( oInputStyle->m_oTableProp.m_oBorderRight );

		if( nBoundLeft == nCurX )
			oOutputProperty.m_oBorderLeft.Merge( oInputStyle->m_oTableProp.m_oBorderLeft );
		else //if (oInputStyle->m_oTableProp.m_bAutoNoColBand != 1)
			oOutputProperty.m_oBorderLeft.Merge( oInputStyle->m_oTableProp.m_oBorderVert );
	}
	else
	{
		oOutputProperty.m_oBorderLeft.Merge( oInputStyle->m_oTableProp.m_oBorderVert );
		oOutputProperty.m_oBorderRight.Merge( oInputStyle->m_oTableProp.m_oBorderVert );
	}

	if( nBoundTop == nCurY )
	{
		oOutputProperty.m_oBorderTop.Merge( oInputStyle->m_oTableProp.m_oBorderTop );
		if( nBoundBottom == nCurY )
			oOutputProperty.m_oBorderBottom.Merge( oInputStyle->m_oTableProp.m_oBorderBottom );
		else// if (oInputStyle->m_oTableProp.m_bAutoNoRowBand != 1)
			oOutputProperty.m_oBorderBottom.Merge( oInputStyle->m_oTableProp.m_oBorderHor );
	}
	else if( nBoundBottom == nCurY )
	{
		oOutputProperty.m_oBorderBottom.Merge( oInputStyle->m_oTableProp.m_oBorderBottom );
		if( nBoundTop == nCurX )
			oOutputProperty.m_oBorderTop.Merge( oInputStyle->m_oTableProp.m_oBorderTop );
		else //if (oInputStyle->m_oTableProp.m_bAutoNoRowBand != 1)
			oOutputProperty.m_oBorderTop.Merge( oInputStyle->m_oTableProp.m_oBorderHor );
	}
	else
	{
		oOutputProperty.m_oBorderTop.Merge( oInputStyle->m_oTableProp.m_oBorderHor );
		oOutputProperty.m_oBorderBottom.Merge( oInputStyle->m_oTableProp.m_oBorderHor );
	}
}
void CcnfStyle::ApplyOneStyleToCellBorder( RtfTableStylePtr oInputStyle, RtfCellProperty& oOutputProperty, int nCurX, int nCurY, int nBoundLeft, int nBoundRight, int nBoundTop, int nBoundBottom )
{
	if( NULL == oInputStyle )
		return;
	if( !(nBoundLeft <= nCurX && nCurX <= nBoundRight && nBoundTop <= nCurY && nCurY <= nBoundBottom) )
		return;
	//на них ни что не влияет
	oOutputProperty.m_oBorderDiagonalLR.Merge( oInputStyle->m_oCellProp.m_oBorderDiagonalLR );
	oOutputProperty.m_oBorderDiagonalRL.Merge( oInputStyle->m_oCellProp.m_oBorderDiagonalRL );

	if( nBoundLeft == nCurX )
	{
		oOutputProperty.m_oBorderLeft.Merge( oInputStyle->m_oCellProp.m_oBorderLeft );
		if( nBoundRight == nCurX )
			oOutputProperty.m_oBorderRight.Merge( oInputStyle->m_oCellProp.m_oBorderRight );
		else
			oOutputProperty.m_oBorderRight.Merge( oInputStyle->m_oCellProp.m_oBorderInsideV );
	}
	else if( nBoundRight == nCurX )
	{
		oOutputProperty.m_oBorderLeft.Merge( oInputStyle->m_oCellProp.m_oBorderInsideV );
		oOutputProperty.m_oBorderRight.Merge( oInputStyle->m_oCellProp.m_oBorderRight );
	}
	else
	{
		oOutputProperty.m_oBorderLeft.Merge( oInputStyle->m_oCellProp.m_oBorderInsideV );
		oOutputProperty.m_oBorderRight.Merge( oInputStyle->m_oCellProp.m_oBorderInsideV );
	}

	if( nBoundTop == nCurY )
	{
		oOutputProperty.m_oBorderTop.Merge( oInputStyle->m_oCellProp.m_oBorderTop );
		if( nBoundBottom == nCurY )
			oOutputProperty.m_oBorderBottom.Merge( oInputStyle->m_oCellProp.m_oBorderBottom );
		else
			oOutputProperty.m_oBorderBottom.Merge( oInputStyle->m_oCellProp.m_oBorderInsideH );
	}
	else if( nBoundBottom == nCurY )
	{
		oOutputProperty.m_oBorderTop.Merge( oInputStyle->m_oCellProp.m_oBorderInsideH );
		oOutputProperty.m_oBorderBottom.Merge( oInputStyle->m_oCellProp.m_oBorderBottom );
	}
	else
	{
		oOutputProperty.m_oBorderTop.Merge( oInputStyle->m_oCellProp.m_oBorderInsideH );
		oOutputProperty.m_oBorderBottom.Merge( oInputStyle->m_oCellProp.m_oBorderInsideH );
	}
}

OOXcnfStyleReader::OOXcnfStyleReader(ComplexTypes::Word::CCnf *ooxCnfStyle)
{
	m_ooxCnfStyle = ooxCnfStyle;
}
bool OOXcnfStyleReader::Parse( ReaderParameter oParam, CcnfStyle& oOutput)
{
	if (m_ooxCnfStyle == NULL) return false;

	if (m_ooxCnfStyle->m_oVal.IsInit())
	{
		oOutput.bFirstRow	= m_ooxCnfStyle->m_oVal->IsFirstRow();
		oOutput.bLastRow	= m_ooxCnfStyle->m_oVal->IsLastRow();
		oOutput.bFirstCol	= m_ooxCnfStyle->m_oVal->IsFirstColumn();
		oOutput.bLastCol	= m_ooxCnfStyle->m_oVal->IsLastColumn();
		oOutput.bOddVBand	= m_ooxCnfStyle->m_oVal->IsBand1Vertical();
		oOutput.bEvenVBand	= m_ooxCnfStyle->m_oVal->IsBand2Vertical();
		oOutput.bOddHBand	= m_ooxCnfStyle->m_oVal->IsBand1Horizontal();
		oOutput.bEvenHBand	= m_ooxCnfStyle->m_oVal->IsBand2Horizontal();
		oOutput.bFirstRowLastCol	= m_ooxCnfStyle->m_oVal->IsNeCell(); //северо восток
		oOutput.bFirstRowFirstCol	= m_ooxCnfStyle->m_oVal->IsNwCell();
		oOutput.bLastRowLastCol		= m_ooxCnfStyle->m_oVal->IsSeCell();
		oOutput.bLastRowFirstCol	= m_ooxCnfStyle->m_oVal->IsSwCell();
	}
	else
	{
		if ( m_ooxCnfStyle->m_oFirstRow.IsInit())
			oOutput.bFirstRow =  m_ooxCnfStyle->m_oFirstRow->ToBool();

		if ( m_ooxCnfStyle->m_oLastRow.IsInit())
			oOutput.bLastRow = m_ooxCnfStyle->m_oLastRow->ToBool();

		if ( m_ooxCnfStyle->m_oFirstColumn.IsInit())
			oOutput.bFirstCol = m_ooxCnfStyle->m_oFirstColumn->ToBool();

		if ( m_ooxCnfStyle->m_oLastColumn.IsInit())
			oOutput.bLastCol = m_ooxCnfStyle->m_oLastColumn->ToBool();

		if ( m_ooxCnfStyle->m_oOddVBand.IsInit())
			oOutput.bOddVBand = m_ooxCnfStyle->m_oOddVBand->ToBool();

		if ( m_ooxCnfStyle->m_oEvenVBand.IsInit())
			oOutput.bEvenVBand = m_ooxCnfStyle->m_oEvenVBand->ToBool();

		if ( m_ooxCnfStyle->m_oOddHBand.IsInit())
			oOutput.bOddHBand = m_ooxCnfStyle->m_oOddHBand->ToBool();

		if ( m_ooxCnfStyle->m_oEvenHBand.IsInit())
			oOutput.bEvenHBand = m_ooxCnfStyle->m_oEvenHBand->ToBool();

		if ( m_ooxCnfStyle->m_oFirstRowFirstColumn.IsInit())
			oOutput.bFirstRowFirstCol = m_ooxCnfStyle->m_oFirstRowFirstColumn->ToBool();

		if ( m_ooxCnfStyle->m_oFirstRowLastColumn.IsInit())
			oOutput.bFirstRowLastCol = m_ooxCnfStyle->m_oFirstRowLastColumn->ToBool();

		if ( m_ooxCnfStyle->m_oLastRowFirstColumn.IsInit())
			oOutput.bLastRowFirstCol = m_ooxCnfStyle->m_oLastRowFirstColumn->ToBool();

		if ( m_ooxCnfStyle->m_oLastRowLastColumn.IsInit())
			oOutput.bLastRowLastCol = m_ooxCnfStyle->m_oLastRowLastColumn->ToBool();
	}
	return true;
}
