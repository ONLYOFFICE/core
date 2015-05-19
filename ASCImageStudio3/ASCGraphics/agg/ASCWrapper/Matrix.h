#pragma once

#include "..\ap_AggPlusEnums.h"
#include "..\ap_AggPlusTypes.h"
#include "..\include\agg_trans_affine.h"

namespace Aggplus
{

class CMatrix  
{
	friend class CGraphics;
	friend class CGraphicsPath;

public:
	CMatrix(double m11, double m12, double m21, double m22, double dx, double dy) : m_agg_mtx(m11, m12, m21, m22, dx, dy)
	{
	}

	CMatrix() : m_agg_mtx() 
	{
	}
	
	~CMatrix()
	{
	}
	
	void Translate(double offsetX, double offsetY, MatrixOrder order = MatrixOrderPrepend)
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

	void Scale(double scaleX, double scaleY, MatrixOrder order = MatrixOrderPrepend)
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

	void Shear(double shearX, double shearY, MatrixOrder order = MatrixOrderPrepend)
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

	void TransformVectors(PointF* pts, int count)
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

	void TransformPoints(PointF* pts, int count)
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

	void TransformPoint(double& x, double& y)
	{
		m_agg_mtx.transform(&x, &y);
	}

	void Rotate(double angle, MatrixOrder order = MatrixOrderPrepend)
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

	void RotateAt(double angle, const PointF &center, MatrixOrder order = MatrixOrderPrepend)
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

	void RotateAt(double angle, double x, double y, MatrixOrder order = MatrixOrderPrepend)
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

	void Multiply(const CMatrix* matrix, MatrixOrder order = MatrixOrderPrepend)
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

	double OffsetX() const
	{
		double M[6]; m_agg_mtx.store_to(M);
		return (M[4]);
	}
	double OffsetY() const
	{
		double M[6]; m_agg_mtx.store_to(M);
		return (M[5]);
	}
	
	Status GetElements(REAL* m) const
	{
		double M[6]; m_agg_mtx.store_to(M);
		m[0]=(float)M[0];
		m[1]=(float)M[1];
		m[2]=(float)M[2];
		m[3]=(float)M[3];
		m[4]=(float)M[4];
		m[5]=(float)M[5];
		return Ok;
	}

	Status GetElements(double* m) const
	{
		m_agg_mtx.store_to(m);
		return Ok;
	}

	void Reset()
	{
		m_agg_mtx.reset();
	}

	const CMatrix& operator=(const CMatrix& Src)
	{
		m_agg_mtx	= Src.m_agg_mtx;
		return		*this;
	}

	Status Invert()
	{
		double d = m_agg_mtx.determinant();
		if (0.0001 >= abs(d))
			return InvalidParameter;

		m_agg_mtx.invert();
		return Ok;
	}

	//Temp
	//Used in X_BrushLinearGradient constructor
	double z_Rotation() const 
	{ 
		return agg::rad2deg(m_agg_mtx.rotation()); 
	}

	void TransformPoints( PointF* dst, const PointF* src, int count ) const
	{
		for(int i = 0; i < count; ++i)
		{
			double x = src[i].X * m_agg_mtx.sx + src[i].Y * m_agg_mtx.shx + m_agg_mtx.tx;
			double y = src[i].Y * m_agg_mtx.sy + src[i].X * m_agg_mtx.shy + m_agg_mtx.ty;
			
			dst[i].X = (float)x;
			dst[i].Y = (float)y;
		}
	}

public:
	agg::trans_affine m_agg_mtx;
};

}