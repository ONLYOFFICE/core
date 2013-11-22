#pragma once

#include <ImageColors.h>
#include <Utilities.h>
#include <ImageIO.h>
#include <ImagePaintState.h>
#include <ImageSerializeObjects.h>
#include <TransformConstants.h>
#include <ImageTransformsCore.h>
#include <ImageTransformsCoreConstants.h>
#include <ImageTransformsCoreWrappers.h>
#include <ImageTransformsCoreGdiplus.h>
#include <ImageTransformsCoreBGRA.h>
#include <ImageTransformsCoreCOM.h>
#include <ImagePaintCompose.h>

#include <ImagePaintState.h>
#include <ImageCaching.h>

namespace Tracking
{
	namespace Paint
	{
		namespace Text
		{
			namespace Constants
			{
				enum ETextFillMode
				{
					c_nTextFillMode_Normal = 0,
					c_nTextFillMode_Letter = 1,
					c_nTextFillMode_HorCylinder = 2,
					c_nTextFillMode_VerCylinder = 3
				};

				enum ETextAnimation
				{
					c_nTextAnim_AlphaSimple = 1,              // простой fade на все буквы одновременно
					c_nTextAnim_AlphaLetterAZ = 2,            // побуквенный fade слева на право
					c_nTextAnim_AlphaLetterRandom = 3,        // плавный побуквенный fade в случайном порядке
					c_nTextAnim_AlphaLetterAZSmooth = 4,      // плавный побуквенный fade слева на право
					c_nTextAnim_AlphaLetterRandomGlass = 5,   // плавный побуквенный fade в случайном порядке c эффектом Лупа
					c_nTextAnim_AlphaLetterAZSmoothGlass = 6, // плавный побуквенный fade слева на право с эффектом Лупа
					c_nTextAnim_AlphaLetterRandomScale = 7,   // плавный побуквенный fade в случайном порядке c увеличением
					c_nTextAnim_AlphaLetterAZSmoothScale = 8, // плавный побуквенный fade слева на право с увеличением

					c_nTextAnim_AlphaDualToCenter = 80,       // плавный побуквенный fade от краёв к центру
					c_nTextAnim_AlphaDualFromCenter = 81,     // плавный побуквенный fade от центра к краям

					c_nTextAnim_DropLeft = 10,                // побуквенный drop слева с изменением прозрачности букв
					c_nTextAnim_DropTop = 11,                 // побуквенный drop сверху с изменением прозрачности букв
					c_nTextAnim_DropRight = 12,               // побуквенный drop справа с изменением прозрачности букв
					c_nTextAnim_DropBottom = 13,              // побуквенный drop снизу с изменением прозрачности букв
					c_nTextAnim_DropTopLeft = 14,             // побуквенный drop снизу с изменением прозрачности букв
					c_nTextAnim_DropTopRight = 15,            // побуквенный drop снизу с изменением прозрачности букв
					c_nTextAnim_DropBottomLeft = 16,          // побуквенный drop снизу с изменением прозрачности букв
					c_nTextAnim_DropBottomRight = 17,         // побуквенный drop снизу с изменением прозрачности букв
					c_nTextAnim_DropRotate1 = 20,             // побуквенный rotate против часовой стрелки с изменением прозрачности букв
					c_nTextAnim_DropRotate2 = 21,             // побуквенный rotate по часовой стрелке с изменением прозрачности букв

					c_nTextAnim_MoveLeft = 30,                // движение текста влево
					c_nTextAnim_MoveTop = 31,                 // движение текста вверх
					c_nTextAnim_MoveRight = 32,               // движение текста вправо
					c_nTextAnim_MoveBottom = 33,              // движение текста вниз
					c_nTextAnim_MoveTopLeft = 34,             // движение текста из верхнего левого угла
					c_nTextAnim_MoveTopRight = 35,            // движение текста из верхнего правого угла
					c_nTextAnim_MoveBottomLeft = 36,          // движение текста из нижнего левого угла
					c_nTextAnim_MoveBottomRight = 37,         // движение текста из нижнего правого угла

					c_nTextAnim_ScaleFromSmall = 40,               // увеличение до нормального размера (с альфой)
					c_nTextAnim_ScaleFromLarge = 41,               // уменьшение до нормального размера (с альфой)
					c_nTextAnim_ScaleFromXLargeYSmall = 42,        // уменьшение по Х и увеличение по Y (с альфой)
					c_nTextAnim_ScaleFromXSmallYLarge = 43,        // увеличение по Х и уменьшение по Y (с альфой)
					c_nTextAnim_ScaleFromSmallDropFromTop = 44,    // увеличение и падение сверху (с альфой)
					c_nTextAnim_ScaleFromSmallDropFromBottom = 45, // увеличение и прилёт снизу (с альфой)

					c_nTextAnim_Rotate1 = 50,                // вращение по часовой стрелке
					c_nTextAnim_Rotate2 = 51,                // вращение против часовой стрелки
					c_nTextAnim_RotateFromSmall1 = 52,       // вращение по часовой стрелке с увеличением
					c_nTextAnim_RotateFromSmall2 = 53,       // вращение против часовой стрелки с увеличением
					c_nTextAnim_RotateFromLarge1 = 54,       // вращение по часовой стрелке с уменьшением
					c_nTextAnim_RotateFromLarge2 = 55,       // вращение против часовой стрелки с уменьшением

					c_nTextAnim_StretchScale = 60,           // побуквенное расширение
					c_nTextAnim_StretchHorScale = 61,        // побуквенное расширение по горизонтали
					c_nTextAnim_StretchVerScale = 62,        // побуквенное расширение по вертикали
					c_nTextAnim_StretchLVerScale = 63,       // побуквенное сжатие по вертикали
					c_nTextAnim_StretchLVerHorScale = 64,    // побуквенное растяжение по горизонтали и сжатие по вертикали
					
					c_nTextAnim_StretchSmall = 65,           // растяжение по горизонтали с альфой
					c_nTextAnim_StretchSmallScale = 66,      // растяжение по горизнотали с расширением букв по горизонтали и вертикали
					c_nTextAnim_StretchSmallHorScale = 67,   // растяжение по горизнотали с расширением по горизонтали
					c_nTextAnim_StretchSmallVerScale = 68,   // растяжение по горизнотали с расширением букв по вертикали
					c_nTextAnim_StretchSmallLargeVerScale = 69, // растяжение по горизнотали с сжатием букв по вертикали
					c_nTextAnim_StretchSmallLargeVerHorScale = 70, // растяжение по горизнотали с сжатием букв по вертикали и расширением по горизонтали
					c_nTextAnim_StretchLarge = 71,           // сжатие по горизонтали с альфой
					c_nTextAnim_StretchLargeScale = 72,      // сжатие по горизнотали с масштабированием
					c_nTextAnim_StretchLargeHorScale = 73,   // сжатие по горизнотали с расширением букв по горизонтали
					c_nTextAnim_StretchLargeVerScale = 74,   // сжатие по горизнотали с расширением букв по вертикали
					c_nTextAnim_StretchLargeLargeVerScale = 75, // сжатие по горизнотали с жатием букв по вертикали
					c_nTextAnim_StretchLargeLargeVerHorScale = 76, // сжатие по горизнотали с сжатием букв по вертикали и расширением по горизонтали
				};
			}

			namespace Transforms
			{
				class CTimeMarks
				{
				protected:
					double* m_pBuffer;
					int     m_nCountPoints;
					int     m_nTotalPoints;

				public:
					CTimeMarks() 
						: m_pBuffer( 0 ), m_nCountPoints( 0 ), m_nTotalPoints( 0 )
					{
					}
					CTimeMarks( const CTimeMarks& other )
						: m_pBuffer( 0 ), m_nCountPoints( 0 ), m_nTotalPoints( 0 )
					{
						if( other.m_nCountPoints <= 0 )
							return;

						int     nTotalSize = AlignSize(m_nCountPoints);
						double* pNewBuffer = new double[nTotalSize];
						if( !pNewBuffer )
							return;

						memcpy( pNewBuffer, other.m_pBuffer, other.m_nCountPoints * sizeof(double) );

						m_pBuffer      = pNewBuffer;
						m_nCountPoints = other.m_nCountPoints;
						m_nTotalPoints = nTotalSize;
					}
					~CTimeMarks()
					{
						if( m_pBuffer )
							delete [] m_pBuffer;
					}
					
					void Clear()
					{
						if( m_pBuffer )
						{
							delete [] m_pBuffer;
							m_pBuffer = 0;
						}

						m_nCountPoints = 0;
						m_nTotalPoints = 0;
					}
					
					const CTimeMarks& operator = (const CTimeMarks& other)
					{
						if( m_nTotalPoints < other.m_nCountPoints )
						{
							int     nTotalSize = AlignSize( other.m_nCountPoints );
							double* pNewBuffer = new double[nTotalSize];
							if( !pNewBuffer )
							{
								// копируем хотя бы часть данных
								memcpy( m_pBuffer, other.m_pBuffer, m_nTotalPoints * sizeof(double) );
								
								m_nCountPoints = m_nTotalPoints;

								return *this;
							}

							if( m_pBuffer )
								delete [] m_pBuffer;
							
							m_pBuffer      = pNewBuffer;
							m_nTotalPoints = nTotalSize;
						}

						if( other.m_nCountPoints > 0 )
						{
							memcpy( m_pBuffer, other.m_pBuffer, other.m_nCountPoints * sizeof(double) );
						}

						m_nCountPoints = other.m_nCountPoints;

						return *this;
					}

