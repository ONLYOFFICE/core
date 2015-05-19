
#pragma once

#include <ippdefs.h>

namespace Geometry
{
	class CDot
	{
	public:
		double x, y;

	public:
		CDot()
			: x( 0 ), y( 0 )
		{
		}

		CDot( double dX, double dY )
			: x( dX ), y( dY )
		{
		}

		double SqrLength() const
		{
			// возвращает квадрат длины вектора
			return x * x + y * y;
		}

		double Length() const
		{
			// возвращает длину вектора
			return ::sqrt( SqrLength() );
		}

		void Normalize()
		{
			// нормализует вектор
			double dL = SqrLength();
			if( dL > IPP_EPS52 )
			{
				x = x * abs(x) / dL;
				y = y * abs(y) / dL;
			}
			else
			{
				x = 0;
				y = 0;
			}
		}

		void Move( double dX, double dY )
		{
			x += dX;
			y += dY;
		}

		void Scale( double dX, double dY )
		{
			x *= dX;
			y *= dY;
		}

		void Rotate( double dSinA, double dCosA )
		{
			// вращение точки: dSinA = sin(A), dCosA = cos(A)
			double temp = x * dCosA - y * dSinA;
			          y = y * dCosA + x * dSinA;
			          x = temp;
		}

		void Rotate( double dAngle )
		{
			// вращение точки; dAngle в радианах
			Rotate( ::sin( dAngle ), ::cos( dAngle ) );
		}

		double Orient( const CDot& a, const CDot& b ) const
		{
			// возвращает знак полуплоскости данной точки относительно прямой (a, b)
			return (a.x - x) * (b.y - y) - (a.y - y) * (b.x - x);
		}

		double Distance( const CDot& a, const CDot& b )
		{
			// возвращает растояние данной точки до прямой (a, b)
			CDot c( a.x - b.x, a.y - b.y);
			double D = c.x * (y - a.y) - c.y * (x - a.x);
			return D * D / c.SqrLength();
		}

		BOOL IntersectLines( const CDot& a1, const CDot& b1, const CDot& a2, const CDot& b2 )
		{
			// пересечение прямых (a1, b1), (a2, b2)
			CDot ad( b1.x - a1.x, a1.y - b1.y );
			CDot bd( b2.x - a2.x, a2.y - b2.y );

			// определитель системы
			double D = ad.y * bd.x - ad.x * bd.y;
			if( abs( D ) > IPP_EPS52 ) 
			{
				double aC = ad.x * a1.y + ad.y * a1.x;
				double bC = bd.x * a2.y + bd.y * a2.x;
				
				x = (aC * bd.x - ad.x * bC) / D;
				y = (ad.y * bC - aC * bd.y) / D;
				
				return TRUE;
			}
			
			return FALSE;  // нет решения
		}
	};
	
	class CTriangle
	{
	public:
		CDot v1, v2, v3; // вершины треугольника
	
		BOOL bClockWise; // обход по часовой стрелке

	public:
		CTriangle()
		{
			bClockWise = TRUE;
		}

		CTriangle( double x1, double y1, double x2, double y2, double x3, double y3 )
			: v1( x1, y1 ), v2( x2, y2 ), v3( x3, y3 )
		{
			CheckClockWise();
		}

		CTriangle( const CDot& a, const CDot& b, const CDot& c )
			: v1( a ), v2( b ), v3( c )
		{
			CheckClockWise();
		}

		
		void CheckClockWise()
		{
			bClockWise = v3.Orient( v1, v2 ) > 0;
		}

		BOOL IsInnerPoint( const CDot& dot ) const
		{
			if( (dot.Orient( v1, v2 ) > 0) != bClockWise )
				return FALSE;
			
			if( (dot.Orient( v2, v3 ) > 0) != bClockWise )
				return FALSE;

			if( (dot.Orient( v3, v1 ) > 0) != bClockWise )
				return FALSE;

			return TRUE;
		}
	};

}