#pragma once

#include "Common.h"
#include <vector>

#define WSTR( s ) s, ::wcslen( s )

inline long RealToLong(const double value)
{
	// return int(value + (vulue < 0 ? -0.5 : 0.5));

	const double d = value + 6755399441055744.0;
	return *((int *)(&d));
}

inline long MMtoDXA(double mm)
{
	return RealToLong( mm * (1440 / 25.4) );
}
//namespace NSDocxTable
//{
//	// base types
//	enum EBorderType
//	{
//		BorderTypeNone    = 0,
//		BorderTypeSingle  = 1
//	};

//	enum EWidthType
//	{
//		WidthTypeNil,
//		WidthTypeAuto,
//		WidthTypeDxa,  // (integer) 1/1440 of an inch
//		WidthTypePct,  // (double) percent
//	};

//	enum EMeasureType
//	{
//		MeasureTypeMM, // millimeters
//		MeasureTypeCM, // centimeters
//		MeasureTypeIN, // inches
//		MeasureTypePT, // points
//		MeasureTypePC, // 6 points
//		MeasureTypeIC = MeasureTypePC
//	};

//	enum EAnchorLocation
//	{
//		AnchorLocationPage,
//		AnchorLocationText,
//		AnchorLocationMargin
//	};

//	enum EAlignLocation
//	{
//		AlignLocationTop,
//		AlignLocationBottom,
//		AlignLocationCenter,
//		AlignLocationInline,
//		AlignLocationInside,
//		AlignLocationOutside
//	};

//	class CBorder 
//	{
//	protected:
//		LPCWCH m_szName;
//		EBorderType m_eType;
//		
//		long m_lSize;  // (unsigned) in 1/8 points [2..96] for lines or [1..31] for art
//		long m_lSpace; // (unsigned) in points
//		long m_lColor; // sRGB

//	public:
//		CBorder( const WCHAR* name )
//		{
//			// standart table border
//			m_szName = name;
//			m_eType = BorderTypeSingle;
//			m_lSize = 4;
//			m_lSpace = 0;
//			m_lColor = -1;
//		}

//		void ToXml( NSDocxRenderer::CStringWriter& oWriter ) const
//		{
//			oWriter.WriteFormatString( L"<%s w:val=\"%s\" w:sz=\"%d\" w:space=\"%d\" >",
//				m_szName,
//				GetNameBorderType( m_eType ),
//				m_lSize, 
//				m_lSpace );

//			if( m_lColor < 0 )
//				oWriter.WriteFormatString( L"w:color=\"auto\"/>" );
//			else
//				oWriter.WriteFormatString( L"w:color=\"%06x\"/>", m_lColor );
//		}


//		static const WCHAR* GetNameBorderType( EBorderType type )
//		{
//			static const LPCWCH g_table[] = {
//				L"none",
//				L"single"
//			};

//			return g_table[type];
//		}

//		void SetType( EBorderType type )
//		{
//			m_eType = type;
//		}

//		EBorderType GetType() const
//		{
//			return m_eType;
//		}

//		void SetSizeMM( double size )
//		{
//			size *= 8 * 72 / 25.4;
//			long v = long(size + 0.5);
//			m_lSize = max(2, min(v, 96));
//		}

//		long GetSize() const
//		{
//			return  m_lSize;
//		}

//		void SetSpaceMM( double size )
//		{
//			size *= 72 / 25.4;
//			long v = long(size + 0.5);
//			m_lSpace = max(0, v);
//		}

//		long GetSpace() const
//		{
//			return m_lSpace;
//		}

//		void SetColor( BYTE R, BYTE G, BYTE B )
//		{
//			m_lColor = ((long)R << 16) | ((long)G << 8) | B;
//		}

//		void SetColor( long sRGB )
//		{
//			m_lColor = sRGB;
//		}

//		long GetColor() const
//		{
//			return m_lColor;
//		}

//	};

//	class CWidth
//	{
//	protected:
//		LPCWCH m_szName;
//		EWidthType m_eType;
//		double m_dValue;
//		
//	public:

//		CWidth( const WCHAR* name )
//		{
//			m_szName = name;
//			m_eType = WidthTypeAuto;
//			m_dValue = 0;
//		}

//		void ToXml( NSDocxRenderer::CStringWriter& oWriter ) const
//		{
//			oWriter.WriteFormatString( L"<%s w:type=\"%s\" ", m_szName, GetNameWidthType( m_eType ) );

//			switch( m_eType )
//			{
//			case WidthTypeDxa: 
//				oWriter.WriteFormatString( L"w:w=\"%d\"/>", long(m_dValue) ); 
//				break;
//			case WidthTypePct: 
//				oWriter.WriteFormatString( L"w:w=\"%f%%\"/>", m_dValue ); 
//				break;
//			default: 
//				oWriter.WriteFormatString( L"/>" ); 
//			};
//		}

//		void SetDxa( double value )
//		{
//			m_eType = WidthTypeDxa;
//			m_dValue = RealToLong( value );
//		}

//		void SetPercent( double value )
//		{
//			m_eType = WidthTypePct;
//			m_dValue = value;
//		}

//		void SetNil()
//		{
//			m_eType = WidthTypeNil;
//			m_dValue = 0;
//		}

//		void SetAuto()
//		{
//			m_eType = WidthTypeAuto;
//			m_dValue = 0;
//		}

//		double GetValue() const
//		{
//			return m_dValue;
//		}

//		EWidthType GetType() const
//		{
//			return m_eType;
//		}

//		bool IsNil() const
//		{
//			return WidthTypeNil == m_eType;
//		}

//		bool IsAuto() const
//		{
//			return WidthTypeAuto == m_eType;
//		}

//		bool IsDxa() const
//		{
//			return WidthTypeDxa == m_eType;
//		}

//		bool IsPercent() const
//		{
//			return WidthTypePct == m_eType;
//		}

//		static const WCHAR* GetNameWidthType( EWidthType type )
//		{
//			static const LPCWCH g_table[] = {
//				L"nil",
//				L"auto",
//				L"dxa",
//				L"pct"
//			};

//			return g_table[type];
//		}
//	};

//	// struct types
//	class CTableBorders
//	{
//	public:
//		CBorder m_oTop;
//		CBorder m_oLeft;
//		CBorder m_oBottom;
//		CBorder m_oRight;
//		CBorder m_oInsideH;
//		CBorder m_oInsideV;

//	public:
//		CTableBorders() :
//			m_oTop( L"w:top" ),
//			m_oLeft( L"w:start" ),
//			m_oBottom( L"w:bottom" ),
//			m_oRight( L"w:end" ),
//			m_oInsideH( L"w:insideH" ),
//			m_oInsideV( L"w:InsideV" )
//		{
//		}

//		void ToXml( NSDocxRenderer::CStringWriter& oWriter ) const
//		{
//			oWriter.WriteString( WSTR(L"<w:tblBorders>") );
//			
//			m_oTop.ToXml( oWriter );
//			m_oLeft.ToXml( oWriter );
//			m_oBottom.ToXml( oWriter );
//			m_oRight.ToXml( oWriter );
//			m_oInsideH.ToXml( oWriter );
//			m_oInsideV.ToXml( oWriter );
//			
//			oWriter.WriteString( WSTR(L"</w:tblBorders>") );
//		}
//	};

//	class CCellMargin
//	{
//	public:
//		CWidth m_oTop;
//		CWidth m_oLeft;
//		CWidth m_oBottom;
//		CWidth m_oRight;

//	public:
//		CCellMargin() :
//			m_oTop( L"w:top" ),
//			m_oLeft( L"w:start" ),
//			m_oBottom( L"w:bottom" ),
//			m_oRight( L"w:right" )
//		{
//		}

//		void ToXml( NSDocxRenderer::CStringWriter& oWriter ) const
//		{
//			oWriter.WriteString( WSTR(L"<w:tblCellMar>") );
//			
//			m_oTop.ToXml( oWriter );
//			m_oLeft.ToXml( oWriter );
//			m_oBottom.ToXml( oWriter );
//			m_oRight.ToXml( oWriter );