					BOOL ReservePoints( int nCountPoints )
					{
						if( m_nCountPoints + nCountPoints > m_nTotalPoints )
						{
							int     nTotalSize = AlignSize( m_nCountPoints + nCountPoints );
							double* pNewBuffer = new double[nTotalSize];
							if( !pNewBuffer )
								return FALSE;

							if( m_pBuffer )
							{
								memcpy( pNewBuffer, m_pBuffer, m_nCountPoints * sizeof(double) );
								delete [] m_pBuffer;
							}

							m_pBuffer      = pNewBuffer;
							m_nTotalPoints = nTotalSize;
						}

						return TRUE;
					}

					BOOL AddPoint( double duration )
					{
						if( !ReservePoints( 1 ) )
							return FALSE;

						ApplyPoint( duration );

						return TRUE;
					}

					BOOL AddPoints( double* pPoints, int nCountPoints, int nStride )
					{
						if( !pPoints )
							return FALSE;

						if( !ReservePoints( nCountPoints ) )
							return FALSE;

						while( nCountPoints-- > 0 )
						{
							ApplyPoint( *pPoints );

							pPoints = (double*)(((__int8*)pPoints) + nStride);
						}

						return TRUE;
					}

					void RemoveLastPoint()
					{
						if( m_nCountPoints > 0 )
						{
							m_nCountPoints -= 1;
						}
					}
					void RemoveLastPoints( int nCountPoints )
					{
						if( nCountPoints > 0 )
						{
							m_nCountPoints -= nCountPoints;
							if( m_nCountPoints < 0 )
								m_nCountPoints = 0;
						}
					}
					
					int GetCountPoints() const
					{
						// нулевая временная метка присутствует виртуально, 
						// поэтому количество меток всегда равно m_nCountPoints + 1
						return m_nCountPoints + 1;
					}
					int GetTotalPoints() const
					{
						// нулевая временная метка присутствует виртуально, 
						// поэтому количество меток всегда равно m_nTotalPoints + 1
						return m_nTotalPoints + 1;
					}

					void GetMarks( int& nFirstMark, int& nSecondMark, double& dMarkTime, double dTime ) const
					{
						nFirstMark  = 0;
						nSecondMark = 0;
						dMarkTime   = 0;
						
						if( dTime <= 0 )
							return;

						if( dTime >= 1 )
						{
							nFirstMark  = m_nCountPoints;
							nSecondMark = m_nCountPoints;
							dMarkTime   = 1;
							return;
						}

						if( m_nCountPoints <= 1 )
						{
							nSecondMark = m_nCountPoints;
							dMarkTime   = dTime;
							return;
						}

						dTime *= m_pBuffer[m_nCountPoints - 1];

						// TODO: сделать бинарный поиск!

						double offset = 0;
						for( int nPoint = 0; nPoint < m_nCountPoints; ++nPoint )
						{
							if( dTime <= m_pBuffer[nPoint] )
							{
								nFirstMark  = nPoint;
								nSecondMark = nPoint + 1;
								dMarkTime   = (dTime - offset) / (m_pBuffer[nPoint] - offset);
								return;
							}
							offset = m_pBuffer[nPoint];
						}
					}

				protected:
					static int AlignSize( int size )
					{
						return size + 3 & ~3;
					}

					void ApplyPoint( double duration )
					{
						if( duration < IPP_EPS23 ) 
							duration = IPP_EPS23;
						
						if( duration > IPP_MAX_32S )
							duration = IPP_MAX_32S;

						if( m_nCountPoints > 0  )
							duration += m_pBuffer[m_nCountPoints - 1];

						m_pBuffer[m_nCountPoints++] = duration;
					}
				};
				
				template<typename T>
				class CTransforms
				{
				public:
					struct TTransform
					{
						T   transform;
						int interpolation;
						
						TTransform() 
							: transform(), 
							interpolation( TRUE )
						{
						}
					};

					struct TData
					{
						TTransform data;
						double     duration;
						
						TData() 
							: data(), duration( 1 )
						{
						}
					};

				private:
					CTimeMarks  m_oMarks;
					TTransform* m_pData;

				public:
					
					CTransforms() 
						: m_oMarks(), m_pData( 0 )
					{
					}

					CTransforms( const CTransforms& other ) 
						: m_oMarks( other.m_oMarks ), m_pData( 0 )
					{
						CreateBuffer( other.m_pData );		
					}

					~CTransforms()
					{
						Clear();
					}
					
					const CTransforms& operator = (const CTransforms& other)
					{
						RemoveBuffer();

						m_oMarks = other.m_oMarks;

						CreateBuffer( other.m_pData );

						return this*;
					}

					void Clear()
					{
						RemoveBuffer();
						m_oMarks.Clear();
					}
					
					void AddItems( TData* arrItems, int nCountItems )
					{
						if( !arrItems && nCountItems <= 0 )
							return;

						int nSize  = m_oMarks.GetCountPoints();
						int nMarks = nCountItems;
						if( !m_pData )
						{
							--nSize;
							--nMarks;
						}

						if( m_oMarks.AddPoints( &(arrItems[m_pData ? 0 : 1].duration), nMarks, sizeof(TData)) )
						{
							TTransform* pBuffer = AllocateBuffer( nSize + nCountItems );
							if( pBuffer )
							{
								if( m_pData )
								{
									memcpy( pBuffer, m_pData, nSize * sizeof(TTransform) );
									FreeBuffer( m_pData );
								}

								IppiSize bound = {sizeof(TTransform), nCountItems};
								ippiCopy_8u_C1R( (Ipp8u*)(&arrItems->data), sizeof(TData), (Ipp8u*)(pBuffer + nSize), sizeof(TTransform), bound );

								m_pData = pBuffer;
							}
							else
							{
								m_oMarks.RemoveLastPoints( nMarks );
							}
						}
					}		

					void Interpolate( T& result, double time ) const
					{
						if( m_pData )
						{
							int index1 = 0;
							int index2 = 0;

							m_oMarks.GetMarks( index1, index2, time, time );

							TTransform* pItem1 = m_pData + index1;
							TTransform* pItem2 = m_pData + index2;

							if( pItem2->interpolation && pItem1 != pItem2 )
								result.Interpolate( pItem1->transform, pItem2->transform, time );
							else
								result = pItem2->transform;
						}
					}

				protected:
					static TTransform* AllocateBuffer( int nCountItems )
					{
						return (TTransform*)( new __int64[nCountItems * sizeof(TTransform) >> 3] );
					}
					static void FreeBuffer( TTransform* pBuffer )
					{
						delete [] (__int64*)(pBuffer);
					}
					void RemoveBuffer()
					{
						if( m_pData )
						{
							TTransform* pBuffer = m_pData;
							int nCountItems = m_oMarks.GetCountPoints();
							
							for( ; nCountItems > 0; --nCountItems, ++pBuffer )
							{
								pBuffer->~TTransform();
							}
							
							FreeBuffer( m_pData );
							m_pData = 0;

							m_oMarks.RemoveLastPoints( nCountItems );
						}
						
					}
					BOOL CreateBuffer( TTransform* pOtherBuffer )
					{
						int nSize = m_oMarks.GetCountPoints();
						if( pOtherBuffer && nSize > 0 )
						{
							m_pData = AllocateBuffer( nSize );
							if( m_pData )
							{
								memcpy( m_pData, pOtherBuffer, nSize * sizeof(TTransform) );
								return TRUE;
							}
						}

						m_oMarks.RemoveLastPoints( nSize );
						return FALSE;
					}

				};

				
				class CScale
				{
				public:
					double dScaleX;
					double dScaleY;
					
					CScale() 
						: dScaleX( 1 ), dScaleY( 1 )
					{
					}
					
					void SetParams( double x, double y )
					{
						dScaleX = x;
						dScaleY = y;
					}
					void Interpolate( const CScale& src1, const CScale& src2, double time )
					{
						dScaleX = (src2.dScaleX - src1.dScaleX) * time + src1.dScaleX;
						dScaleY = (src2.dScaleY - src1.dScaleY) * time + src1.dScaleY;
					}
				};
				class CScaleAt
				{
				public:
					double dScaleX;
					double dScaleY;
					double dX;
					double dY;
					
					CScaleAt() 
						: dScaleX( 1 ), dScaleY( 1 ), dX( 0 ), dY( 0 )
					{
					}
					
					void SetParams( double scaleX, double scaleY, double x, double y )
					{
						dScaleX = scaleX;
						dScaleY = scaleY;
						dX      = x;
						dY      = y;
					}

					void Interpolate( const CScaleAt& src1, const CScaleAt& src2, double time )
					{
						dScaleX = (src2.dScaleX - src1.dScaleX) * time + src1.dScaleX;
						dScaleY = (src2.dScaleY - src1.dScaleY) * time + src1.dScaleY;
						dX      = (src2.dX - src1.dX)           * time + src1.dX;
						dY      = (src2.dY - src1.dY)           * time + src1.dY;
					}
				};

				class CShear
				{
				public:
					double dX;
					double dY;

					CShear()
						: dX( 0 ), dY( 0 )
					{
					}
					
					void SetParams( double x, double y )
					{
						dX = x;
						dY = y;
					}
					void Interpolate( const CShear& src1, const CShear& src2, double time )
					{
						dX = (src2.dX - src1.dX) * time + src1.dX;
						dY = (src2.dY - src1.dY) * time + src1.dY;
					}
				};

				class CTranslate
				{
				public:
					double dX;
					double dY;

					CTranslate()
						: dX( 0 ), dY( 0 )
					{
					}
					
					void SetParams( double x, double y )
					{
						dX = x;
						dY = y;
					}
					void Interpolate( const CTranslate& src1, const CTranslate& src2, double time )
					{
						dX = (src2.dX - src1.dX) * time + src1.dX;
						dY = (src2.dY - src1.dY) * time + src1.dY;
					}
				};

				class CRotate
				{
				public:
					double dAngle;
					
