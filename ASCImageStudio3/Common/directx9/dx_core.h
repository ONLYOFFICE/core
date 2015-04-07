
#pragma once

#include <d3dx9.h>
#include <dxerr9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxerr9.lib")

namespace ImageStudio
{
	namespace dx9
	{
		D3DCOLOR GetColor( int color, int alpha = 255 )
		{
			return D3DCOLOR_ARGB(min(max(0, alpha), 255), GetRValue(color), GetGValue(color), GetBValue(color));
		}

		double Clamp( double value )
		{
			if( value < 0 ) return 0;
			if( value > 1 ) return 1;
			
			return value;
		}

		float CheckScale( float scale )
		{
			return scale < 0 ? (min(scale, -0.0001f)) : (max(scale, 0.0001f));
		}

		int Round( double value )
		{
			if( value < 0 )
				value -= 0.5;
			else
				value += 0.5;

			return int(value);
		}

		int Ceil( double value )
		{
			int result = int(value);
			if( value > result )
				result += 1;

			return result;
		}

		int Floor( double value )
		{
			int result = int(value);
			
			if( value < result )
				result -= 1;

			return result;
		}

		
		struct FPoint
		{
			float x;
			float y;
		};

		struct TPoint
		{
			double x;
			double y;
		};

		
		void GetBoundRect( const TPoint points[], int count, TPoint& ptMin, TPoint& ptMax )	
		{
			double dMinX, dMaxX, dMinY, dMaxY;
			
			dMinX = dMaxX = points[0].x;
			dMinY = dMaxY = points[0].y;

			for( int i = 1; i < count; i++ )
			{
				if( points[i].x < dMinX ) dMinX = points[i].x; else
				if( points[i].x > dMaxX ) dMaxX = points[i].x;
				if( points[i].y < dMinY ) dMinY = points[i].y; else
				if( points[i].y > dMaxY ) dMaxY = points[i].y;
			}

			ptMin.x = dMinX;
			ptMin.y = dMinY;
			ptMax.x = dMaxX;
			ptMax.y = dMaxY;
		}
		
		class CMatrix3x2
		{
		public:
			double m[3][2];

		public:
			CMatrix3x2()
			{
				SetUnitMatrix();
			}

			CMatrix3x2( double _i00, double _i01, double _i10, double _i11, double _i20, double _i21 )
			{
				m[0][0] = _i00;
				m[0][1] = _i01;
				m[1][0] = _i10;
				m[1][1] = _i11;
				m[2][0] = _i20;
				m[2][1] = _i21;
			}

			
			void SetUnitMatrix()
			{
				m[0][0] = 1;
				m[0][1] = 0;
				m[1][0] = 0;
				m[1][1] = 1;
				m[2][0] = 0;
				m[2][1] = 0;
			}

			void SetTranslateMatrix( double dx, double dy )
			{
				m[0][0] = 1;
				m[0][1] = 0;
				m[1][0] = 0;
				m[1][1] = 1;
				m[2][0] = dx;
				m[2][1] = dy;
			}

			void SetTranslateAndScaleMatrix( double dx, double dy, double sx, double sy )
			{
				m[0][0] = sx;
				m[0][1] = 0;
				m[1][0] = 0;
				m[1][1] = sy;
				m[2][0] = dx * sx;
				m[2][1] = dy * sy;
			}

			void SetTranslateAndRotateMatrix( double dx, double dy, double dSin, double dCos )
			{
				m[0][0] =  dCos;
				m[0][1] =  dSin;
				m[1][0] = -dSin;
				m[1][1] =  dCos;
				m[2][0] =  dx * dCos - dy * dSin;
				m[2][1] =  dx * dSin + dy * dCos;
			}

			void SetScaleMatrix( double sx, double sy )
			{
				m[0][0] = sx;
				m[0][1] = 0;
				m[1][0] = 0;
				m[1][1] = sy;
				m[2][0] = 0;
				m[2][1] = 0;
			}

			void SetRotateMatrix( double dSin, double dCos )
			{
				m[0][0] =  dCos;
				m[0][1] =  dSin;
				m[1][0] = -dSin;
				m[1][1] =  dCos;
				m[2][0] =  0;
				m[2][1] =  0;
			}

			void SetRotateAtMatrix( double dSin, double dCos, double x, double y )
			{
				m[0][0] =  dCos;
				m[0][1] =  dSin;
				m[1][0] = -dSin;
				m[1][1] =  dCos;
				m[2][0] =  x * (1 - dCos) + y * dSin;
				m[2][1] =  y * (1 - dCos) - x * dSin;
			}

			
			void TranslateX( double dx )
			{
				m[2][0] += dx;
			}

			void TranslateY( double dy )
			{
				m[2][1] += dy;
			}

