#pragma once

#include <float.h>
#include <math.h>

static double EPS		=	1.5;
static double END_EPS	=	4.0;
static double SORT_EPS	=	3.5;

typedef ATL::CAtlArray<double> DoubleArray;

namespace NSDocxRenderer	//	DEPRECATED
{
	class CBlock
	{
	public:
		double m_x1, m_y1, m_x2, m_y2;

		long m_color;
		long m_alpha;
		size_t m_id;

	public:

		CBlock()
		{
			m_x1 = 0;
			m_y1 = 0;
			m_x2 = 0;
			m_y2 = 0;

			m_color = 0;
			m_alpha = 0;
			m_id = 0;
		}

		CBlock( double _x1, double _y1, double _x2, double _y2 )
		{
			if( _x1 < _x2 )
			{
				m_x1 = _x1;
				m_x2 = _x2;
			}
			else
			{
				m_x1 = _x2;
				m_x2 = _x1;
			}

			if( _y1 < _y2 )
			{
				m_y1 = _y1;
				m_y2 = _y2;
			}
			else
			{
				m_y1 = _y2;
				m_y2 = _y1;
			}

			m_color = 0;
			m_alpha = 0;
			m_id = 0;
		}

		double GetWidth() const
		{
			return m_x2 - m_x1;
		}

		double GetHeight() const
		{
			return m_y2 - m_y1;
		}

		double GetCenterX() const
		{
			return (m_x1 + m_x2) * 0.5;
		}

		double GetCenterY() const
		{
			return (m_y1 + m_y2) * 0.5;
		}

		bool Intersect( const CBlock& out ) const
		{
			double l = max(m_x1, out.m_x1);
			double t = max(m_y1, out.m_y1);
			double r = min(m_x2, out.m_x2);
			double b = min(m_y2, out.m_y2);

			if( l > r || t > b )
				return false;

			if( l < m_x1 && t < m_y1 && r > m_x2 && b > m_y2 )
				return false;

			return true;
		}

		bool Intersect( const CBlock& out, CBlock& result ) const  
		{
			double l = max(m_x1, out.m_x1);
			double t = max(m_y1, out.m_y1);
			double r = min(m_x2, out.m_x2);
			double b = min(m_y2, out.m_y2);

			if( l > r || t > b )
				return false;

			result.m_x1 = l;
			result.m_y1 = t;
			result.m_x2 = r;
			result.m_y2 = b;

			return true;
		}
	};

	class CTable
	{
	private:
		double m_x;
		double m_y;
		double m_width;
		double m_height;

		ATL::CSimpleArray<double> m_columns;
		ATL::CSimpleArray<double> m_rows;
		ATL::CSimpleArray<CBlock> m_blocks;

	public:
		CTable()
		{
			m_x = 0;
			m_y = 0;
			m_width = 0;
			m_height = 0;
		}


		void Clear()
		{
			m_x = 0;
			m_y = 0;
			m_width = 0;
			m_height = 0;

			m_columns.RemoveAll();
			m_rows.RemoveAll();
		}

		void SetStartPos( double x, double y )
		{
			m_x = x;
			m_y = y;
		}
		void AddRow( double height )
		{
			if( height < DBL_EPSILON )
				return;

			if( m_rows.Add( m_height ) )
			{
				m_height += height;
			}
		}

		void AddColumn( double width )
		{
			if( width < DBL_EPSILON )
				return;

			if( m_columns.Add( m_width ) )
			{
				m_width += width;
			}
		}


		bool IsEmpty() const
		{
			return (!m_width || !m_height);
		}

		double GetX() const
		{
			return m_x;
		}

		double GetY() const
		{
			return m_y;
		}

		double GetWidth() const
		{
			return m_width;
		}

		double GetHeight() const
		{
			return m_height;
		}

		int GetCountRows() const
		{
			return m_rows.GetSize();
		}

		int GetCountColumns() const
		{
			return m_columns.GetSize();
		}

		double GetRowPosition( int id ) const
		{
			return m_rows[id];
		}

		double GetColumnPosition( int id ) const
		{
			return m_columns[id];
		}

		double GetRowSize( int id ) const
		{
			return ((id < m_rows.GetSize() - 1) ? m_rows.m_aT[id + 1] : m_height) - m_rows.m_aT[id];
		}

		double GetColumnSize( int id ) const
		{
			return ((id < m_columns.GetSize() - 1) ? m_columns.m_aT[id + 1] : m_width) - m_columns.m_aT[id];
		}


		void AddBlock( const CBlock& block )
		{
			m_blocks.Add( block );
		}

		CBlock* GetBlocks() const
		{
			return m_blocks.GetData();
		}

		int GetCountBlocks() const
		{
			return m_blocks.GetSize();
		}


		bool IsPointInTable( double x, double y, int* pRow, int* pCol ) const
		{
			y -= m_y;

			if( y <= 0 )
				return false;

			if( y >= m_height )
				return false;

			x -= m_x;

			if( x <= 0 )
				return false;

			if( x >= m_width )
				return false;

			for( int i = m_columns.m_nSize - 1; i >= 0; i-- )
			{
				if( x >= m_columns.m_aT[i] )
				{
					*pCol = i;
					break;
				}
			}

			for( int i = m_rows.m_nSize - 1; i >= 0; i-- )
			{
				if( y >= m_rows.m_aT[i] )
				{
					*pRow = i;
					break;
				}
			}

			return true;
		}

		bool IsRectInTable( double x1, double y1, double x2, double y2, int* pRow, int* pCol ) const
		{
			if( y1 > y2 )
			{
				double tmp = y1;
				y1 = y2;
				y2 = tmp;
			}

			y1 -= m_y;
			y2 -= m_y;

			if( y2 <= 0 /*|| y2 >= m_height*/ )
				return false;

			if( y1 <= 0 || y1 >= m_height )
				return false;

			if( x1 > x2 )
			{
				double tmp = x1;
				x1 = x2;
				x2 = tmp;
			}

			x1 -= m_x;
			x2 -= m_x;

			if( x2 <= 0 /*|| x2 >= m_width*/ )
				return false;

			if( x1 <= 0 || x1 >= m_width )
				return false;

			*pCol = 0;
			for( int i = m_columns.m_nSize - 1; i >= 0; i-- )
			{
				if( x1 >= m_columns.m_aT[i] )
				{
					*pCol = i;
					break;
				}
			}

			*pRow = 0;
			for( int i = m_rows.m_nSize - 1; i >= 0; i-- )
			{
				if( y1 >= m_rows.m_aT[i] )
				{
					*pRow = i;
					break;
				}
			}

			return true;
		}

	};

	class CTableDetector
	{
	protected:
		class TGroup;
		class TBlock : public CBlock
		{
		public:
			TGroup* m_group;

		public:
			TBlock()
			{
				m_group = NULL;
			}

			TBlock( double _x1, double _y1, double _x2, double _y2 ) 
				: CBlock( _x1, _y1, _x2, _y2 )
			{
				m_group = NULL;
			}
		};

		class TCrossPoint
		{
		public:		
			double m_x, m_y;
			TBlock* m_pHorBlock;
			TBlock* m_pVerBlock;

		public:
			TCrossPoint()
			{
				m_x = 0;
				m_y = 0;
				m_pHorBlock = NULL;
				m_pVerBlock = NULL;
			}

			TCrossPoint( double x, double y, TBlock* pHorBlock, TBlock* pVerBlock )
			{
				Set( x, y, pHorBlock, pVerBlock );
			}

			void Set( double x, double y, TBlock* pHorBlock, TBlock* pVerBlock )
			{
				m_x = x;
				m_y = y;
				m_pHorBlock = pHorBlock;
				m_pVerBlock = pVerBlock;
			}
		};

		class TGroup
		{
		public:
			CAtlArray<TBlock*> m_blocks;
			CAtlArray<TCrossPoint*> m_points;
		};

		CAtlList<TBlock> m_HorBlocks;
		CAtlList<TBlock> m_VerBlocks;
		CAtlList<CTable> m_Tables;

		POSITION m_pCurTable;

	public:
		CTableDetector()
		{
			m_pCurTable = NULL;
		}

		void Clear()
		{
			m_HorBlocks.RemoveAll();
			m_VerBlocks.RemoveAll();
			m_Tables.RemoveAll();

			m_pCurTable = NULL;
		}

		CBlock* AddBlock( const double points[4][2] )
		{
			if( points[0][0] != points[1][0] )
			{
				if( abs(points[0][1] - points[1][1]) > FLT_EPSILON || 
					abs(points[1][0] - points[2][0]) > FLT_EPSILON || 
					abs(points[2][1] - points[3][1]) > FLT_EPSILON ||
					abs(points[3][0] - points[0][0]) > FLT_EPSILON )
				{
					return NULL;
				}
			}
			else
			{
				if( abs(points[0][0] - points[1][0]) > FLT_EPSILON || 
					abs(points[1][1] - points[2][1]) > FLT_EPSILON || 
					abs(points[2][0] - points[3][0]) > FLT_EPSILON ||
					abs(points[3][1] - points[0][1]) > FLT_EPSILON )
				{
					return NULL;
				}
			}

			TBlock block( points[0][0], points[0][1], points[2][0], points[2][1] );

			if( block.GetWidth() < block.GetHeight() )
			{
				POSITION pos = m_VerBlocks.AddTail( block );
				if( pos )
					return &m_VerBlocks.GetAt( pos );
			}
			else
			{
				POSITION pos = m_HorBlocks.AddTail( block );
				if( pos )
					return &m_HorBlocks.GetAt( pos );
			}

			return NULL;
		}

		CBlock* AddBlock( double x, double y, double width, double height )
		{
			TBlock block( x, y, x + width, y + height );

			if( block.GetWidth() < block.GetHeight() )
			{
				POSITION pos = m_VerBlocks.AddTail( block );
				if( pos )
					return &m_VerBlocks.GetAt( pos );
			}
			else
			{
				POSITION pos = m_HorBlocks.AddTail( block );
				if( pos )
					return &m_HorBlocks.GetAt( pos );
			}

			return NULL;
		}


		void Calculate()
		{
			int nCountHorBlocks = (int)m_HorBlocks.GetCount();
			int nCountVerBlocks = (int)m_VerBlocks.GetCount();

			// удаление пересекающихся блоков
			if( nCountHorBlocks > 1 )
			{
				TBlock result;

				POSITION i = m_HorBlocks.GetHeadPosition();
				while( i != NULL )
				{
					TBlock* pCurBlock = &m_HorBlocks.GetAt( i );

					POSITION j = i;
					m_HorBlocks.GetNext( j );
					while( j != NULL )
					{
						TBlock* pBlock = &m_HorBlocks.GetAt( j );

						if( pCurBlock->Intersect( *pBlock, result ) )
						{
							if( result.GetWidth() > result.GetHeight() )
							{
								if( pCurBlock->GetHeight() > pBlock->GetHeight() )
								{
									POSITION del = i;
									m_HorBlocks.GetNext( i );
									m_HorBlocks.RemoveAt( del );
									pCurBlock = NULL;
								}
								else
								{
									m_HorBlocks.RemoveAt( j );
								}
								break;
							}
						}

						m_HorBlocks.GetNext( j );
					}

					if( pCurBlock )
						m_HorBlocks.GetNext( i );
				}
			}

			if( nCountVerBlocks > 1 )
			{
				TBlock result;

				POSITION i = m_VerBlocks.GetHeadPosition();
				while( i != NULL )
				{
					TBlock* pCurBlock = &m_VerBlocks.GetAt( i );

					POSITION j = i;
					m_VerBlocks.GetNext( j );
					while( j != NULL )
					{
						TBlock* pBlock = &m_VerBlocks.GetAt( j );

						if( pCurBlock->Intersect( *pBlock, result ) )
						{
							if( result.GetHeight() > result.GetWidth() )
							{
								if( pCurBlock->GetWidth() > pBlock->GetWidth() )
								{
									POSITION del = i;
									m_VerBlocks.GetNext( i );
									m_VerBlocks.RemoveAt( del );
									pCurBlock = NULL;
								}
								else
								{
									m_VerBlocks.RemoveAt( j );
								}
								break;
							}
						}

						m_VerBlocks.GetNext( j );
					}

					if( pCurBlock )
						m_VerBlocks.GetNext( i );
				}
			}


			nCountHorBlocks = (int)m_HorBlocks.GetCount();
			nCountVerBlocks = (int)m_VerBlocks.GetCount();

			if( nCountHorBlocks > 1 && nCountVerBlocks > 1 )
			{
				CAtlList<TCrossPoint> points;
				CAtlList<TGroup> groups;

				for( POSITION i = m_HorBlocks.GetHeadPosition(); i != NULL; m_HorBlocks.GetNext( i ) )
				{
					TBlock* pHorBlock = &m_HorBlocks.GetAt( i );

					for( POSITION j = m_VerBlocks.GetHeadPosition(); j != NULL; m_VerBlocks.GetNext( j ) )
					{
						TBlock* pVerBlock = &m_VerBlocks.GetAt( j );

						if( pHorBlock->Intersect( *pVerBlock ) )
						{
							double cx = pVerBlock->GetCenterX();
							double cy = pHorBlock->GetCenterY();

							points.AddTail( TCrossPoint( cx, cy, pHorBlock, pVerBlock ) );

							TGroup* pHorGroup = pHorBlock->m_group;
							TGroup* pVerGroup = pVerBlock->m_group;

							if( pHorGroup == pVerGroup )
							{
								if( !pHorGroup )
								{
									if( groups.AddTail() )
									{
										TGroup* pNewGroup = &groups.GetTail();

										pNewGroup->m_blocks.Add( pHorBlock );
										pHorBlock->m_group = pNewGroup;

										pNewGroup->m_blocks.Add( pVerBlock );
										pVerBlock->m_group = pNewGroup;
									}
								}
							}
							else
							{
								if( !pHorGroup )
								{
									pVerGroup->m_blocks.Add( pHorBlock );
									pHorBlock->m_group = pVerGroup;
								}
								else if( !pVerGroup )
								{
									pHorGroup->m_blocks.Add( pVerBlock );
									pVerBlock->m_group = pHorGroup;
								}
								else
								{
									for( size_t i = 0; i < pVerGroup->m_blocks.GetCount(); i++ )
									{
										pVerGroup->m_blocks[i]->m_group = pHorGroup;
									}

									pHorGroup->m_blocks.Append( pVerGroup->m_blocks );
									pVerGroup->m_blocks.RemoveAll();
								}
							}
						}
					}
				}

				for( POSITION i = points.GetHeadPosition(); i != NULL; points.GetNext( i ) )
				{
					TCrossPoint* pPoint = &points.GetAt( i );

					TBlock* pHorBlock = pPoint->m_pHorBlock;
					TBlock* pVerBlock = pPoint->m_pVerBlock;

					if( pHorBlock->m_group != pVerBlock->m_group )
					{
						continue;
					}

					pHorBlock->m_group->m_points.Add( pPoint );
				}

				int nCurTable = 0;

				for( POSITION i = groups.GetHeadPosition(); i != NULL; groups.GetNext( i ) )
				{
					TGroup* pGroup = &groups.GetAt( i );

					CAtlArray<TCrossPoint*>& data = pGroup->m_points;

					size_t nCountPoints = data.GetCount();
					if( nCountPoints < 4 )
					{
						continue;
					}

					double* pBufferY = new double[nCountPoints];
					double* pBufferX = new double[nCountPoints];
					TBlock** pBufferPtr = new TBlock*[nCountPoints * 2];

					if( pBufferY && pBufferX && pBufferPtr )
					{
						for( size_t k = 0; k < nCountPoints; k++ )
						{
							pBufferX[k] = data[k]->m_x;
							pBufferY[k] = data[k]->m_y;
							pBufferPtr[k*2 + 0] = data[k]->m_pHorBlock;
							pBufferPtr[k*2 + 1] = data[k]->m_pVerBlock;
						}

						qsort( pBufferX, nCountPoints, sizeof(double), CmpDouble );
						qsort( pBufferY, nCountPoints, sizeof(double), CmpDouble );
						qsort( pBufferPtr, nCountPoints * 2, sizeof(TBlock*), CmpPtr );

						POSITION pPosTable = m_Tables.AddTail();
						if( pPosTable )
						{
							CTable& table = m_Tables.GetAt( pPosTable );
							table.SetStartPos( pBufferX[0], pBufferY[0] );

							size_t pos = 0;
							for( size_t k = 1; k < nCountPoints; k++ )
							{
								double d = abs(pBufferX[k] - pBufferX[pos]);

								if( d < 1 )
								{
									continue;
								}
								pos = k;
								table.AddColumn( d );
							}

							pos = 0;
							for( size_t k = 1; k < nCountPoints; k++ )
							{
								double d = abs(pBufferY[k] - pBufferY[pos]);

								if( d < 1 )
								{
									continue;
								}
								pos = k;
								table.AddRow( d );
							}

							TBlock* ptr = NULL;
							for( size_t k = 0; k < nCountPoints * 2; k++ )
							{
								if( pBufferPtr[k] != ptr )
								{
									table.AddBlock( *(pBufferPtr[k]) );
									ptr = pBufferPtr[k];
								}
							}

							if( table.IsEmpty() )
								m_Tables.RemoveAt( pPosTable );
						}
					}

					delete [] pBufferY;
					delete [] pBufferX;
					delete [] pBufferPtr;
				}
			}
		}