					CRotate() 
						: dAngle( 0 )
					{
					}

					
					void SetParams( double angle )
					{
						dAngle = angle;
					}
					void Interpolate( const CRotate& src1, const CRotate& src2, double time )
					{
						dAngle = (src2.dAngle - src1.dAngle) * time + src1.dAngle;
					}
				};
				class CRotateAt
				{
				public:
					double dAngle;
					double dX;
					double dY;
					
					CRotateAt() 
						: dAngle( 0 ), dX( 0 ), dY( 0 ) 
					{
					}
					
					void SetParams( double angle, double x, double y )
					{
						dAngle = angle;
						dX     = x;
						dY     = y;
					}
					void Interpolate( const CRotateAt& src1, const CRotateAt& src2, double time )
					{
						dAngle = (src2.dAngle - src1.dAngle) * time + src1.dAngle;
						dX     = (src2.dX     - src1.dX)     * time + src1.dX;
						dY     = (src2.dY     - src1.dY)     * time + src1.dY;
					}
				};
				class CTransparency
				{
				public:
					double dFactor;
					
					CTransparency() 
						: dFactor( 0 )
					{
					}

					
					void SetParams( double factor )
					{
						dFactor = factor;
					}
					void Interpolate( const CTransparency& src1, const CTransparency& src2, double time )
					{
						dFactor = (src2.dFactor - src1.dFactor) * time + src1.dFactor;
					}
				};
				
				class CGeometryTransforms
				{
				public:
					virtual void AddToMatrix( Gdiplus::Matrix& oMatrix, double dTime ) = 0;
				};
				class CScaleTransforms : public CTransforms<CScale>, public CGeometryTransforms
				{
				public:
					virtual void AddToMatrix( Gdiplus::Matrix& oMatrix, double dTime )
					{
						CScale oScale;
						Interpolate( oScale, dTime );
						
						float fx = oScale.dScaleX > 0 ? \
							(oScale.dScaleX > IPP_EPS23  ? float(oScale.dScaleX) : IPP_EPS23) : \
							(oScale.dScaleX < -IPP_EPS23 ? float(oScale.dScaleX) : -IPP_EPS23);

						float fy = oScale.dScaleY > 0 ? \
							(oScale.dScaleY > IPP_EPS23  ? float(oScale.dScaleY) : IPP_EPS23) : \
							(oScale.dScaleY < -IPP_EPS23 ? float(oScale.dScaleY) : -IPP_EPS23);

						oMatrix.Scale( fx, fy, Gdiplus::MatrixOrderAppend );
					}
				};
				class CScaleAtTransforms : public CTransforms<CScaleAt>, public CGeometryTransforms
				{
				public:
					virtual void AddToMatrix( Gdiplus::Matrix& oMatrix, double dTime )
					{
						CScaleAt oScale;
						Interpolate( oScale, dTime );

						float fx = oScale.dScaleX > 0 ? \
							(oScale.dScaleX > IPP_EPS23  ? float(oScale.dScaleX) : IPP_EPS23) : \
							(oScale.dScaleX < -IPP_EPS23 ? float(oScale.dScaleX) : -IPP_EPS23);

						float fy = oScale.dScaleY > 0 ? \
							(oScale.dScaleY > IPP_EPS23  ? float(oScale.dScaleY) : IPP_EPS23) : \
							(oScale.dScaleY < -IPP_EPS23 ? float(oScale.dScaleY) : -IPP_EPS23);

						oMatrix.Translate( Gdiplus::REAL(oScale.dX), Gdiplus::REAL(oScale.dY), Gdiplus::MatrixOrderAppend );
						oMatrix.Scale( fx, fy, Gdiplus::MatrixOrderAppend );
						oMatrix.Translate( Gdiplus::REAL(-oScale.dX), Gdiplus::REAL(-oScale.dY), Gdiplus::MatrixOrderAppend );
					}
				};
				class CShearTransforms : public CTransforms<CShear>, public CGeometryTransforms
				{
				public:
					virtual void AddToMatrix( Gdiplus::Matrix& oMatrix, double dTime )
					{
						CShear oShear;
						Interpolate( oShear, dTime );
						oMatrix.Shear( Gdiplus::REAL(oShear.dX), Gdiplus::REAL(oShear.dY), Gdiplus::MatrixOrderAppend );
					}
				};
				class CTranslateTransforms : public CTransforms<CTranslate>, public CGeometryTransforms
				{
				public:
					virtual void AddToMatrix( Gdiplus::Matrix& oMatrix, double dTime )
					{
						CTranslate oTranslate;
						Interpolate( oTranslate, dTime );
						oMatrix.Translate( Gdiplus::REAL(oTranslate.dX), Gdiplus::REAL(oTranslate.dY), Gdiplus::MatrixOrderAppend );
					}
				};

				class CRotateTransforms : public CTransforms<CRotate>, public CGeometryTransforms
				{
				public:
					virtual void AddToMatrix( Gdiplus::Matrix& oMatrix, double dTime )
					{
						CRotate oRotate;
						Interpolate( oRotate, dTime );
						oMatrix.Rotate( Gdiplus::REAL(oRotate.dAngle), Gdiplus::MatrixOrderAppend );
					}
				};

				class CRotateAtTransforms : public CTransforms<CRotateAt>, public CGeometryTransforms
				{
				public:
					virtual void AddToMatrix( Gdiplus::Matrix& oMatrix, double dTime )
					{
						CRotateAt oRotateAt;
						Interpolate( oRotateAt, dTime );
						
						if( abs(oRotateAt.dX) > IPP_EPS23 || abs(oRotateAt.dY) > IPP_EPS23 )
							oMatrix.RotateAt( Gdiplus::REAL(oRotateAt.dAngle), Gdiplus::PointF(Gdiplus::REAL(oRotateAt.dX), Gdiplus::REAL(oRotateAt.dY)), Gdiplus::MatrixOrderAppend );
						else
							oMatrix.Rotate( Gdiplus::REAL(oRotateAt.dAngle) );
					}
				};
				class CTransparencyTransforms : public CTransforms<CTransparency>
				{
				public:
					void AddToFactor( double& factor, double dTime )
					{
						CTransparency transparency;
						Interpolate( transparency, dTime );
						factor *= transparency.dFactor;
					}
				};
				
				class CTransformManager
				{
				protected:
					ATL::CSimpleArray<CGeometryTransforms*>     m_arrpGeometryTransforms;
					ATL::CSimpleArray<CTransparencyTransforms*> m_arrpTransparencyTransforms;

					double m_dTimeOffset;
					double m_dTimeDuration;
				
				public:
					CTransformManager()
					{
						m_dTimeOffset = 0;
						m_dTimeDuration = 1;
					}
					~CTransformManager()
					{
						ReleaseDynamicData();
					}
					void Clear()
					{
						m_dTimeOffset = 0;
						m_dTimeDuration = 1;

						ReleaseDynamicData();
					}
					
					void Update( double dTime, Gdiplus::Matrix& oTransform, double& dTransparency )
					{
						dTime = (dTime - m_dTimeOffset) / m_dTimeDuration;
						
						// выполняем последовательно геометрические трансформации
						CGeometryTransforms** pGeometryData = m_arrpGeometryTransforms.GetData();
						if( pGeometryData )
						{
							int nDataSize = m_arrpGeometryTransforms.GetSize();
							for( ; nDataSize > 0; --nDataSize, ++pGeometryData )
							{
								CGeometryTransforms* pTransforms = *pGeometryData;
								if( pTransforms )
								{
									pTransforms->AddToMatrix( oTransform, dTime );
								}
							}
						}

						// выполняем последовательно цветовые трансформации
						CTransparencyTransforms** pTransparencyData = m_arrpTransparencyTransforms.GetData();
						if( pTransparencyData )
						{
							int nDataSize = m_arrpTransparencyTransforms.GetSize();
							for( ; nDataSize > 0; --nDataSize, ++pTransparencyData )
							{
								CTransparencyTransforms* pTransforms = *pTransparencyData;
								if( pTransforms )
								{
									pTransforms->AddToFactor( dTransparency, dTime );
								}
							}
						}
					}
					
					void SetTimeParams( double offset, double duration )
					{
						m_dTimeOffset   = offset;
						m_dTimeDuration = duration > IPP_EPS52 ? duration : 1;
					}
					void SetTimeParams( double nLetter, double nCountLetters, double dCountActiveLetters )
					{
						double offset = 0;
						double duration = 1;

						if( nCountLetters > 1 && dCountActiveLetters > 0 )
						{
							double factor = (nCountLetters - 1 + dCountActiveLetters);

							offset   = nLetter / factor;
							duration = dCountActiveLetters / factor;
						}
						m_dTimeOffset   = offset;
						m_dTimeDuration = duration;
					}
					
					CScaleTransforms* CreateScaleTransforms()
					{
						CScaleTransforms* pTransforms = new CScaleTransforms();
						if( pTransforms )
						{
							if( m_arrpGeometryTransforms.Add( pTransforms ) )
								return pTransforms;

							delete pTransforms;
						}

						return NULL;
					}
					CScaleAtTransforms* CreateScaleAtTransforms()
					{
						CScaleAtTransforms* pTransforms = new CScaleAtTransforms();
						if( pTransforms )
						{
							if( m_arrpGeometryTransforms.Add( pTransforms ) )
								return pTransforms;

							delete pTransforms;
						}

						return NULL;
					}
					CShearTransforms* CreateShearTransforms()
					{
						CShearTransforms* pTransforms = new CShearTransforms();
						if( pTransforms )
						{
							if( m_arrpGeometryTransforms.Add( pTransforms ) )
								return pTransforms;

							delete pTransforms;
						}

						return NULL;
					}
					CTranslateTransforms* CreateTranslateTransforms()
					{
						CTranslateTransforms* pTransforms = new CTranslateTransforms();
						if( pTransforms )
						{
							if( m_arrpGeometryTransforms.Add( pTransforms ) )
								return pTransforms;

							delete pTransforms;
						}

						return NULL;
					}
					CRotateTransforms* CreateRotateTransforms()
					{
						CRotateTransforms* pTransforms = new CRotateTransforms();
						if( pTransforms )
						{
							if( m_arrpGeometryTransforms.Add( pTransforms ) )
								return pTransforms;

							delete pTransforms;
						}

						return NULL;
					}
					CRotateAtTransforms* CreateRotateAtTransforms()
					{
						CRotateAtTransforms* pTransforms = new CRotateAtTransforms();
						if( pTransforms )
						{
							if( m_arrpGeometryTransforms.Add( pTransforms ) )
								return pTransforms;

							delete pTransforms;
						}

						return NULL;
					}
					CTransparencyTransforms* CreateTransparencyTransforms()
					{
						CTransparencyTransforms* pTransforms = new CTransparencyTransforms();
						if( pTransforms )
						{
							if( m_arrpTransparencyTransforms.Add( pTransforms ) )
								return pTransforms;

							delete pTransforms;
						}

						return NULL;
					}
					
