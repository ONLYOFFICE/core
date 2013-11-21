#pragma once 
#include "Basic.h"
#include "../Common/XmlUtils.h"
#include "RtfTableRow.h"


//TODO не работает если сделать вертикальный мерж и убрать разделение на пераграфы
class RtfTable : public ITextItem, public ItemContainer< RtfTableRowPtr >
{
	public: RtfTableProperty m_oProperty;

	public: CAtlArray<int> m_aTableGrid;
	public: int GetType( )
			{
				return TYPE_RTF_TABLE;
			}

	public: RtfTable()
			{
			}
	public: RtfTable(const RtfTable& oTabl)
			{
			}
	public: RtfTable& operator=(const RtfTable& oTabl)
			{
				return *this;
			}

	public: CString RenderToOOX(RenderParameter oRenderParameter)
			{
				CString sResult;

					sResult.Append( _T("<w:tbl>") );
					sResult.Append(m_oProperty.RenderToOOX(oRenderParameter));
					sResult.Append(_T("<w:tblGrid>"));
					for( int i = 0; i < (int)m_aTableGrid.GetCount(); i++ )
						sResult.AppendFormat(_T("<w:gridCol w:w=\"%d\"/>"), m_aTableGrid[i]);
					sResult.Append(_T("</w:tblGrid>"));	

					for(int i = 0; i < (int)m_aArray.GetCount(); i++)
					{
						sResult.Append( m_aArray[i]->RenderToOOX(oRenderParameter ) );
					}
					sResult.Append( _T("</w:tbl>") );
				return sResult;
	}
	public: CString RenderToRtf(RenderParameter oRenderParameter)
			{
				CString result;
				result.Append(_T("\n"));
				for(int i = 0 ; i < (int)m_aArray.GetCount(); i++)
					result.Append( m_aArray[i]->RenderToRtf(oRenderParameter) );
				result.Append(_T("\n"));
				return result;
			}
private: void AddToArray( CAtlArray<int>& aArray, int nValue )
		 {//todo можно применить то что он упорядоченный
			bool bNeedAdd = true;
			for(int k = 0; k < (int)aArray.GetCount(); k++)
			{
				if( aArray[k] == nValue )
				{
					bNeedAdd = false;
					break;
				}
				else if( aArray[k] > nValue )
				{
					bNeedAdd = false;
					aArray.InsertAt(k, nValue);
					break;
				}
			}
			if( true == bNeedAdd )
			{
				aArray.Add( nValue );
			}
		 }
public: void CalculateGridProp()
		{
			//массив всевозможных cellx
			CAtlArray<int> aCellx; // упорядочен по возрастанию
			int nLastCellx = 0;


			//m_aArray - строки
			for( int nCurRow = 0; nCurRow < (int)m_aArray.GetCount(); nCurRow++ ) 
			{
				nLastCellx = 0;
				RtfTableRowPtr oCurRow = m_aArray[ nCurRow ];

				int nWidthBefore = 0;
				int nWidthAfter = 0;
				if( PROP_DEF != oCurRow->m_oProperty.m_nWidthStartInvCell && mu_Twips == oCurRow->m_oProperty.m_eMUStartInvCell)
					nWidthBefore = oCurRow->m_oProperty.m_nWidthStartInvCell;
				if( PROP_DEF != oCurRow->m_oProperty.m_nWidthEndInvCell && mu_Twips == oCurRow->m_oProperty.m_eMUEndInvCell)
					nWidthAfter = oCurRow->m_oProperty.m_nWidthEndInvCell;

				int nDelta = 0;// поправка на margin и  indent и spacing
				if( PROP_DEF != oCurRow->m_oProperty.m_nLeft ) //для каждого row свой
					nDelta = -oCurRow->m_oProperty.m_nLeft;
				else
				{
					if( PROP_DEF != m_oProperty.nTableIndent && 3 == m_oProperty.nTableIndentUnits )
						nDelta -= m_oProperty.nTableIndent;
					if( PROP_DEF != m_oProperty.m_nDefCellMarLeft && 3 == m_oProperty.m_nDefCellMarLeftUnits )
						nDelta += m_oProperty.m_nDefCellMarLeft;
					if( PROP_DEF != m_oProperty.m_nDefCellSpLeft && 3 == m_oProperty.m_nDefCellSpLeftUnits )
						nDelta += 2 * m_oProperty.m_nDefCellSpLeft;
					if( PROP_DEF != oCurRow->m_oProperty.m_nWidthStartInvCell && mu_Twips == oCurRow->m_oProperty.m_eMUStartInvCell)
						nDelta -= oCurRow->m_oProperty.m_nWidthStartInvCell;
				}

				//добавляем widthBefore
				if( 0 != nWidthBefore )
				{
					AddToArray( aCellx, nWidthBefore );
					nLastCellx = nWidthBefore + nDelta;
				}

				int nCellx = 0;
				for( int nCurCell = 0; nCurCell < (int)oCurRow->GetCount(); nCurCell++ )
				{
					RtfTableCellPtr oCurCell = oCurRow->operator []( nCurCell );
					
					int nCellx = nWidthBefore + nDelta + oCurCell->m_oProperty.m_nCellx;
					AddToArray( aCellx, nCellx );
					//те свойства, что остались в row не трогаем - они не важны для конвертации в oox
					nLastCellx = nCellx;
				}
				//добавляем widthAfter
				if(  0 != nWidthAfter)
					AddToArray( aCellx, nLastCellx + nWidthAfter );
			}
			//вычисляем Span
			for(int i = 0; i < (int)m_aArray.GetCount();i++) 
			{
				RtfTableRowPtr oCurRow= m_aArray[ i ];
				//индекс последнего минимального элемента
				int nLastIndex = 0;
				int nLastCellx = 0;

				int nWidthBefore = 0;
				int nWidthAfter = 0;
				if( PROP_DEF != oCurRow->m_oProperty.m_nWidthStartInvCell && mu_Twips == oCurRow->m_oProperty.m_eMUStartInvCell)
					nWidthBefore = oCurRow->m_oProperty.m_nWidthStartInvCell;
				if( PROP_DEF != oCurRow->m_oProperty.m_nWidthEndInvCell && mu_Twips == oCurRow->m_oProperty.m_eMUEndInvCell)
					nWidthAfter = oCurRow->m_oProperty.m_nWidthEndInvCell;

				int nDelta = 0;// поправка на margin и  indent и spacing и border
				if( PROP_DEF != oCurRow->m_oProperty.m_nLeft ) //для каждого row свой
					nDelta = -oCurRow->m_oProperty.m_nLeft;
				else
				{
					if( PROP_DEF != m_oProperty.nTableIndent && 3 == m_oProperty.nTableIndentUnits )
						nDelta -= m_oProperty.nTableIndent;
					if( PROP_DEF != m_oProperty.m_nDefCellMarLeft && 3 == m_oProperty.m_nDefCellMarLeftUnits )
						nDelta += m_oProperty.m_nDefCellMarLeft;
					if( PROP_DEF != m_oProperty.m_nDefCellSpLeft && 3 == m_oProperty.m_nDefCellSpLeftUnits )
						nDelta += 2 * m_oProperty.m_nDefCellSpLeft;
					if( PROP_DEF != oCurRow->m_oProperty.m_nWidthStartInvCell && mu_Twips == oCurRow->m_oProperty.m_eMUStartInvCell)
						nDelta -= oCurRow->m_oProperty.m_nWidthStartInvCell;
				}

				//добавляем gridBefore
				if( 0 != nWidthBefore )
					for( int k = nLastIndex; k < (int)aCellx.GetCount(); k++ )
					{
						if( aCellx[k] == nWidthBefore )
						{
							oCurRow->m_oProperty.m_nGridBefore = k - nLastIndex + 1;
							nLastIndex = k + 1;
							break;
						}
					}
				for(int j = 0; j < (int)oCurRow->GetCount() ; j++)
				{
					RtfTableCellPtr oCurCell = oCurRow->operator []( j );
					int nCellx = nWidthBefore + nDelta + oCurCell->m_oProperty.m_nCellx;
					for(int k = nLastIndex; k < (int)aCellx.GetCount(); k++)
					{
						if( aCellx[k] == nCellx )
						{
							oCurCell->m_oProperty.m_nSpan = k - nLastIndex + 1;
							int nWidth;
							if( 0 == nLastIndex )
								nWidth = aCellx[k];
							else
								nWidth = aCellx[k] - aCellx[nLastIndex - 1];
							oCurCell->m_oProperty.m_nWidth = nWidth;
							oCurCell->m_oProperty.m_eWidthUnits = mu_Twips;
							nLastIndex = k + 1;
							break;
						}
					}
					nLastCellx = nCellx;

				}
				//добавляем gridAfter
				if( 0 != nWidthAfter )
					for(int k = nLastIndex; k < (int)aCellx.GetCount(); k++)
					{
						if( aCellx[k] == nLastCellx + nWidthAfter )
						{
							m_aArray[i]->m_oProperty.m_nGridAfter = k - nLastIndex + 1;
							nLastIndex = k + 1;
							break;
						}
					}
			}
			//вычисляем gridTable
			for( int i = 0; i < (int)aCellx.GetCount(); i++ )
			{
				if( i == 0 )
					m_aTableGrid.Add( aCellx[0] );
				else
					m_aTableGrid.Add( aCellx[ i ] - aCellx[i - 1] );
			}
		}	

public: void CalculateCellx( RtfDocument& oDocument )//todo учитывать margin indent
		{
			if( m_aTableGrid.GetCount() == 0 && m_aArray.GetCount() > 0 )
			{
				//если отсутствует <w:tblGrid/> делаем пропорционально
				m_oProperty.m_nAutoFit = 1;
				if( ( PROP_DEF == m_oProperty.m_nWidth || m_oProperty.m_nWidth <= 0 ) )
				{
					//если не задана ширина таблицы, считаем ее 100%
					// Width = PageWidth - MarginLeft - MarginRight - Gutter
					int nGutter = oDocument.m_oProperty.m_nGutterWidth;
					if( 1 == oDocument.m_oProperty.m_bGutterAtTop )//не учитываем если это Top gutter
						nGutter = 0;
					m_oProperty.m_nWidth = oDocument.m_oProperty.m_nPaperWidth - oDocument.m_oProperty.m_nMarginLeft - oDocument.m_oProperty.m_nMarginRight - nGutter;
					m_oProperty.m_eMUWidth = mu_Twips;
				}

				for(int i = 0; i < (int)m_aArray.GetCount(); i++) 
				{
					RtfTableRowPtr oCurRow = m_aArray[i];
					int nCellCount = oCurRow->GetCount();
					if( oCurRow->m_oProperty.GetCount() < nCellCount )
						nCellCount = oCurRow->m_oProperty.GetCount();
					if( nCellCount > 0 )
					{
						int nCellWidth = m_oProperty.m_nWidth / nCellCount;
						int nCurCellX = 0;
						for( int j = 0; j < nCellCount; j++ )
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
				for(int i = 0; i < (int)m_aArray.GetCount(); i++) 
				{
					RtfTableRowPtr oCurRow= m_aArray[ i ];

					int nLeft = 0;
					if( PROP_DEF != m_oProperty.nTableIndent && 3 == m_oProperty.nTableIndentUnits )
						nLeft += m_oProperty.nTableIndent;
					if( PROP_DEF != m_oProperty.m_nDefCellMarLeft && 3 == m_oProperty.m_nDefCellMarLeftUnits )
						nLeft -= m_oProperty.m_nDefCellMarLeft;
					if( PROP_DEF != m_oProperty.m_nDefCellSpLeft && 3 == m_oProperty.m_nDefCellSpLeftUnits )
						nLeft += 2 * m_oProperty.m_nDefCellSpLeft;
					int nDelta = nLeft;//в left учитывается GrindBefore

					if( PROP_DEF != oCurRow->m_oProperty.m_nGridBefore )
					{
						int nGridBefore = oCurRow->m_oProperty.m_nGridBefore;
						if( (int)m_aTableGrid.GetCount() > nGridBefore - 1)
						{
							int nWidthBefore = 0;
							for( int k = 0; k < nGridBefore ; k++ )
								nWidthBefore += m_aTableGrid[k];
							oCurRow->m_oProperty.m_nWidthStartInvCell = nWidthBefore;
							oCurRow->m_oProperty.m_eMUStartInvCell = mu_Twips;
							nLeft += nWidthBefore;
						}
					}
					if( PROP_DEF != oCurRow->m_oProperty.m_nGridAfter )
					{
						int nGridAfter = oCurRow->m_oProperty.m_nGridAfter;
						if( (int)m_aTableGrid.GetCount() > nGridAfter - 1)
						{
							int nWidthAfter = 0;
							for( int k = (int)m_aTableGrid.GetCount() - 1; k >= (int)m_aTableGrid.GetCount() - 1 - nGridAfter; k-- )
								nWidthAfter += m_aTableGrid[k];
							oCurRow->m_oProperty.m_nWidthEndInvCell = nWidthAfter;
							oCurRow->m_oProperty.m_eMUEndInvCell = mu_Twips;
						}
					}

					if( 0 != nLeft )
						oCurRow->m_oProperty.m_nLeft = nLeft;

					int nCurWidth = 0;
					int nCurIndex = 0;
					for(int j = 0; j < (int)oCurRow->m_oProperty.GetCount(); j++)
					{
						RtfTableCellPtr oCurCell = oCurRow->operator [](j);

						int nSpan = 1;
						if( PROP_DEF != oCurCell->m_oProperty.m_nSpan )
							nSpan = oCurCell->m_oProperty.m_nSpan;
						else
							nSpan = 1;
						if( j == 0 && PROP_DEF != oCurRow->m_oProperty.m_nGridBefore )
							nCurIndex += oCurRow->m_oProperty.m_nGridBefore;
						//if( j == oCurRow->m_oProperty.GetCount() - 1 && PROP_DEF != oCurRow->m_oProperty.m_nGridAfter )
						//	nSpan += oCurRow->m_oProperty.m_nGridAfter;

						for( int k = nCurIndex; k < nCurIndex + nSpan && k < (int)m_aTableGrid.GetCount(); k++  )
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

};
typedef boost::shared_ptr<RtfTable> RtfTablePtr;