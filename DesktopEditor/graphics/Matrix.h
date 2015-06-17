#ifndef _BUILD_MATRIX_H_
#define _BUILD_MATRIX_H_

#include "AggPlusEnums.h"
#include "../agg-2.4/include/agg_trans_affine.h"

namespace Aggplus
{

class CMatrix  
{
	friend class CGraphics;
	friend class CGraphicsPath;

public:
	CMatrix(double m11, double m12, double m21, double m22, double dx, double dy);
	CMatrix();
    CMatrix(const CMatrix& oSrc);
	
	~CMatrix();
	
	void Translate(double offsetX, double offsetY, MatrixOrder order = MatrixOrderPrepend);
	void Scale(double scaleX, double scaleY, MatrixOrder order = MatrixOrderPrepend);
	void Shear(double shearX, double shearY, MatrixOrder order = MatrixOrderPrepend);

	void TransformVectors(PointF* pts, int count);
	void TransformPoints(PointF* pts, int count);
	void TransformPoint(double& x, double& y);

	void Rotate(double angle, MatrixOrder order = MatrixOrderPrepend);
	void RotateAt(double angle, const PointF &center, MatrixOrder order = MatrixOrderPrepend);
	void RotateAt(double angle, double x, double y, MatrixOrder order = MatrixOrderPrepend);

	void Multiply(const CMatrix* matrix, MatrixOrder order = MatrixOrderPrepend);

	double OffsetX() const;
	double OffsetY() const;
	
	Status GetElements(float* m) const;
	Status GetElements(double* m) const;

	void Reset();
	bool IsIdentity() const;
	bool IsIdentity2() const;

	const CMatrix& operator=(const CMatrix& Src);

	Status Invert();

	//Temp
	//Used in X_BrushLinearGradient constructor
	double z_Rotation() const;

	void TransformPoints( PointF* dst, const PointF* src, int count ) const;

public:
	agg::trans_affine m_agg_mtx;
};

}

#endif // _BUILD_MATRIX_H_