					void AddScaleTransforms( CScaleTransforms::TData* arrTransforms, int nCountTransforms )
					{
						CScaleTransforms* pTransforms = CreateScaleTransforms();
						if( pTransforms )
						{
							pTransforms->AddItems( arrTransforms, nCountTransforms );
						}
					}
					void AddScaleAtTransforms( CScaleAtTransforms::TData* arrTransforms, int nCountTransforms )
					{
						CScaleAtTransforms* pTransforms = CreateScaleAtTransforms();
						if( pTransforms )
						{
							pTransforms->AddItems( arrTransforms, nCountTransforms );
						}
					}
					void AddShearTransforms( CShearTransforms::TData* arrTransforms, int nCountTransforms )
					{
						CShearTransforms* pTransforms = CreateShearTransforms();
						if( pTransforms )
						{
							pTransforms->AddItems( arrTransforms, nCountTransforms );
						}
					}
					void AddTranslateTransforms( CTranslateTransforms::TData* arrTransforms, int nCountTransforms )
					{
						CTranslateTransforms* pTransforms = CreateTranslateTransforms();
						if( pTransforms )
						{
							pTransforms->AddItems( arrTransforms, nCountTransforms );
						}
					}
					void AddRotateTransforms( CRotateTransforms::TData* arrTransforms, int nCountTransforms )
					{
						CRotateTransforms* pTransforms = CreateRotateTransforms();
						if( pTransforms )
						{
							pTransforms->AddItems( arrTransforms, nCountTransforms );
						}
					}
					void AddRotateAtTransforms( CRotateAtTransforms::TData* arrTransforms, int nCountTransforms )
					{
						CRotateAtTransforms* pTransforms = CreateRotateAtTransforms();
						if( pTransforms )
						{
							pTransforms->AddItems( arrTransforms, nCountTransforms );
						}
					}
					void AddTransparencyTransforms( CTransparencyTransforms::TData* arrTransforms, int nCountTransforms )
					{
						CTransparencyTransforms* pTransforms = CreateTransparencyTransforms();
						if( pTransforms )
						{
							pTransforms->AddItems( arrTransforms, nCountTransforms );
						}
					}
					
					void AddShear( double x1, double y1, double x2, double y2, BOOL interpolation = TRUE )
					{
						Text::Transforms::CShearTransforms::TData arrShears[2];
						arrShears[0].data.transform.SetParams( x1, y1 );
						arrShears[1].data.transform.SetParams( x2, y2 );
						arrShears[1].data.interpolation = interpolation;
						
						AddShearTransforms( arrShears, 2 );
					}
					void AddScale( double dStartScale, double dFinishScale, BOOL interpolation = TRUE )
					{
						Text::Transforms::CScaleTransforms::TData arrScales[2];
						arrScales[0].data.transform.SetParams( dStartScale, dStartScale );
						arrScales[1].data.transform.SetParams( dFinishScale, dFinishScale );
						arrScales[1].data.interpolation = interpolation;
						
						AddScaleTransforms( arrScales, 2 );
					}
					void AddScale( double dStartScaleX, double dStartScaleY, double dFinishScaleX, double dFinishScaleY, BOOL interpolation = TRUE )
					{
						Text::Transforms::CScaleTransforms::TData arrScales[2];
						arrScales[0].data.transform.SetParams( dStartScaleX, dStartScaleY );
						arrScales[1].data.transform.SetParams( dFinishScaleX, dFinishScaleY );
						arrScales[1].data.interpolation = interpolation;
						
						AddScaleTransforms( arrScales, 2 );
					}
					void AddRotate( double dStartAngle, double dFinishAngle, BOOL interpolation = TRUE )
					{
						Text::Transforms::CRotateTransforms::TData arrAngles[2];
						arrAngles[0].data.transform.SetParams( dStartAngle );
						arrAngles[1].data.transform.SetParams( dFinishAngle );
						arrAngles[1].data.interpolation = interpolation;
						
						AddRotateTransforms( arrAngles, 2 );
					}					
					void AddRotateAt( double dAngle1, double dAngle2, double dX1, double dY1, double dX2, double dY2, BOOL interpolation = TRUE )
					{
						Text::Transforms::CRotateAtTransforms::TData arrAngles[2];
						arrAngles[0].data.transform.SetParams( dAngle1, dX1, dY1 );
						arrAngles[1].data.transform.SetParams( dAngle2, dX2, dY2 );
						arrAngles[1].data.interpolation = interpolation;
						
						AddRotateAtTransforms( arrAngles, 2 );
					}
					void AddRotateAt( double dAngle1, double dAngle2, double dX, double dY, BOOL interpolation = TRUE )
					{
						Text::Transforms::CRotateAtTransforms::TData arrAngles[2];
						arrAngles[0].data.transform.SetParams( dAngle1, dX, dY );
						arrAngles[1].data.transform.SetParams( dAngle2, dX, dY );
						arrAngles[1].data.interpolation = interpolation;
						
						AddRotateAtTransforms( arrAngles, 2 );
					}
					void AddTranslate( double x1, double y1, double x2, double y2, BOOL interpolation = TRUE )
					{
						Text::Transforms::CTranslateTransforms::TData arrMoves[2];
						arrMoves[0].data.transform.SetParams( x1, y1 );
						arrMoves[1].data.transform.SetParams( x2, y2 );
						arrMoves[1].data.interpolation = interpolation;
						
						AddTranslateTransforms( arrMoves, 2 );
					}
					void AddTransparency( double dStartFactor, double dEndFactor, BOOL interpolation = TRUE )
					{
						Text::Transforms::CTransparencyTransforms::TData arrFactors[2];
						arrFactors[0].data.transform.SetParams( dStartFactor );
						arrFactors[1].data.transform.SetParams( dEndFactor );
						arrFactors[1].data.interpolation = interpolation;
						
						AddTransparencyTransforms( arrFactors, 2 );
					}
					void AddFadeTransparency( double dStartFactor, double dEndFactor, double nStartDuration = 1, double nEndDuration = 1 )
					{
						Text::Transforms::CTransparencyTransforms::TData arrFactors[3];
						arrFactors[0].data.transform.SetParams( dStartFactor );
						arrFactors[1].data.transform.SetParams( dEndFactor );
						arrFactors[2].data.transform.SetParams( dEndFactor );
						arrFactors[2].data.interpolation = FALSE;
						arrFactors[2].duration = nEndDuration;
						arrFactors[1].duration = nStartDuration;
						
						AddTransparencyTransforms( arrFactors, 3 );
					}
				
				protected:
					void ReleaseDynamicData()
					{
						// последовательно удаляем геометрические трансформации
						CGeometryTransforms** pGeometryData = m_arrpGeometryTransforms.GetData();
						if( pGeometryData )
						{
							int nDataSize = m_arrpGeometryTransforms.GetSize();
							for( ; nDataSize > 0; --nDataSize, ++pGeometryData )
							{
								CGeometryTransforms* pItem = *pGeometryData;
								if( pItem )
								{
									delete pItem;
									pItem = NULL;
								}
							}
							m_arrpGeometryTransforms.RemoveAll();
						}
						
						// последовательно удаляем цветовые трансформации
						CTransparencyTransforms** pTransparencyData = m_arrpTransparencyTransforms.GetData();
						if( pTransparencyData )
						{
							int nDataSize = m_arrpTransparencyTransforms.GetSize();
							for( ; nDataSize > 0; --nDataSize, ++pTransparencyData )
							{
								CTransparencyTransforms* pItem = *pTransparencyData;
								if( pItem )
								{
									delete pItem;
									pItem = NULL;
								}
							}
							m_arrpTransparencyTransforms.RemoveAll();
						}
					}
				};
			}			
			namespace Utils
			{
				enum
				{
					c_nLeftBound =   0,
					c_nTopBound =    1,
					c_nRightBound =  2,
					c_nBottomBound = 3,

					c_nEmptyBound = -1
				};