//			oWriter.WriteString( WSTR(L"</w:tblCellMar>") );
//		}
//	};
//	class CTablePosition
//	{
//	public:
//		long m_dLeft;   // left from text in pt
//		long m_dTop;    // top from text in pt
//		long m_dRight;  // right from text in pt
//		long m_dBottom; // bottom from text in pt

//		EAnchorLocation m_eHorAnchor;
//		EAnchorLocation m_eVerAnchor;

//		long m_dX;
//		long m_dY;

//	public:
//		CTablePosition()
//		{
//			m_dLeft   = 0;
//			m_dTop    = 0;
//			m_dRight  = 0;
//			m_dBottom = 0;

//			m_eHorAnchor = AnchorLocationPage;
//			m_eVerAnchor = AnchorLocationPage;

//			m_dX = 0;
//			m_dY = 0;


//		}
//	};

//	class CTableProperties
//	{
//	public:
//		CTableBorders m_oBorders;
//		CWidth        m_oWidth;

//	public:
//		CTableProperties()
//			: m_oWidth( L"w:tblW" )
//		{
//		}

//		void ToXml( NSDocxRenderer::CStringWriter& oWriter ) const
//		{
//			oWriter.WriteString( WSTR(L"<w:tblPr>") );
//			
//			m_oBorders.ToXml( oWriter );
//			m_oWidth.ToXml( oWriter );

//			oWriter.WriteString( WSTR(L"</w:tblPr>") );
//		}

//	};
//};

namespace NSDocxRenderer
{
	class CCell
	{
	public:

		CCell (double dWidth, long nSpan) : m_nGridSpan(nSpan)
		{
			m_lWidth		=	dWidth;
			m_pCurrentLine	=	NULL;
			m_pManagerLight =	NULL;
		};

		~CCell()
		{
			for( int i = 0; i < m_arrTextLines.m_nSize; ++i )
			{
				delete m_arrTextLines.m_aT[i];
			}
			m_arrTextLines.RemoveAll();

			for( int i = 0; i < m_arrParagraphs.m_nSize; ++i )
			{
				delete m_arrParagraphs.m_aT[i];
			}
			m_arrParagraphs.RemoveAll();
		}

		inline CParagraph* AddParagraph(TextAssociationType type)
		{
			CParagraph* pParagraph = new CParagraph( type );
			if( pParagraph )
			{
				if( m_arrParagraphs.Add( pParagraph ) )
					return pParagraph;

				delete pParagraph;
			}
			return NULL;
		}

		inline CTextLine* AddTextLine()
		{
			CTextLine* pTextLine = new CTextLine();
			if( pTextLine )
			{
				if( m_arrTextLines.Add( pTextLine ) )
					return pTextLine;

				delete pTextLine;
			}
			return NULL;
		}

		inline CTextLine* SetCurrentLineByBaseline(double dBaseLinePos)
		{
			if (NULL == m_pCurrentLine)
			{
				// пустая (в плане текста) страница
				m_pCurrentLine = new CTextLine();

				m_pCurrentLine->m_dBaselinePos = dBaseLinePos;
				m_arrTextLines.Add(m_pCurrentLine);
				return m_pCurrentLine;
			}
			if (fabs(m_pCurrentLine->m_dBaselinePos - dBaseLinePos) <= THE_SAME_STRING_Y_PRECISION_MM)
			{
				return m_pCurrentLine;
			}
			int nCount = m_arrTextLines.GetSize();
			for (int i = 0; i < nCount; ++i)
			{
				if (fabs(m_arrTextLines[i]->m_dBaselinePos - dBaseLinePos) <= THE_SAME_STRING_Y_PRECISION_MM)
				{
					m_pCurrentLine = m_arrTextLines[i];
					return m_pCurrentLine;
				}
			}

			// линия не нашлась - не беда - создадим новую
			m_pCurrentLine = new CTextLine();
			m_pCurrentLine->m_dBaselinePos = dBaseLinePos;
			m_arrTextLines.Add(m_pCurrentLine);
			return m_pCurrentLine;
		}

