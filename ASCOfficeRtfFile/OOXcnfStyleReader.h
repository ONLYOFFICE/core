#pragma once
#include "RtfProperty.h"
#include "OOXReaderBasic.h"
#include "OOXtblLookReader.h"
#include "RtfTableCell.h"

class CcnfStyle
{
public: bool bFirstRow;
public: bool bLastRow;
public: bool bFirstCol;
public: bool bLastCol;
public: bool bOddVBand;
public: bool bEvenVBand;
public: bool bOddHBand;
public: bool bEvenHBand;
public: bool bFirstRowLastCol;
public: bool bFirstRowFirstCol;
public: bool bLastRowLastCol;
public: bool bLastRowFirstCol;

public: CcnfStyle()
		{
			SetDefault();
		}
public: void SetDefault()
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
public: CcnfStyle Normalize()
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
public: void Merge( CcnfStyle& ocnfStyle )
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
public: bool operator <( CcnfStyle& ocnfStyle )
		{
			return ToInt() < ocnfStyle.ToInt();
		}
public: int ToInt()
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
public: RtfTableStylePtr ApplyTableStyle( RtfTableStylePtr oInputStyle )
		{
			if( NULL == oInputStyle )
				return oInputStyle;
			RtfTableStylePtr oOutputStyle = RtfTableStylePtr( new RtfTableStyle() );
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
public: void ApplyTableStyleToCellBorder( RtfTableStylePtr oInputStyle, RtfCellProperty& oOutputProperty, int nCurX, int nCurY, int nWidth, int nHeight )
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
private: void ApplyTableStyleToCellBorder( RtfTableStylePtr oInputStyle, RtfCellProperty& oOutputProperty, int nCurX, int nCurY, int nBoundLeft, int nBoundRight, int nBoundTop, int nBoundBottom )
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
				else
					oOutputProperty.m_oBorderRight.Merge( oInputStyle->m_oTableProp.m_oBorderVert );
			}
			else if( nBoundRight == nCurX )
			{
				oOutputProperty.m_oBorderLeft.Merge( oInputStyle->m_oTableProp.m_oBorderVert );
				oOutputProperty.m_oBorderRight.Merge( oInputStyle->m_oTableProp.m_oBorderRight );
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
				else
					oOutputProperty.m_oBorderBottom.Merge( oInputStyle->m_oTableProp.m_oBorderHor );
			}
			else if( nBoundBottom == nCurY )
			{
				oOutputProperty.m_oBorderTop.Merge( oInputStyle->m_oTableProp.m_oBorderHor );
				oOutputProperty.m_oBorderBottom.Merge( oInputStyle->m_oTableProp.m_oBorderBottom );
			}
			else
			{
				oOutputProperty.m_oBorderTop.Merge( oInputStyle->m_oTableProp.m_oBorderHor );
				oOutputProperty.m_oBorderBottom.Merge( oInputStyle->m_oTableProp.m_oBorderHor );
			}
		}
private: void ApplyOneStyleToCellBorder( RtfTableStylePtr oInputStyle, RtfCellProperty& oOutputProperty, int nCurX, int nCurY, int nBoundLeft, int nBoundRight, int nBoundTop, int nBoundBottom )
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
};

class OOXcnfStyleReader
{
public: bool Parse( ReaderParameter oParam, CcnfStyle& oOutput)
		{
			XmlUtils::CXmlReader oXmlReader;
			oXmlReader.OpenFromXmlNode( oParam.oNode );
			CString sVal = oXmlReader.ReadNodeAttribute( _T("w:val"), _T("") );
			if( sVal.GetLength() == 12 )
			{
				if( '0' == sVal[0] )
					oOutput.bFirstRow = false;
				else
					oOutput.bFirstRow = true;
				if( '0' == sVal[1] )
					oOutput.bLastRow = false;
				else
					oOutput.bLastRow = true;
				if( '0' == sVal[2] )
					oOutput.bFirstCol = false;
				else
					oOutput.bFirstCol = true;
				if( '0' == sVal[3] )
					oOutput.bLastCol = false;
				else
					oOutput.bLastCol = true;
				if( '0' == sVal[4] )
					oOutput.bOddVBand = false;
				else
					oOutput.bOddVBand = true;
				if( '0' == sVal[5] )
					oOutput.bEvenVBand = false;
				else
					oOutput.bEvenVBand = true;
				if( '0' == sVal[6] )
					oOutput.bOddHBand = false;
				else
					oOutput.bOddHBand = true;
				if( '0' == sVal[7] )
					oOutput.bEvenHBand = false;
				else
					oOutput.bEvenHBand = true;
				if( '0' == sVal[8] )
					oOutput.bFirstRowLastCol = false;
				else
					oOutput.bFirstRowLastCol = true;
				if( '0' == sVal[9] )
					oOutput.bFirstRowFirstCol = false;
				else
					oOutput.bFirstRowFirstCol = true;
				if( '0' == sVal[10] )
					oOutput.bLastRowLastCol = false;
				else
					oOutput.bLastRowLastCol = true;
				if( '0' == sVal[11] )
					oOutput.bLastRowFirstCol = false;
				else
					oOutput.bLastRowFirstCol = true;
			}
			else
			{
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("firstRow"), oOutput.bFirstRow , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("lastRow"), oOutput.bLastRow , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("firstColumn"), oOutput.bFirstCol , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("lastColumn"), oOutput.bLastCol , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("oddVBand"), oOutput.bOddVBand , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("evenVBand"), oOutput.bEvenVBand , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("oddHBand"), oOutput.bOddHBand , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("evenHBand"), oOutput.bEvenHBand , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("firstRowFirstColumn"), oOutput.bFirstRowFirstCol , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("firstRowLastColumn"), oOutput.bFirstRowLastCol , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("lastRowFirstColumn"), oOutput.bLastRowFirstCol , oXmlReader )
				COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("lastRowLastColumn"), oOutput.bLastRowLastCol , oXmlReader )
			}
			return true;

		}


};