				float GetOffsetRect( const Gdiplus::RectF& oLayoutRect, const Gdiplus::RectF& oInnerRect, int nBound )
				{
					// ============================================================================
					// вычисляет смещение внутреннего прямоугольника относительно внешнего так, 
					// что бы они соприкасались
					// 
					// nBound: 0 - left, 1 - top, 2 - right, 3 - bottom
					// ============================================================================
					
					float dMove = 0;

					switch( nBound )
					{
					case c_nLeftBound:   dMove = oLayoutRect.X - oInnerRect.X - oInnerRect.Width; break;
					case c_nTopBound:    dMove = oLayoutRect.Y - oInnerRect.Y - oInnerRect.Height; break;
					case c_nRightBound:  dMove = oLayoutRect.X - oInnerRect.X + oLayoutRect.Width; break;
					case c_nBottomBound: dMove = oLayoutRect.Y - oInnerRect.Y + oLayoutRect.Height; break;
					}

					return dMove;
				}
				float GetHorOffsetCenters( const Gdiplus::RectF& oRect1, const Gdiplus::RectF& oRect2 )
				{
					return oRect2.X - oRect1.X + (oRect2.Width - oRect1.Width)/2;
				}
				float GetVerOffsetCenters( const Gdiplus::RectF& oRect1, const Gdiplus::RectF& oRect2 )
				{
					return oRect2.Y - oRect1.Y + (oRect2.Height - oRect1.Height)/2;
				}
				float GetHorCenter( const Gdiplus::RectF& oRect )
				{
					return oRect.X + oRect.Width * 0.5f;
				}
				float GetVerCenter( const Gdiplus::RectF& oRect )
				{
					return oRect.Y + oRect.Height * 0.5f;
				}
				Gdiplus::PointF GetCenter( const Gdiplus::RectF& oRect )
				{
					return Gdiplus::PointF( oRect.X + oRect.Width * 0.5f, oRect.Y + oRect.Height * 0.5f );
				}
				
				void ClearRect( Gdiplus::RectF& oRect )
				{
					oRect.X = oRect.Y = oRect.Width = oRect.Height = 0;
				}
				void ClearPoint( Gdiplus::PointF& oPoint )
				{
					oPoint.X = oPoint.Y = 0;
				}
				void ClearSize( Gdiplus::SizeF& oSize )
				{
					oSize.Width = oSize.Height = 0;
				}

				
				template<typename T>
				class ArrayReflection
				{
				public:
					T*  ptr;
					int count;

					ArrayReflection() 
						: ptr( 0 ), count( 0 )
					{
					}
					ArrayReflection( T* pData, int nCount )
						: ptr( pData ), count( nCount )
					{
					}
					ArrayReflection( ATL::CSimpleArray< T >& oArray )
						: ptr( oArray.GetData() ), count( oArray.GetSize() )
					{
					}
					ArrayReflection( ATL::CAtlArray< T >& oArray )
						: ptr( oArray.GetData() ), count((int)oArray.GetCount() )
					{
					}
					ArrayReflection( ATL::CSimpleArray< T >& oArray, int offset, int size )
						: ptr( oArray.GetData() + offset ), count( size )
					{
					}
					ArrayReflection( ATL::CAtlArray< T >& oArray, int offset, int size )
						: ptr( oArray.GetData() + offset ), count( size )
					{
					}
					
					T& operator [](int index)
					{
						return ptr[index];
					}
					const T& operator [](int index) const
					{
						return ptr[index];
					}
					
				};
				
				class CPermutator
				{
					int* m_pValues;
					int  m_nCount;
				
				public:
					
					CPermutator()
					{
						m_pValues = NULL;
						m_nCount  = 0;
					}
					CPermutator( int nCount, BOOL bPermutate )
					{
						m_pValues = NULL;
						m_nCount  = 0;

						Create( nCount, bPermutate );
					}
					virtual ~CPermutator()
					{
						Destroy();
					}
					
					void Destroy()
					{
						if (NULL != m_pValues)
						{
							delete[] m_pValues;
							m_pValues = NULL;
						}

						m_nCount = 0;
					}
					void Create(int nCount, BOOL bPermutate)
					{
						Destroy();

						if (nCount < 1)
							return;

						m_nCount = nCount;

						m_pValues = new int[nCount];

						for (int index = 0; index < nCount; ++index)
							m_pValues[index] = index;

						if (!bPermutate || nCount < 2)
							return;

						Refresh();
					}
					void Refresh()
					{
						for (int index = 0; index < m_nCount; ++index)
						{
							int nIndexFrom = index;
							int nIndexTo   = ::rand() % m_nCount;

							int nValue1 = m_pValues[nIndexFrom];
							int nValue2 = m_pValues[nIndexTo];

							m_pValues[nIndexFrom] = nValue2;
							m_pValues[nIndexTo]   = nValue1;
						}						
					}
					
					int GetCount()
					{
						return m_nCount;
					}
					int operator[](int nIndex)
					{
						//if (nIndex < 0 || nIndex >= m_nCount)
						//	return -1;

						return m_pValues[nIndex];
					}
				};
			}
			
			class CEntity
			{
			protected:
				Gdiplus::Matrix m_oTransform; // текущая трансформация
				double m_dTransparency;       // текущaя прозрачность

				Transforms::CTransformManager m_oPrevTransforms; // менеджер предтрансформаций
				Transforms::CTransformManager m_oPostTransforms; // менеджер посттрансформаций

			public:
				
				Gdiplus::PointF m_oOrigin;     // координаты относительно родителя
				Gdiplus::RectF  m_oPathBoundRect; // ограничивающий прямоугольник патча без трансформаций
				Gdiplus::PointF m_oPathCenter; // координаты центра патча (не обязательно геометрический центр!)

			public:

				CEntity() : m_dTransparency( 1 )
				{
				}
				CEntity( const CEntity& other )
					: m_dTransparency( other.m_dTransparency ),
					m_oPrevTransforms( other.m_oPrevTransforms ),
					m_oPostTransforms( other.m_oPostTransforms ),
					m_oOrigin        ( other.m_oOrigin ),
					m_oPathBoundRect ( other.m_oPathBoundRect ),
					m_oPathCenter    ( other.m_oPathCenter )
				{
					float items[6];
					other.m_oTransform.GetElements( items );
					m_oTransform.SetElements( items[0], items[1], items[2], items[3], items[4], items[5] );
				}
				//virtual const CEntity& operator = (const CEntity& other)
				//{
				//	return *this
				//}
				virtual ~CEntity()
				{
				}
				
				virtual void Clear()
				{
					m_oTransform.Reset();
					m_dTransparency = 1;

					m_oPrevTransforms.Clear();
					m_oPostTransforms.Clear();

					Utils::ClearPoint( m_oOrigin );
					Utils::ClearRect ( m_oPathBoundRect );
					Utils::ClearPoint( m_oPathCenter );
				}
				virtual void Update( double dTime, const Gdiplus::Matrix& oParentTransform, double dParentTransparency )
				{
					// очищаем текущие значения
					m_oTransform.Reset();
					m_dTransparency = 1;

					// выполняем последовательные трансформации
					if( dTime < 2 )
						m_oPrevTransforms.Update( dTime, m_oTransform, m_dTransparency );
					else
						m_oPostTransforms.Update( 3 - dTime, m_oTransform, m_dTransparency );

					// переносим позицию относительно внешней системы координат
					m_oTransform.Translate( m_oOrigin.X, m_oOrigin.Y, Gdiplus::MatrixOrderAppend );

					// объединяем с внешними данными
					m_oTransform.Multiply( &oParentTransform, Gdiplus::MatrixOrderAppend );
					m_dTransparency *= dParentTransparency;
				}
				
				const Gdiplus::Matrix& GetTransform() const
				{
					return m_oTransform;
				}

				const double GetAlphaFactor() const
				{
					return m_dTransparency;
				}

				Transforms::CTransformManager& GetTransformManager( BOOL bPhaseIn )
				{
					return bPhaseIn ? m_oPrevTransforms : m_oPostTransforms;
				}

				void RemoveTransforms()
				{
					m_oPrevTransforms.Clear();
					m_oPostTransforms.Clear();
				}
			};

			class CLetterEntity : public CEntity
			{
			protected:
				Gdiplus::RectF m_oLayoutRect; // ограничивающий прямоугольник для патча после применения трансформов
				CLetterEntity* m_pNextLetter; // указатель на следующую букву в списке

			public:
				Gdiplus::GraphicsPath* m_pPath; // указатель на внешний патч. не требует применения new/delete
				
				BOOL m_bNotLetter; // признак линии перечёркивания(подчёркивания) для стилей Strikeout и Underline

			public:
				CLetterEntity()
				{
					m_pNextLetter = NULL;
					m_pPath       = NULL;
					m_bNotLetter  = FALSE;
				}
				virtual ~CLetterEntity()
				{
					ReleaseChildLetter();
				}
				
				virtual void Clear()
				{
					ReleaseChildLetter();
					
					CEntity::Clear();

					m_pPath = NULL;
					
					Utils::ClearRect( m_oLayoutRect );

					m_bNotLetter = FALSE;
				}
				virtual void Update( double dTime, const Gdiplus::Matrix& oParentTransform, double dParentTransparency )
				{
					CEntity::Update( dTime, oParentTransform, dParentTransparency );

					// корректируем относительно траектории
					m_oTransform.Translate( -m_oPathCenter.X, -m_oPathCenter.Y, Gdiplus::MatrixOrderPrepend );
					
					// расчитываем ограничивающий прямоугольник
					if( m_pPath )
					{
						m_pPath->GetBounds( &m_oLayoutRect, &m_oTransform );
					}
				}	
				
				void Draw( Gdiplus::Graphics& oRender, const Gdiplus::RectF& oClipRect, Painter::CBrush& oBrush, Painter::CTextureManager* pTextureManager, Gdiplus::RectF* pBoundClip )
				{
					if( !m_pPath || m_dTransparency < IPP_EPS23 )
						return;

					Gdiplus::RectF oRenderRect = m_oLayoutRect;
					oRenderRect.Inflate( 1, 1 );
					
					if( FALSE == oRenderRect.Intersect( oClipRect ) )
						return;

					//oRender.SetTransform( &m_oTransform );

					oBrush.SetTransparency( m_dTransparency );

					Gdiplus::Brush* pBrush = oBrush.GetBrush( pTextureManager, pBoundClip );
					
					if( pBrush )
					{
						oRender.FillPath( pBrush, m_pPath );
					}
				}

