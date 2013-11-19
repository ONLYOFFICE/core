namespace NSEBook
{
	class CTable
	{
	private:
		double m_x;
		double m_y;
		double m_width;
		double m_height;

		ATL::CSimpleArray<double> m_columns;
		ATL::CSimpleArray<double> m_rows;

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
			m_rows.GetSize();
		}

		int GetCountColumns() const
		{
			m_columns.GetSize();
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
			return ((id < m_rows.GetSize() - 1) ? m_rows[id + 1] : m_height) - m_rows[id];
		}

		double GetColumnSize( int id ) const
		{
			return ((id < m_columns.GetSize() - 1) ? m_columns[id + 1] : m_height) - m_columns[id];
		}

	};

	class CTableDetector
	{
	protected:
		class TGroup;
		
		class TBlock
		{
			double m_x1, m_y1, m_x2, m_y2;
			TGroup* m_group;

		public:

			TBlock()
			{
				m_x1 = 0;
				m_y1 = 0;
				m_x2 = 0;
				m_y2 = 0;
				m_group = NULL;
			}

			TBlock( double _x1, double _y1, double _x2, double _y2 )
			{
				Set( _x1, _y1, _x2, _y2 );
			}

			void Set( double _x1, double _y1, double _x2, double _y2 )
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

				m_group = NULL;
			}

			double GetX() const
			{
				return m_x1;
			}

			double GetY() const
			{
				return m_y1;
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

			double GetX2() const
			{
				return m_x2;
			}
			double GetY2() const
			{
				return m_y2;
			}
			bool Intersect( const TBlock& out ) const
			{
				double l = max(m_x1, out.m_x1);
				double t = max(m_y1, out.m_y1);
				double r = min(m_x2, out.m_x2);
				double b = min(m_y2, out.m_y2);

				if( l > r || t > b )
					return false;

				if( l <= m_x1 && t <= m_y1 && r >= m_x2 && b >= m_y2 )
					return false;
				
				return true;
			}

			bool Intersect( const TBlock& out, TBlock& result ) const
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

			void SetGroup( TGroup* pGroup )
			{
				m_group = pGroup;
			}

			TGroup* GetGroup() const
			{
				return m_group;
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

		void AddBlock( const double points[4][2] )
		{
			if( points[0][0] != points[1][0] )
			{
				if( abs(points[0][1] - points[1][1]) > FLT_EPSILON || 
					abs(points[1][0] - points[2][0]) > FLT_EPSILON || 
					abs(points[2][1] - points[3][1]) > FLT_EPSILON ||
					abs(points[3][0] - points[0][0]) > FLT_EPSILON )
				{
					return;
				}
			}
			else
			{
				if( abs(points[0][0] - points[1][0]) > FLT_EPSILON || 
					abs(points[1][1] - points[2][1]) > FLT_EPSILON || 
					abs(points[2][0] - points[3][0]) > FLT_EPSILON ||
					abs(points[3][1] - points[0][1]) > FLT_EPSILON )
				{
					return;
				}
			}

			TBlock block( points[0][0], points[0][1], points[2][0], points[2][1] );

			if( block.GetWidth() < block.GetHeight() )
			{
				m_VerBlocks.AddTail( block );
			}
			else
			{
				m_HorBlocks.AddTail( block );
			}
		}

		void AddBlock( double x, double y, double width, double height )
		{
			TBlock block( x, y, x + width, y + height );

			if( block.GetWidth() < block.GetHeight() )
			{
				m_VerBlocks.AddTail( block );
			}
			else
			{
				m_HorBlocks.AddTail( block );
			}
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

							TGroup* pHorGroup = pHorBlock->GetGroup();
							TGroup* pVerGroup = pVerBlock->GetGroup();

							if( pHorGroup == pVerGroup )
							{
								if( !pHorGroup )
								{
									if( groups.AddTail() )
									{
										TGroup* pNewGroup = &groups.GetTail();
										
										pNewGroup->m_blocks.Add( pHorBlock );
										pHorBlock->SetGroup( pNewGroup );

										pNewGroup->m_blocks.Add( pVerBlock );
										pVerBlock->SetGroup( pNewGroup );
									}
								}
							}
							else
							{
								if( !pHorGroup )
								{
									pVerGroup->m_blocks.Add( pHorBlock );
									pHorBlock->SetGroup( pVerGroup );
								}
								else if( !pVerGroup )
								{
									pHorGroup->m_blocks.Add( pVerBlock );
									pVerBlock->SetGroup( pHorGroup );
								}
								else
								{
									for( size_t i = 0; i < pVerGroup->m_blocks.GetCount(); i++ )
									{
										pVerGroup->m_blocks[i]->SetGroup( pHorGroup );
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

					if( pHorBlock->GetGroup() != pVerBlock->GetGroup() )
					{
						continue;
					}

					pHorBlock->GetGroup()->m_points.Add( pPoint );
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

					if( pBufferY && pBufferX )
					{
						for( size_t k = 0; k < nCountPoints; k++ )
						{
							pBufferX[k] = data[k]->m_x;
							pBufferY[k] = data[k]->m_y;
						}

						qsort( pBufferX, nCountPoints, sizeof(double), CmpDouble );
						qsort( pBufferY, nCountPoints, sizeof(double), CmpDouble );

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

							if( table.IsEmpty() )
								m_Tables.RemoveAt( pPosTable );
						}
					}

					delete [] pBufferY;
					delete [] pBufferX;
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


	private:
		static int CmpDouble( const void* p1, const void* p2 )
		{
			return (*((double*)p1) <= *((double*)p2)) ? -1 : 1;
		}

	};
}