#pragma once

#include <ImagePaintState.h>
#include <ImageCaching.h>

namespace ImageStudio
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
				class CMatrix3x3
				{
				private:
					double m[3][3];

					BOOL m_bPerspective;
				
				public:

					CMatrix3x3()
					{
						Reset();
					};

					void Reset()
					{
						m[0][0] = 1;
						m[0][1] = 0;
						m[0][2] = 0;
						m[1][0] = 0;
						m[1][1] = 1;
						m[1][2] = 0;
						m[2][0] = 0;
						m[2][1] = 0;
						m[2][2] = 1;

						m_bPerspective = FALSE;
					}


					
					void SetTranslate( double dX, double dY )
					{
						m[0][0] = 1;
						m[0][1] = 0;
						m[0][2] = 0;
						m[1][0] = 0;
						m[1][1] = 1;
						m[1][2] = 0;
						m[2][0] = dX;
						m[2][1] = dY;
						m[2][2] = 1;

						m_bPerspective = FALSE;
					}
					void SetScale( double dScaleX, double dScaleY )
					{
						m[0][0] = dScaleX;
						m[0][1] = 0;
						m[0][2] = 0;
						m[1][0] = 0;
						m[1][1] = dScaleY;
						m[1][2] = 0;
						m[2][0] = 0;
						m[2][1] = 0;
						m[2][2] = 1;

						m_bPerspective = FALSE;
					}

					void SetScaleAt( double dScaleX, double dScaleY, double dX, double dY )
					{
						m[0][0] = dScaleX;
						m[0][1] = 0;
						m[0][2] = 0;
						m[1][0] = 0;
						m[1][1] = dScaleY;
						m[1][2] = 0;
						m[2][0] = dX - dX * dScaleX;
						m[2][1] = dY - dY * dScaleY;
						m[2][2] = 1;

						m_bPerspective = FALSE;
					}

					void SetRotate( double dAngle /*degrees*/ )
					{
						dAngle *= IPP_PI180;
						
						double dSin = sin( dAngle );
						double dCos = cos( dAngle );

						SetRotate( dSin, dCos );
					}

					void SetRotate( double dSin, double dCos )
					{
						m[0][0] = dCos;
						m[0][1] = -dSin;
						m[0][2] = 0;
						m[1][0] = dSin;
						m[1][1] = dCos;
						m[1][2] = 0;
						m[2][0] = 0;
						m[2][1] = 0;
						m[2][2] = 1;

						m_bPerspective = FALSE;
					}

					void SetRotateAt( double dAngle /*degrees*/, double dX, double dY )
					{
						dAngle *= IPP_PI180;

						double dSin = sin( dAngle );
						double dCos = cos( dAngle );

						SetRotateAt( dSin, dCos, dX, dY );						
					}

					void SetRotateAt( double dSin, double dCos, double dX, double dY )
					{
						m[0][0] = dCos;
						m[0][1] = -dSin;
						m[0][2] = 0;
						m[1][0] = dSin;
						m[1][1] = dCos;
						m[1][2] = 0;
						m[2][0] = dX - dX * dCos - dY * dSin;
						m[2][1] = dY + dX * dSin - dY * dCos;
						m[2][2] = 1;

						m_bPerspective = FALSE;
					}
					void SetShear( double dX, double dY )
					{
						m[0][0] = 1;
						m[0][1] = dY;
						m[0][2] = 0;
						m[1][0] = dX;
						m[1][1] = 1;
						m[1][2] = 0;
						m[2][0] = 0;
						m[2][1] = 0;
						m[2][2] = 1;

						m_bPerspective = FALSE;
					}

					void SetPerspective( double dX, double dY )
					{
						m[0][0] = 1;
						m[0][1] = 0;
						m[0][2] = dX;
						m[1][0] = 0;
						m[1][1] = 1;
						m[1][2] = dY;
						m[2][0] = 0;
						m[2][1] = 0;
						m[2][2] = 1;

						m_bPerspective = TRUE;
					}
				

					void Translate( double dX, double dY )
					{
						m[2][0] += m[0][0] * dX + m[1][0] * dY;
						m[2][1] += m[0][1] * dX + m[1][1] * dY;
						
						if( !m_bPerspective )
							return;
						
						m[2][2] = m[2][2] + m[0][2] * dX + m[1][2] * dY;
					}
					void Scale( double dScaleX, double dScaleY )
					{
						m[0][0] *= dScaleX;
						m[0][1] *= dScaleX;
						m[1][0] *= dScaleY;
						m[1][1] *= dScaleY;

						if( !m_bPerspective )
							return;

						m[0][2] *= dScaleX;
						m[1][2] *= dScaleY;
					}

					void ScaleAt( double dScaleX, double dScaleY, double dX, double dY )
					{
						double x = dX - dX * dScaleX;
						double y = dY - dY * dScaleY;

						double tmp[6] = {
							m[0][0] * dScaleX,
							m[0][1] * dScaleX,
							m[1][0] * dScaleY,
							m[1][1] * dScaleY,
							m[0][0] * x + m[1][0] * y + m[2][0],
							m[0][1] * x + m[1][1] * y + m[2][1]
						};

						m[0][0] = tmp[0];
						m[0][1] = tmp[1];
						m[1][0] = tmp[2];
						m[1][1] = tmp[3];
						m[2][0] = tmp[4];
						m[2][1] = tmp[5];

						if( !m_bPerspective )
							return;

						tmp[0] = m[0][2] * dScaleX;
						tmp[1] = m[1][2] * dScaleY;
						tmp[2] = m[0][2] * x + m[1][2] * y + m[2][2];

						m[0][2] = tmp[0];
						m[1][2] = tmp[1];
						m[2][2] = tmp[2];
					}

					void Rotate( double dAngle /*degrees*/ )
					{
						dAngle *= IPP_PI180;
						
						double dSin = sin( dAngle );
						double dCos = cos( dAngle );

						Rotate( dSin, dCos );
					}

					void Rotate( double dSin, double dCos )
					{
						double tmp[4] = {
							m[0][0] * dCos - m[1][0] * dSin,
							m[0][1] * dCos - m[1][1] * dSin,
							m[0][0] * dSin + m[1][0] * dCos,
							m[0][1] * dSin + m[1][1] * dCos
						};
						
						m[0][0] = tmp[0];
						m[0][1] = tmp[1];
						m[1][0] = tmp[2];
						m[1][1] = tmp[3];

						if( !m_bPerspective )
							return;

						tmp[0] = m[0][2] * dCos - m[1][2] * dSin;
						tmp[1] = m[0][2] * dSin + m[1][2] * dCos;
						
						m[0][2] = tmp[0];
						m[1][2] = tmp[1];
					}

					void RotateAt( double dAngle /*degrees*/, double dX, double dY )
					{
						dAngle *= IPP_PI180;

						double dSin = sin( dAngle );
						double dCos = cos( dAngle );

						RotateAt( dSin, dCos, dX, dY );
					}

					void RotateAt( double dSin, double dCos, double dX, double dY )
					{
						double x = dX - dX * dCos - dY * dSin;
						double y = dY + dX * dSin - dY * dCos;

						double tmp[6] = {
							m[0][0] * dCos - m[1][0] * dSin,
							m[0][1] * dCos - m[1][1] * dSin,
							m[0][0] * dSin + m[1][0] * dCos,
							m[0][1] * dSin + m[1][1] * dCos,
							m[0][0] * x + m[1][0] * y + m[2][0],
							m[0][1] * x + m[1][1] * y + m[2][1]
						};
						
						m[0][0] = tmp[0];
						m[0][1] = tmp[1];
						m[1][0] = tmp[2];
						m[1][1] = tmp[3];
						m[2][0] = tmp[4];
						m[2][1] = tmp[5];

						if( !m_bPerspective )
							return;

						tmp[0] = m[0][2] * dCos - m[1][2] * dSin;
						tmp[1] = m[0][2] * dSin + m[1][2] * dCos;
						tmp[2] = m[0][2] * x + m[1][2] * y + m[2][2];
						
						m[0][2] = tmp[0];
						m[1][2] = tmp[1];
						m[2][2] = tmp[2];
					}
					void Shear( double dX, double dY )
					{
						double tmp[4] = {
							m[0][0] + m[1][0] * dY,
							m[0][1] + m[1][1] * dY,
							m[1][0] + m[0][0] * dX,
							m[1][1] + m[0][1] * dX
						};

						m[0][0] = tmp[0];
						m[0][1] = tmp[1];
						m[1][0] = tmp[2];
						m[1][1] = tmp[3];

						if( !m_bPerspective )
							return;

						tmp[0] = m[0][2] + m[1][2] * dY;
						tmp[1] = m[1][2] + m[0][2] * dX;

						m[0][2] = tmp[0];
						m[1][2] = tmp[1];
					}

					void Perspective( double dX, double dY )
					{
						m[0][0] += m[2][0] * dX;
						m[0][1] += m[2][1] * dX;
						m[0][2] += m[2][2] * dX;
						m[1][0] += m[2][0] * dY;
						m[1][1] += m[2][1] * dY;
						m[1][2] += m[2][2] * dY;

						m_bPerspective = TRUE;
					}


					
					void PreTranslate( double dX, double dY )
					{
						if( !m_bPerspective )
						{
							m[2][0] += dX;
							m[2][1] += dY;
							return;
						}

						m[0][0] += m[0][2] * dX;
						m[0][1] += m[0][2] * dY;
						m[1][0] += m[1][2] * dX;
						m[1][1] += m[1][2] * dY;
						m[2][0] += m[2][2] * dX;
						m[2][1] += m[2][2] * dY;
					}
					
					static void Multiply( const CMatrix3x3& m1, const CMatrix3x3& m2, CMatrix3x3& result )
					{
						// result = m1 * m2;

						if( m1.m_bPerspective || m2.m_bPerspective )
						{
							result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[1][0] * m2.m[0][1] + m1.m[2][0] * m2.m[0][2];
							result.m[0][1] = m1.m[0][1] * m2.m[0][0] + m1.m[1][1] * m2.m[0][1] + m1.m[2][1] * m2.m[0][2];
							result.m[0][2] = m1.m[0][2] * m2.m[0][0] + m1.m[1][2] * m2.m[0][1] + m1.m[2][2] * m2.m[0][2];
							result.m[1][0] = m1.m[0][0] * m2.m[1][0] + m1.m[1][0] * m2.m[1][1] + m1.m[2][0] * m2.m[1][2];
							result.m[1][1] = m1.m[0][1] * m2.m[1][0] + m1.m[1][1] * m2.m[1][1] + m1.m[2][1] * m2.m[1][2];
							result.m[1][2] = m1.m[0][2] * m2.m[1][0] + m1.m[1][2] * m2.m[1][1] + m1.m[2][2] * m2.m[1][2];
							result.m[2][0] = m1.m[0][0] * m2.m[2][0] + m1.m[1][0] * m2.m[2][1] + m1.m[2][0] * m2.m[2][2];
							result.m[2][1] = m1.m[0][1] * m2.m[2][0] + m1.m[1][1] * m2.m[2][1] + m1.m[2][1] * m2.m[2][2];
							result.m[2][2] = m1.m[0][2] * m2.m[2][0] + m1.m[1][2] * m2.m[2][1] + m1.m[2][2] * m2.m[2][2];
							
							result.m_bPerspective = TRUE;
						}
						else
						{
							result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[1][0] * m2.m[0][1];
							result.m[0][1] = m1.m[0][1] * m2.m[0][0] + m1.m[1][1] * m2.m[0][1];
							result.m[0][2] = 0;
							result.m[1][0] = m1.m[0][0] * m2.m[1][0] + m1.m[1][0] * m2.m[1][1];
							result.m[1][1] = m1.m[0][1] * m2.m[1][0] + m1.m[1][1] * m2.m[1][1];
							result.m[1][2] = 0;
							result.m[2][0] = m1.m[0][0] * m2.m[2][0] + m1.m[1][0] * m2.m[2][1] + m1.m[2][0];
							result.m[2][1] = m1.m[0][1] * m2.m[2][0] + m1.m[1][1] * m2.m[2][1] + m1.m[2][1];
							result.m[2][2] = 1;
							
							result.m_bPerspective = FALSE;
						}
					}

					void Multiply( const CMatrix3x3& other )
					{
						CMatrix3x3 tmp( *this );

						Multiply( tmp, other, *this );
					}
				
					void TransformPoints( Gdiplus::PointF* pPoints, int nCountPoints ) const
					{
						for( int i = 0; i < nCountPoints; ++i, ++pPoints )
						{
							double x = pPoints->X * m[0][0] + pPoints->Y * m[1][0] + m[2][0];
							double y = pPoints->X * m[0][1] + pPoints->Y * m[1][1] + m[2][1];

							if( m_bPerspective )
							{
								double w = pPoints->X * m[0][2] + pPoints->Y * m[1][2] + m[2][2];
								x /= w;
								y /= w;
							}

							pPoints->X = float(x);
							pPoints->Y = float(y);
						}
					}

					void GetBounds( const Gdiplus::PointF* pPoints, int nCountPoints, Gdiplus::RectF& result ) const
					{
						if( nCountPoints <= 0 )
							return;

						double dMinX = pPoints->X;
						double dMaxX = pPoints->X;
						double dMinY = pPoints->Y;
						double dMaxY = pPoints->Y;
						
						for( int i = 0; i < nCountPoints; ++i, ++pPoints )
						{
							double x = pPoints->X * m[0][0] + pPoints->Y * m[1][0] + m[2][0];
							double y = pPoints->X * m[0][1] + pPoints->Y * m[1][1] + m[2][1];

							if( m_bPerspective )
							{
								double w = pPoints->X * m[0][2] + pPoints->Y * m[1][2] + m[2][2];
								x /= w;
								y /= w;
							}

							if( x < dMinX ) dMinX = x;
							if( x > dMaxX ) dMaxX = x;
							if( y < dMinY ) dMinY = y;
							if( y > dMaxY ) dMaxY = y;
						}

						result.X = float(dMinX);
						result.Y = float(dMinY);
						result.Width = float(dMaxX - dMinX);
						result.Height = float(dMaxY - dMinY);
					}

				};
				
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
					virtual void AddToMatrix( CMatrix3x3& oMatrix, double dTime ) = 0;
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

					virtual void AddToMatrix( CMatrix3x3& oMatrix, double dTime )
					{
						CScale oScale;
						Interpolate( oScale, dTime );
						oMatrix.Scale( oScale.dScaleX, oScale.dScaleY );
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

					virtual void AddToMatrix( CMatrix3x3& oMatrix, double dTime )
					{
						CScaleAt oScale;
						Interpolate( oScale, dTime );
						oMatrix.ScaleAt( oScale.dScaleX, oScale.dScaleY, oScale.dX, oScale.dY );
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

					virtual void AddToMatrix( CMatrix3x3& oMatrix, double dTime )
					{
						CShear oShear;
						Interpolate( oShear, dTime );
						oMatrix.Shear( oShear.dX, oShear.dY );
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

					virtual void AddToMatrix( CMatrix3x3& oMatrix, double dTime )
					{
						CTranslate oTranslate;
						Interpolate( oTranslate, dTime );
						oMatrix.Translate( oTranslate.dX, oTranslate.dY );
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

					virtual void AddToMatrix( CMatrix3x3& oMatrix, double dTime )
					{
						CRotate oRotate;
						Interpolate( oRotate, dTime );
						oMatrix.Rotate( oRotate.dAngle );
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

					virtual void AddToMatrix( CMatrix3x3& oMatrix, double dTime )
					{
						CRotateAt oRotateAt;
						Interpolate( oRotateAt, dTime );
						oMatrix.RotateAt( oRotateAt.dAngle, oRotateAt.dX, oRotateAt.dY );
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
					void Update( double dTime, CMatrix3x3& oTransform, double& dTransparency )
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
					if( dTime < 1 || dTime > 2 )
					{
						if( dTime < 2 )
							m_oPrevTransforms.Update( dTime, m_oTransform, m_dTransparency );
						else
							m_oPostTransforms.Update( 3 - dTime, m_oTransform, m_dTransparency );
					}

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
				
				double GetPathLength( const Gdiplus::GraphicsPath* pSource )
				{
					if( !pSource )
						return 0;

					Gdiplus::GraphicsPathIterator iter( pSource );
					if( Gdiplus::Ok != iter.GetLastStatus() )
						return 0;

					double dLength = 0;

					Gdiplus::GraphicsPath path;
					BOOL bPathClosed;

					int nSubPathCount = iter.GetSubpathCount();
					if( nSubPathCount > 1 )
					{
						for( int i = 0; i < nSubPathCount; i++ )
						{
							if( iter.NextSubpath( &path, &bPathClosed ) )
							{
								dLength += GetPathLength( &path );
							}
						}

						return dLength;
					}

					if( !iter.NextSubpath( &path, &bPathClosed ) )
						return 0;

					path.Outline();

					int nPointCount = path.GetPointCount();
					if( nPointCount <= 1 )
						return 0;

					CBuffer<Gdiplus::PointF> oPathPoints( nPointCount + 1 );
					if( oPathPoints.IsEmpty() )
						return 0;

					path.GetPathPoints( oPathPoints.GetPtr(), nPointCount );

					if( bPathClosed )
					{
						oPathPoints[nPointCount++] = oPathPoints[0];
					}

					for( int i = 1; i < nPointCount; i++ )
					{
						double dx = oPathPoints[i].X - oPathPoints[i - 1].X;
						double dy = oPathPoints[i].Y - oPathPoints[i - 1].Y;

						dLength += sqrt( dx * dx + dy * dy );
					}

					return dLength;
				}


				void Draw( Gdiplus::Graphics& oRender, const Gdiplus::RectF& oClipRect, Painter::CBrush& oBrush, Painter::CTextureManager* pTextureManager, Gdiplus::RectF* pBoundClip )
				{
					if( !m_pPath || m_dTransparency < IPP_EPS23 )
						return;

					Gdiplus::RectF oRenderRect = m_oLayoutRect;
					oRenderRect.Inflate( 1, 1 );
					
					if( FALSE == oRenderRect.Intersect( oClipRect ) )
						return;

					oRender.SetTransform( &m_oTransform );

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

				void DrawEdge2( Gdiplus::Graphics& oRender, const Gdiplus::RectF& oClipRect, Painter::CEdgeText& oEdge, double dCompleteness )
				{
					if( !m_pPath || m_dTransparency < IPP_EPS23 || dCompleteness < IPP_EPS23 )
						return;

					float fPenWidth = (float)oEdge.GetDistance();
					Gdiplus::RectF oRenderRect = m_oLayoutRect;
					oRenderRect.Inflate( 1 + fPenWidth, 1 + fPenWidth );
					
					if( FALSE == oRenderRect.Intersect( oClipRect ) )
						return;

					oEdge.SetTransparency( m_dTransparency );

					Gdiplus::Pen* pPen = oEdge.GetPen();
					if( !pPen )
						return;

					if( dCompleteness > (1.0 - IPP_EPS23) )
					{
						oRender.SetTransform( &m_oTransform );
						oRender.DrawPath( pPen, m_pPath );
						return;
					}

					Gdiplus::GraphicsPath path;
					path.AddPath( m_pPath, FALSE );

					if( path.GetLastStatus() != Gdiplus::Ok )
						return;
					
					path.Outline( &m_oTransform );

					int nPointCount = path.GetPointCount();
					if( nPointCount <= 1 )
						return;

					CBuffer<Gdiplus::PointF> oPathPoints( nPointCount + 1 );
					CBuffer<BYTE> oPathTypes( nPointCount + 1 );
					CBuffer<double> oPathLengths( nPointCount + 1 );

					if( oPathTypes.IsEmpty() || oPathPoints.IsEmpty() || oPathLengths.IsEmpty() )
						return;

					path.GetPathPoints( oPathPoints.GetPtr(), nPointCount );
					path.GetPathTypes( oPathTypes.GetPtr(), nPointCount );

					oPathTypes[nPointCount] = 0;
					oPathLengths[0] = 0;

					int nStartSubPathIndex = 0;
					double dMaxLength = 0;

					for( int i = 1; i <= nPointCount; i++ )
					{
						double dx, dy;

						int nPointType = oPathTypes[i] & PathPointTypePathTypeMask;
						if( nPointType != PathPointTypeStart )
						{
							dx = oPathPoints[i].X;
							dy = oPathPoints[i].Y;
						}
						else
						{
							dx = oPathPoints[nStartSubPathIndex].X;
							dy = oPathPoints[nStartSubPathIndex].Y;
							nStartSubPathIndex = i;
						}

						dx -= oPathPoints[i - 1].X;
						dy -= oPathPoints[i - 1].Y;

						dMaxLength += sqrt( dx * dx + dy * dy );
						oPathLengths[i] = dMaxLength;
					}

					dMaxLength *= dCompleteness;
					nStartSubPathIndex = 0;

					for( int i = 1; i <= nPointCount; i++ )
					{
						int nPointType = oPathTypes[i] & PathPointTypePathTypeMask;
						if( nPointType != PathPointTypeStart )
						{
							if( oPathLengths[i] <= dMaxLength )
								continue;

							double dPercent = (oPathLengths[i] - dMaxLength) / (oPathLengths[i] - oPathLengths[i - 1]);
							double x = oPathPoints[i].X;
							double y = oPathPoints[i].Y;

							oPathPoints[i].X = float(x - (x - oPathPoints[i - 1].X) * dPercent);
							oPathPoints[i].Y = float(y - (y - oPathPoints[i - 1].Y) * dPercent);

							oRender.ResetTransform();
							oRender.DrawLines( pPen, &oPathPoints[nStartSubPathIndex], i - nStartSubPathIndex + 1 );
							return;
						}
						else
						{
							if( oPathLengths[i] <= dMaxLength )
							{
								Gdiplus::PointF tmp = oPathPoints[i];
								oPathPoints[i] = oPathPoints[nStartSubPathIndex];

								oRender.ResetTransform();
								oRender.DrawLines( pPen, &oPathPoints[nStartSubPathIndex], i - nStartSubPathIndex + 1 );

								oPathPoints[i] = tmp;
								nStartSubPathIndex = i;
								continue;
							}

							double dPercent = (oPathLengths[i] - dMaxLength) / (oPathLengths[i] - oPathLengths[i - 1]);
							double x = oPathPoints[nStartSubPathIndex].X;
							double y = oPathPoints[nStartSubPathIndex].Y;

							oPathPoints[i].X = float(x - (x - oPathPoints[i - 1].X) * dPercent);
							oPathPoints[i].Y = float(y - (y - oPathPoints[i - 1].Y) * dPercent);

							oRender.ResetTransform();
							oRender.DrawLines( pPen, &oPathPoints[nStartSubPathIndex], i - nStartSubPathIndex + 1 );
							return;
						}
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

			class CTextPainter2 : public ImageStudio::Caching::CCacheItemPointer
			{
			protected:
				CTextEntity* m_pText;
				CAtlArray<Gdiplus::GraphicsPath> m_arrPaths;
				
				Gdiplus::RectF m_oLayoutRect;
				//Gdiplus::RectF m_oClipRect;
				BOOL m_bClipping;
				BOOL m_bRotateClipping;
				
				float m_fLineSpacing;
				BOOL  m_bUnderLine;

				double m_dTime; // последняя временная метка обновления

				Painter::CBrush    m_oFillBrush;
				Painter::CEdgeText m_oEdge;
				Painter::CShadow   m_oShadow;
				
			public:
				IUnknown* pImage;
				Painter::CTextureManager m_oTextureManager;

				int   m_nFillMode;
				float m_fAngle;

				// направление текста в относительных координатах
				double m_dStartX;
				double m_dStartY;
				double m_dEndX;
				double m_dEndY;

				BOOL m_bFillText;
				BOOL m_bDrawEdge;
				BOOL m_bDrawShadow;
				int m_nTypeWrittingEdge;

			public:
				CTextPainter2()
				{
					m_pText = NULL;
					m_bClipping = FALSE;
					m_bRotateClipping = FALSE;
					
					m_fLineSpacing = 0;
					m_bUnderLine = FALSE;
					
					m_dTime = 0;
					
					pImage = NULL;

					m_nFillMode = 0;
					m_fAngle = 0;

					m_dStartX = 0;
					m_dStartY = 0;
					m_dEndX = 0;
					m_dEndY = 0;

					m_bFillText = TRUE;
					m_bDrawEdge = TRUE;
					m_bDrawShadow = TRUE;
					m_nTypeWrittingEdge = 0;
				}
				~CTextPainter2()
				{
					ReleaseDynamicData();
				}
				
				void Clear()
				{
					ReleaseDynamicData();

					Utils::ClearRect( m_oLayoutRect );

					m_fLineSpacing = 0;
					m_bUnderLine = FALSE;

					m_oFillBrush.Clear();
					m_oEdge.Clear();
					m_oShadow.Clear();

					m_nFillMode = 0;
					m_fAngle = 0;

					m_bFillText = TRUE;
					m_bDrawEdge = TRUE;
					m_bDrawShadow = TRUE;
					m_nTypeWrittingEdge = 0;
				}
				
				BOOL CreateText( BSTR bstrText, const Gdiplus::Font& oFont, const Gdiplus::RectF& oClipLayoutRect, int nHorAlign, int nVerAlign, int nPlaceType = 0, int nFrameWidth = 0, int nFrameHeight = 0 )
				{
					Clear();

					if( NULL == bstrText || FALSE == oFont.IsAvailable() )
						return FALSE;

					// создаём патч всего текста
					Gdiplus::FontFamily      oFontFamily;
					Gdiplus::StringFormat    oStrFormat;
					Gdiplus::StringAlignment eStrAlign;
					Gdiplus::StringAlignment eLineAlign;
					Gdiplus::GraphicsPath    oTextPath;

					if( Gdiplus::Ok != oFont.GetFamily( &oFontFamily ) )
						return FALSE;

					Gdiplus::RectF oLayoutRect = oClipLayoutRect;
					
					if( abs(oLayoutRect.Width)  < IPP_EPS23 ) oLayoutRect.Width  = 0;
					if( abs(oLayoutRect.Height) < IPP_EPS23 ) oLayoutRect.Height = 0;

					// нормализация прямоугольника вывода
					if( oLayoutRect.Width < 0 )
					{
						oLayoutRect.X     += oLayoutRect.Width;
						oLayoutRect.Width = -oLayoutRect.Width;
					}
					if( oLayoutRect.Height < 0 )
					{
						oLayoutRect.Y      += oLayoutRect.Height;
						oLayoutRect.Height = -oLayoutRect.Height;
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

					switch( nVerAlign )
					{
					case Gdiplus::StringAlignmentCenter: 
						eLineAlign = Gdiplus::StringAlignmentCenter; break;
					case Gdiplus::StringAlignmentFar:
						eLineAlign = Gdiplus::StringAlignmentFar; break;
					default:
						eLineAlign = Gdiplus::StringAlignmentNear;
					}
					
					oStrFormat.SetAlignment( eStrAlign );
					oStrFormat.SetLineAlignment( eLineAlign );

					Gdiplus::PointF oBufSize;

					switch( nPlaceType & 0x7fffffff )
					{
					case 1:
						switch( eStrAlign )
						{
						case Gdiplus::StringAlignmentCenter:
							oBufSize.X = oLayoutRect.Width / 2;
							break;
						case Gdiplus::StringAlignmentFar:
							oBufSize.X = oLayoutRect.Width;
							break;
						default:;
						}

						oLayoutRect.Width = 0;
						m_bClipping = TRUE;
						m_bRotateClipping = (nPlaceType & 0x80000000) ? TRUE : FALSE;
						break;
					
					case 2:
						switch( eStrAlign )
						{
						case Gdiplus::StringAlignmentCenter:
							oBufSize.X = oLayoutRect.Width / 2;
							break;
						case Gdiplus::StringAlignmentFar:
							oBufSize.X = oLayoutRect.Width;
							break;
						default:;
						}

						oLayoutRect.Width = 0;
						m_bClipping = m_bRotateClipping = (nPlaceType & 0x80000000) ? TRUE : FALSE;
						break;

					default:
						switch( eLineAlign )
						{
						case Gdiplus::StringAlignmentCenter:
							oBufSize.Y = oLayoutRect.Height / 2;
							break;
						case Gdiplus::StringAlignmentFar:
							oBufSize.Y = oLayoutRect.Height;
							break;
						default:;
						}

						oLayoutRect.Height = 0;
						m_bClipping = m_bRotateClipping = (nPlaceType & 0x80000000) ? TRUE : FALSE;
					};
					
					int   nFontStyle = oFont.GetStyle();
					float fFontSize  = oFont.GetSize();

					// получаем общий контур текста
					if( Gdiplus::Ok != oTextPath.AddString( bstrText, -1, &oFontFamily, nFontStyle, fFontSize, oLayoutRect, &oStrFormat ))
						return FALSE;

					Gdiplus::Matrix matrix( 1, 0, 0, 1, oBufSize.X, oBufSize.Y );
					oTextPath.Transform( &matrix );

					// задаём межстрочный интервал
					float fFontAscent  = oFontFamily.GetCellAscent( nFontStyle );
					float fFontDescent = oFontFamily.GetCellDescent( nFontStyle );
					float fFontHeight  = oFontFamily.GetEmHeight( nFontStyle );
					float fFontSpacing = oFontFamily.GetLineSpacing( nFontStyle );

					float fSize = fFontSize / fFontHeight;
					m_fLineSpacing = fFontSpacing * fSize;
					m_bUnderLine   = ((nFontStyle & Gdiplus::FontStyleStrikeout)|(nFontStyle & Gdiplus::FontStyleUnderline)) ? TRUE : FALSE;

					BOOL bRetValue = CreateLetters( oTextPath );
					if( bRetValue )
					{
						float fHeight = (fFontAscent + fFontDescent) * fSize;
						float fOffset = fFontDescent * fSize / 2;
						
						CorrectionLettersPositions( eLineAlign, fHeight, fOffset );
					}

					
					//Gdiplus::PointF oLayoutCenter( Utils::GetCenter( m_oLayoutRect ) );
					if( nFrameWidth > 0 )
					{
						m_oLayoutRect.X = 0;
						m_oLayoutRect.Width = (float)nFrameWidth;
					}

					if( nFrameHeight > 0 )
					{
						m_oLayoutRect.Y = 0;
						m_oLayoutRect.Height = (float)nFrameHeight;
					}

					return bRetValue;
				}
				BOOL CreateEffects( int nAnimationFadeIn, int nAnimationFadeOut, int nTypeEffects, BOOL bInitRandom = TRUE )
				{
					RemoveTransforms();
					
					if( bInitRandom )
						::srand( ::GetTickCount() );

					CreateTransforms( nAnimationFadeIn, TRUE, nTypeEffects );
					CreateTransforms( nAnimationFadeOut, FALSE, nTypeEffects );

					Update( 0 );

					return TRUE;
				}
				
				void Draw( double dTime, Gdiplus::Graphics& oRender )
				{
					if( dTime != m_dTime )
					{
						Update( dTime );
					}

					if( !m_pText )
						return;

					Gdiplus::GraphicsState oRenderState;
					Gdiplus::RectF oClipRect;

					oRenderState = oRender.Save();

					oRender.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
					oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
					oRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);

					if( m_bClipping )
					{
						if( !m_bRotateClipping || abs(m_fAngle) < 0.001 )
						{
							oRender.SetClip( m_oLayoutRect, Gdiplus::CombineModeIntersect );
						}
						else
						{
							Gdiplus::Matrix matrix(1, 0, 0, 1, 0, 0);
							matrix.RotateAt( m_fAngle, Utils::GetCenter( m_oLayoutRect ), Gdiplus::MatrixOrderAppend );

							Gdiplus::Region region( m_oLayoutRect );
							region.Transform( &matrix );

							oRender.SetClip( &region, Gdiplus::CombineModeIntersect );
						}
					}

					oRender.GetClipBounds( &oClipRect );
					oRender.ResetTransform();

					CRefLetters letters( m_pText->m_arrLetters );

					if( m_oShadow.Visible && m_bDrawShadow )
					{
						int alpha = min(255, max(0,  m_oShadow.GetAlpha()));
						int color = m_oShadow.GetColor();
						int nBlur = (int)m_oShadow.GetBlurSize();
						float dx  = (float)m_oShadow.GetDistanceX();
						float dy  = (float)m_oShadow.GetDistanceY();

						Gdiplus::Color oColor( alpha, GetRValue(color), GetGValue(color), GetBValue(color) );

						for( int i = 0; i < letters.count; i++ )
						{
							letters.ptr[i].DrawShadow( oRender, oClipRect, oColor, dx, dy, nBlur );
						}
					}

					int nCurLetter = 0;

					if( m_oEdge.Visible && m_bDrawEdge )
					{
						for( int i = 0; i < letters.count; i++ )
						{
							if( letters.ptr[i].m_bNotLetter )
								continue;

							switch( m_nTypeWrittingEdge )
							{
							case 1:
								{
									double t = m_dTime * letters.count;
									if( nCurLetter >= t )
										break;

									t -= nCurLetter;
									if( t >= 1 )
									{
										letters.ptr[i].DrawEdge( oRender, oClipRect, m_oEdge );
										break;
									}

									letters.ptr[i].DrawEdge2( oRender, oClipRect, m_oEdge, t );
									break;
								}
								break;
							case 2:
								letters.ptr[i].DrawEdge2( oRender, oClipRect, m_oEdge, m_dTime );
								break;
							default:
								letters.ptr[i].DrawEdge( oRender, oClipRect, m_oEdge );
							}

							++nCurLetter;
						}
					}

					Gdiplus::RectF oBrushRect( m_pText->m_oPathBoundRect );

					for( int i = 0; i < letters.count; i++ )
					{
						if( letters.ptr[i].m_bNotLetter && m_oEdge.Visible && m_bDrawEdge )
						{
							switch( m_nTypeWrittingEdge )
							{
							case 1:
								{
									double t = m_dTime * letters.count;
									if( nCurLetter >= t )
										break;

									t -= nCurLetter;
									if( t >= 1 )
									{
										letters.ptr[i].DrawEdge( oRender, oClipRect, m_oEdge );
										break;
									}

									letters.ptr[i].DrawEdge2( oRender, oClipRect, m_oEdge, t );
									break;
								}
								break;
							case 2:
								letters.ptr[i].DrawEdge2( oRender, oClipRect, m_oEdge, m_dTime );
								break;
							default:
								letters.ptr[i].DrawEdge( oRender, oClipRect, m_oEdge );
							}

							++nCurLetter;
						}
						
						if( m_bFillText )
						{
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

							letters.ptr[i].Draw( oRender, oClipRect, m_oFillBrush, &m_oTextureManager, &oBrushRect );
						}
					}

					oRender.Restore( oRenderState );
				}
				
				void SetFillBrush( const Painter::CBrush& oBrush )
				{
					m_oFillBrush = oBrush;
				}
				void SetShadow( const Painter::CShadow& oShadow )
				{
					m_oShadow = oShadow;
				}
				void SetEdge( const Painter::CEdgeText& oEdge )
				{
					m_oEdge = oEdge;
				}
				
			protected:
				void ReleaseDynamicData()
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
				void RemoveTransforms()
				{
					if( !m_pText )
						return;

					m_pText->RemoveTransforms();

					CRefLetters letters( m_pText->m_arrLetters );
					for( ; letters.count > 0; --letters.count, ++letters.ptr )
					{
						letters.ptr->RemoveTransforms();
					}
				}
				
				void Update( double dFrame )
				{
					if( m_pText )
					{
						m_dTime = dFrame;

						Gdiplus::Matrix oMatrix(1, 0, 0, 1, 0, 0);

						double dStartX = m_dStartX * m_oLayoutRect.Width;
						double dStartY = m_dStartY * m_oLayoutRect.Height;
						double dEndX   = m_dEndX * m_oLayoutRect.Width;
						double dEndY   = m_dEndY * m_oLayoutRect.Height;
						double dAngle  = m_fAngle + atan2( dEndY - dStartY, dEndX - dStartX ) * (180 / IPP_PI);

						if( abs(dAngle) > 0.001 )
						{
							oMatrix.RotateAt( float(dAngle), Utils::GetCenter( m_oLayoutRect ), Gdiplus::MatrixOrderAppend );
						}

						m_pText->Update( dFrame, oMatrix, 1 );
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
				
				BOOL CreateLetters( const Gdiplus::GraphicsPath& oTextPath )
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
					m_pText->m_oOrigin     = Utils::GetCenter( m_oLayoutRect ); //m_pText->m_oPathCenter;
					
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

				void CorrectionLettersPositions( int nVerAlign, float fLineHeight, float fOffsetY )
				{
					CRefLines lines( m_pText->m_arrLines );

					float fTop    = +1000000;
					float fBottom = -1000000;
					float fStart  = 0;
					float factor  = fLineHeight - m_fLineSpacing;
					float offset  = 0;

					switch( nVerAlign )
					{
					case Gdiplus::StringAlignmentFar:
						fStart = m_oLayoutRect.GetBottom();
						offset = fOffsetY + factor;
						break;
					case Gdiplus::StringAlignmentCenter:
						fStart = Utils::GetVerCenter( m_oLayoutRect );
						offset = fOffsetY / 2;
						break;
					default:
						fStart = m_oLayoutRect.GetTop();
						offset = 0;
					}

				
					for( int nLine = 0; nLine < lines.count; nLine++ )
					{
						float pos  = floor( (Utils::GetVerCenter( lines[nLine].m_oPathBoundRect ) - fStart) / m_fLineSpacing );
						float disp = pos * factor + offset;

						Gdiplus::Matrix matrix( 1, 0, 0, 1, 0, disp );

						CRefLetters letters( m_pText->m_arrLetters, lines[nLine].m_nFirstLetter, lines[nLine].m_nLength );
						for( int nLetter = 0; nLetter < letters.count; nLetter++ )
						{
							Text::CLetterEntity* pLetter = letters.ptr + nLetter;
							
							pLetter->m_oOrigin.Y        += disp;
							pLetter->m_oPathBoundRect.Y += disp;
							pLetter->m_oPathCenter.Y    += disp;
							pLetter->m_pPath->Transform( &matrix );

							if( pLetter->m_oPathBoundRect.GetTop() < fTop )
								fTop = pLetter->m_oPathBoundRect.GetTop();

							if( pLetter->m_oPathBoundRect.GetBottom() > fBottom )
								fBottom = pLetter->m_oPathBoundRect.GetBottom();
						}
					}

					m_pText->m_oPathBoundRect.Y = fTop;
					m_pText->m_oPathBoundRect.Height = fBottom - fTop;
				}

				
				void CreateTransforms( int nId, BOOL bPhaseIn, int nTypeEffects )
				{
					if( !m_pText )
						return;

					if( m_pText->m_arrLetters.GetCount() < 1 )
						return;
					
					if( 0 == nTypeEffects )
					{
						switch( nId )
						{
						case Constants::c_nTextAnim_AlphaSimple:
							SetAnimationAlphaBlendSimple( bPhaseIn );
							break;
						case Constants::c_nTextAnim_AlphaLetterAZ:
							SetAnimationAlphaBlendLetterAZ( bPhaseIn );
							break;
						case Constants::c_nTextAnim_AlphaLetterRandom:
							SetAnimationAlphaBlendLetterRandom( bPhaseIn, TRUE, FALSE );
							break;
						case Constants::c_nTextAnim_AlphaLetterAZSmooth:
							SetAnimationAlphaBlendLetterRandom( bPhaseIn, FALSE, FALSE );
							break;
						case Constants::c_nTextAnim_AlphaLetterRandomGlass:
							SetAnimationAlphaBlendLetterRandomGlass( bPhaseIn, TRUE );
							break;
						case Constants::c_nTextAnim_AlphaLetterAZSmoothGlass:
							SetAnimationAlphaBlendLetterRandomGlass( bPhaseIn, FALSE );
							break;
						case Constants::c_nTextAnim_AlphaLetterRandomScale:
							SetAnimationAlphaBlendLetterRandom( bPhaseIn, TRUE, TRUE );
							break;
						case Constants::c_nTextAnim_AlphaLetterAZSmoothScale:
							SetAnimationAlphaBlendLetterRandom( bPhaseIn, FALSE, TRUE );
							break;
						
						case Constants::c_nTextAnim_DropLeft:
							SetAnimationMove( bPhaseIn, TRUE, -1, 0 );
							break;
						case Constants::c_nTextAnim_DropTop:
							SetAnimationMove( bPhaseIn, TRUE, 0, -1 );
							break;
						case Constants::c_nTextAnim_DropRight:
							SetAnimationMove( bPhaseIn, TRUE, 1, 0 );
							break;
						case Constants::c_nTextAnim_DropBottom:
							SetAnimationMove( bPhaseIn, TRUE, 0, 1 );
							break;
						case Constants::c_nTextAnim_DropTopLeft:
							SetAnimationMove( bPhaseIn, TRUE, -1, -1 );
							break;
						case Constants::c_nTextAnim_DropTopRight:
							SetAnimationMove( bPhaseIn, TRUE, 1, -1 );
							break;
						case Constants::c_nTextAnim_DropBottomLeft:
							SetAnimationMove( bPhaseIn, TRUE, -1, 1 );
							break;
						case Constants::c_nTextAnim_DropBottomRight:
							SetAnimationMove( bPhaseIn, TRUE, -1, -1 );
							break;
						case Constants::c_nTextAnim_MoveLeft:
							SetAnimationMove( bPhaseIn, FALSE, -1, 0 );
							break;
						case Constants::c_nTextAnim_MoveTop:
							SetAnimationMove( bPhaseIn, FALSE, 0, -1 );
							break;
						case Constants::c_nTextAnim_MoveRight:
							SetAnimationMove( bPhaseIn, FALSE, 1, 0 );
							break;
						case Constants::c_nTextAnim_MoveBottom:
							SetAnimationMove( bPhaseIn, FALSE, 0, 1 );
							break;
						case Constants::c_nTextAnim_MoveTopLeft:
							SetAnimationMove( bPhaseIn, FALSE, -1, -1 );
							break;
						case Constants::c_nTextAnim_MoveTopRight:
							SetAnimationMove( bPhaseIn, FALSE, 1, -1 );
							break;
						case Constants::c_nTextAnim_MoveBottomLeft:
							SetAnimationMove( bPhaseIn, FALSE, -1, 1 );
							break;
						case Constants::c_nTextAnim_MoveBottomRight:
							SetAnimationMove( bPhaseIn, FALSE, 1, 1 );
							break;
						
						case Constants::c_nTextAnim_DropRotate1:
							SetAnimationDropRotate( bPhaseIn, FALSE, TRUE );
							break;
						case Constants::c_nTextAnim_DropRotate2:
							SetAnimationDropRotate( bPhaseIn, TRUE, TRUE );
							break;

						case Constants::c_nTextAnim_ScaleFromSmall:
							SetAnimationScaleFrom( bPhaseIn, TRUE );
							break;
						case Constants::c_nTextAnim_ScaleFromLarge:
							SetAnimationScaleFrom( bPhaseIn, FALSE );
							break;
						case Constants::c_nTextAnim_ScaleFromXLargeYSmall:
							SetAnimationScaleFromXY( bPhaseIn, FALSE );
							break;
						case Constants::c_nTextAnim_ScaleFromXSmallYLarge:
							SetAnimationScaleFromXY( bPhaseIn, TRUE );
							break;
						case Constants::c_nTextAnim_ScaleFromSmallDropFromTop:
							SetAnimationScaleFrom( bPhaseIn, TRUE, -1 );
							break;
						case Constants::c_nTextAnim_ScaleFromSmallDropFromBottom:
							SetAnimationScaleFrom( bPhaseIn, TRUE, 1 );
							break;

						case Constants::c_nTextAnim_Rotate1:
							SetAnimationRotate( bPhaseIn, TRUE );
							break;
						case Constants::c_nTextAnim_Rotate2:
							SetAnimationRotate( bPhaseIn, FALSE );
							break;
						case Constants::c_nTextAnim_RotateFromSmall1:
							SetAnimationRotate( bPhaseIn, TRUE, 1 );
							break;
						case Constants::c_nTextAnim_RotateFromSmall2:
							SetAnimationRotate( bPhaseIn, FALSE, 1 );
							break;
						case Constants::c_nTextAnim_RotateFromLarge1:
							SetAnimationRotate( bPhaseIn, TRUE, -1 );
							break;
						case Constants::c_nTextAnim_RotateFromLarge2:
							SetAnimationRotate( bPhaseIn, FALSE, -1 );
							break;
						
						case Constants::c_nTextAnim_StretchScale:
							SetAnimationStretchScale( bPhaseIn, 1, 1 );
							break;
						case Constants::c_nTextAnim_StretchHorScale:
							SetAnimationStretchScale( bPhaseIn, 1, 0 );
							break;
						case Constants::c_nTextAnim_StretchVerScale:
							SetAnimationStretchScale( bPhaseIn, 0, 1 );
							break;
						case Constants::c_nTextAnim_StretchLVerScale:
							SetAnimationStretchScale( bPhaseIn, 0, -1 );
							break;
						case Constants::c_nTextAnim_StretchLVerHorScale:
							SetAnimationStretchScale( bPhaseIn, 1, -1 );
							break;
						
						case Constants::c_nTextAnim_StretchSmall:
							SetAnimationStretchScaleMove( bPhaseIn, TRUE );
							break;
						case Constants::c_nTextAnim_StretchSmallScale:
							SetAnimationStretchScaleMove2( bPhaseIn, TRUE );
							break;
						case Constants::c_nTextAnim_StretchSmallHorScale:
							SetAnimationStretchScaleMove3( bPhaseIn, -1, 0, TRUE );
							break;
						case Constants::c_nTextAnim_StretchSmallVerScale:
							SetAnimationStretchScaleMove3( bPhaseIn, 0, -1, TRUE );
							break;
						case Constants::c_nTextAnim_StretchSmallLargeVerScale:
							SetAnimationStretchScaleMove3( bPhaseIn, 0, 1, TRUE );
							break;
						case Constants::c_nTextAnim_StretchSmallLargeVerHorScale:
							SetAnimationStretchScaleMove3( bPhaseIn, -1, 1, TRUE );
							break;
						case Constants::c_nTextAnim_StretchLarge:
							SetAnimationStretchScaleMove( bPhaseIn, FALSE );
							break;
						case Constants::c_nTextAnim_StretchLargeScale:
							SetAnimationStretchScaleMove2( bPhaseIn, FALSE );
							break;
						case Constants::c_nTextAnim_StretchLargeHorScale:
							SetAnimationStretchScaleMove3( bPhaseIn, -1, 0, FALSE );
							break;
						case Constants::c_nTextAnim_StretchLargeVerScale:
							SetAnimationStretchScaleMove3( bPhaseIn, 0, -1, FALSE );
							break;
						case Constants::c_nTextAnim_StretchLargeLargeVerScale:
							SetAnimationStretchScaleMove3( bPhaseIn, 0, 1, FALSE );
							break;
						case Constants::c_nTextAnim_StretchLargeLargeVerHorScale:
							SetAnimationStretchScaleMove3( bPhaseIn, -1, 1, FALSE );
							break;
						
						// дополнение набора
						// Appear Displace	
						case 91: RandomMovements( bPhaseIn ); break;
						case 92: AppearFade( bPhaseIn ); break;
						case 93: AppearShade( bPhaseIn ); break;
						case 94: AppearRotate( bPhaseIn ); break;
						case 95: AppearRandom( bPhaseIn ); break;
						case 96: Airport( bPhaseIn ); break;
						case 97: AppearShake( bPhaseIn ); break;
						case 98: AppearWedge( bPhaseIn ); break;
						case 99: AppearStream( bPhaseIn ); break;

						// Appear Waves
						case 101: WaveSimple( bPhaseIn ); break;
						case 102: WaveZoom( bPhaseIn ); break;
						case 103: WaveFall(  bPhaseIn ); break;
						case 104: WaveRunShift( bPhaseIn ); break;
						case 105: WaveSwing( bPhaseIn ); break;
						case 106: WaveRun( bPhaseIn ); break;
						case 107: WaveZoomVertical( bPhaseIn ); break;
						case 108: WaveShift( bPhaseIn ); break;
						case 109: WavePulse( bPhaseIn ); break;
						
						// New
						case 402: Ejection( bPhaseIn ); break;
						case 403: Ellipse( bPhaseIn ); break;
						case 404: Wheel( bPhaseIn ); break;
						case 405: Miscarry( bPhaseIn ); break;
						case 406: Rotation( bPhaseIn ); break;
						case 407: Increase( bPhaseIn ); break;
						case 408: TwoString( bPhaseIn ); break;
						case 409: Tumbling( bPhaseIn ); break;
						case 410: Leave( bPhaseIn ); break;
						case 411: Unfold( bPhaseIn ); break;
						case 412: Centrifuge( bPhaseIn ); break;
						case 413: HorizontalRotation( bPhaseIn ); break;
						case 414: HorizontalRotation3D( bPhaseIn ); break;
						case 415: VerticalRotation( bPhaseIn ); break;
						case 416: CheChe( bPhaseIn ); break;
						
						// New2
						case 601: Boomerang( bPhaseIn ); break;
						case 602: Flash( bPhaseIn ); break;
						case 603: Flight( bPhaseIn ); break;
						case 604: Jump( bPhaseIn ); break;
						case 605: Compress( bPhaseIn ); break;
						case 606: Motion( bPhaseIn ); break;
						case 607: Manifestation( bPhaseIn ); break;
						case 608: Shot( bPhaseIn ); break;
						case 609: Reduction( bPhaseIn ); break;
						case 610: VHRotation( bPhaseIn ); break;
						case 611: Rotate3D( bPhaseIn ); break;
						case 612: Spiral( bPhaseIn ); break;
						case 613: Flag( bPhaseIn ); break;
						case 614: Perpendicular( bPhaseIn ); break;
						case 615: Clock( bPhaseIn ); break;
						
						// New3
						case 801: Galaxy( bPhaseIn ); break;
						case 802: WaveRotate3D( bPhaseIn ); break;
						case 803: RandomJump( bPhaseIn ); break;
						case 804: Galaxy2( bPhaseIn ); break;
						case 805: Roll( bPhaseIn ); break;
						case 806: Conga( bPhaseIn ); break;
						case 807: _Matrix( bPhaseIn ); break;
						case 808: HotPinkSpin( bPhaseIn ); break;
						case 809: HopInJumpOut( bPhaseIn ); break;
						case 810: ConJoin( bPhaseIn ); break;
						case 811: HopsCotch( bPhaseIn ); break;
						case 812: Twist( bPhaseIn ); break;
						case 813: StarWars( bPhaseIn ); break;
						case 814: Squeeze( bPhaseIn ); break;
						case 815: Terminator( bPhaseIn ); break;
						}
					}
					else if( 1 == nTypeEffects )
					{
						switch( nId )
						{
						// Appear Displace	
						case 1: RandomMovements( bPhaseIn ); break;
						case 2: AppearFade( bPhaseIn ); break;
						case 3: AppearShade( bPhaseIn ); break;
						case 4: AppearRotate( bPhaseIn ); break;
						case 5: AppearRandom( bPhaseIn ); break;
						case 6: Airport( bPhaseIn ); break;
						case 7: AppearShake( bPhaseIn ); break;
						case 8: AppearWedge( bPhaseIn ); break;
						case 9: AppearStream( bPhaseIn ); break;

						// Appear Waves
						case 101: WaveSimple( bPhaseIn ); break;
						case 102: WaveZoom( bPhaseIn ); break;
						case 103: WaveFall(  bPhaseIn ); break;
						case 104: WaveRunShift( bPhaseIn ); break;
						case 105: WaveSwing( bPhaseIn ); break;
						case 106: WaveRun( bPhaseIn ); break;
						case 107: WaveZoomVertical( bPhaseIn ); break;
						case 108: WaveShift( bPhaseIn ); break;
						case 109: WavePulse( bPhaseIn ); break;
						
						// New
						case 402: Ejection( bPhaseIn ); break;
						case 403: Ellipse( bPhaseIn ); break;
						case 404: Wheel( bPhaseIn ); break;
						case 405: Miscarry( bPhaseIn ); break;
						case 406: Rotation( bPhaseIn ); break;
						case 407: Increase( bPhaseIn ); break;
						case 408: TwoString( bPhaseIn ); break;
						case 409: Tumbling( bPhaseIn ); break;
						case 410: Leave( bPhaseIn ); break;
						case 411: Unfold( bPhaseIn ); break;
						case 412: Centrifuge( bPhaseIn ); break;
						case 413: HorizontalRotation( bPhaseIn ); break;
						case 414: HorizontalRotation3D( bPhaseIn ); break;
						case 415: VerticalRotation( bPhaseIn ); break;
						case 416: CheChe( bPhaseIn ); break;
						
						// New2
						case 601: Boomerang( bPhaseIn ); break;
						case 602: Flash( bPhaseIn ); break;
						case 603: Flight( bPhaseIn ); break;
						case 604: Jump( bPhaseIn ); break;
						case 605: Compress( bPhaseIn ); break;
						case 606: Motion( bPhaseIn ); break;
						case 607: Manifestation( bPhaseIn ); break;
						case 608: Shot( bPhaseIn ); break;
						case 609: Reduction( bPhaseIn ); break;
						case 610: VHRotation( bPhaseIn ); break;
						case 611: Rotate3D( bPhaseIn ); break;
						case 612: Spiral( bPhaseIn ); break;
						case 613: Flag( bPhaseIn ); break;
						case 614: Perpendicular( bPhaseIn ); break;
						case 615: Clock( bPhaseIn ); break;
						
						// New3
						case 801: Galaxy( bPhaseIn ); break;
						case 802: WaveRotate3D( bPhaseIn ); break;
						case 803: RandomJump( bPhaseIn ); break;
						case 804: Galaxy2( bPhaseIn ); break;
						case 805: Roll( bPhaseIn ); break;
						case 806: Conga( bPhaseIn ); break;
						case 807: _Matrix( bPhaseIn ); break;
						case 808: HotPinkSpin( bPhaseIn ); break;
						case 809: HopInJumpOut( bPhaseIn ); break;
						case 810: ConJoin( bPhaseIn ); break;
						case 811: HopsCotch( bPhaseIn ); break;
						case 812: Twist( bPhaseIn ); break;
						case 813: StarWars( bPhaseIn ); break;
						case 814: Squeeze( bPhaseIn ); break;
						case 815: Terminator( bPhaseIn ); break;
						}
					}
				}
				
				void SetAnimationAlphaBlendSimple( BOOL bPhaseIn )
				{
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );
				}

				void SetAnimationAlphaBlendLetterAZ( BOOL bPhaseIn )
				{
					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::Transforms::CTransformManager& oManager = letters[nLetter].GetTransformManager( bPhaseIn );
						oManager.AddTransparency( 0, 1, FALSE );
						oManager.SetTimeParams( bPhaseIn ? nLetter : letters.count - 1 - nLetter, letters.count, 1 );
					}
				}

				void SetAnimationAlphaBlendLetterRandom( BOOL bPhaseIn, BOOL bRandom, BOOL bScale )
				{
					double dStartScale = bScale ? 0 : 1;

					CRefLetters letters( m_pText->m_arrLetters );
					
					Text::Utils::CPermutator oPermutator( letters.count, bRandom );

					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::Transforms::CTransformManager& oManager = letters[nLetter].GetTransformManager( bPhaseIn );
						
						oManager.AddTransparency( 0, 1 );
						oManager.AddScale( dStartScale, 1 );
						oManager.SetTimeParams( bPhaseIn ? oPermutator[nLetter] : letters.count - 1 - oPermutator[nLetter], letters.count, 3 );
					}
				}

				void SetAnimationAlphaBlendLetterRandomGlass( BOOL bPhaseIn, BOOL bRandom )
				{
					Text::Transforms::CScaleTransforms::TData arrScales[3];

					arrScales[0].data.transform.SetParams( 0, 0 );
					arrScales[1].data.transform.SetParams( 2, 2 );
					arrScales[2].data.transform.SetParams( 1, 1 );

					arrScales[1].duration = 2;
					arrScales[2].duration = 1;

					CRefLetters letters( m_pText->m_arrLetters );

					Text::Utils::CPermutator oPermutator( letters.count, bRandom );

					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::Transforms::CTransformManager& oManager = letters[nLetter].GetTransformManager( bPhaseIn );
						oManager.AddFadeTransparency( 0, 1, 3, 2 );
						oManager.AddScaleTransforms( arrScales, 3 );
						oManager.SetTimeParams( bPhaseIn ? oPermutator[nLetter] : letters.count - 1 - oPermutator[nLetter], letters.count, 3 );
					}
				}

				void SetAnimationMove( BOOL bPhaseIn, BOOL bFade, int nHorMove, int nVerMove )
				{
					if     ( nHorMove < 0 )	nHorMove = Utils::c_nLeftBound;
					else if( nHorMove > 0 )	nHorMove = Utils::c_nRightBound;
					else					nHorMove = Utils::c_nEmptyBound;

					if     ( nVerMove < 0 )	nVerMove = Utils::c_nTopBound;
					else if( nVerMove > 0 )	nVerMove = Utils::c_nBottomBound;
					else					nVerMove = Utils::c_nEmptyBound;

					float fX = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, nHorMove );
					float fY = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, nVerMove );

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTranslate( fX, fY, 0, 0 );
					oTextManager.AddTransparency( 0, 1, bFade );
				}

				void SetAnimationDropRotate( BOOL bPhaseIn, BOOL bClockText, BOOL bRotateLetters )
				{
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );
					oTextManager.AddRotate( bClockText ? -720 : 720, 0 );

					if( bRotateLetters )
					{
						for( CRefLetters letters( m_pText->m_arrLetters ); letters.count > 0; --letters.count, ++letters.ptr )
						{
							Text::Transforms::CTransformManager& oLetterManager = letters.ptr->GetTransformManager( bPhaseIn );
							oLetterManager.AddRotate( bClockText ? -500 : 500, 0 );
							oLetterManager.AddTranslate( letters.ptr->m_oOrigin.X, 0, 0, 0 );
						}
					}
				}
				void SetAnimationScaleFrom( BOOL bPhaseIn, BOOL bSmall, int nDropDir = 0 )
				{
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );

					oTextManager.AddTransparency( 0, 1 );
					oTextManager.AddScale( bSmall ? 0 : 2, 1 );

					if( bSmall && nDropDir != 0 )
					{
						if     ( nDropDir < 0 )	nDropDir = Utils::c_nTopBound;
						else if( nDropDir > 0 )	nDropDir = Utils::c_nBottomBound;
						
						float fY = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, nDropDir );

						oTextManager.AddTranslate( 0, fY, 0, 0 );
					}
				}

				void SetAnimationScaleFromXY( BOOL bPhaseIn, BOOL bRevers )
				{
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );
					
					double dx = 0;
					double dy = 0;

					if( bPhaseIn )
					{
						if( bRevers )
							dy = 2;
						else
							dx = 2;
					}
					else
					{
						if( bRevers )
							dx = 2;
						else
							dy = 2;
					}
					oTextManager.AddScale( dx, dy, 1, 1 );
				}

				void SetAnimationRotate( BOOL bPhaseIn, BOOL bClockText, int nScaleDir = 0 )
				{
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );

					double angle = 0;
					if( bPhaseIn )
						angle = bClockText ? -1440 : 1440;
					else
						angle = bClockText ? 1440 : -1440;

					oTextManager.AddRotate( angle, 0 );

					if( nScaleDir != 0 )
					{
						double scale = 0;
						if( bPhaseIn )
							scale = nScaleDir < 0 ? 2 : 0;
						else
							scale = nScaleDir < 0 ? 0 : 2;

						oTextManager.AddScale( scale, 1 );
					}
				}
				void SetAnimationStretchScale( BOOL bPhaseIn, int nHorScaleDir, int nVerScaleDir )
				{
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );
					
					if( bPhaseIn )
					{
						double dScaleX = 1;
						double dScaleY = 1;

						if( nHorScaleDir != 0 )
							dScaleX = nHorScaleDir < 0 ? 1.5 : 0;

						if( nVerScaleDir != 0 )
							dScaleY = nVerScaleDir < 0 ? 2 : 0;

						CRefLetters letters( m_pText->m_arrLetters );
						for( int nLetter = 0; nLetter < letters.count; nLetter++ )
						{
							Text::Transforms::CTransformManager& oLetterManager = letters[nLetter].GetTransformManager( bPhaseIn );
							oLetterManager.AddScale( dScaleX, dScaleY, 1, 1 );
						}
					}
					else
					{
						double dScaleX = 1;
						double dScaleY = 1;

						if( nHorScaleDir != 0 )
							dScaleX = nHorScaleDir < 0 ? 0 : 1.5;

						if( nVerScaleDir != 0 )
							dScaleY = nVerScaleDir < 0 ? 0 : 1.5;

						oTextManager.AddScale( dScaleX, dScaleY, 1, 1 );
					}
				}
				void SetAnimationStretchScaleMove( BOOL bPhaseIn, BOOL bStretch )
				{
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );

					double dMoveFactor = 0;
					if( bPhaseIn )
						dMoveFactor = bStretch ? -0.5 : 0.5;
					else
						dMoveFactor = bStretch ? 0.75 : -0.75;
					
					double dScaleX = bPhaseIn ? 0 : 1;

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::Transforms::CTransformManager& oLetterManager = letters[nLetter].GetTransformManager( bPhaseIn );
						oLetterManager.AddScale( dScaleX, 1, 1, 1 );

						double dx = letters[nLetter].m_oOrigin.X * dMoveFactor;
						oLetterManager.AddTranslate( dx, 0, 0, 0 );
					}
				}
				void SetAnimationStretchScaleMove2( BOOL bPhaseIn, BOOL bStretch )
				{
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );

					double dMoveFactor = 0;
					if( bPhaseIn )
						dMoveFactor = bStretch ? -0.5 : 0.5;
					else
						dMoveFactor = bStretch ? 0.75 : -0.75;

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::Transforms::CTransformManager& oLetterManager = letters[nLetter].GetTransformManager( bPhaseIn );
						oLetterManager.AddScale( 0, 0, 1, 1 );

						double dx = letters[nLetter].m_oOrigin.X * dMoveFactor;
						double dy = letters[nLetter].m_oOrigin.Y * dMoveFactor;
						oLetterManager.AddTranslate( dx, dy, 0, 0 );
					}
				}
				void SetAnimationStretchScaleMove3(  BOOL bPhaseIn, int nHorScaleDir, int nVerScaleDir, BOOL bStretch )
				{
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );

					double dMoveFactor = 0;
					if( bPhaseIn )
						dMoveFactor = bStretch ? -0.5 : 0.5;
					else
						dMoveFactor = bStretch ? 0.75 : -0.75;

					double dScaleX = 1;
					if( nHorScaleDir != 0 )
						dScaleX = nHorScaleDir < 0 ? 0 : 1.5;

					double dScaleY = 1;
					if( nVerScaleDir != 0 )
						dScaleY = nVerScaleDir < 0 ? 0 : 1.5;

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::Transforms::CTransformManager& oLetterManager = letters[nLetter].GetTransformManager( bPhaseIn );
						oLetterManager.AddScale( dScaleX, dScaleY, 1, 1 );

						double dx = letters[nLetter].m_oOrigin.X * dMoveFactor;
						double dy = letters[nLetter].m_oOrigin.Y * (nVerScaleDir > 0 ? 1 : 0);
						oLetterManager.AddTranslate( dx, dy, 0, 0 );
					}
				}

				
				// Appear Displace
				void RandomMovements( BOOL bPhaseIn )
				{
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::Transforms::CTransformManager& oManager = letters[nLetter].GetTransformManager( bPhaseIn );

						float scale = Random( 0.0f, 2.0f );
						oManager.AddScale( scale, 1 );
						
						float dx = m_pText->m_oPathBoundRect.Width  * Random( -0.5f, 0.5f );
						float dy = m_pText->m_oPathBoundRect.Height * Random( -0.5f, 0.5f );
						oManager.AddTranslate( dx, dy, 0, 0 );

						float angle = Random( -1440.0f, 1440.0f );
						oManager.AddRotate( angle, 0 );
					}
				}
				void AppearFade( BOOL bPhaseIn )
				{
					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::Transforms::CTransformManager& oManager = letters[nLetter].GetTransformManager( bPhaseIn );
						
						oManager.SetTimeParams( nLetter, letters.count, 1 );
						oManager.AddTransparency( 0, 1 );
					}
				}
				void AppearShade( BOOL bPhaseIn )
				{
					// сдвиг
					Text::Transforms::CShearTransforms::TData arrShears[3];
					arrShears[0].data.transform.SetParams( 0, 0 );
					arrShears[1].data.transform.SetParams( -1.5, 0 );
					arrShears[2].data.transform.SetParams( 0, 0 );

					// движение
					float fX = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, Utils::c_nLeftBound );

					Text::Transforms::CTranslateTransforms::TData arrMoves[3];
					arrMoves[0].data.transform.SetParams( fX, 0 );
					arrMoves[1].data.transform.SetParams( 0, 0 );
					arrMoves[2].data.transform.SetParams( 0, 0 );

					// применить трансформы
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddShearTransforms( arrShears, 3 );
					oTextManager.AddTranslateTransforms( arrMoves, 3 );
					oTextManager.AddTransparency( 0, 1, FALSE );

					// теперь буквы
					Text::Transforms::CScaleTransforms::TData arrScales[3];
					arrScales[0].data.transform.SetParams( 1, 1 );
					arrScales[1].data.transform.SetParams( 1, 1 );
					arrScales[2].data.transform.SetParams( 1, 1 );
					
					float fTextRightBound = m_pText->m_oPathBoundRect.GetRight();
					arrMoves[0].data.transform.SetParams( 0, 0 );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						CLetterEntity* pLetter = letters.ptr + nLetter;
						
						float fLetterRightBound = pLetter->m_oPathBoundRect.GetRight();
						
						if( pLetter->m_bNotLetter )
						{
							arrMoves [1].data.transform.SetParams( pLetter->m_oPathBoundRect.Width * 0.1, 0 );
							arrScales[1].data.transform.SetParams( 0.8, 1 );
						}
						else
						{
							arrMoves [1].data.transform.SetParams( (fTextRightBound - fLetterRightBound) * 0.2, 0 );
							arrScales[1].data.transform.SetParams( 0.7, 1 );
						}

						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
						oManager.AddScaleTransforms( arrScales, 3 );
						oManager.AddTranslateTransforms( arrMoves, 3 );
					}
					
				}
				void AppearRotate( BOOL bPhaseIn )
				{
					Text::Transforms::CScaleTransforms::TData arrScales[3];
					arrScales[0].data.transform.SetParams( 0, 0 );
					arrScales[1].data.transform.SetParams( 1.5, 1.5 );
					arrScales[2].data.transform.SetParams( 1, 1 );
									
					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::Transforms::CTransformManager& oManager = letters[nLetter].GetTransformManager( bPhaseIn );
						oManager.AddRotate( 0, 360*3 );
						oManager.AddScaleTransforms( arrScales, 3 );
					}
				}
				void AppearRandom( BOOL bPhaseIn )
				{
					Text::Transforms::CScaleTransforms::TData arrScales[3];
					Text::Transforms::CTranslateTransforms::TData arrMoves[3];

					arrScales[0].data.transform.SetParams( 0, 0);
					arrScales[1].data.transform.SetParams( 1.5, 1.5 );
					arrScales[2].data.transform.SetParams( 1, 1 );

					float dx = m_pText->m_oPathBoundRect.Width * 0.1f;
					arrMoves[0].data.transform.SetParams( 0,  0);
					arrMoves[1].data.transform.SetParams( dx, 0 );
					arrMoves[2].data.transform.SetParams( 0,  0 );

					CRefLetters letters( m_pText->m_arrLetters );
					
					Text::Utils::CPermutator oPermutator( letters.count, TRUE );

					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::Transforms::CTransformManager& oManager = letters[nLetter].GetTransformManager( bPhaseIn );
						oManager.AddFadeTransparency( 0, 1 );
						oManager.AddScaleTransforms( arrScales, 3 );
						oManager.AddTranslateTransforms( arrMoves, 3 );
						oManager.SetTimeParams( nLetter, letters.count, 1 );
					}
				}
				void Airport( BOOL bPhaseIn )
				{
				}
				void AppearShake( BOOL bPhaseIn )
				{
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1, FALSE );

					float fY = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, Utils::c_nTopBound );
					
					const int c_nDropPaths = 6;
					const int c_nRandomPaths = 5;

					Text::Transforms::CTransparencyTransforms::TData arrAlphas[3 + c_nRandomPaths];
					arrAlphas[0].data.transform.SetParams( 0 );
					for( int i = 1; i < 3 + c_nRandomPaths; i++ )
					{
						arrAlphas[i].data.transform.SetParams( 1 );
						arrAlphas[i].data.interpolation = FALSE;
					}

					Text::Transforms::CRotateTransforms::TData arrAngles[3 + c_nRandomPaths];
					Text::Transforms::CTranslateTransforms::TData arrMoves[3 + c_nRandomPaths];

					arrAngles[0].data.transform.SetParams( 0 );
					arrAngles[1].data.transform.SetParams( 0 );
					arrAngles[2 + c_nRandomPaths].data.transform.SetParams( 0 );

					arrMoves[0].data.transform.SetParams( 0, fY );
					arrMoves[1].data.transform.SetParams( 0, 0 );
					arrMoves[2 + c_nRandomPaths].data.transform.SetParams( 0, 0 );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						arrAlphas[1].duration = arrMoves[1].duration = arrAngles[1].duration = c_nDropPaths + 1;
						
						Gdiplus::PointF factor(
							m_fLineSpacing * 0.1f,
							m_fLineSpacing * 0.2f
						);

						float angle = (!letters[nLetter].m_bNotLetter) ? 20 : float(::atan2( m_fLineSpacing, letters[nLetter].m_oPathBoundRect.Width * 2 ) / IPP_PI180);
						
						for( int i = 2; i < 2 + c_nRandomPaths; i++ )
						{
							arrMoves [i].data.transform.SetParams( Random(-1.0f, 1.0f) * factor.X, Random(-1.0f, 1.0f) * factor.Y );
							arrAngles[i].data.transform.SetParams( Random(0.3f, 1.0f) * angle * ((i % 2) * 2 - 1) );
						}

						Text::Transforms::CTransformManager& oManager = letters[nLetter].GetTransformManager( bPhaseIn );
						oManager.AddRotateTransforms( arrAngles, 3 + c_nRandomPaths );
						oManager.AddTranslateTransforms( arrMoves, 3 + c_nRandomPaths );
						oManager.AddTransparencyTransforms( arrAlphas, 3 + c_nRandomPaths );
						oManager.SetTimeParams( nLetter, letters.count, 5 );
					}
				}
				void AppearWedge( BOOL bPhaseIn )
				{
					const int nRandomPaths = 4;

					int nCountRepeats = 0;
					int arrRepeats[1 + nRandomPaths];
					int i = 0;
					for( i = 0; i <= nRandomPaths; i++ )
					{
						arrRepeats[i] = ::rand() % 3 + 5;
						nCountRepeats += arrRepeats[i];
					}

					Text::Transforms::CTransparencyTransforms::TData arrAlphas[2 + nRandomPaths];
					arrAlphas[0].data.transform.SetParams( 0 );
					for( i = 1; i < nRandomPaths + 2; i++ )
					{
						arrAlphas[i].data.transform.SetParams( 1 );
						arrAlphas[i].duration = nCountRepeats / nRandomPaths + 1;
					}
						
					Utils::CPermutator oPermutator( nRandomPaths + 1, TRUE );
					
					Text::Transforms::CScaleTransforms::TData arrScales[2 + nRandomPaths];
					arrScales[0].data.transform.SetParams( 0, 0 );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
						oManager.AddTransparencyTransforms( arrAlphas, nRandomPaths + 2 );

						float dx = m_fLineSpacing * 0.2f;
						float dy = m_fLineSpacing * 0.5f;

						dy *= float((/*::rand()*/ nLetter % 2) * 2 - 1);
						oManager.AddTranslate( dx, dy, 0, 0 );

						oPermutator.Refresh();
						for( i = 1; i <= nRandomPaths; i++ )
						{
							float scale = Random( 0.3f, 1.5f );
							arrScales[i].data.transform.SetParams( scale, scale );
							arrScales[i].duration = oPermutator[i - 1] + 1;
						}
						arrScales[i].data.transform.SetParams( 1, 1 );
						arrScales[i].duration = oPermutator[i - 1] + 1;
						oManager.AddScaleTransforms( arrScales, nRandomPaths + 2 );
					}
				}
				void AppearStream( BOOL bPhaseIn )
				{
				}

				// Appear Waves
				void WaveSimple( BOOL bPhaseIn )
				{
					float fX = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, Utils::c_nLeftBound );

					const int c_nPaths = 25;
					Text::Transforms::CTranslateTransforms::TData arrMoves[c_nPaths + 4];
					Text::Transforms::CScaleTransforms::TData arrScales[5];
					arrScales[2].data.transform.SetParams( 2, 2 );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

						double phase = IPP_2PI * (Utils::GetHorCenter(pLetter->m_oPathBoundRect) - m_pText->m_oPathBoundRect.X) / m_pText->m_oPathBoundRect.Width;

						for( int i = 0; i < c_nPaths; i++ )
						{
							double dy = ::sin(IPP_2PI * i / (c_nPaths - 1) + phase) * m_fLineSpacing;
							arrMoves[i].data.transform.SetParams( 0, dy );
						}
						arrMoves[c_nPaths + 0].data.transform.dY = arrMoves[c_nPaths - 1].data.transform.dY;
						arrMoves[c_nPaths + 0].duration = letters.count - nLetter;
						arrMoves[c_nPaths + 1].data.transform.SetParams( m_fLineSpacing * 0.5, m_fLineSpacing * 0.5 );
						arrMoves[c_nPaths + 1].duration = 3;
						arrMoves[c_nPaths + 2].data.transform.SetParams( 0, 0 );
						arrMoves[c_nPaths + 2].duration = 3;
						arrMoves[c_nPaths + 3].data.transform.SetParams( 0, 0 );
						arrMoves[c_nPaths + 3].duration = nLetter;
						
						arrScales[1].duration = c_nPaths + letters.count - nLetter;
						arrScales[2].duration = 3;
						arrScales[3].duration = 3;
						arrScales[4].duration = nLetter + 1;

						if( !pLetter->m_bNotLetter )
						{
							oManager.AddScaleTransforms( arrScales, 5 );
						}
						oManager.AddTranslateTransforms( arrMoves, c_nPaths + 4 );
					}

					double nRepeats = 0;
					for( int i = 1; i < c_nPaths + 4; i++ )
					{
						nRepeats += arrMoves[i].duration;
					}
					
					Text::Transforms::CTranslateTransforms::TData arrTextMoves[3];
					arrTextMoves[0].data.transform.SetParams( fX, 0 );
					arrTextMoves[1].data.transform.SetParams( 0, 0 );
					arrTextMoves[1].duration = c_nPaths;
					arrTextMoves[2].data.transform.SetParams( 0, 0 );
					arrTextMoves[2].duration = nRepeats - c_nPaths + 1;

					// применить трансформы
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTranslateTransforms( arrTextMoves, 3 );
					oTextManager.AddTransparency( 0, 1, FALSE );
				}
				void WaveZoom( BOOL bPhaseIn )
				{
					Text::Transforms::CScaleTransforms::TData arrScales[3];
					arrScales[0].data.transform.SetParams( 1, 1 );
					arrScales[1].data.transform.SetParams( 2.5, 2.5 );
					arrScales[2].data.transform.SetParams( 1, 1 );

					Text::Transforms::CTranslateTransforms::TData arrMoves[3];
					arrMoves[0].data.transform.SetParams( 0, 0 );
					arrMoves[1].data.transform.SetParams( m_fLineSpacing * 0.5, 0 );
					arrMoves[2].data.transform.SetParams( 0, 0 );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

						oManager.AddScaleTransforms( arrScales, 3 );
						oManager.AddTranslateTransforms( arrMoves, 3 );
						oManager.AddFadeTransparency( 0, 1 );
						oManager.SetTimeParams( nLetter, letters.count, 3 );
					}
				}
				void WaveFall( BOOL bPhaseIn )
				{
					float fX = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, Utils::c_nLeftBound );

					const int c_nPaths = 25;
					Text::Transforms::CTranslateTransforms::TData arrMoves[c_nPaths + 4];
					Text::Transforms::CScaleTransforms::TData arrScales[5];
					Text::Transforms::CRotateTransforms::TData arrAngles[5];
					arrScales[2].data.transform.SetParams( 2, 2 );

					arrAngles[2].data.transform.SetParams( 360 );
					arrAngles[3].data.transform.SetParams( 720 );
					arrAngles[4].data.transform.SetParams( 720 );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

						double phase = IPP_2PI * (Utils::GetHorCenter(pLetter->m_oPathBoundRect) - m_pText->m_oPathBoundRect.X) / m_pText->m_oPathBoundRect.Width;

						for( int i = 0; i < c_nPaths; i++ )
						{
							double dy = ::sin(IPP_2PI * i / (c_nPaths - 1) + phase) * m_fLineSpacing;
							arrMoves[i].data.transform.SetParams( 0, dy );
						}
						arrMoves[c_nPaths + 0].data.transform.dY = arrMoves[c_nPaths - 1].data.transform.dY;
						arrMoves[c_nPaths + 0].duration = letters.count - nLetter;
						arrMoves[c_nPaths + 1].data.transform.SetParams( m_fLineSpacing * 0.5, m_fLineSpacing * 0.5 );
						arrMoves[c_nPaths + 1].duration = 3;
						arrMoves[c_nPaths + 2].data.transform.SetParams( 0, 0 );
						arrMoves[c_nPaths + 2].duration = 3;
						arrMoves[c_nPaths + 3].data.transform.SetParams( 0, 0 );
						arrMoves[c_nPaths + 3].duration = nLetter;
						
						arrAngles[1].duration = arrScales[1].duration = c_nPaths + letters.count - nLetter;
						arrAngles[2].duration = arrScales[2].duration = 3;
						arrAngles[3].duration = arrScales[3].duration = 3;
						arrAngles[4].duration = arrScales[4].duration = nLetter + 1;

						if( !pLetter->m_bNotLetter )
						{
							oManager.AddRotateTransforms( arrAngles, 5 );
							oManager.AddScaleTransforms( arrScales, 5 );
						}
						oManager.AddTranslateTransforms( arrMoves, c_nPaths + 4 );
					}

					double nRepeats = 0;
					for( int i = 1; i < c_nPaths + 4; i++ )
					{
						nRepeats += arrMoves[i].duration;
					}
					
					Text::Transforms::CTranslateTransforms::TData arrTextMoves[3];
					arrTextMoves[0].data.transform.SetParams( fX, 0 );
					arrTextMoves[1].data.transform.SetParams( 0, 0 );
					arrTextMoves[1].duration = c_nPaths;
					arrTextMoves[2].data.transform.SetParams( 0, 0 );
					arrTextMoves[2].duration = nRepeats - c_nPaths + 1;

					// применить трансформы
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTranslateTransforms( arrTextMoves, 3 );
					oTextManager.AddTransparency( 0, 1, FALSE );
				}
				void WaveRunShift( BOOL bPhaseIn )
				{
					float fX = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, Utils::c_nLeftBound );

					const int c_nPaths = 25;
					Text::Transforms::CTranslateTransforms::TData arrMoves[c_nPaths + 3];

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

						double phase = IPP_2PI * (Utils::GetHorCenter(pLetter->m_oPathBoundRect) - m_pText->m_oPathBoundRect.X) / m_pText->m_oPathBoundRect.Width;

						for( int i = 0; i < c_nPaths; i++ )
						{
							double dy = ::sin(IPP_2PI * i / (c_nPaths - 1) + phase) * m_fLineSpacing;
							arrMoves[i].data.transform.SetParams( 0, dy );
						}
						arrMoves[c_nPaths + 0].data.transform.dY = arrMoves[c_nPaths - 1].data.transform.dY;
						arrMoves[c_nPaths + 0].duration = letters.count - nLetter;
						arrMoves[c_nPaths + 1].data.transform.SetParams( 0, 0 );
						arrMoves[c_nPaths + 1].duration = 3;
						arrMoves[c_nPaths + 2].data.transform.SetParams( 0, 0 );
						arrMoves[c_nPaths + 2].duration = nLetter;
						
						oManager.AddTranslateTransforms( arrMoves, c_nPaths + 3 );
					}

					double nRepeats = 0;
					for( int i = 1; i < c_nPaths + 3; i++ )
					{
						nRepeats += arrMoves[i].duration;
					}
					
					Text::Transforms::CTranslateTransforms::TData arrTextMoves[3];
					arrTextMoves[0].data.transform.SetParams( fX, 0 );
					arrTextMoves[1].data.transform.SetParams( 0, 0 );
					arrTextMoves[1].duration = c_nPaths;
					arrTextMoves[2].data.transform.SetParams( 0, 0 );
					arrTextMoves[2].duration = nRepeats - c_nPaths;

					// применить трансформы
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTranslateTransforms( arrTextMoves, 3 );
					oTextManager.AddTransparency( 0, 1, FALSE );
				}
				void WaveSwing( BOOL bPhaseIn )
				{
					float fY = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, Utils::c_nTopBound );

					const int c_nPoints = 8;

					Text::Transforms::CTransparencyTransforms::TData arrAlphas[c_nPoints];
					arrAlphas[1].data.transform.SetParams( 0 );
					
					for( int i = 1; i < c_nPoints; i++ )
					{
						arrAlphas[i].data.transform.SetParams( 1 );
						arrAlphas[i].data.interpolation = FALSE;
					}
					
					Text::Transforms::CTranslateTransforms::TData arrMoves[c_nPoints];

					for( int i = 0; i < c_nPoints - 1; i += 2 )
					{
						arrMoves[i].data.transform.SetParams( 0, fY / (1 << (i/2)) );
						arrMoves[i + 1].data.transform.SetParams( 0, m_fLineSpacing * 0.2 / (1 << (i/2)) );
					}
					arrMoves[c_nPoints - 1].data.transform.SetParams( 0, 0 );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

						oManager.AddTransparencyTransforms( arrAlphas, c_nPoints );
						oManager.AddTranslateTransforms( arrMoves, c_nPoints );
						oManager.SetTimeParams( nLetter, letters.count, letters.count );
					}

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1, FALSE );
				}
				void WaveRun( BOOL bPhaseIn )
				{
				}
				void WaveZoomVertical(  BOOL bPhaseIn )
				{
					const int c_nRepeats = 2;

					Text::Transforms::CTransparencyTransforms::TData arrAlphas[2 + c_nRepeats];
					arrAlphas[0].data.transform.SetParams( 0 );
					for( int i = 1; i < c_nRepeats + 2; i++ )
					{
						arrAlphas[i].data.transform.SetParams( 1 );
						arrAlphas[i].data.interpolation = FALSE;
					}
		
					Text::Transforms::CScaleTransforms::TData arrScales[2 + c_nRepeats];
					arrScales[0].data.transform.SetParams( 1, 0 );
					for( int i = 1; i < c_nRepeats + 1; i += 2 )
					{
						arrScales[i].data.transform.SetParams( 1, 3 );
						arrScales[i + 1].data.transform.SetParams( 1, 0.5 );
					}
					arrScales[c_nRepeats + 1].data.transform.SetParams( 1, 1 );
					
					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

						oManager.AddScaleTransforms( arrScales, c_nRepeats + 2 );
						oManager.AddTransparencyTransforms( arrAlphas, c_nRepeats + 2 );
						oManager.SetTimeParams( nLetter, letters.count, letters.count );
					}
				}
				void WaveShift( BOOL bPhaseIn )
				{
					const int c_nPhase1 = 1;
					const int c_nPhase2 = 7;
					const int c_nPhase3 = 9;
					const int c_nPoints = c_nPhase3;

					Text::Transforms::CTransparencyTransforms::TData arrAlphas[c_nPoints];
					Text::Transforms::CScaleTransforms::TData        arrScales[c_nPoints];
					Text::Transforms::CTranslateTransforms::TData    arrMoves [c_nPoints];

					double dTop    = m_fLineSpacing * -0.7;
					double dBottom = m_fLineSpacing * 1.4;
					double dLeft   = m_fLineSpacing * 0.25;
					double dRight  = m_fLineSpacing * 0.75;
					int i = 0;

					// phase 1
					arrAlphas[0].data.transform.SetParams( 0 );
					arrScales[0].data.transform.SetParams( 0, 0 );
					arrMoves [0].data.transform.SetParams( dRight, dBottom );
					// phase 2
					for( i = c_nPhase1; i < c_nPhase2; i++ )
					{
						arrAlphas[i].data.transform.SetParams( 1 );
						
						double x = double(i - c_nPhase1 + 1)/(c_nPhase2 - c_nPhase1);
						double scale = x * (2 - x) * 1.4;
						arrScales[i].data.transform.SetParams( scale, scale );
						
						double dX = dRight - x * (dRight - dLeft);
						double dY = dBottom - x * (2 - x) * (dBottom - dTop);
						arrMoves[i].data.transform.SetParams( dX, dY );
					}
					// phase 3
					for( i = c_nPhase2; i < c_nPhase3; i++ )
					{
						arrAlphas[i].data.transform.SetParams( 1 );
						
						double x = double(i - c_nPhase2 + 1)/(c_nPhase3 - c_nPhase2);
						double scale = 1.4 - x * x * (1.4 - 1) ;
						arrScales[i].data.transform.SetParams( scale, scale );

						double dX = dLeft * (1 - x);
						double dY = dTop * (1 - x * x);
						arrMoves[i].data.transform.SetParams( dX, dY );
					}

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

						oManager.AddTransparencyTransforms( arrAlphas, c_nPoints );
						oManager.AddScaleTransforms( arrScales, c_nPoints );
						oManager.AddTranslateTransforms( arrMoves, c_nPoints );
						oManager.SetTimeParams( nLetter, letters.count, 5 );
					}
				}
				void WavePulse( BOOL bPhaseIn )
				{
					const int c_nRepeats = 6;

					Text::Transforms::CTransparencyTransforms::TData arrAlphas[2 + c_nRepeats];
					arrAlphas[0].data.transform.SetParams( 0 );
					for( int i = 1; i < c_nRepeats + 2; i++ )
					{
						arrAlphas[i].data.transform.SetParams( 1 );
						arrAlphas[i].data.interpolation = FALSE;
					}
		
					Text::Transforms::CScaleTransforms::TData arrScales[2 + c_nRepeats];
					for( int i = 0; i < c_nRepeats + 1; i += 2 )
					{
						arrScales[i + 0].data.transform.SetParams( 1, 0 );
						arrScales[i + 1].data.transform.SetParams( 1, 3 );
					}
					arrScales[c_nRepeats + 1].data.transform.SetParams( 1, 1 );
					
					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

						oManager.AddScaleTransforms( arrScales, c_nRepeats + 2 );
						oManager.AddTransparencyTransforms( arrAlphas, c_nRepeats + 2 );
						oManager.SetTimeParams( nLetter, letters.count, letters.count * 3 );
					}
				}

				// New
				void Ejection( BOOL bPhaseIn )
				{
					Text::Transforms::CTranslateTransforms::TData arrMoves[3];

					float fY = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, Utils::c_nBottomBound );

					arrMoves[0].data.transform.SetParams( 0, fY );
					arrMoves[1].data.transform.SetParams( 0, -m_fLineSpacing * 0.75 );
					arrMoves[2].data.transform.SetParams( 0, 0 );

					arrMoves[1].duration = fY * 100;
					arrMoves[2].duration = m_fLineSpacing * 100;

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTranslateTransforms( arrMoves, 3 );
					oTextManager.AddTransparency( 0, 1 );
				}
				void Ellipse( BOOL bPhaseIn )
				{
					const int c_nPoints = 5;

					Text::Transforms::CTransparencyTransforms::TData arrAlphas[c_nPoints];
					Text::Transforms::CScaleTransforms::TData        arrScales[c_nPoints];
					Text::Transforms::CRotateAtTransforms::TData     arrAngles[c_nPoints];

					arrAlphas[0].data.transform.SetParams( 0 );
					arrScales[0].data.transform.SetParams( 0, 0 );
					arrAngles[0].data.transform.SetParams( 0, 0, m_fLineSpacing );
					for( int i = 1; i < c_nPoints; i++ )
					{
						arrAlphas[i].data.transform.SetParams( 1 );
						arrScales[i].data.transform.SetParams( 1, 1 );
						arrAngles[i].data.transform.SetParams( i * 90,  0, m_fLineSpacing );
					}

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
					
						oManager.AddTransparencyTransforms( arrAlphas, c_nPoints );
						oManager.AddScaleTransforms( arrScales, c_nPoints );
						oManager.AddRotateAtTransforms( arrAngles, c_nPoints );
						oManager.SetTimeParams( nLetter, letters.count, letters.count );
					}
				}
				void Wheel( BOOL bPhaseIn )
				{
					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::Transforms::CTransformManager& oManager = letters[nLetter].GetTransformManager( bPhaseIn );
					
						oManager.AddTransparency( 0, 1 );
						oManager.AddScale( 0, 1 );
						oManager.AddRotate( 0, 720 );
						oManager.SetTimeParams( nLetter, letters.count, 4 );
					}
				}
				void Miscarry( BOOL bPhaseIn )
				{
					Text::Transforms::CTranslateTransforms::TData arrMoves[3];
					Text::Transforms::CRotateTransforms::TData    arrAngles[3];
					Text::Transforms::CScaleTransforms::TData     arrScales[3]; // для линий перечёркивания

					arrAngles[1].data.transform.SetParams( 90 );

					arrScales[0].data.transform.SetParams( 1, 1 );
					arrScales[1].data.transform.SetParams( 1, 1 );
					arrScales[2].data.transform.SetParams( 1, 1 );

					CRefLetters letters( m_pText->m_arrLetters );
					CRefLines   lines( m_pText->m_arrLines );
					for( int nLine = 0; nLine < lines.count; nLine++ )
					{
						CTextEntity::CTextLine* pLine   = lines.ptr   + nLine;
						CLetterEntity*          pLetter = letters.ptr + pLine->m_nFirstLetter;
						
						float fLeft  = Utils::GetOffsetRect( m_oLayoutRect, pLetter->m_oPathBoundRect, Utils::c_nLeftBound );
						float fRight = Utils::GetOffsetRect( m_pText->m_oPathBoundRect, pLetter->m_oPathBoundRect, Utils::c_nRightBound ) - pLetter->m_oPathBoundRect.Width;
						arrMoves[0].data.transform.SetParams( fLeft,  0 );
						arrMoves[1].data.transform.SetParams( fRight, 0 );

						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
						
						if( pLetter->m_bNotLetter )
						{
							float fScale = m_fLineSpacing * 0.5f / pLetter->m_oPathBoundRect.Width;
							arrScales[1].data.transform.SetParams( fScale, 1 );
							oManager.AddScaleTransforms( arrScales, 3 );
							
							arrMoves[1].data.transform.dX += pLetter->m_oPathBoundRect.Width * 0.5f - pLetter->m_oPathBoundRect.Height;
						}
						oManager.AddRotateTransforms( arrAngles, 3 );
						oManager.AddTranslateTransforms( arrMoves, 3 );
						oManager.AddTransparency( 0, 1, FALSE );

						for( int nLetter = 1; nLetter < pLine->m_nLength; nLetter++ )
						{
							++pLetter;

							fRight = Utils::GetOffsetRect( m_pText->m_oPathBoundRect, pLetter->m_oPathBoundRect, Utils::c_nRightBound ) - pLetter->m_oPathBoundRect.Width;

							Text::Transforms::CTransformManager& oLocalManager = pLetter->GetTransformManager( bPhaseIn );

							if( pLetter->m_bNotLetter )
							{
								float fScale = m_fLineSpacing * 0.5f / pLetter->m_oPathBoundRect.Width;
								oLocalManager.AddScale( fScale, 1, 1, 1 );
								
								fRight += pLetter->m_oPathBoundRect.Width * 0.5f - pLetter->m_oPathBoundRect.Height;
							}
							oLocalManager.AddRotate( 90, 0 );
							oLocalManager.AddTranslate( fRight, 0, 0, 0 );
							oLocalManager.AddTransparency( 0, 1 );
							oLocalManager.SetTimeParams( 0.5, 0.5 );
						}
					}
				}
				void Rotation( BOOL bPhaseIn )
				{
					Text::Transforms::CScaleTransforms::TData arrScales[3];
					arrScales[0].data.transform.SetParams(  1, 1 );
					arrScales[1].data.transform.SetParams( -1, 1 );
					arrScales[2].data.transform.SetParams(  1, 1 );
					
					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
					
						if( !pLetter->m_bNotLetter )
						{
							oManager.AddScaleTransforms( arrScales, 3 );
						}
						oManager.AddTransparency( 0, 1 );
						oManager.SetTimeParams( nLetter, letters.count, 4 );
					}
				}
				void Increase( BOOL bPhaseIn )
				{
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );
					
					Text::Transforms::CScaleTransforms::TData arrScales[3];
					Text::Transforms::CTranslateTransforms::TData arrMoves[3];

					arrScales[0].data.transform.SetParams( 0, 0 );
					arrScales[1].data.transform.SetParams( 3, 3 );
					arrScales[2].data.transform.SetParams( 1, 1 );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
					
						float scale = !pLetter->m_bNotLetter ? 3 : (2 * m_fLineSpacing / pLetter->m_oPathBoundRect.Width);
						arrScales[1].data.transform.SetParams( scale, 3 );

						float fx = Utils::GetHorOffsetCenters( pLetter->m_oPathBoundRect, m_pText->m_oPathBoundRect );
						float fy = Utils::GetVerOffsetCenters( pLetter->m_oPathBoundRect, m_pText->m_oPathBoundRect );
						arrMoves[0].data.transform.SetParams( fx, fy );
						arrMoves[1].data.transform.SetParams( fx, fy );
						
						oManager.AddScaleTransforms( arrScales, 3 );
						oManager.AddTranslateTransforms( arrMoves, 3 );
					}
				}
				void TwoString( BOOL bPhaseIn )
				{
				}
				void Tumbling( BOOL bPhaseIn )
				{
					Text::Transforms::CScaleTransforms::TData arrScales[3];

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
					
						float fy = Utils::GetOffsetRect( m_oLayoutRect, pLetter->m_oPathBoundRect, Utils::c_nTopBound );
						float fx = m_fLineSpacing / 3;
						
						arrScales[0].data.transform.dScaleX = !pLetter->m_bNotLetter ?  1 : 0;
						arrScales[1].data.transform.dScaleX = !pLetter->m_bNotLetter ? -1 : 0.5;

						oManager.AddScaleTransforms( arrScales, 3 );
						oManager.AddRotate( 360, 0 );
						oManager.AddTranslate( fx, fy, 0, 0 );
						oManager.AddTransparency( 0, 1 );
						oManager.SetTimeParams( nLetter, letters.count, 5 );
					}
				}
				void Leave( BOOL bPhaseIn )
				{
					Text::Transforms::CScaleTransforms::TData arrScales[3];
					arrScales[0].data.transform.SetParams( 0, 0 );
					arrScales[1].data.transform.SetParams( 1, 1 );
					arrScales[2].data.transform.SetParams( 1, 1 );
					arrScales[2].data.interpolation = FALSE;
					arrScales[2].duration = 4;

					CRefLines lines( m_pText->m_arrLines );
					for( ; lines.count > 0; --lines.count, ++lines.ptr )
					{
						float offset = Utils::GetOffsetRect( m_pText->m_oPathBoundRect, lines.ptr->m_oPathBoundRect, Utils::c_nLeftBound );

						CRefLetters letters( m_pText->m_arrLetters, lines.ptr->m_nFirstLetter, lines.ptr->m_nLength );
						for( int nLetter = 0; nLetter < letters.count; nLetter++ )
						{
							Text::CLetterEntity* pLetter = letters.ptr + nLetter;

							float fx = Utils::GetOffsetRect( m_pText->m_oPathBoundRect, pLetter->m_oPathBoundRect, Utils::c_nLeftBound );
							double dTimeOffset = abs((offset - fx) / offset);

							Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
							if( pLetter->m_bNotLetter )
							{
								oManager.AddScale( 0, 1, 1, 1 );
								fx += pLetter->m_oPathBoundRect.Width / 2;
							}
							else
							{
								oManager.AddScaleTransforms( arrScales, 3 );
							}
							oManager.AddTranslate( fx, 0, 0, 0 );
							oManager.AddFadeTransparency( 0, 1, 1, 6 );
							oManager.SetTimeParams( dTimeOffset, 1 - dTimeOffset );
						}
					}
				}
				void Unfold( BOOL bPhaseIn )
				{
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1, FALSE );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
					
						float fx = Utils::GetOffsetRect( m_oLayoutRect, pLetter->m_oPathBoundRect, Utils::c_nLeftBound );
						float fy = Utils::GetOffsetRect( m_oLayoutRect, pLetter->m_oPathBoundRect, Utils::c_nTopBound );

						if( pLetter->m_bNotLetter )
						{
							oManager.AddScale( 0, 1, 1, 1 );
							fx += pLetter->m_oPathBoundRect.Width / 2;
						}

						oManager.AddTranslate( fx, fy, 0, 0 );
						oManager.SetTimeParams( 0, (double(nLetter + 1) / letters.count + 1) / 2 );
					}
				}
				void Centrifuge( BOOL bPhaseIn )
				{
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );
					oTextManager.AddScale( 0, 1 );
					oTextManager.AddRotate( -720, 0 );
				}
				void HorizontalRotation( BOOL bPhaseIn )
				{
					Text::Transforms::CScaleTransforms::TData arrScales[6];
					arrScales[0].data.transform.SetParams(  0, 1 );
					arrScales[1].data.transform.SetParams(  1, 1 );
					arrScales[2].data.transform.SetParams(  0, 1 );
					arrScales[3].data.transform.SetParams( -1, 1 );
					arrScales[4].data.transform.SetParams(  0, 1 );
					arrScales[5].data.transform.SetParams(  1, 1 );

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddScaleTransforms( arrScales, 6 );
					oTextManager.AddFadeTransparency( 0, 1, 1, 6 );
				}
				void HorizontalRotation3D( BOOL bPhaseIn )
				{
					HorizontalRotation( bPhaseIn );

					Text::Transforms::CScaleTransforms::TData arrScales[6];

					CRefLines lines( m_pText->m_arrLines );
					for( ; lines.count > 0; --lines.count, ++lines.ptr )
					{
						CRefLetters letters( m_pText->m_arrLetters, lines.ptr->m_nFirstLetter, lines.ptr->m_nLength );
						for( int nLetter = 0; nLetter < letters.count; nLetter++ )
						{
							Text::CLetterEntity* pLetter = letters.ptr + nLetter;
							Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
							
							if( pLetter->m_bNotLetter )
								continue;

							double dScaleY = (pLetter->m_oPathBoundRect.X - m_pText->m_oPathBoundRect.X) / m_pText->m_oPathBoundRect.Width;
							arrScales[0].data.transform.SetParams( dScaleY, IPP_PI * (1 - dScaleY) );
							arrScales[2].data.transform.SetParams( 1      , IPP_PI * dScaleY );
							arrScales[4].data.transform.SetParams( dScaleY, IPP_PI * (1 - dScaleY) );

							oManager.AddScaleTransforms( arrScales, 6 );
						}
					}
				}
				void VerticalRotation( BOOL bPhaseIn )
				{
					Text::Transforms::CScaleTransforms::TData arrScales[6];
					arrScales[0].data.transform.SetParams( 1,  0 );
					arrScales[1].data.transform.SetParams( 1,  1 );
					arrScales[2].data.transform.SetParams( 1,  0 );
					arrScales[3].data.transform.SetParams( 1, -1 );
					arrScales[4].data.transform.SetParams( 1,  0 );
					arrScales[5].data.transform.SetParams( 1,  1 );

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddScaleTransforms( arrScales, 6 );
					oTextManager.AddFadeTransparency( 0, 1, 1, 6 );
				}
				void CheChe( BOOL bPhaseIn )
				{
					const int c_nCycles = 5;
					const int c_nPoints = c_nCycles * 2;
					Text::Transforms::CScaleTransforms::TData        arrScales[c_nPoints];
					Text::Transforms::CTransparencyTransforms::TData arrAlphas[c_nPoints];
					Text::Transforms::CRotateTransforms::TData       arrAngles[c_nPoints];
					Text::Transforms::CTranslateTransforms::TData    arrMoves [c_nPoints];

					for( int i = 0; i < c_nPoints; i += 2 )
					{
						arrScales[i].data.transform.SetParams( 0, 0 );
						arrAlphas[i].data.transform.SetParams( double(i/2) / c_nPoints );
						arrAngles[i].data.transform.SetParams( 60 );
						arrMoves [i].data.transform.SetParams( 0, m_fLineSpacing * -0.5 );
						
						arrScales[i + 1].data.transform.SetParams( 1, 1 );
						arrAlphas[i + 1].data.transform.SetParams( 1 );
						arrAngles[i + 1].data.transform.SetParams( 0 );
						arrMoves [i + 1].data.transform.SetParams( 0, 0 );
					}

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

						if( !pLetter->m_bNotLetter )
						{
							oManager.AddRotateTransforms( arrAngles, c_nPoints );
							oManager.AddScaleTransforms( arrScales, c_nPoints );
							oManager.AddTranslateTransforms( arrMoves, c_nPoints );
						}
						oManager.AddTransparencyTransforms( arrAlphas, c_nPoints );
						oManager.SetTimeParams( nLetter, letters.count, letters.count * 3 );
					}
				}

				// New2
				void Boomerang( BOOL bPhaseIn )
				{
					float dx = m_pText->m_oPathBoundRect.Width / 2;

					Text::Transforms::CTranslateTransforms::TData arrMoves[3];
					arrMoves[0].data.transform.SetParams( dx, -dx );
					arrMoves[1].data.transform.SetParams( dx/2, dx );
					arrMoves[2].data.transform.SetParams( 0, 0 );

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddScale( 0.4, 1, 1, 1 );
					oTextManager.AddRotate( -90, 0 );
					oTextManager.AddTranslateTransforms( arrMoves, 3 );
					oTextManager.AddFadeTransparency( 0, 1, 1, 5 );
				}
				void Flash( BOOL bPhaseIn )
				{
					double dx = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, Utils::c_nLeftBound );
					double dMin = 5 / m_fLineSpacing;
					double dMax = 1.25;

					Text::Transforms::CTranslateTransforms::TData arrMoves[5];
					Text::Transforms::CScaleTransforms::TData arrScales[5];

					arrMoves [0].data.transform.SetParams( dx,   0 );
					arrScales[0].data.transform.SetParams( 1,    dMin );
					arrScales[1].data.transform.SetParams( dMax, dMin );
					arrScales[2].data.transform.SetParams( 1,    1 );
					arrScales[3].data.transform.SetParams( dMax, dMax );
					arrScales[4].data.transform.SetParams( 1,    1 );

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTranslateTransforms( arrMoves, 5 );
					oTextManager.AddScaleTransforms( arrScales, 5 );
					oTextManager.AddFadeTransparency( 0, 1, 1, 8 );
				}
				void Flight( BOOL bPhaseIn )
				{
					float dx = m_pText->m_oPathBoundRect.Width / 2;

					Text::Transforms::CRotateAtTransforms::TData arrAngles[3];
					Text::Transforms::CTranslateTransforms::TData arrMoves[3];

					arrAngles[0].data.transform.SetParams( -90, -dx, 0 );
					arrAngles[1].data.transform.SetParams( 0, -dx, 0 );
					arrAngles[2].data.transform.SetParams( 0, 0, 0 );

					arrMoves[0].data.transform.SetParams( 2*dx, -dx/2 );
					arrMoves[1].data.transform.SetParams( -dx/2, dx/2 );
					arrMoves[2].data.transform.SetParams( 0, 0 );

					arrAngles[1].duration = arrMoves[1].duration = 2;

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddRotateAtTransforms( arrAngles, 3 );
					oTextManager.AddTranslateTransforms( arrMoves, 3 );
					oTextManager.AddFadeTransparency( 0, 1, 1, 5 );
				}
				void Jump( BOOL bPhaseIn )
				{
					const int c_nPoints = 7;

					Text::Transforms::CTranslateTransforms::TData arrMoves[c_nPoints];
					Text::Transforms::CScaleTransforms::TData arrScales[c_nPoints];

					float dx = m_oLayoutRect.X - m_pText->m_oPathBoundRect.X;
					float dy = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, Utils::c_nTopBound );

					for( int i = 0; i < c_nPoints; i++ )
					{
						double factor = double(c_nPoints - 1 - i) / (c_nPoints - 1);
						arrMoves [i].data.transform.SetParams( dx * factor, dy / (1<<i) * ((i + 1)%2) );
						arrScales[i].data.transform.SetParams( 1, 0.5 * ((i + 1)%2 + 1) );
					}

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTranslateTransforms( arrMoves, c_nPoints );
					oTextManager.AddScaleTransforms( arrScales, c_nPoints );
					oTextManager.AddTransparency( 0, 1, FALSE );
				}
				void Compress( BOOL bPhaseIn )
				{
					double scale = 3;

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddFadeTransparency( 0, 1 );

					CRefLines lines( m_pText->m_arrLines );
					for( ; lines.count > 0; --lines.count, ++lines.ptr )
					{
						CRefLetters letters( m_pText->m_arrLetters, lines.ptr->m_nFirstLetter, lines.ptr->m_nLength );
						for( ; letters.count > 0; --letters.count, ++letters.ptr )
						{
							double dx = (1 - scale) * (letters.ptr->m_oPathBoundRect.Width / 2 + lines.ptr->m_oPathBoundRect.GetRight() - letters.ptr->m_oPathBoundRect.GetRight());

							Text::Transforms::CTransformManager& oManager = letters.ptr->GetTransformManager( bPhaseIn );
							oManager.AddScale( scale, 1, 1, 1 );
							oManager.AddTranslate( dx, 0, 0, 0 );
						}
					}
				}
				void Motion( BOOL bPhaseIn )
				{
					double offset = m_pText->m_oPathBoundRect.GetRight() - m_oLayoutRect.X;

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
						
						double dx = Utils::GetOffsetRect( m_oLayoutRect, pLetter->m_oPathBoundRect, Utils::c_nLeftBound );

						if( pLetter->m_bNotLetter )
						{
							oManager.AddScale( abs(dx / offset), 1, 1, 1 );
						}
						oManager.AddTranslate( dx, 0, 0, 0 );
						oManager.SetTimeParams( 0, abs(dx / offset) );
						oManager.AddTransparency( 0, 1 );
					}
				}
				void Manifestation( BOOL bPhaseIn )
				{
					double dy = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, Utils::c_nTopBound );

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );
					oTextManager.AddScale( 0, 1 );
					oTextManager.AddTranslate( 0, dy, 0, 0 );
				}
				void Shot( BOOL bPhaseIn )
				{
					Text::Transforms::CRotateTransforms::TData    arrAngles[3];
					Text::Transforms::CTranslateTransforms::TData arrMoves[3];
					Text::Transforms::CScaleTransforms::TData     arrScales[3];

					double dy = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, Utils::c_nTopBound );
					double dx = m_pText->m_oPathBoundRect.Y - m_oLayoutRect.Y;

					arrAngles[0].data.transform.SetParams( 90 );
					arrAngles[1].data.transform.SetParams( 0 );
					arrAngles[2].data.transform.SetParams( 0 );

					arrMoves[0].data.transform.SetParams( dx, dy );
					arrMoves[1].data.transform.SetParams( 0, m_fLineSpacing );
					arrMoves[2].data.transform.SetParams( 0, 0 );

					arrScales[0].data.transform.SetParams( 1,   1 );
					arrScales[1].data.transform.SetParams( 0.1, 1 );
					arrScales[2].data.transform.SetParams( 1,   1 );

					arrAngles[1].duration = arrMoves[1].duration = arrScales[1].duration = 5;

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
						
						arrScales[0].data.transform.dScaleX = !pLetter->m_bNotLetter ? 1 : m_fLineSpacing / pLetter->m_oPathBoundRect.Width;
						
						oManager.AddScaleTransforms( arrScales, 3 );
						oManager.AddRotateTransforms( arrAngles, 3 );
					}

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTranslateTransforms( arrMoves, 3 );
					oTextManager.AddFadeTransparency( 0, 1 );
				}
				void Reduction( BOOL bPhaseIn )
				{
					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::Transforms::CTransformManager& oManager = letters[nLetter].GetTransformManager( bPhaseIn );
						
						oManager.AddScale( 0.5, 10, 1, 1 );
						oManager.AddFadeTransparency( 0, 1, 3, 2 );
						oManager.SetTimeParams( nLetter, letters.count, letters.count * 0.33 );
					}
				}
				void VHRotation( BOOL bPhaseIn )
				{
					Text::Transforms::CScaleTransforms::TData arrScalesX[7];
					arrScalesX[0].data.transform.SetParams( -1,  1 );
					arrScalesX[1].data.transform.SetParams(  0,  1 );
					arrScalesX[2].data.transform.SetParams(  1,  1 );
					arrScalesX[3].data.transform.SetParams(  0,  1 );
					arrScalesX[4].data.transform.SetParams( -1,  1 );
					arrScalesX[5].data.transform.SetParams(  0,  1 );
					arrScalesX[6].data.transform.SetParams(  1,  1 );

					Text::Transforms::CScaleTransforms::TData arrScalesY[7];
					arrScalesY[0].data.transform.SetParams(  0.5, -2 );
					arrScalesY[1].data.transform.SetParams(  1,    0 );
					arrScalesY[2].data.transform.SetParams(  0.5,  2 );
					arrScalesY[3].data.transform.SetParams(  1,    0 );
					arrScalesY[4].data.transform.SetParams(  0.5, -2 );
					arrScalesY[5].data.transform.SetParams(  1,    0 );
					arrScalesY[6].data.transform.SetParams(  1,    1 );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::Transforms::CTransformManager& oManager = letters[nLetter].GetTransformManager( bPhaseIn );
						oManager.AddScaleTransforms( (nLetter % 2) ? arrScalesX : arrScalesY, 7 );
					}

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );
				}
				void Rotate3D( BOOL bPhaseIn )
				{
					Text::Transforms::CScaleTransforms::TData     arrScales[8];
					Text::Transforms::CTranslateTransforms::TData arrMoves[8];

					double dy = m_fLineSpacing * 1.5;

					arrScales[0].data.transform.SetParams( 0.75,  0   );
					arrScales[1].data.transform.SetParams( 0.50, -0.5 );
					arrScales[2].data.transform.SetParams( 0.75,  0   );
					arrScales[3].data.transform.SetParams( 1   ,  1   );
					arrScales[4].data.transform.SetParams( 0.75,  0   );
					arrScales[5].data.transform.SetParams( 0.50, -0.5 );
					arrScales[6].data.transform.SetParams( 0.75,  0   );
					arrScales[7].data.transform.SetParams( 1   ,  1   );

					arrMoves[0].data.transform.SetParams( 0, -dy);
					arrMoves[1].data.transform.SetParams( 0,  0 );
					arrMoves[2].data.transform.SetParams( 0,  dy);
					arrMoves[3].data.transform.SetParams( 0,  0 );
					arrMoves[4].data.transform.SetParams( 0, -dy);
					arrMoves[5].data.transform.SetParams( 0,  0 );
					arrMoves[6].data.transform.SetParams( 0,  dy);
					arrMoves[7].data.transform.SetParams( 0,  0 );

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddScaleTransforms( arrScales, 8 );
					oTextManager.AddTranslateTransforms( arrMoves, 8 );
					oTextManager.AddTransparency( 0, 1 );
				}
				void Spiral( BOOL bPhaseIn )
				{
					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					double dr = m_fLineSpacing * -2;
					oTextManager.AddTranslate( dr, dr, 0, 0 );
					oTextManager.AddRotate( -720, 0 );
					oTextManager.AddTransparency( 0, 1 );
				}
				void Flag( BOOL bPhaseIn )
				{
				}
				void Perpendicular( BOOL bPhaseIn )
				{
					Text::Transforms::CRotateTransforms::TData    arrAngles[4];
					Text::Transforms::CTranslateTransforms::TData arrMoves [4];

					arrAngles[0].data.transform.SetParams( -90 );
					arrAngles[1].data.transform.SetParams( -90 );
					arrAngles[2].data.transform.SetParams( 0 );
					arrAngles[3].data.transform.SetParams( 0 );

					arrAngles[1].data.interpolation = FALSE;
					arrAngles[3].data.interpolation = FALSE;

					CRefLines lines( m_pText->m_arrLines );
					for( ; lines.count > 0; --lines.count, ++lines.ptr )
					{
						double offset = lines.ptr->m_oPathBoundRect.Height / 2;
						
						CRefLetters letters( m_pText->m_arrLetters, lines.ptr->m_nFirstLetter, lines.ptr->m_nLength );
						for( ; letters.count > 0; --letters.count, ++letters.ptr )
						{
							double dx = lines.ptr->m_oPathBoundRect.X     - letters.ptr->m_oPathCenter.X;
							double dy = lines.ptr->m_oPathBoundRect.Width + dx + m_fLineSpacing;

							arrMoves[0].data.transform.SetParams( dx - offset, dy + offset );
							arrMoves[1].data.transform.SetParams( dx - offset, offset );
							arrMoves[2].data.transform.SetParams( dx, 0 );
							arrMoves[3].data.transform.SetParams(  0, 0 );

							arrAngles[1].duration = arrMoves[1].duration = abs(dy);
							arrAngles[2].duration = arrMoves[2].duration = offset * IPP_SQRT2;
							arrAngles[3].duration = arrMoves[3].duration = abs(dx);

							Text::Transforms::CTransformManager& oManager = letters.ptr->GetTransformManager( bPhaseIn );
							oManager.AddRotateTransforms( arrAngles, 4 );
							oManager.AddTranslateTransforms( arrMoves, 4 );
						}
					}

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddFadeTransparency( 0, 1, 1, 5 );
				}
				void Clock( BOOL bPhaseIn )
				{
					Text::Transforms::CTranslateTransforms::TData arrMoves[3];
					Text::Transforms::CScaleTransforms::TData arrScales[3];

					arrScales[0].data.transform.SetParams( 1.5, 1.5 );
					arrScales[1].data.transform.SetParams( 1.5, 1.5 );
					arrScales[2].data.transform.SetParams( 1.0, 1.0 );
					
					arrMoves[0].data.interpolation = arrScales[1].data.interpolation = FALSE;
					arrMoves[1].duration = 6; arrScales[1].duration = 5;
					arrMoves[2].duration = 1; arrScales[2].duration = 1;

					Gdiplus::PointF distance;

					CRefLines lines( m_pText->m_arrLines );
					for( ; lines.count > 0; --lines.count, ++lines.ptr )
					{
						CRefLetters letters( m_pText->m_arrLetters, lines.ptr->m_nFirstLetter, lines.ptr->m_nLength );

						int nCenter = letters.count / 3;
						CLetterEntity* pCenterLetter = letters.ptr + nCenter;
						
						for( int nLetter = 0; nLetter < nCenter; nLetter++ )
						{
							arrMoves[0].data.transform.SetParams( m_fLineSpacing * -0.5, 0 );
							arrMoves[1].data.transform.SetParams( m_fLineSpacing * -0.5, 0 );

							distance = pCenterLetter->m_oPathCenter - letters[nLetter].m_oPathCenter;

							Text::Transforms::CTransformManager& oManager = letters[nLetter].GetTransformManager( bPhaseIn );
							oManager.AddTranslateTransforms( arrMoves, 3 );
							oManager.AddRotateAt( -300, 0, distance.X, distance.Y );
						}
						for( int nLetter = nCenter + 1; nLetter < letters.count; nLetter++ )
						{
							arrMoves[0].data.transform.SetParams( m_fLineSpacing * 0.5, 0 );
							arrMoves[1].data.transform.SetParams( m_fLineSpacing * 0.5, 0 );

							distance = pCenterLetter->m_oPathCenter - letters[nLetter].m_oPathCenter;

							Text::Transforms::CTransformManager& oManager = letters[nLetter].GetTransformManager( bPhaseIn );
							oManager.AddTranslateTransforms( arrMoves, 3 );
							oManager.AddRotateAt( -1000, 0, distance.X, distance.Y );
						}

						arrScales[0].data.transform.dScaleX = \
						arrScales[1].data.transform.dScaleX = (!pCenterLetter->m_bNotLetter ? 1.5 : 1 );

						Text::Transforms::CTransformManager& oManager = pCenterLetter->GetTransformManager( bPhaseIn );
						oManager.AddScaleTransforms( arrScales, 3 );
					}

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddFadeTransparency( 0, 1, 1, 5 );
				}

				// New3
				void Galaxy( BOOL bPhaseIn )
				{
					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
					
						double angle = abs(pLetter->m_oOrigin.X / m_pText->m_oPathBoundRect.Width * 360);

						oManager.AddTranslate( pLetter->m_oOrigin.X * 3, pLetter->m_oOrigin.Y * 3, 0, 0 );
						oManager.AddRotate( angle, 0 );
						oManager.AddTranslate( -pLetter->m_oOrigin.X, -pLetter->m_oOrigin.Y, 0, 0 );
					}

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );
					oTextManager.AddRotate( 90, 0 );
					oTextManager.AddScale( 0, 1 );
				}
				void WaveRotate3D( BOOL bPhaseIn )
				{
					Text::Transforms::CScaleAtTransforms::TData arrScales[6];

					float step = m_fLineSpacing / 2;

					arrScales[0].data.transform.SetParams( 0.6,  -0.5,  0, step * -2 );
					arrScales[1].data.transform.SetParams( 0.6,  -0.6,  0, 0         );
					arrScales[2].data.transform.SetParams( 0.65, -0.3,  0, step * 2  );
					arrScales[3].data.transform.SetParams( 0.8,   0.01, 0, step * 4  );
					arrScales[4].data.transform.SetParams( 0.95,  0.6,  0, step * 2  );
					arrScales[5].data.transform.SetParams( 1,     1,    0, 0         );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
					
						for( int i = 0 ; i < 6; i++ )
						{
							arrScales[i].data.transform.dX = pLetter->m_oOrigin.X;
						}

						oManager.AddScaleAtTransforms( arrScales, 6 );
						oManager.AddFadeTransparency( 0, 1, 1, 1 );
						oManager.SetTimeParams( nLetter, letters.count, letters.count * 0.3 );
					}
				}
				void RandomJump( BOOL bPhaseIn )
				{
					const int c_nPoints = 10;
					Text::Transforms::CTranslateTransforms::TData arrMoves [c_nPoints];
					Text::Transforms::CScaleTransforms::TData     arrScales[c_nPoints];
					Text::Transforms::CRotateTransforms::TData    arrAngles[c_nPoints];

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

						if( !pLetter->m_bNotLetter )
						{
							int nStartIndex = rand() % (c_nPoints - 5);
							for( int i = nStartIndex; i < c_nPoints - 1; i++ )
							{
								double dx = 0; 
								double dy = (rand() % 2) * m_fLineSpacing * Random( -1.5f, -0.3f );
								arrMoves [i].data.transform.SetParams( dx, dy );

								dx = Random( 0.0f, 2.0f );
								dy = Random( 0.0f, 2.0f );
								arrScales[i].data.transform.SetParams( dx, dy );
								arrAngles[i].data.transform.SetParams( Random( -30.0f, 30.0f ) );
							}
							oManager.AddTranslateTransforms( arrMoves  + nStartIndex, c_nPoints - nStartIndex );
							oManager.AddScaleTransforms    ( arrScales + nStartIndex, c_nPoints - nStartIndex );
							oManager.AddRotateTransforms   ( arrAngles + nStartIndex, c_nPoints - nStartIndex );
						}
					}

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );
				}
				void Galaxy2( BOOL bPhaseIn )
				{
					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
					
						double angle = abs(pLetter->m_oOrigin.X) / m_pText->m_oPathBoundRect.Width * 500;

						oManager.AddScale( 1, 3, 1, 1 );
						oManager.AddShear( -1, 0, 0, 0 );
						oManager.AddTranslate( pLetter->m_oOrigin.X * 3, pLetter->m_oOrigin.Y * 3, 0, 0 );
						oManager.AddScale( 0.4, 1 );
						oManager.AddRotate( angle, 0 );
						oManager.AddTranslate( -pLetter->m_oOrigin.X, -pLetter->m_oOrigin.Y, 0, 0 );
					}

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddTransparency( 0, 1 );
					oTextManager.AddRotate( 360, 0 );
				}
				void Roll( BOOL bPhaseIn )
				{
					double dx = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, Utils::c_nLeftBound );
					double dy = m_fLineSpacing * 0.1;
					
					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

						if( !pLetter->m_bNotLetter )
						{
							oManager.AddRotateAt( -1080, 0, dy, 0, dy, 0 );
						}
					}

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddFadeTransparency( 0, 1, 1, 5 );
					oTextManager.AddTranslate( dx, 0, 0, 0 );
				}
				void Conga( BOOL bPhaseIn )
				{
					const int c_nPoints = 7;
					Text::Transforms::CTranslateTransforms::TData arrMoves [c_nPoints];
					Text::Transforms::CScaleTransforms::TData     arrScales[c_nPoints];

					double step = m_fLineSpacing * 0.5;

					arrMoves[0].data.transform.SetParams( 0,          step * -6 );
					arrMoves[1].data.transform.SetParams( 0,          0 );
					arrMoves[2].data.transform.SetParams( step,       step );
					arrMoves[3].data.transform.SetParams( step * 1.5, step / 2 );
					arrMoves[4].data.transform.SetParams( step * 1.5, step / -2 );
					arrMoves[5].data.transform.SetParams( -step,      0 );
					arrMoves[6].data.transform.SetParams( 0, 0 );

					arrScales[0].data.transform.SetParams( 1.2, 1.2 );
					arrScales[1].data.transform.SetParams( 1.2, 1.2 );
					arrScales[2].data.transform.SetParams( 0.8, 0.8 );
					arrScales[3].data.transform.SetParams( 0.6, 0.6 );
					arrScales[4].data.transform.SetParams( 0.6, 0.6 );
					arrScales[5].data.transform.SetParams( 0.4, 0.4 );
					arrScales[6].data.transform.SetParams( 1, 1 );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

						oManager.AddTranslateTransforms( arrMoves, c_nPoints );
						oManager.AddScaleTransforms( arrScales, c_nPoints );
						oManager.AddFadeTransparency( 0, 1, 1, c_nPoints );
						oManager.SetTimeParams( nLetter, letters.count, letters.count * 0.5 );
					}					
				}
				void _Matrix( BOOL bPhaseIn )
				{
				}
				void HotPinkSpin( BOOL bPhaseIn )
				{
					const int c_nPoints = 6;

					Text::Transforms::CRotateTransforms::TData arrAngles[c_nPoints];
					Text::Transforms::CScaleTransforms::TData  arrScales[c_nPoints];
					Text::Transforms::CShearTransforms::TData  arrShears[c_nPoints];

					arrAngles[0].data.transform.SetParams( 90 );
					arrAngles[1].data.transform.SetParams( 45 );
					arrAngles[2].data.transform.SetParams( 0 );
					arrAngles[3].data.transform.SetParams( 0 );
					arrAngles[4].data.transform.SetParams( 0 );
					arrAngles[5].data.transform.SetParams( 0 );

					arrShears[0].data.transform.SetParams( -2, 0 );
					arrShears[1].data.transform.SetParams( -3, 0 );
					arrShears[2].data.transform.SetParams( -4, 0 );
					arrShears[3].data.transform.SetParams( -2.25, 0 );
					arrShears[4].data.transform.SetParams( -0.5, 0 );
					arrShears[5].data.transform.SetParams(  0, 0 );

					arrScales[0].data.transform.SetParams( -3, 5 );
					arrScales[1].data.transform.SetParams( -1, 5 );
					arrScales[2].data.transform.SetParams(  1, 5 );
					arrScales[3].data.transform.SetParams(  1, 4 );
					arrScales[4].data.transform.SetParams(  1, 3 );
					arrScales[5].data.transform.SetParams(  1, 1 );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

						oManager.AddRotateTransforms( arrAngles, c_nPoints );
						if( !pLetter->m_bNotLetter )
						{
							oManager.AddShearTransforms ( arrShears, c_nPoints );
							oManager.AddScaleTransforms ( arrScales, c_nPoints );
						}
						oManager.AddFadeTransparency( 0, 1, 1, c_nPoints );
						oManager.SetTimeParams( nLetter, letters.count, c_nPoints );
					}					
				}
				void HopInJumpOut( BOOL bPhaseIn )
				{
				}
				void ConJoin( BOOL bPhaseIn )
				{
					const int c_nPoints = 6;
					Text::Transforms::CTranslateTransforms::TData arrMoves [c_nPoints];
					Text::Transforms::CScaleTransforms::TData     arrScales[c_nPoints];

					arrMoves[0].data.transform.SetParams( 0, 0 );
					arrMoves[1].data.transform.SetParams( 0, m_fLineSpacing );

					arrScales[0].data.transform.SetParams( 3,    3 );
					arrScales[1].data.transform.SetParams( 3,    3 );
					arrScales[2].data.transform.SetParams( 0.65, -0.3 );
					arrScales[3].data.transform.SetParams( 0.8,  0 );
					arrScales[4].data.transform.SetParams( 0.95, 0.6 );
					arrScales[5].data.transform.SetParams( 1, 1 );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

						arrMoves[0].data.transform.dX = pLetter->m_oOrigin.X * 0.2;
						arrMoves[1].data.transform.dX = pLetter->m_oOrigin.X * 0.2;

						oManager.AddTranslateTransforms( arrMoves, c_nPoints );
						oManager.AddScaleTransforms( arrScales, c_nPoints );
						oManager.AddFadeTransparency( 0, 1, 1, c_nPoints );
						oManager.SetTimeParams( nLetter, letters.count, letters.count * 0.5 );
					}					
				}
				void HopsCotch( BOOL bPhaseIn )
				{
					const int c_nPoints = 5;
					Text::Transforms::CTranslateTransforms::TData arrMoves [c_nPoints];
					Text::Transforms::CScaleTransforms::TData     arrScales[c_nPoints];
					Text::Transforms::CRotateTransforms::TData    arrAngles[c_nPoints];

					double step = m_fLineSpacing * 0.1;

					arrMoves[0].data.transform.SetParams( step * -5, 0 );
					arrMoves[1].data.transform.SetParams( step * 10, 0 );
					arrMoves[2].data.transform.SetParams( 0, 0 );
					arrMoves[3].data.transform.SetParams( step * -1, 0 );
					arrMoves[4].data.transform.SetParams( 0, 0 );

					arrScales[0].data.transform.SetParams( 3, 3 );
					arrScales[1].data.transform.SetParams( 3, 3 );
					
					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

						arrAngles[3].data.transform.dAngle = !pLetter->m_bNotLetter ? -50 : 0;

						oManager.AddTranslateTransforms( arrMoves, c_nPoints );
						oManager.AddScaleTransforms( arrScales, c_nPoints );
						oManager.AddRotateTransforms( arrAngles, c_nPoints );
						oManager.AddFadeTransparency( 0, 1, 1, c_nPoints );
						oManager.SetTimeParams( nLetter, letters.count, letters.count * 0.3 );
					}					
				}
				void Twist( BOOL bPhaseIn )
				{
				}
				void StarWars( BOOL bPhaseIn )
				{
					const int c_nPoints = 30;

					Text::Transforms::CTransparencyTransforms::TData arrAlphas[3];
					arrAlphas[0].data.transform.SetParams( 0 );
					arrAlphas[1].data.transform.SetParams( 1 );
					arrAlphas[2].data.transform.SetParams( 1 );

					arrAlphas[1].data.interpolation = FALSE;
					arrAlphas[1].duration = 5;

					Text::Transforms::CTranslateTransforms::TData arrMoves1[c_nPoints];
					Text::Transforms::CTranslateTransforms::TData arrMoves2[c_nPoints];
					Text::Transforms::CShearTransforms::TData     arrShears[c_nPoints];
					Text::Transforms::CScaleTransforms::TData     arrScales[c_nPoints];
					
					CRefLines lines( m_pText->m_arrLines );
					for( int nLine = 0; nLine < lines.count; nLine++ )
					{
						double dTop    = Utils::GetOffsetRect( m_oLayoutRect, lines[nLine].m_oPathBoundRect, Utils::c_nTopBound );
						double dBottom = Utils::GetOffsetRect( m_oLayoutRect, lines[nLine].m_oPathBoundRect, Utils::c_nBottomBound );

						CRefLetters letters( m_pText->m_arrLetters, lines[nLine].m_nFirstLetter, lines[nLine].m_nLength );
						for( int nLetter = 0; nLetter < letters.count; nLetter++ )
						{
							Text::CLetterEntity* pLetter = letters.ptr + nLetter;
							Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );

							for( int i = 0; i < c_nPoints; i++ )
							{
								double dx = pLetter->m_oOrigin.X;
								double sh = dx * 2 / m_pText->m_oPathBoundRect.Width;
								double scale = 2 - 1.5 / double(c_nPoints) * i;

								arrMoves1[i].data.transform.SetParams( dx, 0 );
								arrShears[i].data.transform.SetParams( sh, 0  );
								arrScales[i].data.transform.SetParams( scale, scale / 1.5);
								arrMoves2[i].data.transform.SetParams( -dx, 0 );
							}

							oManager.AddTranslateTransforms( arrMoves1, c_nPoints );
							oManager.AddShearTransforms( arrShears, c_nPoints );
							oManager.AddScaleTransforms( arrScales, c_nPoints );
							oManager.AddTranslateTransforms( arrMoves2, c_nPoints );
							oManager.AddTransparencyTransforms( arrAlphas, 3 );
							oManager.AddTranslate( 0, dBottom, 0, dTop );
							oManager.SetTimeParams( nLine, lines.count, lines.count );
						}
					}
				}
				void Squeeze( BOOL bPhaseIn )
				{
					const int c_nPoints = 9;
					Text::Transforms::CShearTransforms::TData arrShears[c_nPoints];
					Text::Transforms::CScaleTransforms::TData arrScales[c_nPoints];

					arrScales[0].data.transform.SetParams( 1,    1 );
					arrScales[1].data.transform.SetParams( 0.75, 1 );
					arrScales[2].data.transform.SetParams( 0.5,  1 );
					arrScales[3].data.transform.SetParams( 0.75, 1 );
					arrScales[4].data.transform.SetParams( 1,    1 );
					arrScales[5].data.transform.SetParams( 0,    1 );
					arrScales[6].data.transform.SetParams( -1,   1 );
					arrScales[7].data.transform.SetParams( 0,    1 );
					arrScales[8].data.transform.SetParams( 1,    1 );

					arrShears[0].data.transform.SetParams( 0,  0 );
					arrShears[1].data.transform.SetParams( 0, -0.25 );
					arrShears[2].data.transform.SetParams( 0, -0.5 );
					arrShears[3].data.transform.SetParams( 0, -0.25 );
					arrShears[4].data.transform.SetParams( 0,  0 );
					arrShears[5].data.transform.SetParams( 0,  1 );
					arrShears[6].data.transform.SetParams( 0,  0 );
					arrShears[7].data.transform.SetParams( 0, -1 );
					arrShears[8].data.transform.SetParams( 0,  0 );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
						
						if( !pLetter->m_bNotLetter )
						{
							oManager.AddScaleTransforms( arrScales, c_nPoints );
							oManager.AddShearTransforms( arrShears, c_nPoints );
						}
					}

					double dx = Utils::GetOffsetRect( m_oLayoutRect, m_pText->m_oPathBoundRect, Utils::c_nLeftBound );

					Text::Transforms::CTransformManager& oTextManager = m_pText->GetTransformManager( bPhaseIn );
					oTextManager.AddFadeTransparency( 0, 1, 1, 5 );
					oTextManager.AddTranslate( dx, 0, 0, 0 );

				}
				void Terminator( BOOL bPhaseIn )
				{
					const int c_nPoints = 5;
					Text::Transforms::CScaleAtTransforms::TData arrScales[c_nPoints];

					arrScales[0].data.transform.SetParams( 0,    0,   0, 0 );
					arrScales[1].data.transform.SetParams( 0.01, 0.1, 0, 0 );
					arrScales[2].data.transform.SetParams( 1,    0.1, 0, 0 );
					arrScales[3].data.transform.SetParams( 1,    0.1, 0, 0 );
					arrScales[4].data.transform.SetParams( 1,    1,   0, 0 );

					CRefLetters letters( m_pText->m_arrLetters );
					for( int nLetter = 0; nLetter < letters.count; nLetter++ )
					{
						Text::CLetterEntity* pLetter = letters.ptr + nLetter;
						Text::Transforms::CTransformManager& oManager = pLetter->GetTransformManager( bPhaseIn );
						
						arrScales[0].data.transform.dX = \
						arrScales[1].data.transform.dX = \
						arrScales[2].data.transform.dX = \
						arrScales[3].data.transform.dX = pLetter->m_oOrigin.X;
						
						oManager.AddScaleAtTransforms( arrScales, c_nPoints );
					}
				}
			};			

		}
	}
}	