		CTable* GetFirstTable()
		{
			m_pCurTable = m_Tables.GetHeadPosition();

			return GetNextTable();
		}

		CTable* GetNextTable()
		{
			if( !m_pCurTable )
				return NULL;

			return &m_Tables.GetNext( m_pCurTable );
		}

		size_t GetCountTables() const
		{
			return m_Tables.GetCount();
		}


	private:
		static int CmpDouble( const void* p1, const void* p2 )
		{
			return (*((double*)p1) <= *((double*)p2)) ? -1 : 1;
		}

		static int CmpPtr( const void* p1, const void* p2 )
		{
			return (int)(*((LONG_PTR*)p1) - *((LONG_PTR*)p2));
		}

	};

	class CTableDetector2
	{
	protected:
		class TGroup;

		class TInputBlock : public CBlock
		{
		public:
			TInputBlock* m_pNextHorBlock;
			TInputBlock* m_pNextVerBlock;

			BOOL m_bHor;
			BOOL m_bWrite;

		public:
			TInputBlock( double _x1, double _y1, double _x2, double _y2 ) 
				: CBlock( _x1, _y1, _x2, _y2 )
			{
				m_pNextHorBlock = NULL;
				m_pNextVerBlock = NULL;

				m_bHor = (m_y2 - m_y1) <= (m_x2 - m_x1);
				m_bWrite = FALSE;
			}
		};

		class TMultiBlock
		{
		public:
			double m_x1, m_y1, m_x2, m_y2;
			TInputBlock* m_pSourceBlocks;
			TGroup* m_group;

		public:
			double GetWidth() const
			{
				return m_x2 - m_x1;
			}

			double GetHeight() const
			{
				return m_y2 - m_y1;
			}

			double GetCenterX() const
			{
				return (m_x2 + m_x1) * 0.5;
			}

			double GetCenterY() const
			{
				return (m_y2 + m_y1) * 0.5;
			}

			TMultiBlock()
			{
				m_group = NULL;
			}

			bool Intersect( const TMultiBlock& out ) const
			{
				double l = max(m_x1, out.m_x1);
				double t = max(m_y1, out.m_y1);
				double r = min(m_x2, out.m_x2);
				double b = min(m_y2, out.m_y2);

				if( l > r || t > b )
					return false;

				if( l < m_x1 && t < m_y1 && r > m_x2 && b > m_y2 )
					return false;

				return true;
			}

			bool Intersect( const TMultiBlock& out, TMultiBlock& result ) const
			{
				double l = max(m_x1, out.m_x1);
				double t = max(m_y1, out.m_y1);
				double r = min(m_x2, out.m_x2);
				double b = min(m_y2, out.m_y2);

				if( l > r || t > b )
					return false;

				result.m_x1 = l;
				result.m_y1 = t;
				result.m_x2 = r;
				result.m_y2 = b;

				return true;
			}
		};

		ATL::CSimpleArray<TInputBlock*> m_arrInputBlocks;

		//ATL::CSimpleArray<TMultiBlock*> m_arrHorBlocks;
		//ATL::CSimpleArray<TMultiBlock*> m_arrVerBlocks;

		class TCrossPoint
		{
		public:		
			double m_x, m_y;
			TMultiBlock* m_pHorBlock;
			TMultiBlock* m_pVerBlock;

		public:
			TCrossPoint()
			{
				m_x = 0;
				m_y = 0;
				m_pHorBlock = NULL;
				m_pVerBlock = NULL;
			}

			TCrossPoint( double x, double y, TMultiBlock* pHorBlock, TMultiBlock* pVerBlock )
			{
				Set( x, y, pHorBlock, pVerBlock );
			}

			void Set( double x, double y, TMultiBlock* pHorBlock, TMultiBlock* pVerBlock )
			{
				m_x = x;
				m_y = y;
				m_pHorBlock = pHorBlock;
				m_pVerBlock = pVerBlock;
			}
		};

		class TGroup
		{
		public:
			CAtlArray<TMultiBlock*> m_blocks;
			CAtlArray<TCrossPoint*> m_points;
		};

		CAtlList<TMultiBlock> m_HorBlocks;
		CAtlList<TMultiBlock> m_VerBlocks;
		CAtlList<CTable> m_Tables;

		POSITION m_pCurTable;

	public:
		CTableDetector2()
		{
		}

		~CTableDetector2()
		{
			Clear();
		}

		void Clear()
		{
			for( int i = 0; i < m_arrInputBlocks.m_nSize; i++ )
			{
				delete m_arrInputBlocks.m_aT[i];
			}
			m_arrInputBlocks.RemoveAll();

			//for( int i = 0; i < m_arrHorBlocks.m_nSize; i++ )
			//{
			//	delete m_arrHorBlocks.m_aT[i];
			//}
			//m_arrHorBlocks.RemoveAll();

			//for( int i = 0; i < m_arrVerBlocks.m_nSize; i++ )
			//{
			//	delete m_arrVerBlocks.m_aT[i];
			//}
			//m_arrVerBlocks.RemoveAll();

			m_HorBlocks.RemoveAll();
			m_VerBlocks.RemoveAll();
		}

		CBlock* AddBlock( const double points[4][2] )
		{
			if( points[0][0] != points[1][0] )
			{
				if( abs(points[0][1] - points[1][1]) > FLT_EPSILON || 
					abs(points[1][0] - points[2][0]) > FLT_EPSILON || 
					abs(points[2][1] - points[3][1]) > FLT_EPSILON ||
					abs(points[3][0] - points[0][0]) > FLT_EPSILON )
				{
					return NULL;
				}
			}
			else
			{
				if( abs(points[0][0] - points[1][0]) > FLT_EPSILON || 
					abs(points[1][1] - points[2][1]) > FLT_EPSILON || 
					abs(points[2][0] - points[3][0]) > FLT_EPSILON ||
					abs(points[3][1] - points[0][1]) > FLT_EPSILON )
				{
					return NULL;
				}
			}

			TInputBlock* pBlock = new TInputBlock( points[0][0], points[0][1], points[2][0], points[2][1] );
			if( pBlock )
			{
				if( m_arrInputBlocks.Add( pBlock ) )
					return pBlock;

				delete pBlock;
			}

			return NULL;
		}


	private:
		void AddMultiBlock( double x1, double y1, double x2, double y2, TInputBlock* pSource )
		{
			TMultiBlock pBlock;
			pBlock.m_x1 = x1;
			pBlock.m_y1 = y1;
			pBlock.m_x2 = x2;
			pBlock.m_y2 = y2;
			pBlock.m_pSourceBlocks = pSource;

			CAtlList<TMultiBlock>& blocks = ((y2 - y1) <= (x2 - x1)) ? m_HorBlocks : m_VerBlocks;
			blocks.AddTail( pBlock );
		}

		static int CmpInputBlocksHor( const void* p1, const void* p2 )
		{
			const TInputBlock* pBlock1 = *((const TInputBlock**)(p1));
			const TInputBlock* pBlock2 = *((const TInputBlock**)(p2));

			if( pBlock1->m_y1 != pBlock2->m_y1 )
				return pBlock1->m_y1 < pBlock2->m_y1 ? -1 : 1;

			return pBlock1->m_x1 < pBlock2->m_x1 ? -1 : 1;
		}

		static int CmpInputBlocksVer( const void* p1, const void* p2 )
		{
			const TInputBlock* pBlock1 = *((const TInputBlock**)(p1));
			const TInputBlock* pBlock2 = *((const TInputBlock**)(p2));

			if( pBlock1->m_x1 != pBlock2->m_x1 )
				return pBlock1->m_x1 < pBlock2->m_x1 ? -1 : 1;

			return pBlock1->m_y1 < pBlock2->m_y1 ? -1 : 1;
		}

		void CreateMultiBlocks()
		{
			m_HorBlocks.RemoveAll();
			m_VerBlocks.RemoveAll();

			for( int i = 0; i < m_arrInputBlocks.m_nSize; i++ )
			{
				m_arrInputBlocks.m_aT[i]->m_bWrite = FALSE;
			}

			// по горизонтали
			qsort( m_arrInputBlocks.m_aT, m_arrInputBlocks.m_nSize, sizeof(TInputBlock*), CmpInputBlocksHor );

			for( int i = 0; i < m_arrInputBlocks.m_nSize; )
			{
				TInputBlock* pStartBlock = m_arrInputBlocks.m_aT[i];
				int nStart = i;

				while( ++i < m_arrInputBlocks.m_nSize )
				{
					if( pStartBlock->m_y1 < m_arrInputBlocks.m_aT[i]->m_y1 )
						break;
				}

				pStartBlock->m_pNextHorBlock = NULL;
				double x = pStartBlock->m_x2;

				for( int k = nStart + 1; k < i; k++ )
				{
					TInputBlock* pBlock = m_arrInputBlocks.m_aT[k];
					pBlock->m_pNextHorBlock = NULL;

					if( pBlock->m_y2 == pStartBlock->m_y2 &&
						pBlock->m_x1 == x)
					{
						x = pBlock->m_x2;
						m_arrInputBlocks.m_aT[k - 1]->m_pNextHorBlock = pBlock;
					}
					else
					{
						if( pStartBlock->m_pNextHorBlock != NULL || 
							pStartBlock->m_bHor != FALSE )
						{
							AddMultiBlock( pStartBlock->m_x1, pStartBlock->m_y1, x, pStartBlock->m_y2, pStartBlock );
						}

						pStartBlock = pBlock;
						x = pBlock->m_x2;
					}
				}

				if( pStartBlock->m_pNextHorBlock != NULL || 
					pStartBlock->m_bHor != FALSE )
				{
					AddMultiBlock( pStartBlock->m_x1, pStartBlock->m_y1, x, pStartBlock->m_y2, pStartBlock );
				}
			}

			// по вертикали
			qsort( m_arrInputBlocks.m_aT, m_arrInputBlocks.m_nSize, sizeof(TInputBlock*), CmpInputBlocksVer );

			for( int i = 0; i < m_arrInputBlocks.m_nSize; )
			{
				TInputBlock* pStartBlock = m_arrInputBlocks.m_aT[i];
				int nStart = i;

				while( ++i < m_arrInputBlocks.m_nSize )
				{
					if( pStartBlock->m_x1 < m_arrInputBlocks.m_aT[i]->m_x1 )
						break;
				}

				pStartBlock->m_pNextVerBlock = NULL;
				double y = pStartBlock->m_y2;

				for( int k = nStart + 1; k < i; k++ )
				{
					TInputBlock* pBlock = m_arrInputBlocks.m_aT[k];
					pBlock->m_pNextVerBlock = NULL;

					if( pBlock->m_x2 == pStartBlock->m_x2 &&
						pBlock->m_y1 == y)
					{
						y = pBlock->m_y2;
						m_arrInputBlocks.m_aT[k - 1]->m_pNextVerBlock = pBlock;
					}
					else
					{
						if( pStartBlock->m_pNextVerBlock != NULL || 
							pStartBlock->m_bHor == FALSE )
						{
							AddMultiBlock( pStartBlock->m_x1, pStartBlock->m_y1, pStartBlock->m_x2, y, pStartBlock );
						}

						pStartBlock = pBlock;
						y = pBlock->m_y2;
					}
				}

				if( pStartBlock->m_pNextVerBlock != NULL || 
					pStartBlock->m_bHor == FALSE )
				{
					AddMultiBlock( pStartBlock->m_x1, pStartBlock->m_y1, pStartBlock->m_x2, y, pStartBlock );
				}
			}

		}