				void DrawEdge( Gdiplus::Graphics& oRender, const Gdiplus::RectF& oClipRect, Painter::CEdgeText& oEdge )
				{
					if( !m_pPath || m_dTransparency < IPP_EPS23 )
						return;

					float fPenWidth = (float)oEdge.GetDistance();
					Gdiplus::RectF oRenderRect = m_oLayoutRect;
					oRenderRect.Inflate( 1 + fPenWidth, 1 + fPenWidth );
					
					if( FALSE == oRenderRect.Intersect( oClipRect ) )
						return;

					oRender.SetTransform( &m_oTransform );

					oEdge.SetTransparency( m_dTransparency );

					Gdiplus::Pen* pPen = oEdge.GetPen();
					if( pPen )
					{
						oRender.DrawPath( pPen, m_pPath );
					}
				}

				void DrawShadow( Gdiplus::Graphics& oRender, const Gdiplus::RectF& oClipRect, const Gdiplus::Color oColor, float dx, float dy, int nBlurSize )
				{
					if( !m_pPath || m_dTransparency < IPP_EPS23 )
						return;

					int nMargin = 5;

					Gdiplus::RectF oRenderRect = m_oLayoutRect;
					oRenderRect.Inflate( float(nMargin + nBlurSize), float(nMargin + nBlurSize) );
					oRenderRect.Offset( dx, dy );
					
					if( FALSE == oRenderRect.Intersect( oClipRect ) )
						return;

					int w = int(oRenderRect.Width  + 0.5f);
					int h = int(oRenderRect.Height + 0.5f);
					if( w < 1 || h < 1 )
						return;

					int alpha = int(oColor.GetAlpha() * m_dTransparency + 0.5f);
					if( alpha < 0 )   alpha = 0;
					if( alpha > 255 ) alpha = 255;

					Gdiplus::Color      oShadowColor(BYTE(alpha), oColor.GetR(), oColor.GetG(), oColor.GetB());
					Gdiplus::SolidBrush oShadowBrush( nBlurSize > 0 ? Gdiplus::Color::Blue : oShadowColor );
					Gdiplus::Bitmap     oTempBitmap( w, h );

					if( true )
					{
						Gdiplus::Graphics oLocalRender(&oTempBitmap);

						oLocalRender.SetTransform( &m_oTransform );
						oLocalRender.TranslateTransform( dx-oRenderRect.X, dy-oRenderRect.Y, MatrixOrderAppend );
						
						oLocalRender.SetInterpolationMode(InterpolationModeBilinear);
						oLocalRender.SetSmoothingMode(SmoothingModeAntiAlias);
						oLocalRender.Clear( Gdiplus::Color(0, 0, 0, 0) );
						
						oLocalRender.FillPath( &oShadowBrush, m_pPath );
					}

					GDIPLUS_ShadowBlur(oTempBitmap, nBlurSize, oShadowColor.GetValue() );

					oRender.DrawImage( &oTempBitmap, oRenderRect.X, oRenderRect.Y );
				}


				
				CLetterEntity* AddChildLetter()
				{
					CLetterEntity* pLetter = new CLetterEntity( *this );
					if( pLetter )
					{
						m_pNextLetter = pLetter;
					}
					return pLetter;
				}
				void ReleaseChildLetter()
				{
					if( m_pNextLetter )
					{
						m_pNextLetter->ReleaseChildLetter();
						delete [] m_pNextLetter;
						m_pNextLetter = NULL;
					}
				}


			protected:
				static void GDIPLUS_ShadowBlur(Gdiplus::Bitmap& oBitmap, int nBlurSize, DWORD dwShadowColor)
				{
					if( nBlurSize < 1 )
					{
						return;
					}

					Gdiplus::BitmapData oBitmapData;
					
					if( Gdiplus::Ok == oBitmap.LockBits(NULL, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &oBitmapData) )
					{
						BGRA_ShadowBlur((BYTE*)oBitmapData.Scan0, oBitmapData.Width, oBitmapData.Height, oBitmapData.Stride, nBlurSize, dwShadowColor);

						oBitmap.UnlockBits(&oBitmapData);
					}
				}
				static void BGRA_ShadowBlur( BYTE* pBGRA, int nWidth, int nHeight, int nStride, int nBlurSize, DWORD dwShadowColor )
				{
					if( !pBGRA || nWidth < 1 || nHeight < 1 || nStride < nWidth*4 || nBlurSize < 1 )
					{
						return;
					}

					int nBufStride = nWidth * 4;
					BYTE* pBuffer  = new BYTE[nBufStride * nHeight];
					if( !pBuffer )
					{
						return;
					}

					int edge = min(nWidth, nBlurSize);

					FillZeroLeftRightEdge( pBuffer, edge, nHeight, nBufStride, nWidth - edge );
					BGRA_ShadowBlurVertical(pBGRA, pBuffer, nWidth, nHeight, nStride, nBufStride, nBlurSize);
					BGRA_ShadowBlurHorizontal(dwShadowColor, pBuffer, pBGRA, nWidth, nHeight, nBufStride, nStride, nBlurSize);

					delete[] pBuffer;
				}
				static BOOL BGRA_ShadowBlurHorizontal( DWORD dwColor, BYTE* pSrcData, BYTE* pDstData, int nWidth, int nHeight, int nSrcStride, int nDstStride, int nBlurSize )
				{
					if( !pSrcData || !pDstData || nWidth < 1 || nHeight < 1 || nSrcStride < 1 || nDstStride < 1 || nBlurSize < 1 )
					{
						return FALSE;
					}

					int nLength;
					int nA, nR, nG, nB;
					int nClr;
					DWORD nFactor;
					DWORD nClrFactor;
					DWORD *pSrc, *pDst;

					nA = (dwColor >> 24 & 0xff);
					nR = (dwColor >> 16 & 0xff);
					nG = (dwColor >> 8  & 0xff);
					nB = (dwColor >> 0  & 0xff);
					nFactor = (nBlurSize << 1) + 1;
					nFactor *= nFactor;

					for( ; nHeight > 0; --nHeight, pSrcData += nSrcStride, pDstData += nDstStride )
					{
						pSrc = (DWORD*)pSrcData;
						pDst = (DWORD*)pDstData;
						nClr = 0;

						// подсчёт начальной суммы
						nLength = nBlurSize < nWidth ? nBlurSize : nWidth - 1;
						for( ; nLength >= 0; --nLength )
						{
							nClr += pSrc[nLength];
						}

						// расчёт новых пикселов в строке
						nLength = nWidth;
						for( ;; )
						{
							nClrFactor = nClr / nFactor + 1;
							dwColor  = (nB * nClrFactor >> 8) << 0;
							dwColor |= (nG * nClrFactor >> 8) << 8;
							dwColor |= (nR * nClrFactor >> 8) << 16;
							dwColor |= (nA * nClrFactor >> 8) << 24;

							*pDst = dwColor;

							// проверка на выход из цикла
							if( nLength <= 1 )
							{
								break;
							}

							// вычитаем крайнее левое значение
							if( nLength <= nWidth - nBlurSize )
							{
								nClr -= pSrc[-nBlurSize];
							}

							// переходим на следующий пиксел
							--nLength;
							++pSrc; 
							++pDst;
							
							// прибавляем следующее крайнее правое значение
							if( nLength > nBlurSize )
							{
								nClr += pSrc[nBlurSize];
							}
						}
					}

					return TRUE;
				}
				static BOOL BGRA_ShadowBlurVertical(BYTE* pSrcData, BYTE* pDstData, int nWidth, int nHeight, int nSrcStride, int nDstStride, int nBlurSize )
				{
					if( !pSrcData || !pDstData || nWidth < 1 || nHeight < 1 || nSrcStride < 1 || nDstStride < 1 || nBlurSize < 1 )
					{
						return FALSE;
					}

					int nLength;
					DWORD nClr;
					BYTE *pSrc, *pDst;
					
					pSrcData += nBlurSize*4;
					pDstData += nBlurSize*4;
					for( nWidth -= nBlurSize*2; nWidth > 0; --nWidth, pSrcData += 4, pDstData += 4 )
					{
						pSrc = pSrcData;
						pDst = pDstData;
						nClr = 0;

						// подсчёт начальной суммы
						nLength = nBlurSize < nHeight ? nBlurSize : nHeight - 1;
						for( nLength *= nSrcStride; nLength >= 0; nLength -= nSrcStride )
						{
							nClr += *(DWORD*)(pSrc + nLength)&0xff;
						}

						// расчёт новых пикселов в строке
						nLength = nHeight;
						for( ;; )
						{
							*(DWORD*)(pDst) = nClr;

							// проверка на выход из цикла
							if( nLength <= 1 )
							{
								break;
							}

							// вычитаем крайнее верхнее значение
							if( nLength <= nHeight - nBlurSize )
							{
								nClr -= *(DWORD*)(pSrc - nBlurSize*nSrcStride)&0xff;
							}

							// переходим на следующий пиксел
							--nLength;
							pSrc += nSrcStride;
							pDst += nDstStride;
							
							// прибавляем следующее крайнее нижнее значение
							if( nLength > nBlurSize )
							{
								nClr += *(DWORD*)(pSrc + nBlurSize*nSrcStride)&0xff;
							}
						}
					}

					return TRUE;
				}
				static void FillZeroLeftRightEdge( BYTE* pARGBData, int nEdgeWidth, int nEdgeHeight, int nStride, int nOffset )
				{
					DWORD* pDst;
					int nWidth;

					for( ; nEdgeHeight > 0; --nEdgeHeight, pARGBData += nStride )
					{
						pDst = (DWORD*)pARGBData;
						for( nWidth = nEdgeWidth; nWidth > 0; --nWidth, ++pDst )
						{
							pDst[0] = 0;
							pDst[nOffset] = 0;
						}
					}
				}
			};
			typedef Utils::ArrayReflection<CLetterEntity> CRefLetters;

			class CTextEntity : public CEntity
			{
			public:
				class CTextLine
				{
				public:	
					int m_nFirstLetter; // номер символа в тексте с которого начинается линия
					int m_nLength;      // количество символов в линии
					