			void Translate( double dx, double dy )
			{
				m[2][0] += dx;
				m[2][1] += dy;
			}

			void ScaleX( double sx )
			{
				m[0][0] *= sx;
				m[1][0] *= sx;
				m[2][0] *= sx;
			}

			void ScaleY( double sy )
			{
				m[0][1] *= sy;
				m[1][1] *= sy;
				m[2][1] *= sy;
			}
			void Scale( double sx, double sy )
			{
				m[0][0] *= sx;
				m[0][1] *= sy;
				m[1][0] *= sx;
				m[1][1] *= sy;
				m[2][0] *= sx;
				m[2][1] *= sy;
			}

			void Scale2( double s )
			{
				m[0][0] *= s;
				m[1][0] *= s;
				m[2][0] *= s;
				m[0][1] *= s;
				m[1][1] *= s;
				m[2][1] *= s;
			}

			void Rotate( double dSin, double dCos )
			{
				double x, y;

				x = m[0][0] * dCos - m[0][1] * dSin;
				y = m[0][0] * dSin + m[0][1] * dCos;
				
				m[0][0] = x;
				m[0][1] = y;

				x = m[1][0] * dCos - m[1][1] * dSin;
				y = m[1][0] * dSin + m[1][1] * dCos;
				
				m[1][0] = x;
				m[1][1] = y;

				x = m[2][0] * dCos - m[2][1] * dSin;
				y = m[2][0] * dSin + m[2][1] * dCos;
				
				m[2][0] = x;
				m[2][1] = y;
			}

			void RotateAt( double dSin, double dCos, double x, double y )
			{
				Translate( -x, -y );
				Rotate( dSin, dCos );
				Translate( x, y );
			}

			
			void Multiply( const TPoint& src, TPoint& dst ) const
			{
				double x = m[0][0] * src.x + m[1][0] * src.y + m[2][0];
				double y = m[0][1] * src.x + m[1][1] * src.y + m[2][1];
				
				dst.x = x;
				dst.y = y;
			}

			void Multiply( const TPoint pSrcArray[], TPoint pDstArray[], int count ) const
			{
				for( int i = 0; i < count; i++ )
				{
					Multiply( pSrcArray[i], pDstArray[i] );
				}
			}

			void Multiply( const CMatrix3x2& src, BOOL bAppend = TRUE )
			{
				double t[3][2];

				if( bAppend )
				{
					// dst = dst * src;
					Multiply( m, src.m, t );
				}
				else
				{
					// dst = src * dst;
					Multiply( src.m, m, t );
				}
				
				Set( t );
			}