	public:
		void Calculate()
		{
			CreateMultiBlocks();

			int nCountHorBlocks = (int)m_HorBlocks.GetCount();
			int nCountVerBlocks = (int)m_VerBlocks.GetCount();

			// удаление пересекающихся блоков
			if( nCountHorBlocks > 1 )
			{
				TMultiBlock result;

				POSITION i = m_HorBlocks.GetHeadPosition();
				while( i != NULL )
				{
					TMultiBlock* pCurBlock = &m_HorBlocks.GetAt( i );

					POSITION j = i;
					m_HorBlocks.GetNext( j );
					while( j != NULL )
					{
						TMultiBlock* pBlock = &m_HorBlocks.GetAt( j );

						if( pCurBlock->Intersect( *pBlock, result ) )
						{
							if( result.GetWidth() > result.GetHeight() )
							{
								if( pCurBlock->GetHeight() > pBlock->GetHeight() )
								{
									POSITION del = i;
									m_HorBlocks.GetNext( i );
									m_HorBlocks.RemoveAt( del );
									pCurBlock = NULL;
								}
								else
								{
									m_HorBlocks.RemoveAt( j );
								}
								break;
							}
						}

						m_HorBlocks.GetNext( j );
					}

					if( pCurBlock )
						m_HorBlocks.GetNext( i );
				}
			}

			if( nCountVerBlocks > 1 )
			{
				TMultiBlock result;

				POSITION i = m_VerBlocks.GetHeadPosition();
				while( i != NULL )
				{
					TMultiBlock* pCurBlock = &m_VerBlocks.GetAt( i );

					POSITION j = i;
					m_VerBlocks.GetNext( j );
					while( j != NULL )
					{
						TMultiBlock* pBlock = &m_VerBlocks.GetAt( j );

						if( pCurBlock->Intersect( *pBlock, result ) )
						{
							if( result.GetHeight() > result.GetWidth() )
							{
								if( pCurBlock->GetWidth() > pBlock->GetWidth() )
								{
									POSITION del = i;
									m_VerBlocks.GetNext( i );
									m_VerBlocks.RemoveAt( del );
									pCurBlock = NULL;
								}
								else
								{
									m_VerBlocks.RemoveAt( j );
								}
								break;
							}
						}

						m_VerBlocks.GetNext( j );
					}

					if( pCurBlock )
						m_VerBlocks.GetNext( i );
				}
			}

			//// объединение соседних блоков
			//CAtlList<TMultiBlock> m_HorMultiBlocks;
			//CAtlList<TMultiBlock> m_VerMultiBlocks;
			//
			//nCountHorBlocks = (int)m_HorBlocks.GetCount();
			//nCountVerBlocks = (int)m_VerBlocks.GetCount();

			//if( nCountHorBlocks > 1 )
			//{
			//	TMultiBlock result;
			//	
			//	POSITION i = m_HorBlocks.GetHeadPosition();
			//	while( i != NULL )
			//	{
			//		TMultiBlock* pCurBlock = &m_HorBlocks.GetAt( i );
			//		if( pCurBlock->m_bPartMultiBlock )
			//		{
			//			m_HorBlocks.GetNext( i );
			//			continue;
			//		}

			//		TMultiBlock multi;
			//		multi.m_x1 = pCurBlock->m_x1();
			//		multi.m_y1 = pCurBlock->m_y1();
			//		multi.m_x2 = pCurBlock->m_x2();
			//		multi.m_y2 = pCurBlock->m_y2();
			//		multi.m_blocks = pCurBlock;

			//		POSITION j = i;
			//		m_HorBlocks.GetNext( j );
			//		while( j != NULL )
			//		{
			//			TMultiBlock* pBlock = &m_HorBlocks.GetAt( j );
			//			if( pBlock->m_bPartMultiBlock )
			//			{
			//				m_HorBlocks.GetNext( j );
			//				continue;
			//			}

			//			if( pBlock->m_y1 == multi.m_y1 &&  )
			//			
			//			if( pCurBlock->Intersect( *pBlock, result ) )
			//			{
			//				if( result.GetWidth() > result.GetHeight() )
			//				{
			//					if( pCurBlock->GetHeight() > pBlock->GetHeight() )
			//					{
			//						POSITION del = i;
			//						m_HorBlocks.GetNext( i );
			//						m_HorBlocks.RemoveAt( del );
			//						pCurBlock = NULL;
			//					}
			//					else
			//					{
			//						m_HorBlocks.RemoveAt( j );
			//					}
			//					break;
			//				}
			//			}

			//			m_HorBlocks.GetNext( j );
			//		}

			//		m_HorMultiBlocks.AddTail( multi );
			//		
			//		if( pCurBlock )
			//			m_HorBlocks.GetNext( i );
			//	}
			//}

			nCountHorBlocks = (int)m_HorBlocks.GetCount();
			nCountVerBlocks = (int)m_VerBlocks.GetCount();

			if( nCountHorBlocks > 1 && nCountVerBlocks > 1 )
			{
				CAtlList<TCrossPoint> points;
				CAtlList<TGroup> groups;

				for( POSITION i = m_HorBlocks.GetHeadPosition(); i != NULL; m_HorBlocks.GetNext( i ) )
				{
					TMultiBlock* pHorBlock = &m_HorBlocks.GetAt( i );

					for( POSITION j = m_VerBlocks.GetHeadPosition(); j != NULL; m_VerBlocks.GetNext( j ) )
					{
						TMultiBlock* pVerBlock = &m_VerBlocks.GetAt( j );

						if( pHorBlock->Intersect( *pVerBlock ) )
						{
							double cx = pVerBlock->GetCenterX();
							double cy = pHorBlock->GetCenterY();

							points.AddTail( TCrossPoint( cx, cy, pHorBlock, pVerBlock ) );

							TGroup* pHorGroup = pHorBlock->m_group;
							TGroup* pVerGroup = pVerBlock->m_group;

							if( pHorGroup == pVerGroup )
							{
								if( !pHorGroup )
								{
									if( groups.AddTail() )
									{
										TGroup* pNewGroup = &groups.GetTail();

										pNewGroup->m_blocks.Add( pHorBlock );
										pHorBlock->m_group = pNewGroup;

										pNewGroup->m_blocks.Add( pVerBlock );
										pVerBlock->m_group = pNewGroup;
									}
								}
							}
							else
							{
								if( !pHorGroup )
								{
									pVerGroup->m_blocks.Add( pHorBlock );
									pHorBlock->m_group = pVerGroup;
								}
								else if( !pVerGroup )
								{
									pHorGroup->m_blocks.Add( pVerBlock );
									pVerBlock->m_group = pHorGroup;
								}
								else
								{
									for( size_t i = 0; i < pVerGroup->m_blocks.GetCount(); i++ )
									{
										pVerGroup->m_blocks[i]->m_group = pHorGroup;
									}

									pHorGroup->m_blocks.Append( pVerGroup->m_blocks );
									pVerGroup->m_blocks.RemoveAll();
								}
							}
						}
					}
				}

				for( POSITION i = points.GetHeadPosition(); i != NULL; points.GetNext( i ) )
				{
					TCrossPoint* pPoint = &points.GetAt( i );

					TMultiBlock* pHorBlock = pPoint->m_pHorBlock;
					TMultiBlock* pVerBlock = pPoint->m_pVerBlock;

					if( pHorBlock->m_group != pVerBlock->m_group )
					{
						continue;
					}

					pHorBlock->m_group->m_points.Add( pPoint );
				}

				int nCurTable = 0;

				for( POSITION i = groups.GetHeadPosition(); i != NULL; groups.GetNext( i ) )
				{
					TGroup* pGroup = &groups.GetAt( i );

					CAtlArray<TCrossPoint*>& data = pGroup->m_points;

					size_t nCountPoints = data.GetCount();
					if( nCountPoints < 4 )
					{
						continue;
					}

					double* pBufferY = new double[nCountPoints];
					double* pBufferX = new double[nCountPoints];
					TMultiBlock** pBufferPtr = new TMultiBlock*[nCountPoints * 2];

					if( pBufferY && pBufferX && pBufferPtr )
					{
						for( size_t k = 0; k < nCountPoints; k++ )
						{
							pBufferX[k] = data[k]->m_x;
							pBufferY[k] = data[k]->m_y;
							pBufferPtr[k*2 + 0] = data[k]->m_pHorBlock;
							pBufferPtr[k*2 + 1] = data[k]->m_pVerBlock;
						}

						qsort( pBufferX, nCountPoints, sizeof(double), CmpDouble );
						qsort( pBufferY, nCountPoints, sizeof(double), CmpDouble );
						qsort( pBufferPtr, nCountPoints * 2, sizeof(TMultiBlock*), CmpPtr );

						POSITION pPosTable = m_Tables.AddTail();
						if( pPosTable )
						{
							CTable& table = m_Tables.GetAt( pPosTable );
							table.SetStartPos( pBufferX[0], pBufferY[0] );

							size_t pos = 0;
							for( size_t k = 1; k < nCountPoints; k++ )
							{
								double d = abs(pBufferX[k] - pBufferX[pos]);

								if( d < 1 )
								{
									continue;
								}
								pos = k;
								table.AddColumn( d );
							}

							pos = 0;
							for( size_t k = 1; k < nCountPoints; k++ )
							{
								double d = abs(pBufferY[k] - pBufferY[pos]);

								if( d < 1 )
								{
									continue;
								}
								pos = k;
								table.AddRow( d );
							}

							TMultiBlock* ptr = NULL;
							for( size_t k = 0; k < nCountPoints * 2; k++ )
							{
								if( pBufferPtr[k] != ptr )
								{
									TInputBlock* pSourceBlock = pBufferPtr[k]->m_pSourceBlocks;
									while( pSourceBlock != NULL )
									{
										if( !pSourceBlock->m_bWrite )
										{
											pSourceBlock->m_bWrite = TRUE;
											table.AddBlock( *pSourceBlock );
										}
										pSourceBlock = pSourceBlock->m_pNextHorBlock;
									}

									pSourceBlock = pBufferPtr[k]->m_pSourceBlocks;
									while( pSourceBlock != NULL )
									{
										if( !pSourceBlock->m_bWrite )
										{
											pSourceBlock->m_bWrite = TRUE;
											table.AddBlock( *pSourceBlock );
										}
										pSourceBlock = pSourceBlock->m_pNextVerBlock;
									}

									ptr = pBufferPtr[k];
								}
							}

							if( table.IsEmpty() )
								m_Tables.RemoveAt( pPosTable );
						}
					}

					delete [] pBufferY;
					delete [] pBufferX;
					delete [] pBufferPtr;
				}
			}
		}

		CTable* GetFirstTable()
		{
			m_pCurTable = m_Tables.GetHeadPosition();

			return GetNextTable();
		}

		CTable* GetNextTable()
		{
			if( !m_pCurTable )
				return NULL;

			return &m_Tables.GetNext( m_pCurTable );
		}

		size_t GetCountTables() const
		{
			return m_Tables.GetCount();
		}


	private:
		static int CmpDouble( const void* p1, const void* p2 )
		{
			return (*((double*)p1) <= *((double*)p2)) ? -1 : 1;
		}

		static int CmpPtr( const void* p1, const void* p2 )
		{
			return (int)(*((LONG_PTR*)p1) - *((LONG_PTR*)p2));
		}

	};


	class CTableDetector3
	{
		struct TLine
		{
			CBlock* m_pSrcBlock;

			long m_x1, m_x2;
			long m_y;
		};

		class TMultiLine
		{
		public:
			ATL::CSimpleArray<CBlock*> m_arrSrcBlocks;

			long m_x1, m_x2;
			long m_y;

		public:
			TMultiLine( const TLine& line )
			{
				Set( line );
			}

			void Set( const TLine& line )
			{
				m_arrSrcBlocks.m_nSize = 0;
				m_arrSrcBlocks.Add( line.m_pSrcBlock );
				m_x1 = line.m_x1;
				m_x2 = line.m_x2;
				m_y = line.m_y;
			}

			void Merge( const TLine& line )
			{
				m_arrSrcBlocks.Add( line.m_pSrcBlock );
				if( m_x2 < line.m_x2 )
					m_x2 = line.m_x2;
			}
		};

		class TMultiBlock
		{
		public:
			long m_x1, m_x2, m_y1, m_y2;

			ATL::CSimpleArray<CBlock*> m_arrSrcBlocks;

			ATL::CSimpleArray<TMultiBlock*> m_arrChilds;
			int m_nCountTopChilds;

			int m_nGroup;
			int m_nCurChild;
			TMultiBlock* m_pParent;

		public:
			TMultiBlock()
			{
				m_nCountTopChilds = 0;
			}

			TMultiBlock* GetLeftTopChild() const
			{
				if( m_nCountTopChilds <= 0 )
					return NULL;

				TMultiBlock* result = m_arrChilds.m_aT[0];

				for( int i = 1; i < m_nCountTopChilds; i++ )
				{
					TMultiBlock* pBlock = m_arrChilds.m_aT[i];

					if( pBlock->m_x1 < result->m_x1 )
						result = pBlock;
				}

				return result;
			}

			TMultiBlock* GetRightTopChild() const
			{
				if( m_nCountTopChilds <= 0 )
					return NULL;

				TMultiBlock* result = m_arrChilds.m_aT[0];

				for( int i = 1; i < m_nCountTopChilds; i++ )
				{
					TMultiBlock* pBlock = m_arrChilds.m_aT[i];

					if( pBlock->m_x1 > result->m_x1 )
						result = pBlock;
				}

				return result;
			}

			TMultiBlock* GetLeftBottomChild() const
			{
				if( m_nCountTopChilds >= m_arrChilds.m_nSize )
					return NULL;

				TMultiBlock* result = m_arrChilds.m_aT[m_nCountTopChilds];

				for( int i = m_nCountTopChilds; i < m_arrChilds.m_nSize; i++ )
				{
					TMultiBlock* pBlock = m_arrChilds.m_aT[i];

					if( pBlock->m_x1 < result->m_x1 )
						result = pBlock;
				}

				return result;
			}

			TMultiBlock* GetRightBottomChild() const
			{
				if( m_nCountTopChilds >= m_arrChilds.m_nSize )
					return NULL;

				TMultiBlock* result = m_arrChilds.m_aT[m_nCountTopChilds];

				for( int i = m_nCountTopChilds; i < m_arrChilds.m_nSize; i++ )
				{
					TMultiBlock* pBlock = m_arrChilds.m_aT[i];

					if( pBlock->m_x1 > result->m_x1 )
						result = pBlock;
				}

				return result;
			}

			void SortChilds()
			{
				int end = m_arrChilds.m_nSize - 1;
				int begin = 0;

				while( begin < end )
				{
					if( m_arrChilds.m_aT[end]->m_y1 < m_y2 )
						end--;
					else
						break;
				}

				while( begin < end )
				{
					if( m_arrChilds.m_aT[begin]->m_y1 < m_y2 )
					{
						TMultiBlock* tmp = m_arrChilds.m_aT[begin];
						m_arrChilds.m_aT[begin] = m_arrChilds.m_aT[end];
						m_arrChilds.m_aT[end] = tmp;
						end--;
					}
					else
					{
						begin++;
					}
				}

				if( !(m_arrChilds.m_aT[end]->m_y1 < m_y2) )
					end++;

				m_nCountTopChilds = end;
			}

			void SetLine( const TMultiLine& line )
			{
				m_x1 = line.m_x1;
				m_x2 = line.m_x2;
				m_y1 = line.m_y;
				m_y2 = line.m_y;

				m_arrSrcBlocks.m_nSize = 0;

				for( int i = 0; i < line.m_arrSrcBlocks.m_nSize; i++ )
				{
					m_arrSrcBlocks.Add( line.m_arrSrcBlocks.m_aT[i] );
				}
			}

			void AddLine( const TMultiLine& line )
			{
				m_y2 = line.m_y;

				for( int i = 0; i < line.m_arrSrcBlocks.m_nSize; i++ )
				{
					AddSrcBlock( line.m_arrSrcBlocks.m_aT[i] );
				}
			}

			long GetCenterX() const
			{
				return (m_x2 + m_x1) / 2;
			}

			long GetCenterY() const
			{
				return (m_y2 + m_y1) / 2;
			}

			long GetWidth() const
			{
				return m_x2 - m_x1;
			}

			long GetHeight() const
			{
				return m_y2 - m_y1;
			}


			//int FindChild( TMultiBlock* pBlock ) const
			//{
			//	for( int i = 0; i < m_arrChilds.m_nSize; i++ )
			//	{
			//		if( m_arrChilds.m_aT[i] == pBlock )
			//			return i;
			//	}
			//	return -1;
			//}

			void RemoveChild( TMultiBlock* pBlock )
			{
				for( int i = 0; i < m_arrChilds.m_nSize; i++ )
				{
					if( m_arrChilds.m_aT[i] == pBlock )
					{
						for( int j = i + 1; j < m_arrChilds.m_nSize; j++ )
						{
							m_arrChilds.m_aT[j - 1] = m_arrChilds.m_aT[j];
						}

						if( m_nCountTopChilds > i )
							m_nCountTopChilds--;

						m_arrChilds.m_nSize--;
						break;
					}
				}
			}

			void RemoveTopChild( TMultiBlock* pBlock )
			{
				for( int i = 0; i < m_nCountTopChilds; i++ )
				{
					if( m_arrChilds.m_aT[i] == pBlock )
					{
						for( int j = i + 1; j < m_arrChilds.m_nSize; j++ )
						{
							m_arrChilds.m_aT[j - 1] = m_arrChilds.m_aT[j];
						}

						m_nCountTopChilds--;
						m_arrChilds.m_nSize--;
						break;
					}
				}
			}

			void RemoveBottomChild( TMultiBlock* pBlock )
			{
				for( int i = m_nCountTopChilds; i < m_arrChilds.m_nSize; i++ )
				{
					if( m_arrChilds.m_aT[i] == pBlock )
					{
						for( int j = i + 1; j < m_arrChilds.m_nSize; j++ )
						{
							m_arrChilds.m_aT[j - 1] = m_arrChilds.m_aT[j];
						}

						m_arrChilds.m_nSize--;
						break;
					}
				}
			}

			//void ChangeChild( TMultiBlock* pBlock, TMultiBlock* pNewBlock )
			//{
			//	int index = FindChild( pBlock );
			//	if( index >= 0 )
			//	{
			//		m_arrChilds.m_aT[index] = pNewBlock;
			//	}
			//}

