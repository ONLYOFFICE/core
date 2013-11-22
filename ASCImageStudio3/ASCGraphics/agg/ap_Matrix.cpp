//----------------------------------------------------------------------------
// Matrix class  - Version 0.1 - 28-10-2004
// Marlon Mircevski
//----------------------------------------------------------------------------
//#include "stdafx.h"
#include "ap_Matrix.h"

namespace Aggplus
{

		Matrix::Matrix()
		{
		}
	
		Matrix::~Matrix()
		{
		}

		const Matrix& Matrix::operator=(const Matrix& Src)
		{
			m_agg_mtx=Src.m_agg_mtx;
			return(*this);
		}

		void Matrix::Reset()
		{
			m_agg_mtx.reset();
		}

		void Matrix::Translate(float offsetX, float offsetY, MatrixOrder order)
		{
			if(order==MatrixOrderPrepend) 
			{ 
				m_agg_mtx.premultiply(agg::trans_affine_translation(offsetX, offsetY)); 
			}
			else                            
			{ 
				m_agg_mtx.multiply(agg::trans_affine_translation(offsetX, offsetY)); 
			}
		}

		void Matrix::Scale(float scaleX, float scaleY, MatrixOrder  order)
		{
			if(order==MatrixOrderPrepend) 
			{ 
				m_agg_mtx.premultiply(agg::trans_affine_scaling(scaleX, scaleY)); 
			}
			else                            
			{ 
				m_agg_mtx.multiply(agg::trans_affine_scaling(scaleX, scaleY)); 
			}
		}

		void Matrix::Multiply(const Matrix* matrix, MatrixOrder order)
		{
			if(order==MatrixOrderPrepend) 
			{ 
				m_agg_mtx.premultiply(matrix->m_agg_mtx); 
			}
			else
			{ 
				m_agg_mtx.multiply(matrix->m_agg_mtx); 
			}
		}

		void Matrix::Rotate(float angle, MatrixOrder order)
		{
			if(order==MatrixOrderPrepend) 
			{ 
				m_agg_mtx.premultiply(agg::trans_affine_rotation(agg::deg2rad(angle))); 
			}
			else
			{ 
				m_agg_mtx.multiply(agg::trans_affine_rotation(agg::deg2rad(angle))); 
			}
		}

		void Matrix::RotateAt(float angle, const PointF &center, MatrixOrder order)
		{
			Translate(center.X, center.Y, order);
			if(order==MatrixOrderPrepend) 
			{ 
				m_agg_mtx.premultiply(agg::trans_affine_rotation(agg::deg2rad(angle))); 
			}
			else                            
			{ 
				m_agg_mtx.multiply(agg::trans_affine_rotation(agg::deg2rad(angle))); 
			}
			Translate(-center.X, -center.Y, order);
		}

		void Matrix::Shear(float shearX, float shearY, MatrixOrder order)
		{
			if(order==MatrixOrderPrepend) 
			{ 
				m_agg_mtx.premultiply(agg::trans_affine_skewing(shearX, shearY)); 
			}
			else                            
			{ 
				m_agg_mtx.multiply(agg::trans_affine_skewing(shearX, shearY)); 
			}
		}

		REAL Matrix::OffsetX() const
		{
			// Store matrix to an array [6] of double
			double M[6]; m_agg_mtx.store_to(M);
			return (REAL)(M[4]);
		}

		REAL Matrix::OffsetY() const
		{
			// Store matrix to an array [6] of double
			double M[6]; m_agg_mtx.store_to(M);
			return (REAL)(M[5]);
		}

		Status Matrix::GetElements(REAL* m) const
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

		void Matrix::TransformPoints(PointF* pts, int count)
		{
			for(int i=0;i<count;i++)
			{
				double x=pts[i].X;
				double y=pts[i].Y;
				m_agg_mtx.transform(&x, &y);
				pts[i].X=(float)x;
				pts[i].Y=(float)y;
			}
		}

		void Matrix::TransformVectors(PointF* pts, int count)
		{
			// Store matrix to an array [6] of double
			double M[6]; m_agg_mtx.store_to(M);
			//TRACE("Matrix dump=%.2f %.2f %.2f %.2f %.2f %.2f\n", M[0], M[1], M[2], M[3], M[4], M[5]);

			for(int i=0;i<count;i++)
			{
				double x=pts[i].X;
				double y=pts[i].Y;
				m_agg_mtx.transform(&x, &y);
				pts[i].X=(float)(x-M[4]);
				pts[i].Y=(float)(y-M[5]);
			}
		}


}
