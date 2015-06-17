#include "Matrix.h"

namespace Aggplus
{
	CMatrix::CMatrix(double m11, double m12, double m21, double m22, double dx, double dy) : m_agg_mtx(m11, m12, m21, m22, dx, dy)
	{
	}

    CMatrix::CMatrix(const CMatrix& oSrc) : m_agg_mtx()
    {
        m_agg_mtx	= oSrc.m_agg_mtx;
    }

	CMatrix::CMatrix() : m_agg_mtx() 
	{
	}
	
	CMatrix::~CMatrix()
	{
	}
	
	void CMatrix::Translate(double offsetX, double offsetY, MatrixOrder order)
	{
		if (order == MatrixOrderPrepend) 
		{ 
			m_agg_mtx.premultiply(agg::trans_affine_translation(offsetX, offsetY)); 
		}
		else                            
		{ 
			m_agg_mtx.multiply(agg::trans_affine_translation(offsetX, offsetY)); 
		}
	}

	void CMatrix::Scale(double scaleX, double scaleY, MatrixOrder order)
	{	
		if (order == MatrixOrderPrepend) 
		{ 
			m_agg_mtx.premultiply(agg::trans_affine_scaling(scaleX, scaleY)); 
		}
		else                            
		{ 
			m_agg_mtx.multiply(agg::trans_affine_scaling(scaleX, scaleY)); 
		}
	}

	void CMatrix::Shear(double shearX, double shearY, MatrixOrder order)
	{
		if (order == MatrixOrderPrepend) 
		{ 
			m_agg_mtx.premultiply(agg::trans_affine_skewing(shearX, shearY)); 
		}
		else                            
		{ 
			m_agg_mtx.multiply(agg::trans_affine_skewing(shearX, shearY)); 
		}
	}

	void CMatrix::TransformVectors(PointF* pts, int count)
	{
		// Store matrix to an array [6] of double
		double M[6]; m_agg_mtx.store_to(M);

		for(int i = 0; i < count; ++i)
		{
			double x =pts[i].X;
			double y =pts[i].Y;
			m_agg_mtx.transform(&x, &y);
			pts[i].X =(float)(x-M[4]);
			pts[i].Y =(float)(y-M[5]);
		}
	}

	void CMatrix::TransformPoints(PointF* pts, int count)
	{
		for(int i = 0; i < count; ++i)
		{
			double x = pts[i].X;
			double y = pts[i].Y;
			m_agg_mtx.transform(&x, &y);
			pts[i].X = (float)x;
			pts[i].Y = (float)y;
		}
	}

	void CMatrix::TransformPoint(double& x, double& y)
	{
		m_agg_mtx.transform(&x, &y);
	}

	void CMatrix::Rotate(double angle, MatrixOrder order)
	{
		if (order == MatrixOrderPrepend) 
		{ 
			m_agg_mtx.premultiply(agg::trans_affine_rotation(agg::deg2rad(angle))); 
		}
		else
		{ 
			m_agg_mtx.multiply(agg::trans_affine_rotation(agg::deg2rad(angle))); 
		}
	}

	void CMatrix::RotateAt(double angle, const PointF &center, MatrixOrder order)
	{
		Translate(-center.X, -center.Y, order);
		if(order == MatrixOrderPrepend) 
		{ 
			m_agg_mtx.premultiply(agg::trans_affine_rotation(agg::deg2rad(angle))); 
		}
		else                            
		{ 
			m_agg_mtx.multiply(agg::trans_affine_rotation(agg::deg2rad(angle))); 
		}
		Translate(center.X, center.Y, order);
		return;
	}

	void CMatrix::RotateAt(double angle, double x, double y, MatrixOrder order)
	{
		Translate(-x, -y, order);
		if (order == MatrixOrderPrepend) 
		{ 
			m_agg_mtx.premultiply(agg::trans_affine_rotation(agg::deg2rad(angle))); 
		}
		else                            
		{ 
			m_agg_mtx.multiply(agg::trans_affine_rotation(agg::deg2rad(angle))); 
		}
		Translate(x, y, order);
	}

	void CMatrix::Multiply(const CMatrix* matrix, MatrixOrder order)
	{
		if (order == MatrixOrderPrepend) 
		{ 
			m_agg_mtx.premultiply(matrix->m_agg_mtx); 
		}
		else
		{ 
			m_agg_mtx.multiply(matrix->m_agg_mtx); 
		}
	}

	double CMatrix::OffsetX() const
	{
		double M[6]; m_agg_mtx.store_to(M);
		return (M[4]);
	}
	double CMatrix::OffsetY() const
	{
		double M[6]; m_agg_mtx.store_to(M);
		return (M[5]);
	}
	
	Status CMatrix::GetElements(REAL* m) const
	{
		double M[6]; m_agg_mtx.store_to(M);
		m[0]=(REAL)M[0];
		m[1]=(REAL)M[1];
		m[2]=(REAL)M[2];
		m[3]=(REAL)M[3];
		m[4]=(REAL)M[4];
		m[5]=(REAL)M[5];
		return Ok;
	}

	Status CMatrix::GetElements(double* m) const
	{
		m_agg_mtx.store_to(m);
		return Ok;
	}

	void CMatrix::Reset()
	{
		m_agg_mtx.reset();
	}

	const CMatrix& CMatrix::operator=(const CMatrix& Src)
	{
		m_agg_mtx	= Src.m_agg_mtx;
		return		*this;
	}

	Status CMatrix::Invert()
	{
		double d = m_agg_mtx.determinant();
        if (0.0001 >= fabs(d))
			return InvalidParameter;

		m_agg_mtx.invert();
		return Ok;
	}

	//Temp
	//Used in X_BrushLinearGradient constructor
	double CMatrix::z_Rotation() const 
	{ 
		return agg::rad2deg(m_agg_mtx.rotation()); 
	}

	void CMatrix::TransformPoints( PointF* dst, const PointF* src, int count ) const
	{
		for(int i = 0; i < count; ++i)
		{
			double x = src[i].X * m_agg_mtx.sx + src[i].Y * m_agg_mtx.shx + m_agg_mtx.tx;
			double y = src[i].Y * m_agg_mtx.sy + src[i].X * m_agg_mtx.shy + m_agg_mtx.ty;
			
			dst[i].X = (float)x;
			dst[i].Y = (float)y;
		}
	}

	bool CMatrix::IsIdentity() const
	{
		return m_agg_mtx.is_identity();
	}
	bool CMatrix::IsIdentity2() const
	{
		return agg::is_equal_eps(m_agg_mtx.sx,  1.0, agg::affine_epsilon) &&
               agg::is_equal_eps(m_agg_mtx.shy, 0.0, agg::affine_epsilon) &&
               agg::is_equal_eps(m_agg_mtx.shx, 0.0, agg::affine_epsilon) && 
               agg::is_equal_eps(m_agg_mtx.sy,  1.0, agg::affine_epsilon);
	}
}