			long GetLeftBoundChilds() const
			{
				if( m_arrChilds.m_nSize <= 0 )
					return m_x1;

				long result = m_arrChilds.m_aT[0]->m_x1;
				for( int i = 1; i < m_arrChilds.m_nSize; i++ )
				{
					long x = m_arrChilds.m_aT[i]->m_x1;
					if( result > x )
						result = x;
				}
				return result;
			}

			void GetTopBoundChilds( long& _x1, long& _x2 ) const
			{
				int nCount = m_nCountTopChilds;
				if( nCount <= 0 )
				{
					_x1 = m_x1;
					_x2 = m_x2;
					return;
				}

				long x1 = m_arrChilds.m_aT[0]->m_x1;
				long x2 = m_arrChilds.m_aT[0]->m_x2;

				for( int i = 1; i < nCount; i++ )
				{
					long x = m_arrChilds.m_aT[i]->m_x1;
					if( x1 > x )
						x1 = x;

					x = m_arrChilds.m_aT[i]->m_x2;
					if( x2 < x )
						x2 = x;
				}

				_x1 = x1;
				_x2 = x2;
			}

			void GetBottomBoundChilds( long& _x1, long& _x2 ) const
			{
				int nCount = m_arrChilds.m_nSize - m_nCountTopChilds;
				if( nCount <= 0 )
				{
					_x1 = m_x1;
					_x2 = m_x2;
					return;
				}

				long x1 = m_arrChilds.m_aT[0]->m_x1;
				long x2 = m_arrChilds.m_aT[0]->m_x2;

				for( int i = 1; i < nCount; i++ )
				{
					long x = m_arrChilds.m_aT[i]->m_x1;
					if( x1 > x )
						x1 = x;

					x = m_arrChilds.m_aT[i]->m_x2;
					if( x2 < x )
						x2 = x;
				}

				_x1 = x1;
				_x2 = x2;
			}

			long GetRightBoundChilds() const
			{
				if( m_arrChilds.m_nSize <= 0 )
					return m_x2;

				long result = m_arrChilds.m_aT[0]->m_x2;
				for( int i = 1; i < m_arrChilds.m_nSize; i++ )
				{
					long x = m_arrChilds.m_aT[i]->m_x2;
					if( result < x )
						result = x;
				}
				return result;
			}

			int GetCountTopChilds() const
			{
				return m_nCountTopChilds;
			}

			int GetCountBottomChilds() const
			{
				return m_arrChilds.m_nSize - m_nCountTopChilds;
			}

			bool IsMultiLinks() const
			{
				return (m_nCountTopChilds > 1 || (m_arrChilds.m_nSize - m_nCountTopChilds) > 1);
			}

			bool IsDualLinks() const
			{
				return (m_nCountTopChilds == 1 && m_arrChilds.m_nSize == 2);
			}

			void BreakExtraLinks()
			{
				if( IsMultiLinks() )
				{
					int offset = 0;

					// просматриваем верх
					int nTerminator = m_nCountTopChilds;
					for( int i = 0; i < nTerminator; i++ )
					{
						TMultiBlock* pChild = m_arrChilds.m_aT[i];
						if( pChild->IsMultiLinks() )
						{
							pChild->RemoveBottomChild( this );
							offset++;
							continue;
						}

						if( offset )
							m_arrChilds.m_aT[i - offset] = pChild;
					}
					m_nCountTopChilds -= offset;

					// просматриваем низ
					for( int i = nTerminator; i < m_arrChilds.m_nSize; i++ )
					{
						TMultiBlock* pChild = m_arrChilds.m_aT[i];
						if( pChild->IsMultiLinks() )
						{
							pChild->RemoveTopChild( this );
							offset++;
							continue;
						}

						if( offset )
							m_arrChilds.m_aT[i - offset] = pChild;
					}
					m_arrChilds.m_nSize -= offset;
				}

				if( IsDualLinks() )
				{
					TMultiBlock* pTopChild    = m_arrChilds.m_aT[0];
					TMultiBlock* pBottomChild = m_arrChilds.m_aT[1];

					if( pTopChild->GetCountBottomChilds() == 1 ||
						m_x1 < pTopChild->m_x1 || m_x2 > pTopChild->m_x2 )
					{
						pTopChild->RemoveBottomChild( this );
						RemoveTopChild( pTopChild );					
					}

					if( pBottomChild->GetCountTopChilds() == 1 ||
						m_x1 < pBottomChild->m_x1 || m_x2 > pBottomChild->m_x2)
					{
						pBottomChild->RemoveTopChild( this );
						RemoveBottomChild( pBottomChild );
					}
				}
			}

			TMultiBlock* GetTopChild( int index ) const
			{
				return m_arrChilds.m_aT[index];
			}

			TMultiBlock* GetBottomChild( int index ) const
			{
				return m_arrChilds.m_aT[m_nCountTopChilds + index];
			}


		private:
			void AddSrcBlock( CBlock* pBlock )
			{
				for( int i = 0; i < m_arrSrcBlocks.m_nSize; i++ )
				{
					if( pBlock == m_arrSrcBlocks.m_aT[i] )
						return;
				}
				m_arrSrcBlocks.Add( pBlock );
			}
		};


		enum
		{
			dir_l = 0,
			dir_t = 1,
			dir_r = 2,
			dir_b = 3,

			dir_all,
			dir_first = dir_l,

			dir_left   = dir_l,
			dir_top    = dir_t,
			dir_right  = dir_r,
			dir_bottom = dir_b


		};

		struct TCrossPoint
		{
			long m_x, m_y;

			TMultiBlock* m_pBlocks[dir_all];
			TCrossPoint* m_pPoints[dir_all];
		};

		class TVerEdge
		{
		public:
			long m_x;
			long m_y1, m_y2;

			TMultiBlock* m_pBlocks[dir_all];
			TVerEdge*    m_pEdges[dir_all];

		public:

		};

		class TCell
		{
		public:
			long m_x1, m_y1;
			long m_x2, m_y2;

			TMultiBlock* m_left;
			TMultiBlock* m_top;
			TMultiBlock* m_right;
			TMultiBlock* m_bottom;

		public:
			TCell()
			{
				m_x1 = 0;
				m_y1 = 0;
				m_x2 = 0;
				m_y2 = 0;

				m_left = NULL;
				m_top = NULL;
				m_right = NULL;
				m_bottom = NULL;
			}

			void Normalize()
			{
				if( m_left )
					m_x1 = (m_left->m_x2 + m_left->m_x1) / 2;

				if( m_top )
					m_y1 = (m_top->m_y2 + m_top->m_y1) / 2;

				if( m_right )
					m_x2 = (m_right->m_x2 + m_right->m_x1) / 2;

				if( m_bottom )
					m_y2 = (m_bottom->m_y2 + m_bottom->m_y1) / 2;
			}

			bool IsValid() const
			{
				return (m_x1 < m_x2 && m_y1 < m_y2);
			}

		};

		ATL::CSimpleArray<CBlock*> m_arrSrcBlocks;
		ATL::CSimpleArray<TMultiBlock*> m_arrMultiBlocks;

	public:
		~CTableDetector3()
		{
			Clear();
		}

		void Clear()
		{
			for( int i = 0; i < m_arrSrcBlocks.m_nSize; i++ )
			{
				delete m_arrSrcBlocks.m_aT[i];
			}
			m_arrSrcBlocks.RemoveAll();

			for( int i = 0; i < m_arrMultiBlocks.m_nSize; i++ )
			{
				delete m_arrMultiBlocks.m_aT[i];
			}
			m_arrMultiBlocks.RemoveAll();
		}

		CBlock* AddBlock( const double points[4][2] )
		{
			if( points[0][0] != points[1][0] )
			{
				if( abs(points[0][1] - points[1][1]) > FLT_EPSILON || 
					abs(points[1][0] - points[2][0]) > FLT_EPSILON || 
					abs(points[2][1] - points[3][1]) > FLT_EPSILON ||
					abs(points[3][0] - points[0][0]) > FLT_EPSILON )
				{
					return NULL;
				}
			}
			else
			{
				if( abs(points[0][0] - points[1][0]) > FLT_EPSILON || 
					abs(points[1][1] - points[2][1]) > FLT_EPSILON || 
					abs(points[2][0] - points[3][0]) > FLT_EPSILON ||
					abs(points[3][1] - points[0][1]) > FLT_EPSILON )
				{
					return NULL;
				}
			}

			CBlock* pNewBlock = new CBlock( points[0][0], points[0][1], points[2][0], points[2][1] );
			if( pNewBlock )
			{
				if( m_arrSrcBlocks.Add( pNewBlock ) )
					return pNewBlock;

				delete pNewBlock;
			}

			return NULL;
		}

		void Calculate()
		{
			if( m_arrSrcBlocks.m_nSize <= 0 )
				return;

			// сортируем блоки по цветам
			qsort( m_arrSrcBlocks.m_aT, m_arrSrcBlocks.m_nSize, sizeof(CBlock*), CmpColorBlocks );

			// выделяем таблицы среди групп
			int nStart = 0;
			int nColor = m_arrSrcBlocks.m_aT[0]->m_color;

			for( int i = 1; i < m_arrSrcBlocks.m_nSize; i++ )
			{
				if( nColor != m_arrSrcBlocks.m_aT[i]->m_color )
				{
					nColor = m_arrSrcBlocks.m_aT[i]->m_color;
					CalculateColorGroup( m_arrSrcBlocks.m_aT + nStart, i - nStart );
					nStart = i;
				}
			}
		}

	private:
		void CalculateColorGroup( CBlock* pSrcBlocks[], int nCount )
		{
			if( nCount < 4 )
				return;

			CSimpleArray<TLine> arrLines;

			for( int i = 0; i < nCount; i++ )
			{
				long x1 = RealToLong( pSrcBlocks[i]->m_x1 / 25.4 * 1440 );
				long x2 = RealToLong( pSrcBlocks[i]->m_x2 / 25.4 * 1440 );
				long y1 = RealToLong( pSrcBlocks[i]->m_y1 / 25.4 * 1440 );
				long y2 = RealToLong( pSrcBlocks[i]->m_y2 / 25.4 * 1440 );

				long lCountLines = y2 - y1 + 1;
				for( int j = 0; j < lCountLines; j++ )
				{
					TLine line;
					line.m_pSrcBlock = pSrcBlocks[i];
					line.m_x1 = x1;
					line.m_x2 = x2;
					line.m_y = y1 + j;
					arrLines.Add( line );
				}
			}

			{
				// задаём дополнительную завершающую линию в список
				TLine line = arrLines.m_aT[arrLines.m_nSize - 1];
				line.m_y += 2;
				arrLines.Add( line );
			}

			// сортируем линии слева на право, сверху в низ
			qsort( arrLines.m_aT, arrLines.m_nSize, sizeof(TLine), CmpLines );

			ATL::CSimpleArray<TMultiBlock*> arrBufBlocks;

			// задаём текущую точку пересечения блоков
			TMultiBlock* pCrossBlock1 = NULL;
			TMultiBlock* pCrossBlock2 = NULL;
			long lCrossX = 0;
			long lCrossY = 0;

			// задаём текущую линию
			TMultiLine oCurLine( arrLines.m_aT[0] );

			// делаем обход по всем линиям, занося их в новые блоки
			for( int i = 1; i < arrLines.m_nSize; i++ )
			{
				const TLine& line = arrLines.m_aT[i];

				if( line.m_y != oCurLine.m_y )
				{
					// переход на новую линию
					if( line.m_y > oCurLine.m_y + 1 )
					{
						// закрываем все открытые блоки
						for( int j = 0; j < arrBufBlocks.m_nSize; j++ )
						{
							m_arrMultiBlocks.Add( arrBufBlocks.m_aT[j] );
						}
						arrBufBlocks.m_nSize = 0;
					}
					else
					{
						int offset = 0;

						for( int j = 0; j < arrBufBlocks.m_nSize; j++ )
						{
							if( oCurLine.m_y != arrBufBlocks.m_aT[j]->m_y2 )
							{
								m_arrMultiBlocks.Add( arrBufBlocks.m_aT[j] );
								offset += 1;
							}
							else
							{
								if( offset )
									arrBufBlocks.m_aT[j - offset] = arrBufBlocks.m_aT[j];
							} 
						}
						arrBufBlocks.m_nSize -= offset;
					}
				}
				else
				{
					if( line.m_x1 <= oCurLine.m_x2 + 1 )
					{
						oCurLine.Merge( line );
						continue;
					}
				}

				// добавляем текущую линию к блокам
				BOOL bNewBlock = TRUE;
				for( int i = 0; i < arrBufBlocks.m_nSize; i++ )
				{
					TMultiBlock& block = *(arrBufBlocks.m_aT[i]);

					if( oCurLine.m_x1 == block.m_x1 && oCurLine.m_x2 == block.m_x2 )
					{
						block.AddLine( oCurLine );
						bNewBlock = FALSE;
						break;
					}
				}

				// создаём новый блок
				if( bNewBlock )
				{
					TMultiBlock* pNewBlock = new TMultiBlock;
					if( pNewBlock )
					{
						if( arrBufBlocks.Add( pNewBlock ) )
						{
							pNewBlock->SetLine( oCurLine );

							// задаём новые связи с блоками окружения
							for( int j = 0; j < arrBufBlocks.m_nSize - 1; j++ )
							{
								TMultiBlock* pCurBlock = arrBufBlocks.m_aT[j];

								if( oCurLine.m_x1 <= pCurBlock->m_x2 &&
									oCurLine.m_x2 >= pCurBlock->m_x1)
								{
									pCurBlock->m_arrChilds.Add( pNewBlock );
									pNewBlock->m_arrChilds.Add( pCurBlock );
								}
							}
						}
						else
						{
							delete pNewBlock;
						}
					}
				}

				// задаём новую линию
				oCurLine.Set( line );
			}

			// очищаем временные буферы
			arrLines.RemoveAll();
			arrBufBlocks.RemoveAll();

			// сортируем связи блоков
			for( int i = 0; i < m_arrMultiBlocks.m_nSize; i++ )
			{
				m_arrMultiBlocks.m_aT[i]->SortChilds();
			}

			// разрываем лишние связи
			for( int i = 0; i < m_arrMultiBlocks.m_nSize; i++ )
			{
				m_arrMultiBlocks.m_aT[i]->BreakExtraLinks();
			}

			// удаляем одиночные блоки
			DeleteSingleBlocks();

			// удаляем промежуточный блоки
			//DeleteExcessBlocks();

			// сортируем мультиблоки по группам
			SetGroups();

			// обрабатываем группы по отдельности
			int nStart = 0;
			int id = m_arrMultiBlocks.m_aT[0]->m_nGroup;

			for( int i = 1; i < m_arrMultiBlocks.m_nSize; i++ )
			{
				if( id != m_arrMultiBlocks.m_aT[i]->m_nGroup )
				{
					CalculateGroup( m_arrMultiBlocks.m_aT + nStart, i - nStart );
					nStart = i;
				}
			}
			CalculateGroup( m_arrMultiBlocks.m_aT + nStart, m_arrMultiBlocks.m_nSize - nStart );


		}

		void BreakExtraLinks()
		{
			// блоки с несколькими связями по одну сторону должны чередоваться по вертикали 
			// с блоками у которых только одна связь на сторону

			for( int i = 0; i < m_arrMultiBlocks.m_nSize; i++ )
			{
				TMultiBlock* pBlock = m_arrMultiBlocks.m_aT[i];

				int nCountTopChilds    = pBlock->m_nCountTopChilds;
				int nCountBottomChilds = pBlock->m_arrChilds.m_nSize - pBlock->m_nCountTopChilds;

				if( nCountTopChilds == 1 )
				{

				}
			}
		}