					Gdiplus::RectF m_oPathBoundRect;

					CTextLine()
					{
						m_nFirstLetter   = 0;
						m_nLength        = 0;
					}
				};
			
			public:
				CAtlArray<CLetterEntity>          m_arrLetters; // список букв
				CAtlArray<CTextEntity::CTextLine> m_arrLines;

			public:

				CTextEntity()
				{
				}

				virtual void Clear()
				{
					CEntity::Clear();

					m_arrLetters.RemoveAll();
					m_arrLines.RemoveAll();
				}

				virtual void Update( double dTime, const Gdiplus::Matrix& oParentTransform, const float fParentAlphaFactor )
				{
					CEntity::Update( dTime, oParentTransform, fParentAlphaFactor );

					// обходим последовательно все буквы
					CRefLetters letters( m_arrLetters );
					for( ; letters.count > 0; --letters.count, ++letters.ptr )
					{
						letters.ptr->Update( dTime, m_oTransform, m_dTransparency );
					}
				}
			};
			typedef Utils::ArrayReflection<CTextEntity::CTextLine> CRefLines;

			class CTextPainter
			{
			public:
				CTextPainter()
				{
					m_pText = NULL;
					
					m_fLineSpacing = 0;
					m_bUnderLine = FALSE;
					
					pImage = NULL;

					m_nFillMode = 0;
					m_fAngle = 0;
				}
				~CTextPainter()
				{
					ReleaseDynamicData();
				}
				
				inline void Clear()
				{
					ReleaseDynamicData();

					Utils::ClearRect( m_oLayoutRect );

					m_fLineSpacing = 0;
					m_bUnderLine = FALSE;

					m_oFillBrush.Clear();
					m_oEdge.Clear();
					m_oShadow.Clear();

					m_nFillMode	=	0;
					m_fAngle	=	0;
					
					m_PointLT	=	Gdiplus::PointF ( 0.0f, 0.0f );
				}
				
				inline BOOL CreateText( BSTR bstrText, const Gdiplus::Font& oFont, const Gdiplus::RectF& oClipLayoutRect, int nHorAlign, int nVerAlign, const Gdiplus::RectF& oImageRect, int nPlaceType = 0 )
				{
					Clear();
				
					if( NULL == bstrText || FALSE == oFont.IsAvailable() || oImageRect.IsEmptyArea() )
						return FALSE;

					// создаём патч всего текста
					Gdiplus::FontFamily      oFontFamily;
					Gdiplus::StringFormat    oStrFormat;
					Gdiplus::StringAlignment eStrAlign;
					Gdiplus::GraphicsPath    oTextPath;

					if( Gdiplus::Ok != oFont.GetFamily( &oFontFamily ) )
						return FALSE;

					Gdiplus::RectF oLayoutRect = oClipLayoutRect;
					//oLayoutRect.X -= 0.5f; // смещение относительно обычного текста по X

					// нормализация прямоугольника вывода
					if( oLayoutRect.Width < 0 )
					{
						oLayoutRect.X     += oLayoutRect.Width;
						oLayoutRect.Width *= -1;
					}
					if( oLayoutRect.Height < 0 )
					{
						oLayoutRect.Y      += oLayoutRect.Height;
						oLayoutRect.Height *= -1;
					}

					// задаём прямоугольник отсечения вывода
					m_oLayoutRect = oLayoutRect;

					switch( nHorAlign )
					{
					case Gdiplus::StringAlignmentCenter: 
						eStrAlign = Gdiplus::StringAlignmentCenter; break;
					case Gdiplus::StringAlignmentFar:    
						eStrAlign = Gdiplus::StringAlignmentFar; break;
					default: 
						eStrAlign = Gdiplus::StringAlignmentNear;
					}
					oStrFormat.SetAlignment( eStrAlign );

					switch( nVerAlign )
					{
					case Gdiplus::StringAlignmentCenter: 
						oLayoutRect.Y += oLayoutRect.Height / 2;
						break;
					case Gdiplus::StringAlignmentFar:    
						oLayoutRect.Y += oLayoutRect.Height;
						break;
					}
					oStrFormat.SetLineAlignment( Gdiplus::StringAlignmentNear );

					float fViewHeight = oLayoutRect.Height;
					oLayoutRect.Height = 0;

					int   nFontStyle   = oFont.GetStyle();
					float fFontSize    = oFont.GetSize();

					// получаем общий контур текста
					if( Gdiplus::Ok != oTextPath.AddString( bstrText, -1, &oFontFamily, nFontStyle, fFontSize, oLayoutRect, &oStrFormat ))
						return FALSE;

					// задаём межстрочный интервал
					float fFontAscent  = oFontFamily.GetCellAscent( nFontStyle );
					float fFontDescent = oFontFamily.GetCellDescent( nFontStyle );
					float fFontHeight  = oFontFamily.GetEmHeight( nFontStyle );
					float fFontSpacing = oFontFamily.GetLineSpacing( nFontStyle );

					m_fLineSpacing = fFontSpacing * fFontSize / fFontHeight;
					m_bUnderLine   = ((nFontStyle & Gdiplus::FontStyleStrikeout)|(nFontStyle & Gdiplus::FontStyleUnderline)) ? TRUE : FALSE;

					BOOL bRetValue = CreateLetters ( oTextPath );
					if( bRetValue )
					{
						float fHeight = (fFontAscent + fFontDescent) * fFontSize / fFontHeight;
						float fFactor = (fFontSpacing - (fFontAscent + fFontDescent)) * fFontSize / fFontHeight;
						
						CorrectionLettersPositions( nVerAlign, fHeight, m_fLineSpacing, fFactor, fViewHeight );
					}

					//m_oLayoutRect = oImageRect;
					m_oClipRect = oImageRect;

					return bRetValue;
				}
				
				inline void Draw ( double dTime, Gdiplus::Graphics& oRender )
				{
					if( m_pText )
					{
						Gdiplus::GraphicsState oRenderState;
						Gdiplus::RectF oClipRect;

						oRenderState = oRender.Save();

						oRender.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
						oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
						oRender.SetInterpolationMode(Gdiplus::InterpolationModeDefault );//InterpolationModeBilinear);
						
						oRender.SetClip( m_oClipRect, Gdiplus::CombineModeIntersect );
						oRender.GetClipBounds( &oClipRect );
						oRender.ResetTransform();

						oRender.SetTransform ( &m_BitmapTransform );

						Gdiplus::Bitmap oBitmap ( (int)m_oLayoutRect.Width + 5, (int)m_oLayoutRect.Height + 5, PixelFormat32bppARGB );
						
						// ATLTRACE ( _T("BitmapSize : %f, %f\t"), (float)oBitmap.GetWidth (), (float)oBitmap.GetHeight () );

						Gdiplus::Graphics GraphicsText ( &oBitmap );
						
						GraphicsText.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
						GraphicsText.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
						GraphicsText.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
#if _DEBUG
						GraphicsText.Clear ( 0x1F0000FF );
#endif
						
						GraphicsText.SetTransform ( &m_TextTransform );
											  
						CRefLetters letters( m_pText->m_arrLetters );

						//if( m_oShadow.Visible )
						//{
						//	int alpha = min(255, max(0,  m_oShadow.GetAlpha()));
						//	int color = m_oShadow.GetColor();
						//	int nBlur = (int)m_oShadow.GetBlurSize();
						//	float dx  = (float)m_oShadow.GetDistanceX();
						//	float dy  = (float)m_oShadow.GetDistanceY();

						//	Gdiplus::Color oColor( alpha, GetRValue(color), GetGValue(color), GetBValue(color) );

						//	for( int i = 0; i < letters.count; i++ )
						//	{
						//		letters.ptr[i].DrawShadow( oRender, oClipRect, oColor, dx, dy, nBlur );
						//	}
						//}

						//if( m_oEdge.Visible )
						//{
						//	for( int i = 0; i < letters.count; i++ )
						//	{
						//		if( letters.ptr[i].m_bNotLetter )
						//			continue;

						//		letters.ptr[i].DrawEdge( oRender, oClipRect, m_oEdge );
						//	}
						//}

						if( TRUE )
						{
							Gdiplus::RectF oBrushRect( m_pText->m_oPathBoundRect );

							for( int i = 0; i < letters.count; i++ )
							{
								if( letters.ptr[i].m_bNotLetter && m_oEdge.Visible )
									letters.ptr[i].DrawEdge( oRender, oClipRect, m_oEdge );
								
								if( m_nFillMode != Constants::c_nTextFillMode_Normal )
								{
									switch( m_nFillMode )
									{
									case Constants::c_nTextFillMode_Letter:
										oBrushRect = letters.ptr[i].m_oPathBoundRect;
										break;
									case Constants::c_nTextFillMode_HorCylinder:
										break;
									case Constants::c_nTextFillMode_VerCylinder:
										break;
									}
								}

								letters.ptr[i].Draw( GraphicsText, oClipRect, m_oFillBrush, &m_oTextureManager, &oBrushRect );
							}
						}

						oRender.DrawImage ( &oBitmap, (int)m_oLayoutRect.X, (int)m_oLayoutRect.Y, (int)oBitmap.GetWidth (), (int)oBitmap.GetHeight () );
						
						// ATLTRACE ( _T("DrawImage : %f, %f, %f, %f\n"), (float)m_oLayoutRect.X, (float)m_oLayoutRect.Y, (float)oBitmap.GetWidth (), (float)oBitmap.GetHeight () );
						
#if _DEBUG
						oRender.DrawRectangle ( new Gdiplus::Pen ( Gdiplus::Color(192,255,255,0), 4 ), m_oLayoutRect );
#endif
						oRender.Restore( oRenderState );
					}
				}
				