		inline void Build()
		{
			if( !m_pManagerLight )
				return;

			TextAssociationType type = m_eTextAssociationType;
			CFontManagerLight& oManagerLight = *m_pManagerLight;

			switch (type)
			{
			case TextAssociationTypeDefault:
			case TextAssociationTypeLine:
				{
					int nCount = m_arrTextLines.GetSize();
					for (int i = 0; i < nCount; ++i)
					{
						CTextLine* pTextLine = m_arrTextLines[i];

						CParagraph* pParagraph = new CParagraph(type);
						pParagraph->m_pManagerLight = &oManagerLight;
						pParagraph->m_bIsTextFrameProperties = true;

						pParagraph->m_dLeft	= pTextLine->m_dX;
						pParagraph->m_dTop	= pTextLine->m_dBaselinePos - pTextLine->m_dHeight + pTextLine->m_dBaselineOffset;
						pParagraph->m_dHeight = pTextLine->m_dHeight;

						pParagraph->m_arLines.Add(pTextLine);

						m_arrParagraphs.Add(pParagraph);
					}

					// удалим все линии
					m_arrTextLines.RemoveAll();
					break;
				}
			case TextAssociationTypeBlock:
				{
					int nCount = m_arrTextLines.GetSize();
					if (0 == nCount)
						break;

					CTextLine* pFirstLine = m_arrTextLines[0];

					CParagraph* pParagraph = new CParagraph(type);
					pParagraph->m_pManagerLight = &oManagerLight;
					pParagraph->m_bIsTextFrameProperties = true;

					pParagraph->m_dLeft	= pFirstLine->m_dX;
					pParagraph->m_dTop	= pFirstLine->m_dBaselinePos - pFirstLine->m_dHeight + pFirstLine->m_dBaselineOffset;

					pParagraph->m_arLines.Add(pFirstLine);

					m_arrParagraphs.Add(pParagraph);

					for (int i = 1; i < nCount; ++i)
					{
						CTextLine* pTextLine = m_arrTextLines[i];

						CParagraph* pParagraph = new CParagraph(type);
						pParagraph->m_pManagerLight = &oManagerLight;
						pParagraph->m_bIsTextFrameProperties = true;

						if (((fabs(pTextLine->m_dBaselinePos - pTextLine->m_dHeight - pFirstLine->m_dBaselinePos) > STANDART_STRING_HEIGHT_MM) && (pTextLine->m_dX == pFirstLine->m_dX)) ||
							((pTextLine->m_dX != pFirstLine->m_dX) && (pTextLine->m_dBaselinePos != pFirstLine->m_dBaselinePos)))
						{
							pParagraph->m_dLeft	= pTextLine->m_dX;
							pParagraph->m_dTop	= pTextLine->m_dBaselinePos - pTextLine->m_dHeight + pTextLine->m_dBaselineOffset;
						}
						else
						{
							pParagraph->m_dLeft	= pFirstLine->m_dX;
							pParagraph->m_dTop	= pFirstLine->m_dBaselinePos - pFirstLine->m_dHeight + pFirstLine->m_dBaselineOffset;
						}

						pFirstLine = pTextLine;

						pParagraph->m_arLines.Add(pTextLine);
						m_arrParagraphs.Add(pParagraph);
					}

					// удалим все линии
					m_arrTextLines.RemoveAll();
					break;
				}
			case TextAssociationTypeNoFrames:
				{
					SortElements(m_arrTextLines);
					Merge(STANDART_STRING_HEIGHT_MM / 3);

					double previousStringOffset = 0;
					int nCount = m_arrTextLines.GetSize();
					for (int i = 0; i < nCount; ++i)
					{
						CTextLine* pTextLine = m_arrTextLines[i];

						CParagraph* pParagraph = new CParagraph(type);
						pParagraph->m_pManagerLight = &oManagerLight;
						pParagraph->m_bIsTextFrameProperties = false;

						pParagraph->m_dLeft	= pTextLine->m_dX;

						double dBeforeSpacing = (pTextLine->m_dBaselinePos - previousStringOffset - pTextLine->m_dHeight + pTextLine->m_dBaselineOffset);

						pParagraph->m_dSpaceBefore = max(dBeforeSpacing, 0);

						double dHeight = 1;
						if (pTextLine->m_dHeight != 0)
						{
							dHeight = pTextLine->m_dHeight;

							if (dBeforeSpacing < 0)
								dHeight += dBeforeSpacing;
						}

						pParagraph->m_dHeight = dHeight;

						previousStringOffset = pTextLine->m_dBaselinePos + pTextLine->m_dBaselineOffset;

						pParagraph->m_arLines.Add(pTextLine);
						m_arrParagraphs.Add(pParagraph);
					}

					m_arrTextLines.RemoveAll();
					break;
				}
			}
		}