		void DeleteExcessBlocks()
		{
			// удаляем лишние выступы с лева и права
			for( int i = 0; i < m_arrMultiBlocks.m_nSize; i++ )
			{
				TMultiBlock* pBlock = m_arrMultiBlocks.m_aT[i];

				long left = pBlock->GetLeftBoundChilds();
				if( pBlock->m_x1 < left )
					pBlock->m_x1 = left;

				long right = pBlock->GetRightBoundChilds();
				if( pBlock->m_x2 > right )
					pBlock->m_x2 = right;
			}

			int offset = 0;
			for( int i = 0; i < m_arrMultiBlocks.m_nSize; i++ )
			{
				TMultiBlock* pBlock = m_arrMultiBlocks.m_aT[i];

				int nCountTop = pBlock->m_nCountTopChilds;
				int nCountBottom = pBlock->m_arrChilds.m_nSize - pBlock->m_nCountTopChilds;

				if( nCountTop == 1 )
				{
					if( nCountBottom <= 0 )
					{
						delete pBlock;
						offset++;
						continue;
					}

					if( nCountBottom <= 0 )
					{

					}
				}


				//if( pBlock->m_arrChilds.m_nSize <= 0 )
				//{
				//	delete pBlock;
				//	offset++;
				//	continue;
				//}
				//else
				//if( pBlock->m_arrChilds.m_nSize == 1 )
				//{
				//	pBlock->m_arrChilds.m_aT[0]->RemoveChild( pBlock );
				//	delete pBlock;
				//	offset++;
				//	continue;
				//}

				if( offset )
					m_arrMultiBlocks.m_aT[i - offset] = pBlock;
			}
			m_arrMultiBlocks.m_nSize -= offset;
		}

		void DeleteSingleBlocks()
		{
			int offset = 0;
			for( int i = 0; i < m_arrMultiBlocks.m_nSize; i++ )
			{
				TMultiBlock* pBlock = m_arrMultiBlocks.m_aT[i];
				if( pBlock->m_arrChilds.m_nSize <= 0 )
				{
					delete pBlock;
					offset++;
					continue;
				}
				else
					if( pBlock->m_arrChilds.m_nSize == 1 )
					{
						pBlock->m_arrChilds.m_aT[0]->RemoveChild( pBlock );
						delete pBlock;
						offset++;
						continue;
					}

					if( offset )
						m_arrMultiBlocks.m_aT[i - offset] = pBlock;
			}
			m_arrMultiBlocks.m_nSize -= offset;
		}

		void CalculateGroup( TMultiBlock* arrBlocks[], int nCountBlocks )
		{
			return;

			if( nCountBlocks <= 1 )
				return;

			typedef TVerEdge *TVerEdgePtr;
			ATL::CSimpleArray<TVerEdge*> arrEdges;

			// создаём список вертикальных линий

			TVerEdge edge;
			edge.m_pEdges[dir_l] = NULL;
			edge.m_pEdges[dir_t] = NULL;
			edge.m_pEdges[dir_r] = NULL;
			edge.m_pEdges[dir_b] = NULL;

			TMultiBlock* pChild;
			TVerEdge* pEdge;

			for( int i = 0; i < nCountBlocks; i++ )
			{
				// left edge
				pEdge = new TVerEdge;
				if( pEdge )
				{
					if( arrEdges.Add( pEdge ) )
					{
						pEdge->m_pEdges[dir_l] = NULL;
						pEdge->m_pEdges[dir_t] = NULL;
						pEdge->m_pEdges[dir_r] = NULL;
						pEdge->m_pEdges[dir_b] = NULL;

						pEdge->m_x = arrBlocks[i]->m_x1 - 1;
						pEdge->m_y1 = arrBlocks[i]->m_y1;
						pEdge->m_y2 = arrBlocks[i]->m_y2;

						pEdge->m_pBlocks[dir_l] = NULL;
						pEdge->m_pBlocks[dir_t] = (pChild = arrBlocks[i]->GetLeftTopChild()) ? ((pEdge->m_x >= pChild->m_x1 && pEdge->m_x <= pChild->m_x2) ? pChild : NULL) : NULL;
						pEdge->m_pBlocks[dir_r] = arrBlocks[i];
						pEdge->m_pBlocks[dir_b] = (pChild = arrBlocks[i]->GetLeftBottomChild()) ? ((pEdge->m_x >= pChild->m_x1 && pEdge->m_x <= pChild->m_x2) ? pChild : NULL) : NULL;
					}
					else
					{
						delete pEdge;
					}
				}

				// right edge
				pEdge = new TVerEdge;
				if( pEdge )
				{
					if( arrEdges.Add( pEdge ) )
					{
						pEdge->m_pEdges[dir_l] = NULL;
						pEdge->m_pEdges[dir_t] = NULL;
						pEdge->m_pEdges[dir_r] = NULL;
						pEdge->m_pEdges[dir_b] = NULL;

						pEdge->m_x = arrBlocks[i]->m_x2 + 1;
						pEdge->m_y1 = arrBlocks[i]->m_y1;
						pEdge->m_y2 = arrBlocks[i]->m_y2;

						pEdge->m_pBlocks[dir_l] = arrBlocks[i];
						pEdge->m_pBlocks[dir_t] = (pChild = arrBlocks[i]->GetRightTopChild()) ? ((pEdge->m_x >= pChild->m_x1 && pEdge->m_x <= pChild->m_x2) ? pChild : NULL) : NULL;
						pEdge->m_pBlocks[dir_r] = NULL;
						pEdge->m_pBlocks[dir_b] = (pChild = arrBlocks[i]->GetRightBottomChild()) ? ((pEdge->m_x >= pChild->m_x1 && pEdge->m_x <= pChild->m_x2) ? pChild : NULL) : NULL;
					}
					else
					{
						delete pEdge;
					}
				}
			}

			// сортируем линии по вертикали
			qsort( arrEdges.m_aT, arrEdges.m_nSize, sizeof(TVerEdge*), CmpVerEdgesY );

			// нормализуем по высоте
			ATL::CSimpleArray<TVerEdge*> arrBufEdges;
			int nCountEdges = arrEdges.m_nSize;
			int nCurEdge = 0;

			for( ;; )
			{
				// добавление в буфер новых линий
				if( !arrBufEdges.m_nSize )
				{
					if( nCurEdge >= nCountEdges )
						break;

					long y = arrEdges.m_aT[nCurEdge]->m_y1;
					do
					{
						if( y < arrEdges.m_aT[nCurEdge]->m_y1 )
							break;

						arrBufEdges.Add( arrEdges.m_aT[nCurEdge] );													
					}
					while( ++nCurEdge < nCountEdges );
				}
				else
				{
					if( nCurEdge < nCountEdges )
					{
						long y = arrBufEdges.m_aT[0]->m_y1;
						do
						{
							if( y < arrEdges.m_aT[nCurEdge]->m_y1 )
								break;

							arrBufEdges.Add( arrEdges.m_aT[nCurEdge] );													
						}
						while( ++nCurEdge < nCountEdges );
					}
				}

				// поиск минимальной нижней границы
				long nMinY = arrBufEdges.m_aT[0]->m_y2;
				for( int i = 1; i < arrBufEdges.m_nSize; i++ )
				{
					long y = arrBufEdges.m_aT[i]->m_y2;
					if( nMinY > y )
						nMinY = y;
				}

				if( nCurEdge < nCountEdges )
				{
					long y = arrEdges[nCurEdge]->m_y1;
					if( nMinY >= y )
						nMinY = y - 1;
				}

				// отсечение всех линий по минимальной границе
				int offset = 0;
				for( int i = 0; i < arrBufEdges.m_nSize; i++ )
				{
					TVerEdge* pEdge = arrBufEdges.m_aT[i];

					if( nMinY >= pEdge->m_y2 )
					{
						offset++;
					}
					else
					{
						// разбиение линии на 2 части
						TVerEdge* pNewEdge = new TVerEdge;
						if( pNewEdge )
						{
							if( arrEdges.Add( pNewEdge ) )
							{
								*pNewEdge = *pEdge;

								pNewEdge->m_pBlocks[dir_b] = NULL;
								pNewEdge->m_y2 = nMinY;
							}
							else
							{
								delete pNewEdge;
							}
						}

						pEdge->m_pBlocks[dir_t] = NULL;
						pEdge->m_y1 = nMinY - 1;

						arrBufEdges.m_aT[i - offset] = pEdge;
					}
				}
			}

			// повторно сортируем линии по вертикали
			qsort( arrEdges.m_aT, arrEdges.m_nSize, sizeof(TVerEdge*), CmpVerEdgesY );

			// создаём ячейки



			// удаляем временные данные
			for( int i = 0; i < arrEdges.m_nSize; i++ )
			{
				delete arrEdges.m_aT[i];
			}

		}

		void SetGroups()
		{
			for( int i = 0; i < m_arrMultiBlocks.m_nSize; i++ )
			{
				TMultiBlock* pBlock = m_arrMultiBlocks.m_aT[i];

				pBlock->m_nGroup = -1;
				pBlock->m_nCurChild = 0;
				pBlock->m_pParent = NULL;
			}

			int group = 0;

			for( int i = 0; i < m_arrMultiBlocks.m_nSize; i++ )
			{
				TMultiBlock* pBlock = m_arrMultiBlocks.m_aT[i];

				if( pBlock->m_nGroup < 0 )
				{
					pBlock->m_nGroup = group++;

					for( ;; )
					{
						if( pBlock->m_nCurChild < pBlock->m_arrChilds.m_nSize )
						{
							TMultiBlock* pChild = pBlock->m_arrChilds.m_aT[pBlock->m_nCurChild++];

							if( pChild->m_nGroup < 0 )
							{
								pChild->m_nGroup = group;
								pChild->m_pParent = pBlock;
								pBlock = pChild;
							}
						}
						else
						{
							if( pBlock->m_pParent )
								pBlock = pBlock->m_pParent;
							else
								break;
						}
					}
				}
			}

			qsort( m_arrMultiBlocks.m_aT, m_arrMultiBlocks.m_nSize, sizeof(TMultiBlock*), CmpGroupMultiBlocks );
		}

		//void CreateCells( TMultiBlock* arrBlocks[], int nCountBlocks, ATL::CSimpleArray<TCell>& arrCells )
		//{
		//	if( nCountBlocks <= 0 )
		//		return;

		//	qsort( arrBlocks, nCountBlocks, sizeof(TMultiBlock*), CmpMultiBlocks );

		//	//long nMinX = arrBlocks[0]->m_x1;
		//	//long nMaxX = arrBlocks[0]->m_x2;
		//	//long nMinY = arrBlocks[0]->m_y1;
		//	//long nMaxY = arrBlocks[0]->m_y2;

		//	//for( int i = 1; i < nCountBlocks; i++ )
		//	//{
		//	//	TMultiBlock* pBlock = arrBlocks[i];

		//	//	if( pBlock->m_x1 < nMinX ) nMinX = pBlock->m_x1;
		//	//	if( pBlock->m_x2 > nMaxX ) nMaxX = pBlock->m_x2;
		//	//	if( pBlock->m_y1 < nMinY ) nMinY = pBlock->m_y1;
		//	//	if( pBlock->m_y2 > nMaxY ) nMaxY = pBlock->m_y2;
		//	//}

		//	TCell cell;

		//	for( int i = 1; i < nCountBlocks; i++ )
		//	{
		//		TMultiBlock* p1 = arrBlocks[i - 1];
		//		TMultiBlock* p2 = arrBlocks[i];

		//		if( p1->m_y1 != p2->m_y1 || p1->m_y2 != p2->m_y2 )
		//		{
		//			continue;
		//		}

		//		cell.m_x1 = p1->GetCenterX();
		//		cell.m_x2 = p2->GetCenterX();

		//		TMultiBlock* border[2] = {NULL, NULL};
		//		

		//		arrCells.Add( cell );
		//	}

		//}


		static int CmpLines( const void* p1, const void* p2 )
		{
			const TLine* pLine1 = (const TLine*)(p1);
			const TLine* pLine2 = (const TLine*)(p2);

			if( pLine1->m_y != pLine2->m_y )
				return pLine1->m_y - pLine2->m_y;

			return pLine1->m_x1 - pLine2->m_x1;
		}

		static int CmpColorBlocks( const void* p1, const void* p2 )
		{
			const CBlock* pBlock1 = *((const CBlock**)(p1));
			const CBlock* pBlock2 = *((const CBlock**)(p2));

			return pBlock1->m_color - pBlock2->m_color;
		}

		static int CmpGroupMultiBlocks( const void* p1, const void* p2 )
		{
			const TMultiBlock* pBlock1 = *((const TMultiBlock**)(p1));
			const TMultiBlock* pBlock2 = *((const TMultiBlock**)(p2));

			return pBlock1->m_nGroup - pBlock2->m_nGroup;
		}

		static int CmpVerEdgesY( const void* _p1, const void* _p2 )
		{
			const TVerEdge* p1 = *((const TVerEdge**)(_p1));
			const TVerEdge* p2 = *((const TVerEdge**)(_p2));

			return p1->m_y1 - p2->m_y1;
		}

		static int CmpCrossPointsY( const void* _p1, const void* _p2 )
		{
			const TCrossPoint* p1 = *((const TCrossPoint**)(_p1));
			const TCrossPoint* p2 = *((const TCrossPoint**)(_p2));

			long y = p1->m_y - p2->m_y;
			if( y )
				return y;

			long x = p1->m_x - p2->m_x;
			if( x )
				return x;

			return p1->m_pBlocks[dir_left] ? 1 : -1;
		}

		static int CmpCrossPointsX( const void* _p1, const void* _p2 )
		{
			const TCrossPoint* p1 = *((const TCrossPoint**)(_p1));
			const TCrossPoint* p2 = *((const TCrossPoint**)(_p2));

			long x = p1->m_x - p2->m_x;
			if( x )
				return x;

			long y = p1->m_y - p2->m_y;
			if( y )
				return y;

			return p1->m_pBlocks[dir_left] ? 1 : -1;
		}
		//static int CmpMultiBlocks( const void* p1, const void* p2 )
		//{
		//	const TMultiBlock* pBlock1 = *((const TMultiBlock**)(p1));
		//	const TMultiBlock* pBlock2 = *((const TMultiBlock**)(p2));

		//	if( pBlock1->m_y != pBlock2->m_y )
		//		return pBlock1->m_y - pBlock2->m_y;

		//	return pBlock1->m_x1 - pBlock2->m_x1;
		//}

		static long RealToLong( const double value )
		{
			// return int(value + (vulue < 0 ? -0.5 : 0.5));

			const double d = value + 6755399441055744.0;
			return *((int *)(&d));
		}

	};
}

/*

-	Пока будут, некоторые допущения и по мере усложения алгоритма будут добавлены вставки в анализатор

-	линии либо горизонтальные либо вертикальные 
-	таблицы не пересекаются
-	таблицы цельные и не имеют объединенных ячеек
-	одна ячейка это не таблица
-	контент пока не обрабатываем

*/

namespace Data
{
	class CBufferItemRef	//	ссылка на набор команд 
	{
	public:

		CBufferItemRef() : m_nBegin(0), m_nEnd (0), m_bIsRemove (FALSE)
		{

		}

		CBufferItemRef(unsigned long nBegin, unsigned long nEnd) : m_nBegin(nBegin), m_nEnd (nEnd), m_bIsRemove (FALSE)
		{

		}

		inline void SetRemove (BOOL bRemove)
		{
			m_bIsRemove	= bRemove;
		}

		inline unsigned long GetBufBegin ()
		{
			return m_nBegin;
		}

		inline unsigned long GetBufEnd ()
		{
			return m_nEnd;
		}

	public:	

		BOOL			m_bIsRemove;

		unsigned long	m_nBegin;
		unsigned long	m_nEnd;
	};
}

namespace Analyzer
{	
	class CPoint
	{
	public:

		CPoint () : m_dX (0.0), m_dY (0.0)
		{

		}

		CPoint (double dX, double dY) : m_dX (dX), m_dY (dY)
		{

		}