			static void Multiply( const double m1[3][2], const double m2[3][2], double res[3][2] )
			{
				res[0][0] = m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0];
				res[0][1] = m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1];
				res[1][0] = m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0];
				res[1][1] = m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1];
				res[2][0] = m1[2][0] * m2[0][0] + m1[2][1] * m2[1][0] + m2[2][0];
				res[2][1] = m1[2][0] * m2[0][1] + m1[2][1] * m2[1][1] + m2[2][1];
			}

			
			void Set( const double src[3][2] )
			{
				m[0][0] = src[0][0];
				m[0][1] = src[0][1];
				m[1][0] = src[1][0];
				m[1][1] = src[1][1];
				m[2][0] = src[2][0];
				m[2][1] = src[2][1];
			}

			void Get( float dst[3][2] ) const
			{
				dst[0][0] = float(m[0][0]);
				dst[0][1] = float(m[0][1]);
				dst[1][0] = float(m[1][0]);
				dst[1][1] = float(m[1][1]);
				dst[2][0] = float(m[2][0]);
				dst[2][1] = float(m[2][1]);
			}
		
			BOOL Build( const TPoint src[3], const TPoint dst[3] )
			{
				// строит матрицу преобразования треугольника src в треугольник dst
				double dx31 = dst[2].x - dst[0].x;
				double dx32 = dst[2].x - dst[1].x;
				double dy31 = dst[2].y - dst[0].y;
				double dy32 = dst[2].y - dst[1].y;

				if( abs(dx31 * dy32 - dx32 * dy31) < IPP_EPS23 )
					return FALSE; // треугольник вырожден

				double sx23 = src[1].x - src[2].x;
				double sx13 = src[0].x - src[2].x;
				double sy23 = src[1].y - src[2].y;
				double sy13 = src[0].y - src[2].y;

				double det = sx23 * sy13 - sx13 * sy23;

				if( abs(det) < IPP_EPS23 )
					return FALSE; // треугольник вырожден

				det = 1.0 / det;

				m[0][0] = (dx31 * sy23 - dx32 * sy13);
				m[0][1] = (dy31 * sy23 - dy32 * sy13);
				m[1][0] = (dx32 * sx13 - dx31 * sx23);
				m[1][1] = (dy32 * sx13 - dy31 * sx23);
				m[2][0] = (m[0][0] * src[2].x + m[1][0] * src[2].y);
				m[2][1] = (m[0][1] * src[2].x + m[1][1] * src[2].y);

				m[0][0] *= det;
				m[0][1] *= det;
				m[1][0] *= det;
				m[1][1] *= det;
				m[2][0] = dst[2].x - m[2][0] * det;
				m[2][1] = dst[2].y - m[2][1] * det;

				return TRUE;
			}

			BOOL Build2( const TPoint src[3] )
			{
				// строит матрицу преобразования треугольника src в треугольник dst {(0, 0), (1, 0), (1, 1)};
				double sx23 = src[1].x - src[2].x;
				double sx13 = src[0].x - src[2].x;
				double sy23 = src[1].y - src[2].y;
				double sy13 = src[0].y - src[2].y;

				double det = sx23 * sy13 - sx13 * sy23;

				if( abs(det) < IPP_EPS23 )
					return FALSE; // треугольник вырожден

				det = 1.0 / det;

				m[0][0] = (sy23);
				m[0][1] = (sy23 - sy13);
				m[1][0] = (-sx23);
				m[1][1] = (sx13 - sx23);
				m[2][0] = (m[0][0] * src[2].x + m[1][0] * src[2].y);
				m[2][1] = (m[0][1] * src[2].x + m[1][1] * src[2].y);

				m[0][0] *= det;
				m[0][1] *= det;
				m[1][0] *= det;
				m[1][1] *= det;
				m[2][0] = 1 - m[2][0] * det;
				m[2][1] = 1 - m[2][1] * det;

				return TRUE;
			}

			
			BOOL Reverse( float dst[3][2] ) const
			{
				// получение обратной матрицы

				double det = Determinant();

				if( abs(det) < IPP_EPS23 )
					return FALSE; // матрица вырождена

				det = 1.0 / det;

				dst[0][0] = float(det * ( m[1][1]));
				dst[0][1] = float(det * (-m[0][1]));
				dst[1][0] = float(det * (-m[1][0]));
				dst[1][1] = float(det * ( m[0][0]));
				dst[2][0] = float(det * ( m[1][0] * m[2][1] - m[1][1] * m[2][0]));
				dst[2][1] = float(det * ( m[0][1] * m[2][0] - m[0][0] * m[2][1]));
				
				return TRUE;
			}

			BOOL Reverse()
			{
				// получение обратной матрицы

				double det = Determinant();

				if( abs(det) < IPP_EPS23 )
					return FALSE; // матрица вырождена

				det = 1.0 / det;

				double tmp[3][2];

				tmp[0][0] = det * ( m[1][1]);
				tmp[0][1] = det * (-m[0][1]);
				tmp[1][0] = det * (-m[1][0]);
				tmp[1][1] = det * ( m[0][0]);
				tmp[2][0] = det * ( m[1][0] * m[2][1] - m[1][1] * m[2][0]);
				tmp[2][1] = det * ( m[0][1] * m[2][0] - m[0][0] * m[2][1]);
				
				Set( tmp );

				return TRUE;
			}

			
			double Determinant() const
			{
				return m[0][0] * m[1][1] - m[0][1] * m[1][0];
			}
		};

		class CMatrix3x3
		{
		public:
			double m[3][3];

		public:
			CMatrix3x3()
			{
				SetUnitMatrix();
			}

			CMatrix3x3( 
				double _i00, double _i01, double _i02, 
				double _i10, double _i11, double _i12, 
				double _i20, double _i21, double _i22 )
			{
				m[0][0] = _i00;
				m[0][1] = _i01;
				m[0][2] = _i02;
				m[1][0] = _i10;
				m[1][1] = _i11;
				m[1][2] = _i12;
				m[2][0] = _i20;
				m[2][1] = _i21;
				m[2][2] = _i22;
			}



			
			void SetUnitMatrix()
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
			}

			void SetTranslateMatrix( double dx, double dy )
			{
				m[0][0] = 1;
				m[0][1] = 0;
				m[0][2] = 0;
				m[1][0] = 0;
				m[1][1] = 1;
				m[1][2] = 0;
				m[2][0] = dx;
				m[2][1] = dy;
				m[2][2] = 1;
			}

			void SetTranslateAndScaleMatrix( double dx, double dy, double sx, double sy )
			{
				m[0][0] = sx;
				m[0][1] = 0;
				m[0][2] = 0;
				m[1][0] = 0;
				m[1][1] = sy;
				m[1][2] = 0;
				m[2][0] = dx * sx;
				m[2][1] = dy * sy;
				m[2][2] = 1;
			}

			void SetTranslateAndRotateMatrix( double dx, double dy, double dSin, double dCos )
			{
				m[0][0] =  dCos;
				m[0][1] =  dSin;
				m[0][2] =  0;
				m[1][0] = -dSin;
				m[1][1] =  dCos;
				m[1][2] =  0;
				m[2][0] =  dx * dCos - dy * dSin;
				m[2][1] =  dx * dSin + dy * dCos;
				m[2][2] =  1;
			}

			void SetScaleMatrix( double sx, double sy )
			{
				m[0][0] = sx;
				m[0][1] = 0;
				m[0][2] = 0;
				m[1][0] = 0;
				m[1][1] = sy;
				m[1][2] = 0;
				m[2][0] = 0;
				m[2][1] = 0;
				m[2][2] = 1;
			}

			void SetRotateMatrix( double dSin, double dCos )
			{
				m[0][0] =  dCos;
				m[0][1] =  dSin;
				m[0][2] =  0;
				m[1][0] = -dSin;
				m[1][1] =  dCos;
				m[1][2] =  0;
				m[2][0] =  0;
				m[2][1] =  0;
				m[2][2] =  1;
			}

			void SetRotateAtMatrix( double dSin, double dCos, double x, double y )
			{
				m[0][0] =  dCos;
				m[0][1] =  dSin;
				m[0][2] =  0;
				m[1][0] = -dSin;
				m[1][1] =  dCos;
				m[1][2] =  0;
				m[2][0] =  x * (1 - dCos) + y * dSin;
				m[2][1] =  y * (1 - dCos) - x * dSin;
				m[2][2] =  1;
			}

			void SetPerspectiveMatrix( double px, double py )
			{
				m[0][0] = 1;
				m[0][1] = 0;
				m[0][2] = px;
				m[1][0] = 0;
				m[1][1] = 1;
				m[1][2] = py;
				m[2][0] = 0;
				m[2][1] = 0;
				m[2][2] = 1;
			}

			
			void TranslateX( double dx )
			{
				m[0][0] += dx * m[0][2];
				m[1][0] += dx * m[1][2];
				m[2][0] += dx * m[2][2];
			}

			void TranslateY( double dy )
			{
				m[0][1] += dy * m[0][2];
				m[1][1] += dy * m[1][2];
				m[2][1] += dy * m[2][2];
			}

			void Translate( double dx, double dy )
			{
				m[0][0] += dx * m[0][2];
				m[0][1] += dy * m[0][2];
				m[1][0] += dx * m[1][2];
				m[1][1] += dy * m[1][2];
				m[2][0] += dx * m[2][2];
				m[2][1] += dy * m[2][2];
			}

			void ScaleX( double sx )
			{
				m[0][0] *= sx;
				m[1][0] *= sx;
				m[2][0] *= sx;
			}

			void ScaleY( double sy )
			{
				m[0][1] *= sy;
				m[1][1] *= sy;
				m[2][1] *= sy;
			}

			void Scale( double sx, double sy )
			{
				m[0][0] *= sx;
				m[0][1] *= sy;
				m[1][0] *= sx;
				m[1][1] *= sy;
				m[2][0] *= sx;
				m[2][1] *= sy;
			}

			void Scale2( double s )
			{
				m[0][0] *= s;
				m[1][0] *= s;
				m[2][0] *= s;
				m[0][1] *= s;
				m[1][1] *= s;
				m[2][1] *= s;
			}

			void Rotate( double dSin, double dCos )
			{
				double x, y;

				x = m[0][0] * dCos - m[0][1] * dSin;
				y = m[0][0] * dSin + m[0][1] * dCos;
				
				m[0][0] = x;
				m[0][1] = y;

				x = m[1][0] * dCos - m[1][1] * dSin;
				y = m[1][0] * dSin + m[1][1] * dCos;
				
				m[1][0] = x;
				m[1][1] = y;

				x = m[2][0] * dCos - m[2][1] * dSin;
				y = m[2][0] * dSin + m[2][1] * dCos;
				
				m[2][0] = x;
				m[2][1] = y;
			}

			void RotateAt( double dSin, double dCos, double x, double y )
			{
				Translate( -x, -y );
				Rotate( dSin, dCos );
				Translate( x, y );
			}

			void PerspectiveX( double px )
			{
				m[0][2] += m[0][0] * px;
				m[1][2] += m[1][0] * px;
				m[2][2] += m[2][0] * px;
			}

			void PerspectiveY( double py )
			{
				m[0][2] += m[0][1] * py;
				m[1][2] += m[1][1] * py;
				m[2][2] += m[2][1] * py;
			}

			void Perspective( double px, double py )
			{
				m[0][2] += m[0][0] * px + m[0][1] * py;
				m[1][2] += m[1][0] * px + m[1][1] * py;
				m[2][2] += m[2][0] * px + m[2][1] * py;
			}

			
			void MultiplyR( const double src[3], double dst[3] ) const
			{
				double x = src[0] * m[0][0] + src[1] * m[0][1] + src[2] * m[0][2];
				double y = src[0] * m[1][0] + src[1] * m[1][1] + src[2] * m[1][2];
				double w = src[0] * m[2][0] + src[1] * m[2][1] + src[2] * m[2][2];

				dst[0] = x;
				dst[1] = y;
				dst[2] = w;
			}

			void Multiply( const TPoint& src, TPoint& dst ) const
			{
				double x = src.x * m[0][0] + src.y * m[1][0] + m[2][0];
				double y = src.x * m[0][1] + src.y * m[1][1] + m[2][1];
				double w = src.x * m[0][2] + src.y * m[1][2] + m[2][2];

				dst.x = x / w;
				dst.y = y / w;
			}

			void Multiply( const TPoint pSrcArray[], TPoint pDstArray[], int count ) const
			{
				for( int i = 0; i < count; i++ )
				{
					Multiply( pSrcArray[i], pDstArray[i] );
				}
			}

			void Multiply( const CMatrix3x3& src, BOOL bAppend = TRUE )
			{
				double t[3][3];

				if( bAppend )
				{
					// dst = dst * src;
					Multiply( m, src.m, t );
				}
				else
				{
					// dst = src * dst;
					Multiply( src.m, m, t );
				}

				Set( t );
			}

			static void Multiply( const double m1[3][3], const double m2[3][3], double res[3][3] )
			{
				res[0][0] = m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0] + m1[0][2] * m2[2][0];
				res[0][1] = m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1] + m1[0][2] * m2[2][1];
				res[0][2] = m1[0][0] * m2[0][2] + m1[0][1] * m2[1][2] + m1[0][2] * m2[2][2];
				res[1][0] = m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0] + m1[1][2] * m2[2][0];
				res[1][1] = m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1] + m1[1][2] * m2[2][1];
				res[1][2] = m1[1][0] * m2[0][2] + m1[1][1] * m2[1][2] + m1[1][2] * m2[2][2];
				res[2][0] = m1[2][0] * m2[0][0] + m1[2][1] * m2[1][0] + m1[2][2] * m2[2][0];
				res[2][1] = m1[2][0] * m2[0][1] + m1[2][1] * m2[1][1] + m1[2][2] * m2[2][1];
				res[2][2] = m1[2][0] * m2[0][2] + m1[2][1] * m2[1][2] + m1[2][2] * m2[2][2];
			}


			
			void Set( const double src[3][3] )
			{
				m[0][0] = src[0][0];
				m[0][1] = src[0][1];
				m[0][2] = src[0][2];
				m[1][0] = src[1][0];
				m[1][1] = src[1][1];
				m[1][2] = src[1][2];
				m[2][0] = src[2][0];
				m[2][1] = src[2][1];
				m[2][2] = src[2][2];
			}

			void Get( float dst[3][3] ) const
			{
				dst[0][0] = float(m[0][0]);
				dst[0][1] = float(m[0][1]);
				dst[0][2] = float(m[0][2]);
				dst[1][0] = float(m[1][0]);
				dst[1][1] = float(m[1][1]);
				dst[1][2] = float(m[1][2]);
				dst[2][0] = float(m[2][0]);
				dst[2][1] = float(m[2][1]);
				dst[2][2] = float(m[2][2]);
			}

			
			BOOL Reverse( float dst[3][3] ) const
			{
				// расчёт обратной матрицы

				double det = Determinant();

				if( abs(det) < IPP_EPS23 )
					return FALSE; // матрица вырождена

				det = 1.0 / det;

				dst[0][0] = float(det * (m[1][1] * m[2][2] - m[1][2] * m[2][1]));
				dst[0][1] = float(det * (m[0][2] * m[2][1] - m[0][1] * m[2][2]));
				dst[0][2] = float(det * (m[0][1] * m[1][2] - m[0][2] * m[1][1]));
				dst[1][0] = float(det * (m[1][2] * m[2][0] - m[1][0] * m[2][2]));
				dst[1][1] = float(det * (m[0][0] * m[2][2] - m[0][2] * m[2][0]));
				dst[1][2] = float(det * (m[0][2] * m[1][0] - m[0][0] * m[1][2]));
				dst[2][0] = float(det * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
				dst[2][1] = float(det * (m[0][1] * m[2][0] - m[0][0] * m[2][1]));
				dst[2][2] = float(det * (m[0][0] * m[1][1] - m[0][1] * m[1][0]));

				return TRUE;
			}

			BOOL Reverse()
			{
				// расчёт обратной матрицы

				double det = Determinant();

				if( abs(det) < IPP_EPS23 )
					return FALSE; // матрица вырождена

				det = 1.0 / det;

				double tmp[3][3];

				tmp[0][0] = float(det * (m[1][1] * m[2][2] - m[1][2] * m[2][1]));
				tmp[0][1] = float(det * (m[0][2] * m[2][1] - m[0][1] * m[2][2]));
				tmp[0][2] = float(det * (m[0][1] * m[1][2] - m[0][2] * m[1][1]));
				tmp[1][0] = float(det * (m[1][2] * m[2][0] - m[1][0] * m[2][2]));
				tmp[1][1] = float(det * (m[0][0] * m[2][2] - m[0][2] * m[2][0]));
				tmp[1][2] = float(det * (m[0][2] * m[1][0] - m[0][0] * m[1][2]));
				tmp[2][0] = float(det * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
				tmp[2][1] = float(det * (m[0][1] * m[2][0] - m[0][0] * m[2][1]));
				tmp[2][2] = float(det * (m[0][0] * m[1][1] - m[0][1] * m[1][0]));

				Set( tmp );

				return TRUE;
			}

			
			double Determinant() const
			{
				return 
					m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) +
					m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2]) +
					m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
			}

			BOOL Build( const TPoint src[4], const TPoint dst[4] )
			{
				double tmp[3][3];

				double xx = dst[0].x * src[0].x;
				double xy = dst[0].x * src[0].y;
				double yx = dst[0].y * src[0].x;
				double yy = dst[0].y * src[0].y;

				double b[9] = {
					dst[3].x * src[3].x - xx,
					dst[3].x * src[3].y - xy,
					dst[3].x - dst[0].x,
					dst[2].x * src[2].x - xx,
					dst[2].x * src[2].y - xy,
					dst[2].x - dst[0].x,
					dst[1].x * src[1].x - xx,
					dst[1].x * src[1].y - xy,
					dst[1].x - dst[0].x,
				};

				double d[9] = {
					dst[3].y * src[3].x - yx,
					dst[3].y * src[3].y - yy,
					dst[3].y - dst[0].y,
					dst[2].y * src[2].x - yx,
					dst[2].y * src[2].y - yy,
					dst[2].y - dst[0].y,
					dst[1].y * src[1].x - yx,
					dst[1].y * src[1].y - yy,
					dst[1].y - dst[0].y,
				};

				double c[6] = {
					src[3].x - src[0].x,
					src[3].y - src[0].y,
					src[2].x - src[0].x,
					src[2].y - src[0].y,
					src[1].x - src[0].x,
					src[1].y - src[0].y,
				};

				double e[6] = {
					b[6] * c[1] - b[0] * c[5],
					b[7] * c[1] - b[1] * c[5],
					b[8] * c[1] - b[2] * c[5],
					b[3] * c[1] - b[0] * c[3],
					b[4] * c[1] - b[1] * c[3],
					b[5] * c[1] - b[2] * c[3],
				};

				double g[6] = {
					d[6] * c[1] - d[0] * c[5],
					d[7] * c[1] - d[1] * c[5],
					d[8] * c[1] - d[2] * c[5],
					d[3] * c[1] - d[0] * c[3],
					d[4] * c[1] - d[1] * c[3],
					d[5] * c[1] - d[2] * c[3],
				};

				double f[2] = {
					c[4] * c[1] - c[0] * c[5],
					c[2] * c[1] - c[0] * c[3],
				};

				double k[6] = {
					e[3] * f[0] - e[0] * f[1],
					e[4] * f[0] - e[1] * f[1],
					e[5] * f[0] - e[2] * f[1],
					g[3] * f[0] - g[0] * f[1],
					g[4] * f[0] - g[1] * f[1],
					g[5] * f[0] - g[2] * f[1],
				};

				double det = k[1] * k[3] - k[0] * k[4];
				if( abs(det) < IPP_EPS23 )
					return FALSE;

				tmp[0][2] = (k[2] * k[4] - k[1] * k[5]) / det;
				tmp[1][2] = (k[0] * k[5] - k[2] * k[3]) / det;

				double sum1 = (src[1].x * tmp[0][2] + src[1].y * tmp[1][2] + 1);
				double sum2 = (src[2].x * tmp[0][2] + src[2].y * tmp[1][2] + 1);
				double sum3 = (src[3].x * tmp[0][2] + src[3].y * tmp[1][2] + 1);

				double m1 = dst[1].x * sum1;
				double m2 = dst[2].x * sum2;
				double m3 = dst[3].x * sum3;

				double n1 = dst[1].y * sum1;
				double n2 = dst[2].y * sum2;
				double n3 = dst[3].y * sum3;

				double r1 = m3 - m1;
				double r2 = m2 - m1;
				
				double s1 = n3 - n1;
				double s2 = n2 - n1;

				double p1 = src[3].x - src[1].x;
				double p2 = src[2].x - src[1].x;

				double q1 = src[3].y - src[1].y;
				double q2 = src[2].y - src[1].y;

				det = p1 * q2 - p2 * q1;
				if( abs(det) < IPP_EPS23 )
					return FALSE;

				det = 1.0 / det;
 
				tmp[0][0] = (r1 * q2 - r2 * q1) * det;
				tmp[0][1] = (s1 * q2 - s2 * q1) * det;
				tmp[1][0] = (r2 * p1 - r1 * p2) * det;
				tmp[1][1] = (s2 * p1 - s1 * p2) * det;
				
				tmp[2][0] = m1 - src[1].x * tmp[0][0] - src[1].y * tmp[1][0];
				tmp[2][1] = n1 - src[1].x * tmp[0][1] - src[1].y * tmp[1][1];
				tmp[2][2] = 1;

				Set( tmp );

				return TRUE;
			}

			static BOOL Build2( const TPoint src[4], float result[3][3] )
			{
				// dst = {0, 0, 1, 0, 1, 1, 0, 1};

				double tmp[8];

				double c[6] = {
					src[3].x - src[0].x,
					src[3].y - src[0].y,
					src[2].x - src[0].x,
					src[2].y - src[0].y,
					src[1].x - src[0].x,
					src[1].y - src[0].y,
				};

				double e[6] = {
					c[1] * src[1].x,
					c[1] * src[1].y,
					c[1],
					c[1] * src[2].x,
					c[1] * src[2].y,
					c[1],
				};

				double g[6] = {
					-c[5] * src[3].x,
					-c[5] * src[3].y,
					-c[5],
					src[2].x * c[1] - src[3].x * c[3],
					src[2].y * c[1] - src[3].y * c[3],
					c[1] - c[3],
				};

				double f[2] = {
					c[4] * c[1] - c[0] * c[5],
					c[2] * c[1] - c[0] * c[3],
				};

				double k[6] = {
					e[3] * f[0] - e[0] * f[1],
					e[4] * f[0] - e[1] * f[1],
					e[5] * f[0] - e[2] * f[1],
					g[3] * f[0] - g[0] * f[1],
					g[4] * f[0] - g[1] * f[1],
					g[5] * f[0] - g[2] * f[1],
				};

				double det = k[1] * k[3] - k[0] * k[4];
				if( abs(det) < IPP_EPS23 )
					return FALSE;

				tmp[2] = (k[2] * k[4] - k[1] * k[5]) / det;
				tmp[5] = (k[0] * k[5] - k[2] * k[3]) / det;

				double sum1 = (src[1].x * tmp[2] + src[1].y * tmp[5] + 1);
				double sum2 = (src[2].x * tmp[2] + src[2].y * tmp[5] + 1);
				double sum3 = (src[3].x * tmp[2] + src[3].y * tmp[5] + 1);

				double r1 =    0 - sum1;
				double r2 = sum2 - sum1;
				
				double s1 = sum3;
				double s2 = sum2;

				double p1 = src[3].x - src[1].x;
				double p2 = src[2].x - src[1].x;

				double q1 = src[3].y - src[1].y;
				double q2 = src[2].y - src[1].y;

				det = p1 * q2 - p2 * q1;
				if( abs(det) < IPP_EPS23 )
					return FALSE;

				det = 1.0 / det;
 
				tmp[0] = (r1 * q2 - r2 * q1) * det;
				tmp[1] = (s1 * q2 - s2 * q1) * det;
				tmp[3] = (r2 * p1 - r1 * p2) * det;
				tmp[4] = (s2 * p1 - s1 * p2) * det;
				
				tmp[6] = sum1 - src[1].x * tmp[0] - src[1].y * tmp[3];
				tmp[7] =    0 - src[1].x * tmp[1] - src[1].y * tmp[4];

				
				result[0][0] = float(tmp[0]);
				result[0][1] = float(tmp[1]);
				result[0][2] = float(tmp[2]);
				result[1][0] = float(tmp[3]);
				result[1][1] = float(tmp[4]);
				result[1][2] = float(tmp[5]);
				result[2][0] = float(tmp[6]);
				result[2][1] = float(tmp[7]);
				result[2][2] = 1;
				
				return TRUE;
			}

		};
		
		class CSampler
		{
		public:
			enum EFilter
			{
				c_nDefault  = 0,
				c_nNearest  = 1,
				c_nLinear   = 2,
				c_nAnisot1  = 3,
				c_nAnisot2  = 4,
				c_nAnisot4  = 5,
				c_nAnisot8  = 6,
				c_nAnisot16 = 7,
			};
		
		public:
			IDirect3DTexture9* m_pTexture;

			DWORD m_eMagFilter;
			DWORD m_eMinFilter;
			DWORD m_eAddressU;
			DWORD m_eAddressV;

			D3DCOLOR m_nBorderColor;
			DWORD    m_nAnisotropic;

		public:
			CSampler()
			{
				m_pTexture = NULL;

				m_eMagFilter = D3DTEXF_LINEAR;
				m_eMinFilter = D3DTEXF_LINEAR;

				m_eAddressU = D3DTADDRESS_MIRROR;
				m_eAddressV = D3DTADDRESS_MIRROR;

				m_nBorderColor = 0;
				m_nAnisotropic = 0;
			}

			
			void SetToDevice( IDirect3DDevice9* pDevice, DWORD stage = 0 ) const
			{
				if( !pDevice )
					return;

				pDevice->SetSamplerState( stage, D3DSAMP_MAGFILTER, m_eMagFilter );
				pDevice->SetSamplerState( stage, D3DSAMP_MINFILTER, m_eMinFilter );
				pDevice->SetSamplerState( stage, D3DSAMP_ADDRESSU, m_eAddressU );
				pDevice->SetSamplerState( stage, D3DSAMP_ADDRESSV, m_eAddressV );
				
				pDevice->SetSamplerState( stage, D3DSAMP_BORDERCOLOR, m_nBorderColor );
				
				if( m_nAnisotropic > 0 )
					pDevice->SetSamplerState( stage, D3DSAMP_MAXANISOTROPY, m_nAnisotropic );

				pDevice->SetTexture( stage, m_pTexture );
			}

			
			void SetFilter( EFilter filter )
			{
				switch( filter )
				{
				case c_nNearest:  SetFilterPoint(); break;
				case c_nLinear:   SetFilterLinear(); break;
				case c_nAnisot1:  SetFilterAnisotropic( 1 ); break;
				case c_nAnisot2:  SetFilterAnisotropic( 2 ); break;
				case c_nAnisot4:  SetFilterAnisotropic( 4 ); break;
				case c_nAnisot8:  SetFilterAnisotropic( 8 ); break;
				case c_nAnisot16: SetFilterAnisotropic( 16 ); break;
				default: SetFilterLinear();
				}
			}

			void SetFilterPoint()
			{
				m_eMagFilter = D3DTEXF_POINT;
				m_eMinFilter = D3DTEXF_POINT;
			}

			void SetFilterLinear()
			{
				m_eMagFilter = D3DTEXF_LINEAR;
				m_eMinFilter = D3DTEXF_LINEAR;
			}

			void SetFilterCubic()
			{
				m_eMagFilter = D3DTEXF_ANISOTROPIC;
				m_eMinFilter = D3DTEXF_ANISOTROPIC;
				m_nAnisotropic = 4;
			}

			void SetFilterSuper()
			{
				m_eMagFilter = D3DTEXF_ANISOTROPIC;
				m_eMinFilter = D3DTEXF_ANISOTROPIC;
				m_nAnisotropic = 16;
			}

			void SetFilterAnisotropic( DWORD count )
			{
				m_eMagFilter = D3DTEXF_ANISOTROPIC;
				m_eMinFilter = D3DTEXF_ANISOTROPIC;
				m_nAnisotropic = count;
			}

			void SetAddressMirrow()
			{
				m_eAddressU = D3DTADDRESS_MIRROR;
				m_eAddressV = D3DTADDRESS_MIRROR;
			}

			void SetAddressWrap()
			{
				m_eAddressU = D3DTADDRESS_WRAP;
				m_eAddressV = D3DTADDRESS_WRAP;
			}

			void SetAddressClamp()
			{
				m_eAddressU = D3DTADDRESS_CLAMP;
				m_eAddressV = D3DTADDRESS_CLAMP;
			}

			void SetAddressMirrowOnce()
			{
				m_eAddressU = D3DTADDRESS_MIRRORONCE;
				m_eAddressV = D3DTADDRESS_MIRRORONCE;
			}

			void SetAddressBorder( D3DCOLOR color )
			{
				m_eAddressU = D3DTADDRESS_BORDER;
				m_eAddressV = D3DTADDRESS_BORDER;
				m_nBorderColor = color;
			}

			void SetTexture( IDirect3DTexture9* pTexture )
			{
				m_pTexture = pTexture;
			}

		};
	}
}