		inline void Merge(double dAffinity)
		{
			int nCount = m_arrTextLines.GetSize();
			if (1 < nCount)
			{
				CTextLine* pPrev = m_arrTextLines[0];
				for (int i = 1; i < nCount; ++i)
				{
					CTextLine* pNext = m_arrTextLines[i];

					if (fabs(pNext->m_dBaselinePos - pPrev->m_dBaselinePos) < dAffinity)
					{
						pPrev->Merge(pNext);

						pNext->m_arConts.RemoveAll();
						RELEASEOBJECT(pNext);

						m_arrTextLines.RemoveAt(i);
						--i;
						--nCount;
						continue;
					}
					pPrev = pNext;
				}
			}
		}

		inline bool GetVerBoundParagraphs(double& dTop, double& dBottom)
		{
			if( m_arrParagraphs.m_nSize <= 0 )
				return false;

			double y1 = m_arrParagraphs.m_aT[0]->m_dTop;
			double y2 = y1 + m_arrParagraphs.m_aT[0]->m_dHeight + m_arrParagraphs.m_aT[0]->m_dSpaceBefore * 2;

			for( int i = 1; i < m_arrParagraphs.m_nSize; i++ )
			{
				y2 += m_arrParagraphs.m_aT[i]->m_dTop;
				y2 += m_arrParagraphs.m_aT[i]->m_dHeight + m_arrParagraphs.m_aT[i]->m_dSpaceBefore * 2;
			}

			if( y1 < dTop ) dTop = y1;
			if( y2 > dBottom ) dBottom = y2;

			return true;
		}
		//
		inline void ToXml(CStringWriter& oWriter)
		{
			oWriter.WriteString(WSTR(L"<w:tc><w:tcPr>"));
			oWriter.WriteFormatString(L"<w:tcW w:w=\"%d\" w:type=\"dxa\"/>", MMtoDXA(m_lWidth));

			if (m_nGridSpan)
				oWriter.WriteFormatString(L"<w:gridSpan w:val=\"%d\"/>", m_nGridSpan + 1); 

			oWriter.WriteString(WSTR(L"</w:tcPr>"));

			int nCountPara = m_arrParagraphs.GetSize();
			if( nCountPara > 0 )
			{
				for(int i = 0; i < m_arrParagraphs.m_nSize; ++i)
				{
					m_arrParagraphs.m_aT[i]->ToXml(oWriter);
				}
			}
			else
			{
				oWriter.WriteString(WSTR(L"<w:p/>"));
			}

			oWriter.WriteString(WSTR(L"</w:tc>"));
		}

	protected:

		ATL::CSimpleArray<CParagraph*>	m_arrParagraphs;
		ATL::CSimpleArray<CTextLine*>	m_arrTextLines;
		CTextLine*						m_pCurrentLine;

	public:
		double							m_lWidth; // in mm
		int								m_nGridSpan;

		TextAssociationType				m_eTextAssociationType;
		CFontManagerLight*				m_pManagerLight;
	};

	class CRow
	{
	public:

		CRow (double dHeight)
		{
			m_dHeight	=	dHeight;
		}