				inline void SetFillBrush( const Painter::CBrush& oBrush )
				{
					m_oFillBrush = oBrush;
				}
				inline void SetShadow( const Painter::CShadow& oShadow )
				{
					m_oShadow = oShadow;
				}
				inline void SetEdge( const Painter::CEdgeText& oEdge )
				{
					m_oEdge = oEdge;
				}
				inline void UpdateRotate ( float Angle )
				{
					m_fAngle		=	Angle;
						
					m_TextTransform.Reset ();
					m_TextTransform.Translate ( -m_oLayoutRect.X, -m_oLayoutRect.Y );

					Gdiplus::Matrix	Transforms;

					m_BitmapTransform.Reset ();
					
					Gdiplus::Matrix	mTransform;

					mTransform.Translate ( m_oLayoutRect.X, m_oLayoutRect.Y );
					m_BitmapTransform.Multiply ( &mTransform );

					mTransform.Reset ();
					mTransform.Rotate ( m_fAngle );
					m_BitmapTransform.Multiply ( &mTransform );

					mTransform.Reset ();
					mTransform.Translate ( -m_oLayoutRect.X , -m_oLayoutRect.Y );

					m_BitmapTransform.Multiply ( &mTransform );
				}

				inline void UpdateTranslate ( float X, float Y )
				{
					m_oLayoutRect.X		+=	X;
					m_oLayoutRect.Y		+=	Y;
				}

				
			protected:
			
				inline void ReleaseDynamicData()
				{
					m_oTextureManager.Clear();

					if( pImage )
					{
						pImage->Release();
						pImage = NULL;
					}

					m_arrPaths.RemoveAll();
					
					if( m_pText )
					{
						delete m_pText;
						m_pText = NULL;
					}

				}
			
				static void GetBoundRectForLetters( const Gdiplus::RectF& oMaxLayoutRect, Gdiplus::RectF& oResultRect, CLetterEntity* arrLetters, int nCountLetters )
				{
					float fLeft   = oMaxLayoutRect.GetRight();
					float fRight  = oMaxLayoutRect.GetLeft();
					float fTop    = oMaxLayoutRect.GetBottom();
					float fBottom = oMaxLayoutRect.GetTop();

					while( nCountLetters-- > 0 )
					{
						Gdiplus::RectF& oRect = arrLetters->m_oPathBoundRect;

						if( oRect.GetLeft()   < fLeft )   fLeft   = oRect.GetLeft();
						if( oRect.GetRight()  > fRight )  fRight  = oRect.GetRight();
						if( oRect.GetTop()    < fTop )    fTop    = oRect.GetTop();
						if( oRect.GetBottom() > fBottom ) fBottom = oRect.GetBottom();

						++arrLetters;
					}

					oResultRect.X      = fLeft;
					oResultRect.Y      = fTop;
					oResultRect.Width  = fRight  - fLeft;
					oResultRect.Height = fBottom - fTop;
				}
				
				inline BOOL CreateLetters( const Gdiplus::GraphicsPath& oTextPath )
				{
					Gdiplus::GraphicsPathIterator oPathIterator( &oTextPath );

					// расчитываем количество символов в тексте, включая линини перечёркивания
					size_t nLetters = 0;
					int nStartIndex, nEndIndex;
					while( oPathIterator.NextMarker( &nStartIndex, &nEndIndex ) > 0 )
					{
						++nLetters;
					}

					if( nLetters < 1 )
						return FALSE;

					// создаём необходимые динамические данные
					if( !m_arrPaths.SetCount( nLetters, 0 ) )
						return FALSE;

					m_pText = new CTextEntity();
					if( !m_pText )
						return FALSE;

					if( !m_pText->m_arrLetters.SetCount( nLetters, 0 ) )
						return FALSE;

					// задаём геометрические параметры для всего текста
					oTextPath.GetBounds( &m_pText->m_oPathBoundRect );

					m_pText->m_oPathCenter = Utils::GetCenter( m_pText->m_oPathBoundRect );
					m_pText->m_oOrigin     = m_pText->m_oPathCenter;
					
					// задаём геметрические параметры для каждого символа в тексте
					oPathIterator.Rewind();

					float fPrevLine = 0.5f;
					CLetterEntity* pPrevLetter = NULL;
					
					Gdiplus::GraphicsPath* pPath   = m_arrPaths.GetData();
					CLetterEntity*         pLetter = m_pText->m_arrLetters.GetData();
					
					float fBaseY = m_pText->m_oPathBoundRect.Y + (m_pText->m_oPathBoundRect.Height - 
						           ceil(m_pText->m_oPathBoundRect.Height / m_fLineSpacing) * m_fLineSpacing) / 2;
					
					int nCountLines = 0;
					for( ; nLetters > 0; --nLetters, ++pLetter, ++pPath )
					{
						oPathIterator.NextMarker( pPath );
						pLetter->m_pPath = pPath;
						pPath->SetFillMode( Gdiplus::FillModeWinding ); // даёт значительное ускорение при отрисовке

						pPath->GetBounds( &pLetter->m_oPathBoundRect );
						
						pLetter->m_oPathCenter = Utils::GetCenter( pLetter->m_oPathBoundRect );

						float dy    = (pLetter->m_oPathCenter.Y - fBaseY) / m_fLineSpacing;
						float fLine = floor(dy);
						
						dy = m_fLineSpacing * (fLine - dy + 0.5f);
						pLetter->m_oPathCenter.Y += dy;
						
						pLetter->m_oOrigin = pLetter->m_oPathCenter - m_pText->m_oOrigin;
						
						pLetter->m_bNotLetter = TRUE;
						++nCountLines;
						if( fPrevLine != fLine )
						{
							fPrevLine = fLine;
						}
						else
						{
							if( pPrevLetter )
							{
								pPrevLetter->m_bNotLetter = FALSE;
								--nCountLines;
							}
						}
						pPrevLetter = pLetter;
					}

					// создаём массив текстовых линий
					if( m_pText->m_arrLines.SetCount( nCountLines, 0 ) )
					{
						CTextEntity::CTextLine* pLine = m_pText->m_arrLines.GetData();
						int nFirstIndex = 0;

						CRefLetters letters( m_pText->m_arrLetters );
						for( int nLetter = 0; nLetter < letters.count; ++nLetter )
						{
							if( letters[nLetter].m_bNotLetter )
							{
								pLine->m_nFirstLetter = nFirstIndex;
								pLine->m_nLength      = nLetter - nFirstIndex + 1;
								GetBoundRectForLetters( m_pText->m_oPathBoundRect, pLine->m_oPathBoundRect, letters.ptr + nFirstIndex, pLine->m_nLength );
							
								nFirstIndex = nLetter + 1;
								++pLine;
							}
						}
					}
					
					// корректируем положение линий перечёркивания
					if( !m_bUnderLine )
					{
						CRefLetters letters( m_pText->m_arrLetters );
						for( ; letters.count > 0; --letters.count, ++letters.ptr )
						{
							letters.ptr->m_bNotLetter = FALSE;
						}
					}

					return TRUE;
				}

				inline void CorrectionLettersPositions( int nVerAlign, float fLineHeight, float fLineSpacing, float fLineFactor, float fViewHeight )
				{
					CRefLines lines( m_pText->m_arrLines );

					float fHeight = (lines.count - 1) * fLineSpacing + fLineHeight;
					float fTop    = +1000000;
					float fBottom = -1000000;
					float fStart  = 0;

					switch( nVerAlign )
					{
					case Gdiplus::StringAlignmentFar:
						fStart = fLineFactor * (lines.count - 1) - fHeight;
						break;
					case Gdiplus::StringAlignmentCenter:
						fStart = (fLineFactor * lines.count - fHeight) / 2;
						break;
					default:
						fStart = 0;
					}
					
					for( int nLine = 0; nLine < lines.count; nLine++ )
					{
						float offset = fStart - fLineFactor * nLine;
						Gdiplus::Matrix matrix( 1, 0, 0, 1, 0, offset );

						CRefLetters letters( m_pText->m_arrLetters, lines[nLine].m_nFirstLetter, lines[nLine].m_nLength );
						for( int nLetter = 0; nLetter < letters.count; nLetter++ )
						{
							Text::CLetterEntity* pLetter = letters.ptr + nLetter;
							
							pLetter->m_oOrigin.Y        += offset;
							pLetter->m_oPathBoundRect.Y += offset;
							pLetter->m_oPathCenter.Y    += offset;
							pLetter->m_pPath->Transform( &matrix );

							if( pLetter->m_oPathBoundRect.Y < fTop )
								fTop = pLetter->m_oPathBoundRect.Y;

							if( pLetter->m_oPathBoundRect.Y + pLetter->m_oPathBoundRect.Height > fBottom )
								fBottom = pLetter->m_oPathBoundRect.Y + pLetter->m_oPathBoundRect.Height;
						}
					}

					m_pText->m_oPathBoundRect.Y = fTop;
					m_pText->m_oPathBoundRect.Height = fBottom - fTop;
				}

			
			protected:
				
				CTextEntity* m_pText;
				CAtlArray<Gdiplus::GraphicsPath> m_arrPaths;

				Gdiplus::RectF m_oLayoutRect;
				Gdiplus::RectF m_oClipRect;

				float m_fLineSpacing;
				BOOL  m_bUnderLine;

				Painter::CBrush    m_oFillBrush;
				Painter::CEdgeText m_oEdge;
				Painter::CShadow   m_oShadow;

				Gdiplus::Matrix		m_TextTransform;		// матрица позиционирования текста в временной картинке 
				Gdiplus::Matrix		m_BitmapTransform;		// матрица позиционирования картинки при отрисовке
				PointF	m_PointLT;
				
			public:
			
				IUnknown* pImage;
				Painter::CTextureManager m_oTextureManager;

				int   m_nFillMode;
				float m_fAngle;
				PointF m_center;

			};			
		}
	}
}	