#pragma once

#include "ap_AggPlusEnums.h"
#include "ap_AggPlusTypes.h"
#include "agg_trans_affine.h"

namespace NSPresentationEditor
{
	class CMatrix  
	{
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
		
		void Translate(double offsetX, double offsetY, Aggplus::MatrixOrder order = Aggplus::MatrixOrderPrepend)
		{
			if (order == Aggplus::MatrixOrderPrepend) 
			{ 
				m_agg_mtx.premultiply(agg::trans_affine_translation(offsetX, offsetY)); 
			}
			else                            
			{ 
				m_agg_mtx.multiply(agg::trans_affine_translation(offsetX, offsetY)); 
			}
		}

		void Scale(double scaleX, double scaleY, Aggplus::MatrixOrder order = Aggplus::MatrixOrderPrepend)
		{	
			if (order == Aggplus::MatrixOrderPrepend) 
			{ 
				m_agg_mtx.premultiply(agg::trans_affine_scaling(scaleX, scaleY)); 
			}
			else                            
			{ 
				m_agg_mtx.multiply(agg::trans_affine_scaling(scaleX, scaleY)); 
			}
		}

		void Shear(double shearX, double shearY, Aggplus::MatrixOrder order = Aggplus::MatrixOrderPrepend)
		{
			if (order == Aggplus::MatrixOrderPrepend) 
			{ 
				m_agg_mtx.premultiply(agg::trans_affine_skewing(shearX, shearY)); 
			}
			else                            
			{ 
				m_agg_mtx.multiply(agg::trans_affine_skewing(shearX, shearY)); 
			}
		}

		void TransformPoint(double& x, double& y)
		{
			m_agg_mtx.transform(&x, &y);
		}

		void Rotate(double angle, Aggplus::MatrixOrder order = Aggplus::MatrixOrderPrepend)
		{
			if (order == Aggplus::MatrixOrderPrepend) 
			{ 
				m_agg_mtx.premultiply(agg::trans_affine_rotation(agg::deg2rad(angle))); 
			}
			else
			{ 
				m_agg_mtx.multiply(agg::trans_affine_rotation(agg::deg2rad(angle))); 
			}
		}

		void RotateAt(double angle, double x, double y, Aggplus::MatrixOrder order = Aggplus::MatrixOrderPrepend)
		{
			Translate(-x, -y, order);
			if (order == Aggplus::MatrixOrderPrepend) 
			{ 
				m_agg_mtx.premultiply(agg::trans_affine_rotation(agg::deg2rad(angle))); 
			}
			else                            
			{ 
				m_agg_mtx.multiply(agg::trans_affine_rotation(agg::deg2rad(angle))); 
			}
			Translate(x, y, order);
		}

		void Multiply(const CMatrix* matrix, Aggplus::MatrixOrder order = Aggplus::MatrixOrderPrepend)
		{
			if (order == Aggplus::MatrixOrderPrepend) 
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
		
		bool GetElements(float* m) const
		{
			double M[6]; m_agg_mtx.store_to(M);
			m[0]=(float)M[0];
			m[1]=(float)M[1];
			m[2]=(float)M[2];
			m[3]=(float)M[3];
			m[4]=(float)M[4];
			m[5]=(float)M[5];
			return true;
		}

		bool GetElements(double* m) const
		{
			m_agg_mtx.store_to(m);
			return true;
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

		bool Invert()
		{
			double d = m_agg_mtx.determinant();
			if (0.0001 >= abs(d))
				return false;

			m_agg_mtx.invert();
			return true;
		}

		//Temp
		//Used in X_BrushLinearGradient constructor
		double z_Rotation() const 
		{ 
			return agg::rad2deg(m_agg_mtx.rotation()); 
		}

	public:
		agg::trans_affine m_agg_mtx;
	};
}