		~CRow ()
		{
			int nCount		=	m_arrCells.GetSize();
			CCell** pItem	=	m_arrCells.GetData();

			for (int i = 0; i < nCount; ++i, ++pItem)
				delete pItem[0];

			m_arrCells.RemoveAll();
		}
		//
		inline CCell* AddCell(double dWidth, long nSpan)
		{
			CCell* pCell = new CCell(dWidth,nSpan);
			if(pCell)
			{
				if(0 == m_arrCells.Add(pCell))
					RELEASEOBJECT(pCell);
			}

			return pCell;
		}

		inline int GetCountCells() const
		{
			return m_arrCells.GetSize();
		}

		inline const CCell* GetCell(int index) const
		{
			return m_arrCells.GetData()[index];
		}

		inline CCell* GetCell(int index)
		{
			return m_arrCells.m_aT[index];
		}

		inline void GetCellPositions(long arrPositions[]) const
		{
			long lCurPos = 0;

			for(int i = 0; i < m_arrCells.m_nSize; ++i)
			{
				lCurPos += MMtoDXA( m_arrCells.m_aT[0]->m_lWidth );
				arrPositions[i] = lCurPos;
			}
		}

		//
		inline void ToXml(CStringWriter& oWriter) const
		{
			double y1 = m_dHeight;
			double y2 = 0;

			for( int i = 0; i < m_arrCells.m_nSize; ++i )
			{
				CCell* pCell = m_arrCells.m_aT[i];
				pCell->Build();
				pCell->GetVerBoundParagraphs( y1, y2 );
			}

			long lMarginTop		=	0;
			long lMarginBottom	=	0;

			if (y1 < y2)
			{
				lMarginTop = MMtoDXA(y1);
				lMarginBottom = MMtoDXA(m_dHeight - y2);
			}

			oWriter.WriteString( WSTR(L"<w:tr>") );
			oWriter.WriteFormatString( L"<w:trPr><w:trHeight w:val=\"%d\" /></w:trPr>", (long)MMtoDXA(max(0, m_dHeight)) );
			oWriter.WriteFormatString( L"<w:tblPrEx><w:tblCellMar><w:bottom w:w=\"%d\" w:type=\"dxa\"/></w:tblCellMar></w:tblPrEx>", max(0, lMarginBottom) );

			for( int i = 0; i < m_arrCells.m_nSize; ++i )
			{
				CCell* pCell = m_arrCells.m_aT[i];
				pCell->ToXml( oWriter );
			}

			oWriter.WriteString( WSTR(L"</w:tr>") );
		}

	private:
		ATL::CSimpleArray<CCell*>	m_arrCells;

	public:
		double						m_dHeight;
	};

	class CDocxTable : public CBaseItem
	{
	public:
		CDocxTable() 
		{
			m_eType			=	etTable;

			m_nCountRows	=	0;

			m_dX			=	0;
			m_dY			=	0;
			m_dWidth		=	0;
			m_dHeight		=	0;
			m_dTextLeft		=	0;
			m_dTextRight	=	0;
		}

		~CDocxTable()
		{
			int nCount		=	m_arrRows.GetSize();
			CRow** pItem	=	m_arrRows.GetData();

			for(int i = 0; i < nCount; ++i, ++pItem)
			{
				RELEASEOBJECT(pItem[0]);
			}

			m_arrRows.RemoveAll();
		}

		//
		inline void AddRow(double dHeight)
		{
			CRow* pRow = new CRow(dHeight);
			if(pRow)
			{
				if(0 == m_arrRows.Add(pRow))
				{
					RELEASEOBJECT(pRow);
					return;
				}
			}

			m_nCountRows += 1;
		}

		inline void AddColumn(double dWidth)	//	DEPRECATED
		{

		}

		inline void AddCell(int nRow, double dWidth, long nSpan)
		{
			if (m_nCountRows)
			{
				m_arrRows[nRow]->AddCell(dWidth,nSpan);
			}
		}

		inline void AddGridCol(const ATL::CAtlArray<double>& arrCol)
		{
			m_arrGridCol.Copy(arrCol);
		}

		inline CCell* GetCell(int nRow, int nPos)
		{
			return m_arrRows.m_aT[nRow]->GetCell( nPos );
		}

