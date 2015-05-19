// Matrix.h: interface for the Matrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(Aggplus_MATRIX_H__57B2B6E0_4E30_43CB_87B4_931EF092CF49__INCLUDED_)
#define Aggplus_MATRIX_H__57B2B6E0_4E30_43CB_87B4_931EF092CF49__INCLUDED_

#include "ap_AggPlusEnums.h"
#include "ap_AggPlusTypes.h"
#include "agg_trans_affine.h"

namespace Aggplus
{
		class Matrix  
		{
			friend class Graphics;
			friend class GraphicsPath;
		public:
			//Matrix(float m11, float m12, float m21, float m22, float dx, float dy); //ZTODO
			Matrix();
			~Matrix();
			void Translate(float offsetX, float offsetY, MatrixOrder order=MatrixOrderPrepend);
			void Scale(float scaleX, float scaleY, MatrixOrder order=MatrixOrderPrepend);
			void Shear(float shearX, float shearY, MatrixOrder order=MatrixOrderPrepend);
			void TransformVectors(PointF* pts, int count);
			void TransformPoints(PointF* pts, int count);
			void Rotate(float angle, MatrixOrder order=MatrixOrderPrepend);
			void RotateAt(float angle, const PointF &center, MatrixOrder order=MatrixOrderPrepend);
			void Multiply(const Matrix* matrix, MatrixOrder order=MatrixOrderPrepend);

			REAL OffsetX() const;
			REAL OffsetY() const;
			Status GetElements(REAL* m) const;

			void Reset();

			const Matrix& operator=(const Matrix& Src);

			//Temp
			//Used in X_BrushLinearGradient constructor
			float z_Rotation() const { return((float)agg::rad2deg(m_agg_mtx.rotation())); }
		protected:
			agg::trans_affine m_agg_mtx;
		};


}

#endif // !defined(Aggplus_MATRIX_H__57B2B6E0_4E30_43CB_87B4_931EF092CF49__INCLUDED_)