		inline double GetX() const
		{
			return m_dX;
		}

		inline double GetY() const
		{
			return m_dY;
		}

		inline CPoint operator+(const CPoint& oSrc)
		{
			return CPoint (m_dX + oSrc.m_dX, m_dY + oSrc.m_dY);
		}

		inline CPoint operator-(const CPoint& oSrc)
		{
			return CPoint (m_dX - oSrc.m_dX, m_dY - oSrc.m_dY);
		}

		inline void Add (double dX, double dY)
		{
			m_dX += dX;
			m_dY += dY;
		}

		//
		static CPoint MiddlePoint (const CPoint& oFrom, const CPoint& oTo)
		{
			return CPoint ((oFrom.m_dX + oTo.m_dX) * 0.5, (oFrom.m_dY + oTo.m_dY) * 0.5);
		}

	private:

		double m_dX;
		double m_dY;
	};

	class CPointInd
	{
	public:

		CPointInd () : m_iX (0), m_iY (0)
		{

		}

		CPointInd (int iX, int iY) : m_iX (iX), m_iY (iY)
		{

		}

		inline int GetX() const
		{
			return m_iX;
		}

		inline int GetY() const
		{
			return m_iY;
		}

		inline BOOL IsValid () const
		{
			if ((m_iX >= 0) && (m_iY >= 0))
				return TRUE;

			return FALSE;
		}

	private:

		int m_iX;
		int m_iY;
	};

	class CLine
	{
	public:

		CLine () : m_bAdd(FALSE)
		{
		}

		CLine (CPoint oFrom, CPoint oTo, Data::CBufferItemRef* pBufRef = NULL) : m_pBufRef(pBufRef), m_bAdd(FALSE)
		{
			m_dX		=	__min(oFrom.GetX(), oTo.GetX());
			m_dEX		=	__max(oFrom.GetX(), oTo.GetX());

			m_dY		=	__min(oFrom.GetY(), oTo.GetY());
			m_dEY		=	__max(oFrom.GetY(), oTo.GetY());

			m_dWidth	=	fabs(m_dEX - m_dX);
			m_dHeight	=	fabs(m_dEY - m_dY);
		}

		CLine (double dX, double dY, double dEX, double dEY, Data::CBufferItemRef* pBufRef = NULL) : m_pBufRef(pBufRef), m_bAdd(FALSE)
		{
			m_dX		=	__min(dX, dEX);
			m_dEX		=	__max(dX, dEX);

			m_dY		=	__min(dY, dEY);
			m_dEY		=	__max(dY, dEY);

			m_dWidth	=	fabs(m_dEX - m_dX);
			m_dHeight	=	fabs(m_dEY - m_dY);
		}

		CLine (const CLine& oSrc)
		{
			*this = oSrc;
		}

		CLine& operator=(const CLine& oSrc)
		{
			//m_dX		=	oSrc.m_dX;
			//m_dY		=	oSrc.m_dY;
			//m_dEX		=	oSrc.m_dEX; 
			//m_dEY		=	oSrc.m_dEY;

			//m_dWidth	=	oSrc.m_dWidth;
			//m_dHeight	=	oSrc.m_dHeight;

			//m_pBufRef	=	oSrc.m_pBufRef;

			//m_bAdd		=	oSrc.m_bAdd;

			return *this;
		}

		inline Data::CBufferItemRef* GetItemRef () const
		{
			return m_pBufRef;
		}

		//
		inline const double& GetX() const
		{
			return m_dX;
		}

		inline const double& GetY()	const
		{
			return m_dY;
		}

		inline const double& GetEX() const
		{
			return m_dEX;
		}

		inline const double& GetEY() const
		{
			return m_dEY;
		}

		inline double GetBX () const
		{
			return m_dWidth;
		}

		inline double GetBY () const
		{
			return m_dHeight;
		}

		inline BOOL Validate ()
		{
			if ((fabs(m_dWidth) < EPS) && (fabs(m_dHeight) < EPS))
				return FALSE;

			return TRUE;
		}

		inline BOOL IsVertical ()
		{
			return (m_dWidth < EPS);
		}

		inline BOOL IsHorizontal ()
		{
			return (m_dHeight < EPS);
		}

		inline BOOL IsAdd () const 
		{
			return m_bAdd;
		}

		inline void SetAdd (BOOL bAdd)
		{
			m_bAdd	=	bAdd;
		}

		//
		static BOOL HaveIntersect (const CLine& oLineTo, const CLine& oLine)
		{
			return FALSE;
		}

		static BOOL HaveInAtEnds (const CLine& oLineTo, const CLine& oLine)
		{
			if (((fabs(oLineTo.m_dX  - oLine.m_dX) < END_EPS) && (fabs(oLineTo.m_dY  - oLine.m_dY)  < END_EPS)) ||
				((fabs(oLineTo.m_dEX - oLine.m_dX) < END_EPS) && (fabs(oLineTo.m_dEY - oLine.m_dY)  < END_EPS)) ||
				((fabs(oLineTo.m_dX  - oLine.m_dEX)< END_EPS) && (fabs(oLineTo.m_dY  - oLine.m_dEY) < END_EPS)) )
				return TRUE;

			return FALSE;
		}

		//
		inline BOOL IsInRegionX (const CPoint& oLeft, const CPoint& oRight, BOOL bUseEps = TRUE) const
		{
			if (bUseEps)
			{
				if ( ( ((oLeft.GetX()-EPS) < m_dX) && (m_dX < (oRight.GetX()+EPS)) ) || ( ((oLeft.GetX()-EPS) < m_dEX) && (m_dEX < (oRight.GetX()+EPS))) )
					return TRUE;

				return FALSE;
			}

			if ( ( (oLeft.GetX() < m_dX) && (m_dX < oRight.GetX()) ) || ( (oLeft.GetX() < m_dEX) && (m_dEX < oRight.GetX()) ) )
				return TRUE;

			return FALSE;
		}

		inline BOOL IsInRegionY (const CPoint& oTop, const CPoint& oBottom, BOOL bUseEps = TRUE) const
		{
			if (bUseEps)
			{
				if ( ( ((oTop.GetY()-EPS) < m_dY) && (m_dY < (oBottom.GetY()+EPS)) ) || ( ((oTop.GetY()-EPS) < m_dEY) && (m_dEY < (oBottom.GetY()+EPS)) ) )
					return TRUE;

				return FALSE;
			}

			if ( ( (oTop.GetY() < m_dY) && (m_dY < oBottom.GetY()) ) ||  ( (oTop.GetY() < m_dEY) && (m_dEY < oBottom.GetY()) ) )
				return TRUE;

			return FALSE;
		}

		//
		static BOOL AlongX (CLine* pLineTo, CLine* pLine, double dEps = 2.0)
		{
			if (fabs(pLineTo->m_dX - pLine->m_dX) < dEps)
				return TRUE;

			return FALSE;
		}

		static BOOL AlongY (CLine* pLineTo, CLine* pLine, double dEps = 2.0)
		{
			if (fabs(pLineTo->m_dY - pLine->m_dY) < dEps)
				return TRUE;

			return FALSE;
		}

		//
		static BOOL CrossPoint (CLine* pLine, CLine* pLine2, CPoint& oPoint)
		{
			double dY12		=	pLine->GetEY()	-	pLine->GetY();
			double dX43		=	pLine2->GetX()	-	pLine2->GetEX();
			double dY13		=	pLine2->GetY()	-	pLine->GetY();
			double dX13		=	pLine2->GetX()	-	pLine->GetX();

			double dD		=	dY12 * dX43 - dY13 * dX13;

			if ((dD < 0.0001) && (dD > -0.0001))
				return FALSE;

			double dY43		=	pLine2->GetY()	-	pLine2->GetEY();
			double dT		=	( dY13 * dX43 - dX13 * dY43 ) / dD;		

			oPoint			=	CPoint( (pLine->GetEX() - pLine->GetX()) * dT + pLine->GetX(), (pLine->GetEY() - pLine->GetY()) * dT + pLine->GetY());

			double dCF		=	(oPoint.GetY() - pLine->GetY()  ) * (pLine->GetEX()  - pLine->GetX() ) - (oPoint.GetX() - pLine->GetX() ) * (pLine->GetEY()  - pLine->GetY());
			double dCN		=	(oPoint.GetY() - pLine2->GetY() ) * (pLine2->GetEX() - pLine2->GetX()) - (oPoint.GetX() - pLine2->GetX()) * (pLine2->GetEY() - pLine2->GetY());

			if ((dCF < 0.0001) && (dCF > -0.0001) && (dCN < 0.0001) && (dCN > -0.0001))
				return TRUE;

			return FALSE;
		}

		static BOOL HaveCross (CLine* pLine, CLine* pLine2)
		{
			double dMaxX	=	__max(pLine->GetX(),pLine2->GetX());
			double dMaxY	=	__max(pLine->GetY(),pLine2->GetY());
			double dMinX	=	__min(pLine->GetEX(),pLine2->GetEX());
			double dMinY	=	__min(pLine->GetEY(),pLine2->GetEY());

			if (dMaxX > dMinX || dMaxY > dMinY)
				return FALSE;

			return TRUE;
		}

	private:

		double m_dX;
		double m_dY;
		double m_dEX; 
		double m_dEY;

		double m_dWidth;
		double m_dHeight;

		Data::CBufferItemRef* m_pBufRef;

		BOOL m_bAdd;
	};

	class CLines
	{
	public:
		CLines() : m_pBufRef (NULL)
		{

		}

		CLines(Data::CBufferItemRef* pBufRef) : m_pBufRef (pBufRef)
		{

		}

		CLines(const CLines& oSrc)
		{
			*this = oSrc;
		}

		CLines& operator=(const CLines& oSrc)
		{
			m_pBufRef	=	oSrc.m_pBufRef;
			m_arLines.Copy(oSrc.m_arLines);

			return *this;
		}

		~CLines()
		{
			//for (size_t i = 0; i < m_arLines.GetCount (); ++i)
			//	RELEASEOBJECT(m_arLines[i]);
		}

		//
		virtual BOOL Add (CLine* pLine)
		{		
			if (pLine)
			{
				BOOL bAdd = TRUE;
				for (size_t i = 0; i < m_arLines.GetCount(); ++i)
				{
					if (m_arLines[i]==pLine)
					{
						bAdd = FALSE;
						break;
					}
				}			

				if (bAdd)
				{
					m_arLines.Add (pLine);
					return TRUE;
				}
			}

			return FALSE;
		}

		virtual BOOL Add (CPoint oFrom, CPoint oTo, Data::CBufferItemRef* pBufRef = NULL)
		{
			return Add (new CLine(oFrom, oTo, pBufRef));
		}

		virtual void ClearRef ()
		{
			m_arLines.RemoveAll ();
		}

		inline size_t GetCount() const
		{
			return m_arLines.GetCount();
		}

		inline CLine* Get(size_t nInd) const 
		{
			if (nInd >= m_arLines.GetCount())
				return NULL;

			return m_arLines [nInd];
		}

		inline CLine* GetLine(size_t nInd)
		{
			if (nInd >= m_arLines.GetCount())
				return NULL;

			return m_arLines [nInd];
		}

		inline CLine* GetMinX(BOOL bOffAdded = FALSE)	//	Off Added - среди добавленных
		{
			if (0 == m_arLines.GetCount())
				return NULL;

			CLine* pLine	=	NULL;
			double dX		=	0;

			if (bOffAdded)
			{
				for (size_t i = 0; i < m_arLines.GetCount(); ++i)
				{
					if (m_arLines[i]->IsAdd())
						continue;

					if (NULL == pLine)
					{
						pLine	=	m_arLines[i];
						dX		=	pLine->GetX();					
					}

					if (m_arLines[i]->GetX() < dX)
					{
						pLine	=	m_arLines[i];
						dX		=	pLine->GetX();
					}
				}

				return pLine;
			}

			pLine	=	m_arLines[0];
			dX		=	pLine->GetX();

			for (size_t i = 0; i < m_arLines.GetCount(); ++i)
			{
				if (m_arLines[i]->GetX() < dX)
				{
					pLine	=	m_arLines[i];
					dX		=	pLine->GetX();
				}
			}

			return pLine;
		}

		inline CLine* GetMaxX(BOOL bOffAdded = FALSE)
		{
			if (0 == m_arLines.GetCount())
				return NULL;

			CLine* pLine	=	NULL;
			double dX		=	0;

			if (bOffAdded)
			{
				for (size_t i = 0; i < m_arLines.GetCount(); ++i)
				{
					if (m_arLines[i]->IsAdd())
						continue;

					if (NULL == pLine)
					{
						pLine	=	m_arLines[i];
						dX		=	pLine->GetX();					
					}

					if (m_arLines[i]->GetX() > dX)
					{
						pLine	=	m_arLines[i];
						dX		=	pLine->GetX();
					}
				}

				return pLine;
			}

			pLine	=	m_arLines[0];
			dX		=	pLine->GetX();

			for (size_t i = 0; i < m_arLines.GetCount(); ++i)
			{
				if (m_arLines[i]->GetX() > dX)
				{
					pLine	=	m_arLines[i];
					dX		=	pLine->GetX();
				}
			}

			return pLine;
		}

		inline CLine* GetMinY(BOOL bOffAdded = FALSE)
		{
			if (0 == m_arLines.GetCount())
				return NULL;

			CLine* pLine	=	NULL;
			double dY		=	0;

			if (bOffAdded)
			{
				for (size_t i = 0; i < m_arLines.GetCount(); ++i)
				{
					if (m_arLines[i]->IsAdd())
						continue;

					if (NULL == pLine)
					{
						pLine	=	m_arLines[i];
						dY		=	pLine->GetY();					
					}

					if (m_arLines[i]->GetY() < dY)
					{
						pLine	=	m_arLines[i];
						dY		=	pLine->GetY();
					}
				}

				return pLine;
			}

			pLine			=	m_arLines[0];
			dY				=	pLine->GetY();

			for (size_t i = 0; i < m_arLines.GetCount(); ++i)
			{
				if (m_arLines[i]->GetY() < dY)
				{
					pLine	=	m_arLines[i];
					dY		=	pLine->GetY();
				}
			}

			return pLine;
		}

		inline CLine* GetMaxY(BOOL bOffAdded = FALSE)
		{
			if (0 == m_arLines.GetCount())
				return NULL;

			CLine* pLine	=	NULL;
			double dY		=	0;

			if (bOffAdded)
			{
				for (size_t i = 0; i < m_arLines.GetCount(); ++i)
				{
					if (m_arLines[i]->IsAdd())
						continue;

					if (NULL == pLine)
					{
						pLine	=	m_arLines[i];
						dY		=	pLine->GetY();					
					}

					if (m_arLines[i]->GetY() > dY)
					{
						pLine	=	m_arLines[i];
						dY		=	pLine->GetY();
					}
				}

				return pLine;
			}

			pLine	=	m_arLines[0];
			dY		=	pLine->GetY();

			for (size_t i = 0; i < m_arLines.GetCount(); ++i)
			{
				if (m_arLines[i]->GetY() > dY)
				{
					pLine	=	m_arLines[i];
					dY		=	pLine->GetY();
				}
			}

			return pLine;
		}

		//
		virtual BOOL InCross (CLine* pLine) const
		{
			// пересечение хоть с одной линией 

			for (size_t i = 0; i < m_arLines.GetCount(); ++i)
			{
				if (CLine::HaveCross(pLine,m_arLines[i]))
					return TRUE;			
			}

			return FALSE;
		}

	protected:

		Data::CBufferItemRef*	m_pBufRef;
		CAtlArray <CLine*>		m_arLines;
	};

	class CSortLines : public CLines
	{
	public:

		CSortLines()
		{

		}

		CSortLines(const CSortLines& oSrc)
		{
			*this = oSrc;
		}