		//
		inline  bool IsBigger(const CDocxTable* pSrc)
		{
			return (m_dY > pSrc->m_dY) ? true : false;
		}

		inline  bool IsBiggerOrEqual(const CDocxTable* pSrc)
		{
			return (m_dY >= pSrc->m_dY) ? true : false;
		}

		//
		virtual void ToXml(NSDocxRenderer::CStringWriter& oWriter)
		{
			oWriter.WriteFormatString(L"<w:tbl><w:tblPr>");

			WriteWidth(oWriter);
			WriteBorders(oWriter);
			WriteCellMargins(oWriter);
			WritePosition(oWriter);

			oWriter.WriteString(WSTR(L"<w:tblLayout w:type=\"autofit\"/>"));
			oWriter.WriteString(WSTR(L"</w:tblPr>"));

			WriteGrid(oWriter);
			WriteRows(oWriter);

			oWriter.WriteString(WSTR(L"</w:tbl>"));
		}

	protected:

		inline void WriteWidth(CStringWriter& oWriter) const
		{
			if (m_arrGridCol.GetCount())
			{
				long nWidth = 0;
				for (size_t i = 0; i < m_arrGridCol.GetCount(); ++i)			
					nWidth += MMtoDXA(m_arrGridCol[i]);

				oWriter.WriteFormatString( L"<w:tblW w:w=\"%d\" w:type=\"dxa\"/>", nWidth );
			}
		}

		inline void WriteBorders(CStringWriter& oWriter) const
		{
			oWriter.WriteString( WSTR(L"<w:tblBorders>\
									   <w:top w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>\
									   <w:start w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>\
									   <w:bottom w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>\
									   <w:end w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>\
									   <w:insideH w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>\
									   <w:insideV w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>\
									   </w:tblBorders>") );
		}

		inline void WriteGrid(CStringWriter& oWriter) const
		{
			if (m_arrGridCol.GetCount())
			{
				oWriter.WriteString(WSTR(L"<w:tblGrid>"));

				for (size_t i = 0; i < m_arrGridCol.GetCount(); ++i)			
					oWriter.WriteFormatString( L"<w:gridCol w:w=\"%d\"/>", MMtoDXA(m_arrGridCol[i]) );

				oWriter.WriteString(WSTR(L"</w:tblGrid>"));
			}
		}

		inline void WriteRows(CStringWriter& oWriter) const
		{
			for(int i = 0; i < m_arrRows.GetSize(); ++i)
			{
				m_arrRows[i]->ToXml(oWriter);
			}
		}

		inline void WriteCellMargins(CStringWriter& oWriter) const
		{
			oWriter.WriteString( WSTR(L"<w:tblCellMar><w:top w:w=\"0\" w:type=\"dxa\"/><w:start w:w=\"0\" w:type=\"dxa\"/><w:bottom w:w=\"0\" w:type=\"dxa\"/><w:end w:w=\"0\" w:type=\"dxa\"/></w:tblCellMar>") );
		}

		inline void WritePosition(CStringWriter& oWriter) const
		{
			oWriter.WriteFormatString( L"<w:tblpPr w:topFromText=\"0\" w:leftFromText=\"%d\" w:bottomFromText=\"0\" w:rightFromText=\"%d\" w:horzAnchor=\"page\" w:tblpX=\"%d\" w:vertAnchor=\"page\" w:tblpY=\"%d\"/>", MMtoDXA( m_dTextLeft ), MMtoDXA( m_dTextRight ), MMtoDXA( m_dX ), MMtoDXA( m_dY ) );
		}

		//
		static int CmpLong(const void *p1, const void *p2)
		{
			return *((long*)p1) - *((long*)p2);
		}

	protected:

		int							m_nCountRows;

		ATL::CSimpleArray<CRow*>	m_arrRows;
		ATL::CAtlArray<double>		m_arrGridCol;

	public:

		double						m_dX; // in mm
		double						m_dY; // in mm
		double						m_dWidth;
		double						m_dHeight;
		double						m_dTextLeft;
		double						m_dTextRight;
	};
}