		CSortLines& operator=(const CSortLines& oSrc)
		{
			m_pBufRef	=	oSrc.m_pBufRef;
			m_arLines.Copy(oSrc.m_arLines);

			m_arHLines.Copy(oSrc.m_arHLines);
			m_arVLines.Copy(oSrc.m_arVLines);

			m_arBeSideVLines.Copy(oSrc.m_arBeSideVLines);
			m_arBeSideHLines.Copy(oSrc.m_arBeSideHLines);

			return *this;
		}

		//
		virtual BOOL Add (CLine* pLine)
		{
			if (CLines::Add (pLine))
			{
				if ((pLine->GetBX () < SORT_EPS) && (pLine->GetBY () < SORT_EPS))
					return FALSE;

				AddHorizontalLine (pLine);
				AddVerticalLine (pLine);
			}

			return FALSE;
		}

		virtual BOOL Add (CPoint oFrom, CPoint oTo, Data::CBufferItemRef* pBufRef = NULL)
		{
			return CSortLines::Add (new CLine(oFrom, oTo, pBufRef));
		}

		virtual void ClearRef ()
		{
			m_arLines.RemoveAll ();
			m_arHLines.RemoveAll ();
			m_arVLines.RemoveAll ();
		}

		//
		inline size_t GetNH() const
		{
			return m_arHLines.GetCount();
		}

		inline const CLine* GetHLine(size_t nInd) const
		{
			if (nInd >= m_arHLines.GetCount())
				return NULL;

			return m_arHLines [nInd];
		}

		inline size_t GetNV() const
		{
			return m_arVLines.GetCount();
		}

		inline const CLine* GetVLine(size_t nInd) const
		{
			if (nInd >= m_arVLines.GetCount())
				return NULL;

			return m_arVLines [nInd];
		}


		//
		virtual BOOL InCross (CLine* pLine) const
		{
			// пересечение хоть с одной линией 

			for (size_t i = 0; i < m_arVLines.GetCount(); ++i)
			{
				if (CLine::HaveCross(pLine, m_arVLines[i]))
					return TRUE;			
			}

			for (size_t i = 0; i < m_arBeSideVLines.GetCount(); ++i)
			{
				if (CLine::HaveCross(pLine, m_arBeSideVLines[i]))
					return TRUE;			
			}

			return FALSE;
		}
	protected:

		inline BOOL AddHorizontalLine (CLine* pLine)
		{
			if (pLine->IsHorizontal())
			{
				if (0 == m_arHLines.GetCount())
				{
					m_arHLines.Add (pLine);
					return TRUE;
				}
				else if (1 == m_arHLines.GetCount())
				{
					if (CLine::AlongY(pLine, m_arHLines[0], SORT_EPS))
						return FALSE;

					if (m_arHLines[0]->GetY() < pLine->GetY())
						m_arHLines.Add (pLine);
					else
						m_arHLines.InsertAt(0, pLine);						

					return TRUE;
				}
				else
				{
					if (m_arHLines[0]->GetY() > pLine->GetY())
					{
						if (CLine::AlongY(pLine, m_arHLines[0], SORT_EPS))
							return FALSE;

						m_arHLines.InsertAt(0, pLine);						
						return TRUE;
					}
					else
					{
						if (m_arHLines[m_arHLines.GetCount() - 1]->GetY() <  pLine->GetY())
						{
							if (CLine::AlongY(pLine, m_arHLines[m_arHLines.GetCount() - 1], SORT_EPS))
								return FALSE;

							m_arHLines.Add (pLine);
							return TRUE;
						}
						else
						{
							size_t nAddPos		=	-1;
							for (size_t i = 0; i < m_arHLines.GetCount() - 1; ++i)
							{
								//	близко находящиеся линии, очевидно не учитываем
								if (CLine::AlongY(pLine, m_arHLines[i], SORT_EPS) || CLine::AlongY(pLine, m_arHLines[i + 1], SORT_EPS))
									return FALSE;

								if (m_arHLines[i]->GetY() < pLine->GetY() && pLine->GetY() < m_arHLines[i + 1]->GetY())
								{
									nAddPos		=	i + 1;
									break;
								}
							}

							if (-1 != nAddPos)
							{
								m_arHLines.InsertAt(nAddPos, pLine);
								return TRUE;
							}
						}
					}
				}
			}

			return FALSE;
		}

		inline BOOL AddVerticalLine (CLine* pLine)
		{
			if (pLine->IsVertical())
			{
				if (0 == m_arVLines.GetCount())
				{
					m_arVLines.Add (pLine);
					return TRUE;
				}
				else if (1 == m_arVLines.GetCount())
				{
					if (CLine::AlongX(pLine,m_arVLines[0], SORT_EPS))
					{
						m_arBeSideVLines.Add(pLine);
						return FALSE;
					}

					if (m_arVLines[0]->GetX() < pLine->GetX())
						m_arVLines.Add (pLine);
					else
						m_arVLines.InsertAt(0, pLine);	

					return TRUE;
				}
				else
				{
					if (m_arVLines[0]->GetX() > pLine->GetX())
					{
						if (CLine::AlongX(pLine,m_arVLines[0], SORT_EPS))
						{
							m_arBeSideVLines.Add(pLine);
							return FALSE;
						}

						m_arVLines.InsertAt(0, pLine);	
						return TRUE;
					}
					else
					{
						if (m_arVLines[m_arVLines.GetCount() - 1]->GetX() <  pLine->GetX())
						{
							if (CLine::AlongX(pLine,m_arVLines[m_arVLines.GetCount() - 1],SORT_EPS))
							{
								m_arBeSideVLines.Add(pLine);
								return FALSE;
							}

							m_arVLines.Add (pLine);
						}
						else
						{
							size_t nAddPos		=	-1;
							for (size_t i = 0; i < m_arVLines.GetCount() - 1; ++i)
							{
								//	близко находящиеся линии, очевидно не учитываем
								if (CLine::AlongX(pLine, m_arVLines[i], SORT_EPS) || CLine::AlongX(pLine, m_arVLines[i + 1], SORT_EPS))
								{
									m_arBeSideVLines.Add(pLine);
									return FALSE;
								}

								if (m_arVLines[i]->GetX() < pLine->GetX() && pLine->GetX() < m_arVLines[i + 1]->GetX())
								{
									nAddPos	=	i + 1;
									break;
								}
							}

							if (-1 != nAddPos)
							{
								m_arVLines.InsertAt(nAddPos, pLine);
								return TRUE;
							}
						}
					}
				}

			}

			return FALSE;
		}

	protected:

		CAtlArray <CLine*>	m_arHLines;
		CAtlArray <CLine*>	m_arVLines;

		CAtlArray <CLine*>	m_arBeSideHLines;
		CAtlArray <CLine*>	m_arBeSideVLines;
	};

	class CBoundingBox
	{
	public:

		CBoundingBox() : m_bZeroBB(TRUE)
		{

		}


		inline void Expand (CLine* pLine)
		{
			if (m_bZeroBB)
			{
				m_oLeftTop		=	CPoint(pLine->GetX(),pLine->GetY());
				m_oRightBottom	=	CPoint(pLine->GetEX(),pLine->GetEY());

				m_bZeroBB		=	FALSE;
			}
			else
			{
				m_oLeftTop		=	CPoint(__min(pLine->GetX(), m_oLeftTop.GetX()),		__min(pLine->GetY(), m_oLeftTop.GetY()));
				m_oRightBottom	=	CPoint(__max(pLine->GetX(), m_oRightBottom.GetX()), __max(pLine->GetY(), m_oRightBottom.GetY()));

				m_oLeftTop		=	CPoint(__min(pLine->GetEX(), m_oLeftTop.GetX()),	 __min(pLine->GetEY(), m_oLeftTop.GetY()));
				m_oRightBottom	=	CPoint(__max(pLine->GetEX(), m_oRightBottom.GetX()), __max(pLine->GetEY(), m_oRightBottom.GetY()));
			}

			m_oLines.Add (pLine);
		}

		inline BOOL Add (CLine* pLine)
		{
			if (InArea(pLine))
			{
				Expand (pLine);
				return TRUE;
			}

			return FALSE;
		}

		inline BOOL InArea (const CLine* pLine, BOOL bUseEps = 0)
		{
			if (pLine)
			{
				if (pLine->IsInRegionX(m_oLeftTop, m_oRightBottom, bUseEps) && pLine->IsInRegionY(m_oLeftTop, m_oRightBottom, bUseEps))
					return TRUE;
			}

			return FALSE;
		}

		inline BOOL InArea (double dX, double dY, double dWidth, double dHeight, BOOL bOblyCXY = FALSE)
		{
			if (bOblyCXY)
			{
				if (( m_oLeftTop.GetX() < (dX+dWidth * 0.5) ) &&
					( (dX+dWidth * 0.5) < m_oRightBottom.GetX() ) && 
					( m_oLeftTop.GetY() < (dY+dHeight * 0.5) ) &&
					( (dY+dHeight * 0.5) < m_oRightBottom.GetY() ) )
					return TRUE;

				return FALSE;
			}

			if ( (( m_oLeftTop.GetX() < dX ) && 
				( dX < m_oRightBottom.GetX() ) &&
				( m_oLeftTop.GetY() < dY ) && 
				( dY < m_oRightBottom.GetY() )) &&
				(( m_oLeftTop.GetX() < (dX+dWidth) ) && 
				( (dX+dWidth) < m_oRightBottom.GetX() ) && 
				( m_oLeftTop.GetY() < (dY+dHeight) ) && 
				( (dY+dHeight) < m_oRightBottom.GetY() ) ) )
				return TRUE;

			return FALSE;
		}

		inline BOOL InArea (const CPoint& oLeftTop, const CPoint& oRightBottom, BOOL bOblyCXY = FALSE)
		{
			if (bOblyCXY)
			{
				CPoint oMiddle	=	CPoint::MiddlePoint (oLeftTop, oRightBottom);

				if (( m_oLeftTop.GetX() < oMiddle.GetX() ) && 
					( oMiddle.GetX() < m_oRightBottom.GetX() ) && 
					( m_oLeftTop.GetY() < oMiddle.GetY() ) &&
					( oMiddle.GetY() < m_oRightBottom.GetY() ) )
					return TRUE;

				return FALSE;
			}

			if ( (( m_oLeftTop.GetX() < oLeftTop.GetX() ) &&
				( oLeftTop.GetX() < m_oRightBottom.GetX() ) &&
				( m_oLeftTop.GetY() < oRightBottom.GetY() ) && 
				( oRightBottom.GetY() < m_oRightBottom.GetY() )) &&
				(( m_oLeftTop.GetX() < oLeftTop.GetX() ) && 
				( oLeftTop.GetX() < m_oRightBottom.GetX() ) &&
				( m_oLeftTop.GetY() < oRightBottom.GetY() ) &&
				( oRightBottom.GetY() < m_oRightBottom.GetY() ) ) )
				return TRUE;

			return FALSE;
		}

		inline BOOL IsValid ()
		{
			// ...

			return TRUE;
		}

		//
		inline const CSortLines& GetInnerLines () const
		{
			return m_oLines;
		}

		inline const CPoint& GetLeftTop () const
		{
			return m_oLeftTop;
		}

		inline const CPoint& GetRightBottom () const
		{
			return m_oRightBottom;
		}

		inline CPoint GetSize () const
		{
			return CPoint (m_oRightBottom.GetX() - m_oLeftTop.GetX(), m_oRightBottom.GetY() - m_oLeftTop.GetY());
		}

		//
		inline void Clear ()
		{
			m_bZeroBB		=	TRUE;

			m_oLeftTop		=	CPoint();
			m_oRightBottom	=	CPoint();

			m_oLines.ClearRef();
		}

	private:

		BOOL m_bZeroBB;

		CPoint m_oLeftTop;
		CPoint m_oRightBottom;

		CSortLines m_oLines;
	};

	class CBoxStorage
	{
	public:
		CBoxStorage ()
		{

		}

		CBoxStorage& operator=(const CBoxStorage& oSrc)
		{
			m_arBoxes.Copy(oSrc.m_arBoxes);

			return *this;
		}

		inline size_t GetCount ()
		{
			return m_arBoxes.GetCount();
		}

		inline CBoundingBox GetBox (size_t nInd)
		{
			if (nInd >= m_arBoxes.GetCount())
				return CBoundingBox();

			return m_arBoxes[nInd];
		}

		//
		inline BOOL Normalize (CAtlArray <CLines*>& arrLines)
		{
			if (0 == arrLines.GetCount())
				return FALSE;

			CLine* pTop = GetOffAddedLinesMinY(arrLines);	//	горизонтальная
			if (NULL == pTop)
				return FALSE;

			pTop->SetAdd (TRUE);

			CBoundingBox oBox;
			oBox.Expand(pTop);

			BOOL bRefresh = FALSE;

			CAtlArray <CLine*> arrEnds;

			while (TRUE)
			{	
				for (size_t i = 0; i < arrLines.GetCount(); ++i)
				{
					for (size_t j = 0; j < arrLines[i]->GetCount(); ++j)
					{
						CLine* pLine = arrLines[i]->GetLine (j);
						if (0==pLine->IsAdd())
						{
							if (CLine::HaveInAtEnds (*pLine, *pTop))
							{
								oBox.Expand(pLine);
								arrEnds.Add (pLine);

								pLine->SetAdd(TRUE);
							}
							else
							{
								//ATLTRACE ( "(%d) ", (j+1) * (i+1));
							}
						}
					}
				}

				if (0 == arrEnds.GetCount())
				{
					m_arBoxes.Add(oBox);
					oBox.Clear();

					pTop = GetOffAddedLinesMinY(arrLines);	//	горизонтальная
					if (NULL == pTop)						//	разбили все линии по прямоугольным областям
						break;
				}
				else
				{
					pTop = arrEnds[0];
					arrEnds.RemoveAt(0);
				}
			}

			return TRUE;
		}


	private:

		inline CLine* GetOffAddedLinesMinY (CAtlArray <CLines*>& arrLines)
		{
			if (0 == arrLines.GetCount())
				return FALSE;

			CLines oLines;

			for (size_t i = 0; i < arrLines.GetCount(); ++i)
			{
				oLines.Add (arrLines[i]->GetMinY(TRUE));			// среди всех линий в данном наборе ищем минимальный (горизонтальную линию) 
			}

			return oLines.GetMinY (TRUE);
		}

	private:

		CAtlArray <CBoundingBox> m_arBoxes;
	};

	class CLinesStorage
	{
	public:

		CLinesStorage ()// : m_nLines(0)
		{

		}

		~CLinesStorage ()
		{
			Clear ();
		}

		inline BOOL AddLines (CLines* pLines)
		{
			if (pLines)
			{
				if (pLines->GetCount())
				{
					m_arLines.Add(pLines);

					//m_nLines += (int)pLines->GetCount();

					return TRUE;
				}
			}

			return FALSE;
		}

		inline BOOL Normalize()
		{
			//ATLTRACE (L"Lines Count : %d\n", m_arLines);

			return m_oBoxStorage.Normalize(m_arLines);
		}

		inline CBoxStorage& GetBBS()
		{
			return m_oBoxStorage;
		}

	private:

		inline void Clear ()
		{
			//for (size_t i = 0; i < m_arTables.GetCount (); ++i)
			//	RELEASEOBJECT(m_arTables[i]);
		}


		inline BOOL AnalyzeWithScheme (int nScheme)
		{

			return FALSE;
		}

	private:

		CAtlArray <CLines*>	m_arLines;
		CBoxStorage	m_oBoxStorage;
	};
}

namespace Analyzer
{	
	class CTextItemRef
	{
	public:

		CTextItemRef() : m_bInTable (FALSE), m_bStr (NULL), m_dX (0.0), m_dY (0.0), m_dWidth (0.0), m_dHeight (0.0), m_dBaseLineOffset(0.0), m_nBegin(0), m_nEnd (0)
		{

		}

		CTextItemRef(BSTR bStr, double dX, double dY, double dWidth, double dHeight, double dBaseLineOffset, unsigned long nBegin, unsigned long nEnd) : m_bInTable (FALSE), m_dX (dX), m_dY (dY), m_dWidth (dWidth), m_dHeight (dHeight), m_dBaseLineOffset(dBaseLineOffset), m_nBegin(nBegin), m_nEnd (nEnd)
		{			
			m_bStr				=	::SysAllocStringLen (bStr, (UINT)wcslen(bStr));

			m_bsUnicodeText		=	NULL;
			m_bsGidText			=	NULL;
			m_bsSourceCodeText	=	NULL;
		}	

		CTextItemRef(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double dX, double dY, double dWidth, double dHeight, double dBaseLineOffset, DWORD lFlags, unsigned long nBegin, unsigned long nEnd) : m_bInTable (FALSE), m_dX (dX), m_dY (dY), m_dWidth (dWidth), m_dHeight (dHeight), m_dBaseLineOffset(dBaseLineOffset), m_lFlags(lFlags), m_nBegin(nBegin), m_nEnd (nEnd)
		{			
			m_bStr				=	NULL;

			m_bsUnicodeText		=	::SysAllocStringLen (bsUnicodeText, (UINT)wcslen(bsUnicodeText));
			m_bsGidText			=	::SysAllocStringLen (bsGidText, (UINT)wcslen(bsGidText));
			m_bsSourceCodeText	=	::SysAllocStringLen (bsSourceCodeText, (UINT)wcslen(bsSourceCodeText)); 
		}	

		~CTextItemRef ()
		{
			if (m_bStr)
				::SysFreeString(m_bStr);

			if (m_bsUnicodeText)
				::SysFreeString(m_bsUnicodeText);

			if (m_bsGidText)
				::SysFreeString(m_bsGidText);

			if (m_bsSourceCodeText)
				::SysFreeString(m_bsSourceCodeText);
		}

		inline void AddToTable ()
		{
			m_bInTable	=	TRUE;
		}
		inline BOOL IsInTable ()
		{
			return m_bInTable;
		}	

		inline CPoint GetLeftTop () const	//	TODO 
		{
			return CPoint (m_dX, m_dY);				//	TODO 
		}

		inline CPoint GetRightBottom () const	//	TODO 
		{
			return CPoint (m_dX + m_dWidth, m_dY + m_dHeight);		//	TODO 
		}

		inline unsigned long GetBufBegin ()
		{
			return m_nBegin;
		}

		inline unsigned long GetBufEnd ()
		{
			return m_nEnd;
		}


	public:

		BOOL			m_bInTable;

		BSTR			m_bStr;

		BSTR			m_bsUnicodeText;
		BSTR			m_bsGidText;
		BSTR			m_bsSourceCodeText; 
		DWORD			m_lFlags;

		double			m_dX;
		double			m_dY;
		double			m_dWidth;
		double			m_dHeight;
		double			m_dBaseLineOffset;

		unsigned long	m_nBegin;
		unsigned long	m_nEnd;
	};

	class CBorderCell
	{
	public:

		enum BORDER
		{
			TOP_BORDER,
			LEFT_BORDER,
			BOTTOM_BORDER,
			RIGHT_BORDER
		};

	public:

		CBorderCell () : m_bEnable(TRUE)
		{

		}

		inline unsigned long GetColor () const
		{
			return m_nColor;
		}

	private:

		BOOL m_bEnable;
		unsigned long m_nColor;
	};

	class CCell
	{
	public:
		enum CELL_TYPE
		{
			REFERENCE_TYPE,
			FREE_CONTENT
		};

	public:

		CCell () : m_dWidth (5.0), m_nSpan (0)
		{

		}

		CCell (double dWidth) : m_dWidth (dWidth), m_nSpan (0)
		{

		}

		inline unsigned long GetColor () const
		{
			return m_nColor;
		}

		inline double GetWidth () const
		{
			return m_dWidth;
		}

		inline void SetWidth (double dWidth)
		{
			m_dWidth	=	dWidth;
		}

		inline void AddTextRef (CTextItemRef* pRef)
		{
			if (pRef)
			{
				m_arrTextItems.Add (pRef);
			}
		}

		inline CTextItemRef* GetTextItem (size_t nInd) const
		{
			if (nInd >= m_arrTextItems.GetCount())
				return NULL;

			return m_arrTextItems [nInd];
		}

		inline size_t GetTextItems() const
		{
			return m_arrTextItems.GetCount ();
		}

		inline void UpdateSpan (BOOL bSetOne = FALSE)
		{
			if (bSetOne)
				m_nSpan	=	1;
			else
				++m_nSpan;
		}

		inline int GetSpan () const
		{
			return m_nSpan;
		}

	private:

		int		m_nType;
		double	m_dWidth;

		int		m_nSpan;

		CBorderCell m_oBorders[4];
		unsigned long m_nColor;

		CAtlArray<CTextItemRef*> m_arrTextItems;
	};

	class CRowCells
	{
	public:
		CRowCells() : m_dHeight (5.0)
		{

		}

		CRowCells(double dHeight) : m_dHeight(dHeight)
		{

		}

		inline BOOL AddCell (double dWidth)
		{
			m_arCells.Add (new CCell(dWidth));

			return TRUE;
		}

		inline double GetHeight ()
		{
			return m_dHeight;
		}

		inline size_t GetCount ()
		{
			return m_arCells.GetCount ();
		}

		inline CCell* GetCell (size_t nInd)
		{
			if (nInd >= m_arCells.GetCount())
				return NULL;

			return m_arCells [nInd];
		}

		inline void RemoveAt (size_t nInd)
		{
			if (nInd < m_arCells.GetCount())
				m_arCells.RemoveAt(nInd);
		}

	private:

		double	m_dHeight;

		BOOL	m_bOneElement;

		CAtlArray <CCell*>	m_arCells;			// 
	};

	class CTable
	{
	public:

		CTable() : m_bIsValid (FALSE)
		{

		}

		CTable(const CPoint& oLeftTop, const CPoint& oSize) : m_bIsValid (FALSE)
		{
			m_oLeftTop	=	oLeftTop;
			m_oSize		=	oSize;
		}

		CTable(const CBoundingBox& oBox) : m_bIsValid (FALSE)
		{
			BuildWithBox (oBox);
		}

		inline const CPoint& GetSize() const
		{
			return m_oSize;
		}

		inline const CPoint& GetLeftTop() const
		{
			return m_oLeftTop;
		}

		//
		inline BOOL IsValid ()
		{
			return m_bIsValid;
		}

		inline size_t GetRowsCount ()
		{
			return m_arrRows.GetCount ();
		}

		inline CRowCells* GetRow (size_t nInd)
		{
			if (nInd >= m_arrRows.GetCount())
				return NULL;

			return m_arrRows [nInd];
		}

		inline const DoubleArray& GetGridCol() const
		{
			return m_arrGridCol;
		}

		//
		inline void AddTextRef (CTextItemRef* pRef)
		{
			double dX	=	m_oLeftTop.GetX();
			double dY	=	m_oLeftTop.GetY();

			double dTX	=	pRef->GetLeftTop ().GetX ();
			double dTY	=	pRef->GetLeftTop ().GetY ();

			for (size_t i = 0; i < m_arrRows.GetCount (); ++i)
			{
				CRowCells* pRow = m_arrRows[i];
				if ( (dY < dTY) && (dTY < dY + pRow->GetHeight()))
				{				
					for (size_t j = 0; j < pRow->GetCount(); ++j)
					{
						CCell* pCell = pRow->GetCell(j);
						if ((dX < dTX ) && (dTX < dX + pCell->GetWidth()))
						{
							pCell->AddTextRef(pRef);
							pRef->AddToTable();			//	TODO : 
						}

						dX	+=	pCell->GetWidth();
					}

					break;
				}

				dX	=	m_oLeftTop.GetX();
				dY	+=	pRow->GetHeight();
			}
		}

		inline CPointInd GetIndCell (double dTX, double dTY)
		{
			CPointInd oInd (-1, -1);

			double dX	=	m_oLeftTop.GetX();
			double dY	=	m_oLeftTop.GetY();

			for (size_t nY = 0; nY < m_arrRows.GetCount (); ++nY)
			{
				CRowCells* pRow = m_arrRows[nY];
				if ( (dY < dTY) && (dTY < dY + pRow->GetHeight()))
				{				
					for (size_t nX = 0; nX < pRow->GetCount(); ++nX)
					{
						CCell* pCell = pRow->GetCell(nX);
						if ((dX < dTX) && (dTX < dX + pCell->GetWidth()))
							return CPointInd (nX, nY);

						dX	+=	pCell->GetWidth();
					}
				}

				dX			=	m_oLeftTop.GetX();
				dY			+=	pRow->GetHeight();
			}

			return CPointInd (-1, -1);
		}

		inline CPoint GetCellPos (const CPointInd& oInd)
		{
			CPoint oPos;

			if (oInd.IsValid())
			{
				if (oInd.GetY() < (int)m_arrRows.GetCount())
				{
					for (int i = 0; i < oInd.GetY(); ++i)
					{
						oPos.Add (0, m_arrRows[i]->GetHeight());
					}

					CRowCells* pRow = m_arrRows[oInd.GetY()];

					for (int j = 0; j < oInd.GetX(); ++j)
					{
						oPos.Add (pRow->GetCell(j)->GetWidth(), 0);
					}					
				}
			}

			return oPos;
		}
		//
		inline BOOL IsContainByY (double dY)
		{
			if (m_oLeftTop.GetY() <= dY && dY <= m_oLeftTop.GetY() + m_oSize.GetY())
				return TRUE;

			return FALSE;
		}

	private:

		inline void BuildWithBox (const CBoundingBox& oBox)
		{
			m_arrGridCol.RemoveAll();

			m_oLeftTop			=	oBox.GetLeftTop();
			m_oSize				=	oBox.GetSize ();

			const CSortLines& oLines	=	oBox.GetInnerLines();

			for (size_t i = 0; i < oLines.GetNH() - 1; ++i)
			{
				double dHeight = oLines.GetHLine(i+1)->GetY() - oLines.GetHLine(i)->GetY();

				CRowCells* pRow	= new CRowCells (dHeight);

				for (size_t j = 0; j < oLines.GetNV() - 1; ++j)
				{
					double dWidth	=	oLines.GetVLine(j+1)->GetX() - oLines.GetVLine(j)->GetX();
					pRow->AddCell (dWidth);

					if (i==0)
					{
						m_arrGridCol.Add(dWidth);
					}
				}

				m_arrRows.Add (pRow);
			}

			TestHorizontalMerge (oBox);

			ATLTRACE (L"--------------------------------------------------\n");
			ATLTRACE (L"Detect Table \n");

			for (size_t i = 0; i < oLines.GetNH() - 1; ++i)
			{
				double dHeight = oLines.GetHLine(i+1)->GetY() - oLines.GetHLine(i)->GetY();

				CRowCells* pRow	= new CRowCells (dHeight);

				for (size_t j = 0; j < oLines.GetNV() - 1; ++j)
				{
					double dWidth	=	oLines.GetVLine(j+1)->GetX() - oLines.GetVLine(j)->GetX();
					ATLTRACE (L"(%d,%d - %d) ", i, j, (int)dWidth);
				}

				ATLTRACE (L" - height : %d\n", (int)dHeight);
			}

			ATLTRACE (L"--------------------------------------------------\n\n");

			m_bIsValid	=	TRUE;
		}

		inline void TestHorizontalMerge (const CBoundingBox& oBox)
		{
			double dX		=	m_oLeftTop.GetX();
			double dY		=	m_oLeftTop.GetY();

			for (size_t nY = 0; nY < m_arrRows.GetCount(); ++nY)		//	Y
			{
				double dCY	=	m_arrRows[nY]->GetHeight() * 0.5;

				dY			+=	dCY;

				BOOL bRebuildRow = TRUE;

				while (bRebuildRow)
				{
					bRebuildRow			=	FALSE;
					dX					=	m_oLeftTop.GetX();

					CRowCells* pRow		=	m_arrRows[nY];

					for (size_t nX = 0; nX < pRow->GetCount() - 1; ++nX)
					{
						CCell* pCur		=	pRow->GetCell(nX);
						CCell* pNext	=	pRow->GetCell(nX + 1);

						// Если нет пересечение хотя бы с одной линией - отрезка из двух центров соседних ячееек, то нужно смержить две ячейки

						CLine oLine (dX + pCur->GetWidth() * 0.5, dY, dX + pCur->GetWidth() + pNext->GetWidth() * 0.5, dY, NULL);
						if (0 == oBox.GetInnerLines().InCross(&oLine))	
						{
							pCur->SetWidth (pCur->GetWidth() + pNext->GetWidth());
							pRow->RemoveAt (nX + 1);
							pCur->UpdateSpan();

							bRebuildRow	=	TRUE;
							break;
						}

						dX	+=	pCur->GetWidth();
					}
				}

				dY	+=	dCY;


				// right border test

				// left border test

			}
		}

	private:

		BOOL m_bIsValid;

		CPoint m_oLeftTop;
		CPoint m_oSize;

		BOOL m_bSimple;

		CAtlArray<CRowCells*>	m_arrRows;
		DoubleArray				m_arrGridCol;
	};

	class CTableCollector
	{
	public:

		CTableCollector()
		{
		}

		~CTableCollector()
		{
			for (size_t i = 0; i < m_arTables.GetCount (); ++i)
				RELEASEOBJECT(m_arTables[i]);

			for (size_t i = 0; i < m_arrTextItems.GetCount (); ++i)
				RELEASEOBJECT(m_arrTextItems[i]);
		}

		inline BOOL AddText (CTextItemRef* pItem)		// TODO :
		{
			if (pItem)
			{
				m_arrTextItems.Add (pItem);
				return TRUE;
			}

			return FALSE;				
		}

		inline CTextItemRef* GetTextItem (size_t nInd) const
		{
			if (nInd >= m_arrTextItems.GetCount())
				return NULL;

			return m_arrTextItems [nInd];
		}

		inline size_t GetTextItems() const
		{
			return m_arrTextItems.GetCount ();
		}

		inline CLinesStorage& GetLines()
		{
			return m_oLines;
		}

		inline size_t GetCount() const
		{
			return m_arTables.GetCount ();
		}

		inline CTable* GetTable (size_t nInd) const
		{
			if (nInd >= m_arTables.GetCount())
				return NULL;

			return m_arTables [nInd];
		}

		//
		inline BOOL Normalize()
		{
			if(m_oLines.Normalize())
			{
				for (size_t i = 0; i < m_oLines.GetBBS().GetCount(); ++i)
				{
					CBoundingBox oBox	=	m_oLines.GetBBS().GetBox(i);
					if (oBox.IsValid())
					{
						CTable* pTable	=	new CTable (oBox);
						if (pTable)
						{
							if (pTable->IsValid())
							{
								m_arTables.Add (pTable);

								for (size_t i = 0; i < m_arrTextItems.GetCount (); ++i)
								{
									CTextItemRef* pItem = m_arrTextItems[i];

									CPoint oLT = pItem->GetLeftTop();
									CPoint oRB = pItem->GetRightBottom();

									if (oBox.InArea(oLT, oRB, TRUE))	// TODO :
										pTable->AddTextRef(pItem);
								}

								continue;
							}

							RELEASEOBJECT(pTable);
						}
					}
				}

				return (m_arTables.GetCount() > 0);
			}

			return FALSE;
		}

	private:


	private:

		CLinesStorage				m_oLines;

		CAtlArray<CTable*>			m_arTables;
		CAtlArray<CTextItemRef*>	m_arrTextItems;
	};
};